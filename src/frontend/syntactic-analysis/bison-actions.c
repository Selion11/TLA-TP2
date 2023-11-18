#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(StatementList * statementList) {
	LogDebug("[Bison] ProgramGrammarAction");

	Program * program = calloc(1, sizeof(Program));
	if(program == NULL){
		LogError("Error from calloc [program]");
		return NULL;
	}

	program->statementList = statementList;
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.result = program;

	return program;
}

StatementList * SimpleStatementAction(Statement * statement){
	LogDebug("[Bison] SimpleStatementAction");
	StatementList * statementList = calloc(1, sizeof(StatementList));
	if(statementList == NULL){
		LogError("Error from calloc [statementList]");
		return NULL;
	}

	statement->nextStatement = NULL;
	statementList->firstStatement = statement;
	statementList->size = 1;

	return statementList;
}

StatementList * RightAppendStatementAction(StatementList * statementList, Statement * statement){
	Statement * aux = statementList->firstStatement;
	while (aux->nextStatement != NULL)
	{
		aux = aux->nextStatement;
	}
	
	aux->nextStatement = statement;
	statementList->size++;

	return statementList;
}

Statement * ConnectNodesStatementAction(ConnectNodes * connectNodes){
	LogDebug("[Bison] ConnectNodesStatementAction");
	Statement * statement = calloc(1, sizeof(Statement));
	if(statement == NULL){
		LogError("Error from calloc [statement]");
		return NULL;
	}

	statement->connectNodes = connectNodes;
	statement->createNode = NULL;
	statement->nextStatement = NULL;
	statement->actionType = UNION_AC;

	return statement;
}

Statement * CreateNodeStatementAction(CreateNode * createNode){
	LogDebug("[Bison] CreateNodeStatementAction");
	Statement * statement = calloc(1, sizeof(Statement));
	if(statement == NULL){
		LogError("Error from calloc [statement]");
		return NULL;
	}

	statement->createNode = createNode;
	statement->connectNodes = NULL;
	statement->nextStatement = NULL;
	statement->actionType = CREATION_AC;

	return statement;
}

CreateNode * CreateNodeAction(char * name, NodeProperties * nodeProperties){
	LogDebug("[Bison] CreateNodeAction");
	CreateNode * createNode = calloc(1, sizeof(CreateNode));
	if(createNode == NULL){
		LogError("Error from calloc [createNode]");
		return NULL;
	}

	createNode->nodeProperties = nodeProperties;
	createNode->name = malloc(sizeof(char) * (strlen(name) + 1));
	if(createNode->name == NULL){
		LogError("Error from malloc [createNode->name]");
		return NULL;
	}
	strcpy(createNode->name, name);

	return createNode;
}

NodeProperties * EmptyNodePropertiesAction(){
	LogDebug("[Bison] EmptyNodePropertiesAction");
	NodeProperties * nodeProperties = calloc(1, sizeof(NodeProperties));
	if(nodeProperties == NULL){
		LogError("Error from calloc [nodeProperties]");
		return NULL;
	}

	nodeProperties->propertyList = NULL;
	
	return nodeProperties;
}

NodeProperties * SetPropertyListAction(PropertyList * propertyList){
	LogDebug("[Bison] SetPropertyListAction");
	NodeProperties * nodeProperties = calloc(1, sizeof(NodeProperties));
	if(nodeProperties == NULL){
		LogError("Error from calloc [nodeProperties]");
		return NULL;
	}

	nodeProperties->propertyList = propertyList;

	return nodeProperties;
}

PropertyList * SimplePropertyAction(Property * property){
	LogDebug("[Bison] SimplePropertyAction");
	PropertyList * propertyList = calloc(1, sizeof(PropertyList));
	if(propertyList == NULL){
		LogError("Error from calloc [propertyList]");
		return NULL;
	}

	property->nextProperty = NULL;
	propertyList->firstProperty = property;
	propertyList->size = 1;

	return propertyList;
}

PropertyList * RightAppendPropertyAction(PropertyList * propertyList, Property * property){
	LogDebug("[Bison] RightAppendPropertyAction");
	Property * aux = propertyList->firstProperty;

	while (aux->nextProperty != NULL)
	{
		aux = aux->nextProperty;
	}

	aux->nextProperty = property;
	propertyList->size++;

	return propertyList;
}

