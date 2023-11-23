%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Patrones reutilizables. */
whitespace		[ \n\r\t]

/* Regex para cadenas entre comillas dobles. */
string_literal  ["]([^"\\\n]|\\(.|\n))*["]

name            [A-Za-z][A-Za-z0-9_]*

%%

"CREATE"                                    { return CreatePatternAction(yytext); }
"NODE"                                      { return NodePatternAction(yytext); }
"TEXT"                                      { return TextPatternAction(yytext); }
"UNION"                                     { return UnionPatternAction(yytext); }
"WITH"                                      { return WithPatternAction(yytext); }
"BACKGROUND"                                { return BackgroundPatternAction(yytext); }
"BORDER"                                    { return BorderPatternAction(yytext); }
"COLOR"                                     { return ColorPatternAction(yytext); }
"RED"                                       { return RedPatternAction(yytext); }
"GREEN"                                     { return GreenPatternAction(yytext); }
"BLUE"                                      { return BluePatternAction(yytext); }
"PURPLE"                                    { return PurplePatternAction(yytext); }
"PINK"                                      { return PinkPatternAction(yytext); }
";"                                         { return SemmicolonPatternAction(yytext); }
"->"                                        { return ArrowPatternAction(yytext); }
","                                         { return CommaPatternAction(yytext); }
"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }
"LINE"                                      { return LinePatternActions(yytext); }
"@"                                         { return LoopPatternActions(yytext); }
"<->"                                       { return DoubleUnionPatternActions(yytext); }
"STRONG"                                    { return LineTypesPatternActions(yytext); }
"DOTTED"                                    { return LineTypesPatternActions(yytext); }
"WIGGLY"                                    { return LineTypesPatternActions(yytext); }

{whitespace}     							{ IgnoredPatternAction(yytext, yyleng); }



{string_literal}                            { return StringPatternAction(yytext, yyleng); }


{name}                                      { if (strcmp(yytext, "node") != 0) return NamePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%