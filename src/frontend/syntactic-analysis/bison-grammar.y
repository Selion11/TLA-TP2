%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int statement;
	int create_node;
	int node_properties;
	int property_list;
	int property;
	int create_union;
	int connect_nodes;

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

%token <strval> NAME
%token <strval> STRING


// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <statement> statement
%type <create_node> create_node
%type <node_properties> node_properties
%type <property_list> property_list
%type <property> property
%type <create_union> create_union
%type <connect_nodes> connect_nodes

// El símbolo inicial de la gramatica.
%start program

%%

program:  statement SEMMICOLON														{$$ = ProgramGrammarAction($1);}														
       ;

statement: create_node																		{$$ = 0;}
         | create_union																		{$$ = 0;}
         | connect_nodes																	{$$ = 0;}
         ;

create_node: CREATE NODE NAME node_properties												{$$ = 0;}
			| CREATE NAME 																	{$$ = 0;}
           ;

node_properties: 																			{$$ = 0;}
               | OPEN_PARENTHESIS property_list CLOSE_PARENTHESIS							{$$ = 0;}
               ;

property_list: property																		{$$ = 0;}
             | property_list COMMA property													{$$ = 0;}
			 ;
             

property: UNION OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS										{$$ = 0;}
		| TEXT OPEN_PARENTHESIS STRING CLOSE_PARENTHESIS								    {$$ = 0;}
        | BACKGROUND OPEN_PARENTHESIS COLOR	CLOSE_PARENTHESIS								{$$ = 0;}
        | BORDER OPEN_PARENTHESIS COLOR	CLOSE_PARENTHESIS								    {$$ = 0;}
		| LINE LINETYPE																		{$$ = 0;}
		;
        

create_union: CREATE NODE NAME UNION OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS					{$$ = 0;}
			;


connect_nodes: NAME ARROW NAME																	{$$ = 0;}
			 ;

%%