Property * CustomUnionPropertyAction(char * nodeToName, LineType lineType) {
	LogDebug("[Bison] CustionUnionPropertyAction");
    Property * property = calloc(1,sizeof(Property));
    if (property == NULL) {
        LogError("Error from calloc [property]");
        return NULL;
    }

    property->propertyType = UNION_PROP;
    property->value.unionProp.nodeName = strdup(nodeToName);
    property->value.unionProp.lineType = lineType;
    property->nextProperty = NULL;

    return property;
}

Property * DefaultUnionPropertyAction(char * nodeToName){
	LogDebug("[Bison] DefaultUnionPropertyAction");
    Property * property = calloc(1,sizeof(Property));
    if (property == NULL) {
        LogError("Error from calloc [property]");
        return NULL;
    }

    property->propertyType = UNION_PROP;
    property->value.unionProp.nodeName = strdup(nodeToName);
    property->value.unionProp.lineType = UNSPECIFIED_LINE;
    property->nextProperty = NULL;

    return property;
}

Property * NodeTextPropertyAction(char * text) {
	LogDebug("[Bison] NodeTextPropertyAction");
    Property * property = calloc(1,sizeof(Property));
    if (property == NULL) {
        LogError("Error from calloc [property]");
        return NULL;
    }

    property->propertyType = TEXT_PROP;
    property->value.text = strdup(text);
    property->nextProperty = NULL;

    return property;
}

Property * NodeBackgroundColorPropertyAction(ColorType color) {
	LogDebug("[Bison] NodeBackgroundColorPropertyAction");
    Property *property = calloc(1,sizeof(Property));
    if (property == NULL) {
        LogError("Error from calloc [property]");
        return NULL;
    }

    property->propertyType = BACKGROUND_PROP;
    property->value.colorType = color;
    property->nextProperty = NULL;

    return property;
}

Property * NodeBorderColorPropertyAction(ColorType color) {
	LogDebug("[Bison] NodeBorderColorPropertyAction");
    Property *property = calloc(1,sizeof(Property));
    if (property == NULL) {
        LogError("Error from calloc [property]");
        return NULL;
    }

    property->propertyType = BORDER_PROP;
    property->value.colorType = color;
    property->nextProperty = NULL;

    return property;
}

ConnectNodes * SimpleShortUnionPropertyAction(char * nodeFromName, char * nodeToName){
	LogDebug("[Bison] SimpleShortUnionPropertyAction");
	ConnectNodes * connectNodes = calloc(1, sizeof(ConnectNodes));
	if(connectNodes == NULL){
		LogError("Error from calloc [connectNodes]");
		return NULL;
	}

	connectNodes->nodeFrom = malloc(sizeof(char) * (strlen(nodeFromName) + 1));
	if(connectNodes->nodeFrom == NULL){
		LogError("Error from malloc [connectNodes->nodeFrom]");
		return NULL;
	}
	connectNodes->nodeTo = malloc(sizeof(char) * (strlen(nodeToName) + 1));
	if(connectNodes->nodeTo == NULL){
		LogError("Error from malloc [connectNodes->nodeTo]");
		return NULL;
	}

	strcpy(connectNodes->nodeFrom, nodeFromName);
	strcpy(connectNodes->nodeTo, nodeToName);

	connectNodes->lineType = UNSPECIFIED_LINE;
	connectNodes->unionType = ARROW_UNION;

	return connectNodes;
}

ConnectNodes * DoubleShortUnionPropertyAction(char * nodeFromName, char * nodeToName){
	LogDebug("[Bison] DoubleShortUnionPropertyAction");
	ConnectNodes * connectNodes = calloc(1, sizeof(ConnectNodes));
	if(connectNodes == NULL){
		LogError("Error from calloc [connectNodes]");
		return NULL;
	}

	connectNodes->nodeFrom = malloc(sizeof(char) * (strlen(nodeFromName) + 1));
	if(connectNodes->nodeFrom == NULL){
		LogError("Error from malloc [connectNodes->nodeFrom]");
		return NULL;
	}
	connectNodes->nodeTo = malloc(sizeof(char) * (strlen(nodeToName) + 1));
	if(connectNodes->nodeTo == NULL){
		LogError("Error from malloc [connectNodes->nodeTo]");
		return NULL;
	}

	strcpy(connectNodes->nodeFrom, nodeFromName);
	strcpy(connectNodes->nodeTo, nodeToName);

	connectNodes->lineType = UNSPECIFIED_LINE;
	connectNodes->unionType = DOUBLE_ARROW_UNION;

	return connectNodes;
}

