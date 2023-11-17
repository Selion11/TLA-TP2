#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
typedef struct {
	int value;
} Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/

typedef enum{
	CREATION_AC,
	UNION_AC
} ActionType;

typedef enum {
	COLOR_PROP,
	BORDER_PROP,
	BACKGROUND_PROP,
	TEXT_PROP
} PropertyType;

typedef enum{
	STRONG_LINE,
	DOTTED_LINE,
	WIGGLY_LINE,
	UNSPECIFIED_LINE
} LineType;

typedef enum {
	ARROW_UNION,
	DOUBLE_ARROW_UNION,
	LOOP_UNION
} UnionType;

typedef struct {
	PropertyType propertyType;
	char * description;
} Property;

typedef struct{
	Property * property;
} PropertyList;

typedef struct{
	PropertyList * propertyList;
} NodeProperties;

typedef struct {
	char * name;
	NodeProperties * nodeProperties;
} CreateNode;

typedef struct{
	UnionType unionType;
	char * node_to;
	char * node_from;
	LineType lineType;
} ConnectNodes;

typedef union{
	ActionType actionType;
	ConnectNodes * connectNodes;
	CreateNode * createNode;
} Statement;

typedef struct {
	Statement * statement;
} StatementList;

typedef struct {
	StatementList * statementList;
} Program;

#endif
