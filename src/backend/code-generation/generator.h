#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include <stdlib.h>
#include <string.h>

// Estructura para representar un elemento de la pila de uniones de nodos.
typedef struct {
    char *nodeFromName;
    char *nodeToName;
    LineType lineType;
} UnionWrappedNode;

void Generator(Program * result);
void generateStatementList(StatementList * statementList, const char * parentNode);
void generateCreateNode(CreateNode * createNode, const char * parentNode);
void generateNodeProperties(NodeProperties * nodeProperties, const char * parentNode, CreateNode * createNode);
void generatePropertyList(PropertyList * propertyList, const char * parentNode, CreateNode * createNode);
void generateConnectNodes(ConnectNodes * connectNodes, const char * parentNode);
void generateLineType(LineType lineType);
void generateUnionType(UnionType unionType, const char * parentNode, ConnectNodes * connectNodes);
void generatePopConnectNodes();

#endif