ConnectNodes * LoopShortUnionPropertyAction(char * nodeName){
	LogDebug("[Bison] LoopShortUnionPropertyAction");
	ConnectNodes * connectNodes = calloc(1, sizeof(ConnectNodes));
	if(connectNodes == NULL){
		LogError("Error from calloc [connectNodes]");
		return NULL;
	}

	connectNodes->nodeFrom = malloc(sizeof(char) * (strlen(nodeName) + 1));
	if(connectNodes->nodeFrom == NULL){
		LogError("Error from malloc [connectNodes->nodeFrom]");
		return NULL;
	}
	connectNodes->nodeTo = malloc(sizeof(char) * (strlen(nodeName) + 1));
	if(connectNodes->nodeTo == NULL){
		LogError("Error from malloc [connectNodes->nodeTo]");
		return NULL;
	}

	strcpy(connectNodes->nodeFrom, nodeName);
	strcpy(connectNodes->nodeTo, nodeName);

	connectNodes->lineType = UNSPECIFIED_LINE;
	connectNodes->unionType = LOOP_UNION;

	return connectNodes;
}

ConnectNodes * CustomSimpleShortUnionPropertyAction(char * nodeFromName, char * nodeToName, LineType lineType){
	LogDebug("[Bison] CustomSimpleShortUnionPropertyAction");
	ConnectNodes * connectNodes = calloc(1, sizeof(ConnectNodes));
	if(connectNodes == NULL){
		LogError("Error from calloc [connectNodes]");
		return NULL;
	}

	connectNodes->nodeFrom = malloc(sizeof(char) * (strlen(nodeFromName) + 1));
	if(connectNodes->nodeFrom == NULL){
		LogError("Error from malloc [connectNodes->nodeFrom]");
		return NULL;
	}
	connectNodes->nodeTo = malloc(sizeof(char) * (strlen(nodeToName) + 1));
	if(connectNodes->nodeTo == NULL){
		LogError("Error from malloc [connectNodes->nodeTo]");
		return NULL;
	}

	strcpy(connectNodes->nodeFrom, nodeFromName);
	strcpy(connectNodes->nodeTo, nodeToName);

	connectNodes->lineType = lineType;
	connectNodes->unionType = ARROW_UNION;

	return connectNodes;
}

ConnectNodes * CustomDoubleShortUnionPropertyAction(char * nodeFromName, char * nodeToName, LineType lineType){
	LogDebug("[Bison] CustomDoubleShortUnionPropertyAction");
	ConnectNodes * connectNodes = calloc(1, sizeof(ConnectNodes));
	if(connectNodes == NULL){
		LogError("Error from calloc [connectNodes]");
		return NULL;
	}

	connectNodes->nodeFrom = malloc(sizeof(char) * (strlen(nodeFromName) + 1));
	if(connectNodes->nodeFrom == NULL){
		LogError("Error from malloc [connectNodes->nodeFrom]");
		return NULL;
	}
	connectNodes->nodeTo = malloc(sizeof(char) * (strlen(nodeToName) + 1));
	if(connectNodes->nodeTo == NULL){
		LogError("Error from malloc [connectNodes->nodeTo]");
		return NULL;
	}

	strcpy(connectNodes->nodeFrom, nodeFromName);
	strcpy(connectNodes->nodeTo, nodeToName);

	connectNodes->lineType = lineType;
	connectNodes->unionType = DOUBLE_ARROW_UNION;

	return connectNodes;
}

ConnectNodes * CustomLoopShortUnionPropertyAction(char * nodeName, LineType lineType){
	LogDebug("[Bison] CustomLoopShortUnionPropertyAction");
	ConnectNodes * connectNodes = calloc(1, sizeof(ConnectNodes));
	if(connectNodes == NULL){
		LogError("Error from calloc [connectNodes]");
		return NULL;
	}

	connectNodes->nodeFrom = malloc(sizeof(char) * (strlen(nodeName) + 1));
	if(connectNodes->nodeFrom == NULL){
		LogError("Error from malloc [connectNodes->nodeFrom]");
		return NULL;
	}
	connectNodes->nodeTo = malloc(sizeof(char) * (strlen(nodeName) + 1));
	if(connectNodes->nodeTo == NULL){
		LogError("Error from malloc [connectNodes->nodeTo]");
		return NULL;
	}

	strcpy(connectNodes->nodeFrom, nodeName);
	strcpy(connectNodes->nodeTo, nodeName);

	connectNodes->lineType = lineType;
	connectNodes->unionType = LOOP_UNION;

	return connectNodes;
}