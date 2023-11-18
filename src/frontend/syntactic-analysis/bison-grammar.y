%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {

	// No-terminales (backend).
	Program * program;
	StatementList * statement_list;
    Statement * statement;
    CreateNode * create_node;
    NodeProperties * node_properties;
    PropertyList * property_list;
    Property * property;
    ConnectNodes * connect_nodes;

	// No-terminales (frontend).
	/*int program;
	int statement;
	int statement_list;
	int create_node;
	int node_properties;
	int property_list;
	int property;
	int create_union;
	int connect_nodes;
	*/

	// Terminales.
	token token;
	char * strval;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.

%token <token> CREATE
%token <token> NODE
%token <token> TEXT
%token <token> UNION
%token <token> WITH
%token <token> BACKGROUND
%token <token> BORDER
%token <token> COLOR
%token <token> RED
%token <token> GREEN
%token <token> BLUE
%token <token> PURPLE
%token <token> PINK
%token <token> SEMMICOLON
%token <token> ARROW
%token <token> COMMA
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> LINE
%token <token> LINETYPE
%token <token> LOOP
%token <token> DOUBLE_UNION

%token <strval> NAME
%token <strval> STRING


// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <statement_list> statement_list
%type <statement> statement
%type <create_node> create_node
%type <node_properties> node_properties
%type <property_list> property_list
%type <property> property
%type <connect_nodes> connect_nodes

// El símbolo inicial de la gramatica.
%start program

%%

program:  statement_list	                             					                    {$$ = ProgramGrammarAction($1);}														
		 ;

statement_list: statement SEMMICOLON															{$$ = SimpleStatementAction($1);}
              | statement_list statement SEMMICOLON												{$$ = RightAppendStatementAction($1,$2);}
              ;		

statement: connect_nodes																		{$$ = ConnectNodesStatementAction($1);}
		 | create_node																			{$$ = CreateNodeStatementAction($1);}
         ;

create_node: CREATE NODE NAME node_properties													{$$ = CreateNodeAction($3,$4);} 																
           ;

node_properties: 																				{$$ = EmptyNodePropertiesAction();}
               | OPEN_PARENTHESIS property_list CLOSE_PARENTHESIS								{$$ = SetPropertyListAction($2);}
			   | OPEN_PARENTHESIS CLOSE_PARENTHESIS												{$$ = EmptyNodePropertiesAction();}
               ;

property_list: property																			{$$ = SimplePropertyAction($1);}
             | property_list COMMA property														{$$ = RightAppendPropertyAction($1,$3);}
			 ;
             

property: UNION OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS	LINE LINETYPE							{$$ = CustomUnionPropertyAction($3, $6);}
		| UNION OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS											{$$ = DefaultUnionPropertyAction($3);}
		| TEXT OPEN_PARENTHESIS STRING CLOSE_PARENTHESIS								    	{$$ = NodeTextPropertyAction($3);}
        | BACKGROUND OPEN_PARENTHESIS COLOR	CLOSE_PARENTHESIS									{$$ = NodeBackgroundColorPropertyAction($3);}
        | BORDER COLOR OPEN_PARENTHESIS COLOR CLOSE_PARENTHESIS								    {$$ = NodeBorderColorPropertyAction($4);}			
		;
        

connect_nodes: 	NAME ARROW NAME																	{$$ = SimpleShortUnionPropertyAction($1,$3);}
			| 	NAME DOUBLE_UNION NAME 															{$$ = DoubleShortUnionPropertyAction($1,$3);}
			| 	NAME LOOP 																		{$$ = LoopShortUnionPropertyAction($1);}
			|   NAME ARROW NAME	LINE LINETYPE													{$$ = CustomSimpleShortUnionPropertyAction($1,$3,$5);}
			| 	NAME DOUBLE_UNION NAME LINE LINETYPE 											{$$ = CustomDoubleShortUnionPropertyAction($1,$3,$5);}
			| 	NAME LOOP LINE LINETYPE															{$$ = CustomLoopShortUnionPropertyAction($1,$4);}
			;

%%
