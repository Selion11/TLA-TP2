#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include "../../backend/semantic-analysis/symbols.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
Program * ProgramGrammarAction(StatementList * statementList);

StatementList * SimpleStatementAction(Statement * statement);
StatementList * RightAppendStatementAction(StatementList * statementList, Statement * statement);

Statement * ConnectNodesStatementAction(ConnectNodes * connectNodes);
Statement * CreateNodeStatementAction(CreateNode * createNode);

CreateNode * CreateNodeAction(char * name, NodeProperties * nodeProperties);

NodeProperties * EmptyNodePropertiesAction();
NodeProperties * SetPropertyListAction(PropertyList * propertyList);

PropertyList * SimplePropertyAction(Property * property);
PropertyList * RightAppendPropertyAction(PropertyList * propertyList, Property * property);

Property * CustomUnionPropertyAction(char * nodeToName, LineType lineType);
Property * DefaultUnionPropertyAction(char * nodeToName);
Property * NodeTextPropertyAction(char * text);
Property * NodeBackgroundColorPropertyAction(ColorType color);
Property * NodeBorderColorPropertyAction(ColorType color);

ConnectNodes * SimpleShortUnionPropertyAction(char * nodeFromName, char * nodeToName);
ConnectNodes * DoubleShortUnionPropertyAction(char * nodeFromName, char * nodeToName);
ConnectNodes * LoopShortUnionPropertyAction(char * nodeName);
ConnectNodes * CustomSimpleShortUnionPropertyAction(char * nodeFromName, char * nodeToName, LineType lineType);
ConnectNodes * CustomDoubleShortUnionPropertyAction(char * nodeFromName, char * nodeToName, LineType lineType);
ConnectNodes * CustomLoopShortUnionPropertyAction(char * nodeName, LineType lineType);

#endif
