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
int unionStackMemorySize = 0;
int unionPropsFlag = 0;

//Utility method
const char* colorTypeToString(ColorType colorType);
void initUnionStack(int size);
void pushUnion(UnionWrappedNode unionNode);
UnionWrappedNode popUnion(void);
void freeUnionWrappedNode(UnionWrappedNode *unionNode);
void freeUnionStack(void);

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
    
    generateStatementList(result->statementList);
    
	if(unionPropsFlag){
		generatePopConnectNodes();
	}

    fprintf(file, "}");

	fclose(file);

    //cleanupMemory(result);
	freeUnionStack();
}

void generateStatementList(StatementList * statementList) {
	LogDebug("[Generator] statementList");
    Statement *statement = statementList->firstStatement;
    
		while (statement != NULL) {
			if (statement->actionType == CREATION_AC) {
				generateCreateNode(statement->createNode);
			} else if (statement->actionType == UNION_AC) {
				generateConnectNodes(statement->connectNodes);
			}
			
			statement = statement->nextStatement;
		}
}

void generateCreateNode(CreateNode * createNode) {
	LogDebug("[Generator] createNode");
    if (createNode != NULL) {
        fprintf(file, "  %s [", createNode->name);
        generateNodeProperties(createNode->nodeProperties, createNode);
		fprintf(file, "];\n");
    }
}

void generateNodeProperties(NodeProperties * nodeProperties, CreateNode * createNode) {
	LogDebug("[Generator] nodeProperties");
    if (nodeProperties != NULL) {
        generatePropertyList(nodeProperties->propertyList, createNode);
    }
}

void generatePropertyList(PropertyList * propertyList, CreateNode * createNode) {
	LogDebug("[Generator] propertyList");
    
    Property * property;
    if(propertyList != NULL){
        property = propertyList->firstProperty;
    } else return;
    
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
            unionNode.isUsed = 1;

			pushUnion(unionNode);
			unionPropsFlag = 1;
		}
        
        property = property->nextProperty;
    }
}

void generateConnectNodes(ConnectNodes * connectNodes) {
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
    unionStack = (UnionWrappedNode *)calloc(size, size * sizeof(UnionWrappedNode));
    unionStackSize = size;
    unionStackTop = -1;
}

void pushUnion(UnionWrappedNode unionNode) {
    if (unionStackTop < unionStackSize - 1) {
        unionStackTop++;
        if(unionStack[unionStackTop].isUsed){
            freeUnionWrappedNode(&unionNode);
        }
        unionStack[unionStackTop] = unionNode;
        unionStackMemorySize++;
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

void freeUnionWrappedNode(UnionWrappedNode *unionNode) {
    if (unionNode != NULL) {
        free(unionNode->nodeFromName);
        free(unionNode->nodeToName);
    }
}

void freeUnionStack(void) {
    for (int i = 0; i <= unionStackMemorySize; ++i) {
        freeUnionWrappedNode(&unionStack[i]);
    }

    free(unionStack);
    unionStackTop = -1;
    unionStackSize = 0;
    unionStack = NULL;
}


/*********************************************************Cleanup**************************************************************************/

void cleanupMemory(Program *result) {
    if (result != NULL) {
        freeStatementList(result->statementList);
        free(result);
    }
    // Agrega aquí más llamadas para liberar otras estructuras dinámicas si es necesario
}

void freeStatementList(StatementList *statementList) {
    if (statementList != NULL) {
        Statement *statement = statementList->firstStatement;
        while (statement != NULL) {
            freeStatement(statement);
            statement = statement->nextStatement;
        }
        free(statementList);
    }
}

void freeStatement(Statement *statement) {
    if (statement != NULL) {
        if (statement->actionType == CREATION_AC) {
            freeCreateNode(statement->createNode);
        } else if (statement->actionType == UNION_AC) {
            freeConnectNodes(statement->connectNodes);
        }
        free(statement);
    }
}

void freeCreateNode(CreateNode *createNode) {
    if (createNode != NULL) {
        freeNodeProperties(createNode->nodeProperties);
        free(createNode->name);
        free(createNode);
    }
}

void freeNodeProperties(NodeProperties *nodeProperties) {
    if (nodeProperties != NULL) {
        freePropertyList(nodeProperties->propertyList);
        free(nodeProperties);
    }
}

void freePropertyList(PropertyList *propertyList) {
    if (propertyList != NULL) {
        Property *property = propertyList->firstProperty;
        while (property != NULL) {
            freeProperty(property);
            property = property->nextProperty;
        }
        free(propertyList);
    }
}

void freeProperty(Property *property) {
    if (property != NULL) {
        if (property->propertyType == TEXT_PROP) {
            free(property->value.text);
        } else if (property->propertyType == UNION_PROP) {
            free(property->value.unionProp.nodeName);
        }
        free(property);
    }
}

void freeConnectNodes(ConnectNodes *connectNodes) {
    if (connectNodes != NULL) {
        free(connectNodes->nodeFrom);
        free(connectNodes->nodeTo);
        free(connectNodes);
    }
}
