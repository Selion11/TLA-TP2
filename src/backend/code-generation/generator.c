#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

//Global variables
FILE * file;
UnionWrappedNode * unionStack = NULL;
int unionStackTop = -1;
int unionStackSize = 0;
int unionPropsFlag = 0;

//Utility method
const char* colorTypeToString(ColorType colorType);
void initUnionStack(int size);
void pushUnion(UnionWrappedNode unionNode);
UnionWrappedNode popUnion(void);

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

	initUnionStack(10); 

	fprintf(file, "digraph AST {\n");
    
    generateStatementList(result->statementList, "program");
    
	if(unionPropsFlag){
		generatePopConnectNodes();
	}

    fprintf(file, "}");

	fclose(file);
	free(unionStack);
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
        fprintf(file, "  %s [", createNode->name);
        generateNodeProperties(createNode->nodeProperties, "createNode_%p", createNode);
		fprintf(file, "];\n");
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
            fprintf(file, "label=%s, ", property->value.text);
        } else if (property->propertyType == BACKGROUND_PROP || property->propertyType == BORDER_PROP) {
            const char * colorString = colorTypeToString(property->value.colorType);
            fprintf(file, "%s=%s, ", (property->propertyType == BACKGROUND_PROP) ? "style=filled, fillcolor" : "color", colorString);
        } else if (property->propertyType == UNION_PROP) {
			UnionWrappedNode unionNode;
			unionNode.nodeFromName = strdup(createNode->name);
			unionNode.nodeToName = strdup(property->value.unionProp.nodeName);
			unionNode.lineType = property->value.unionProp.lineType;

			pushUnion(unionNode);
			unionPropsFlag = 1;
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
			fprintf(file, "  %s -> %s ", connectNodes->nodeFrom, connectNodes->nodeTo);
		}
		else if (unionType == DOUBLE_ARROW_UNION){
			fprintf(file, "  %s -> %s ", connectNodes->nodeFrom, connectNodes->nodeTo);
			fprintf(file, "  %s -> %s ", connectNodes->nodeTo, connectNodes->nodeFrom);
		}
        
        generateLineType(connectNodes->lineType);
    }
}

void generateLineType(LineType lineType) {
	LogDebug("[Generator] lineType");
	if (lineType == DOTTED_LINE){
		fprintf(file, "[style=dotted];\n");
	}
	else if (lineType == STRONG_LINE){
		fprintf(file, "[style=bold];\n");
	} else {
		fprintf(file, ";\n");
	}
}

void generatePopConnectNodes() {
    while (unionStackTop >= 0) {
        UnionWrappedNode unionNode = popUnion();
        fprintf(file, "  %s -> %s ", unionNode.nodeFromName, unionNode.nodeToName);
        generateLineType(unionNode.lineType);
    }

	unionPropsFlag = 0;
}

/*********************************************************Utility Methods**************************************************************************/

const char* colorTypeToString(ColorType colorType) {
    switch (colorType) {
        case PINK_COLOR:
            return "pink";
        case RED_COLOR:
            return "red";
        case GREEN_COLOR:
            return "green";
        case BLUE_COLOR:
            return "blue";
        case PURPLE_COLOR:
            return "violet";
        default:
            return "unknown_color";
    }
}

void initUnionStack(int size) {
    unionStack = (UnionWrappedNode *)malloc(size * sizeof(UnionWrappedNode));
    unionStackSize = size;
    unionStackTop = -1;
}

void pushUnion(UnionWrappedNode unionNode) {
    if (unionStackTop < unionStackSize - 1) {
        unionStack[++unionStackTop] = unionNode;
    } else {
        // Manejar error de desbordamiento de pila
        LogError("Union stack overflow");
    }
}

UnionWrappedNode popUnion(void) {
    if (unionStackTop >= 0) {
        return unionStack[unionStackTop--];
    } else {
        // Manejar error de pila vacía
        LogError("Union stack underflow");
        return (UnionWrappedNode){NULL, NULL, DOTTED_LINE}; // Valor de retorno de emergencia
    }
}