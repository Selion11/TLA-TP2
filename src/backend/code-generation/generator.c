#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

FILE * file;

void Generator(Program * result) {
	LogDebug("[Generator] Program");

	file = fopen("ast_graph.dot", "w+");
	if(file == NULL){
		LogError("Error opening file");
		return;
	}
	if(result == NULL || result->statementList == NULL){
		LogError("Compiling error: result, result->statementList -> NULL");
		return;
	}

	fprintf(file, "digraph AST {\n");
    
    generateStatementList(result->statementList, "program");
    
    fprintf(file, "\n}");

	fclose(file);
}

void generateStatementList(StatementList * statementList, const char * parentNode) {
	LogDebug("[Generator] statementList");
    Statement *statement = statementList->firstStatement;
    
		while (statement != NULL) {
			if (statement->actionType == CREATION_AC) {
				generateCreateNode(statement->createNode, parentNode);
			} else if (statement->actionType == UNION_AC) {
				generateConnectNodes(statement->connectNodes, parentNode);
			}
			
			statement = statement->nextStatement;
		}
}

void generateCreateNode(CreateNode * createNode, const char * parentNode) {
	LogDebug("[Generator] createNode");
    if (createNode != NULL) {
        fprintf(file, "  %s [", createNode->name, createNode->name);
        generateNodeProperties(createNode->nodeProperties, "createNode_%p", createNode);
		fprintf(file, "];");
    }
}

void generateNodeProperties(NodeProperties * nodeProperties, const char * parentNode, CreateNode * createNode) {
	LogDebug("[Generator] nodeProperties");
    if (nodeProperties != NULL) {
        generatePropertyList(nodeProperties->propertyList, parentNode, createNode);
    }
}

void generatePropertyList(PropertyList * propertyList, const char * parentNode, CreateNode * createNode) {
	LogDebug("[Generator] propertyList");
    Property * property = propertyList->firstProperty;
    
    while (property != NULL) {
        if (property->propertyType == TEXT_PROP) {
            fprintf(file, "label=%s, ",property->value.text);
        } else if (property->propertyType == BACKGROUND_PROP) {
            fprintf(file, "fillcolor=\"%s\", ",property->value.colorType);
        } else if (property->propertyType == BORDER_PROP) {
            fprintf(file, "color=\"%s\", ", (void *) property, property->value.colorType);
        }
        
        property = property->nextProperty;
    }
}

void generateConnectNodes(ConnectNodes * connectNodes, const char * parentNode) {
	LogDebug("[Generator] connectNodes");
    if (connectNodes != NULL) {
		UnionType unionType = connectNodes->unionType;
		if (unionType == ARROW_UNION || unionType == LOOP_UNION)
		{
			fprintf(file, " %s -> %s ", connectNodes->nodeFrom, connectNodes->nodeTo);
		}
		else if (unionType == DOUBLE_ARROW_UNION){
			fprintf(file, " %s -> %s ", connectNodes->nodeFrom, connectNodes->nodeTo);
			fprintf(file, " %s -> %s ", connectNodes->nodeTo, connectNodes->nodeFrom);
		}
        
        generateLineType(connectNodes->lineType, "connectNodes_%p", connectNodes);
    }
}

void generateLineType(LineType lineType, const char * parentNode, ConnectNodes * connectNodes) {
	LogDebug("[Generator] lineType");
	if (lineType == DOTTED_LINE){
		fprintf(file, "[style=dotted]");
	}
	else if (lineType == STRONG_LINE){
		fprintf(file, "[style=bold]");
	}
}