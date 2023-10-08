%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digit			[0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]


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
"LINE"                                      { return LinePatterActions(yytext); }
"@"                                         { return LoopPatternActions(yytext); }
"<->"                                       { return DoubleUnionPatternActions(yytext); }
"STRONG"                                    { return LineTypesPatterActions(yytext); }
"DOTTED"                                    { return LineTypesPatterActions(yytext); }
"WIGGLY"                                    { return LineTypesPatterActions(yytext); }

[ \t\n]     								{ IgnoredPatternAction(yytext, yyleng); }



["]([^"\\\n]|\\(.|\n))*["]                  { return StringPatternAction(yytext, yyleng); }


[A-Za-z][A-Za-z0-9_]*                       { return NamePatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
