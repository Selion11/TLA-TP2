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
void generateStatementList(StatementList * statementList);
void generateCreateNode(CreateNode * createNode);
void generateNodeProperties(NodeProperties * nodeProperties, CreateNode * createNode);
void generatePropertyList(PropertyList * propertyList, CreateNode * createNode);
void generateConnectNodes(ConnectNodes * connectNodes);
void generateLineType(LineType lineType);
void generateUnionType(UnionType unionType, ConnectNodes * connectNodes);
void generatePopConnectNodes();

#endif
