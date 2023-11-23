#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

typedef enum{
	CREATION_AC,
	UNION_AC
} ActionType;

typedef enum {
	COLOR_PROP,
	BORDER_PROP,
	BACKGROUND_PROP,
	TEXT_PROP,
	UNION_PROP
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

typedef enum {
	PINK_COLOR,
	RED_COLOR,
	GREEN_COLOR,
	BLUE_COLOR,
	PURPLE_COLOR
} ColorType;

typedef struct Property {
    PropertyType propertyType;
    union {
        char * text;
        ColorType colorType;
        struct {
            char *nodeName;
            LineType lineType;
        } unionProp;
    } value;
    struct Property *nextProperty;
} Property;


typedef struct{
	int size;
	Property * firstProperty;
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
	char * nodeTo;
	char * nodeFrom;
	LineType lineType;
} ConnectNodes;

typedef struct Statement{
	ActionType actionType;
	ConnectNodes * connectNodes;
	CreateNode * createNode;
	struct Statement * nextStatement;
} Statement;

typedef struct {
	int size;
	Statement * firstStatement;
} StatementList;

typedef struct {
	StatementList * statementList;
} Program;

#endif
