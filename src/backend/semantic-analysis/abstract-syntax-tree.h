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
typedef struct {
	Expression * expression;
} Program;
 
typedef struct {
	statement * statements;
} statement_list;

typedef union{
	connect_nodes * connection;
	create_node * creation;
} statement;

typedef struct {
	char * name;
	node_properties * props;
}

typedef union{
	property * prop;
	node_union * uni;
}

typedef enum {
	COLOR,
	BORDER,
	BACKGROUND,
	TEXT
} PropertyType;

typedef struct {
	PropertyType type;
	char * description;
} property;

typedef enum{
	STRONG,
	DOTTED,
	WIGGLY
} LineType;

typedef struct {
	char * name_node_to_unite;
	LineType line;
} node_union;

typedef enum {
	ARROW,
	DOUBLE_ARROW,
	LOOP
} union_types;

typedef struct{
	union_types type;
	char * node_to;
	char * node_from;
	LineType line;
} connect_nodes;




#endif
