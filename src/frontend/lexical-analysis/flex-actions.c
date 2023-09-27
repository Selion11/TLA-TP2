#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

token CreatePatternAction(char * lexeme) {
	LogDebug("[Flex] CreatePatternAction: '%s'.", lexeme);
	yylval.token = CREATE;
	return CREATE;
}

token NodePatternAction(char * lexeme){
	LogDebug("[Flex] NodePatternAction: '%s'.", lexeme);
	yylval.token = NODE;
	return NODE;
}

token TextPatternAction(char * lexeme){
	LogDebug("[Flex] TextPatternAction: '%s'.", lexeme);
	yylval.token = TEXT;
	return TEXT;
}

token UnionPatternAction(char * lexeme){
	LogDebug("[Flex] UnionPatternAction: '%s'.", lexeme);
	yylval.token = UNION;
	return UNION;
}

token WithPatternAction(char * lexeme){
	LogDebug("[Flex] WithPatternAction: '%s'.", lexeme);
	yylval.token = WITH;
	return WITH;
}

token BackgroundPatternAction(char * lexeme){
	LogDebug("[Flex] BackgroundPatternAction: '%s'.", lexeme);
	yylval.token = BACKGROUND;
	return BACKGROUND;
}

token BorderPatternAction(char * lexeme){
	LogDebug("[Flex] BorderPatternAction: '%s'.", lexeme);
	yylval.token = BORDER;
	return BORDER;
}

token ColorPatternAction(char * lexeme){
	LogDebug("[Flex] ColorPatternAction: '%s'.", lexeme);
	yylval.token = COLOR;
	return COLOR;
}

token RedPatternAction(char * lexeme){
	LogDebug("[Flex] RedPatternAction: '%s'.", lexeme);
	yylval.token = RED;
	return RED;
}

token GreenPatternAction(char * lexeme){
	LogDebug("[Flex] GreenPatternAction: '%s'.", lexeme);
	yylval.token = GREEN;
	return GREEN;
}

token BluePatternAction(char * lexeme){
	LogDebug("[Flex] BluePatternAction: '%s'.", lexeme);
	yylval.token = BLUE;
	return BLUE;
}

token PurplePatternAction(char * lexeme){
	LogDebug("[Flex] PurplePatternAction: '%s'.", lexeme);
	yylval.token = PURPLE;
	return PURPLE;
}

token PinkPatternAction(char * lexeme){
	LogDebug("[Flex] PinkPatternAction: '%s'.", lexeme);
	yylval.token = PINK;
	return PINK;
}

token SemmicolonPatternAction(char * lexeme){
	LogDebug("[Flex] SemmicolonPatternAction: '%s'.", lexeme);
	yylval.token = SEMMICOLON;
	return SEMMICOLON;
}

token ArrowPatternAction(char * lexeme){
	LogDebug("[Flex] ArrowPatternAction: '%s'.", lexeme);
	yylval.token = ARROW;
	return ARROW;
}

token CommaPatternAction(char * lexeme){
	LogDebug("[Flex] CommaPatternAction: '%s'.", lexeme);
	yylval.token = COMMA;
	return COMMA;
}

token CloseParenthesisPatternAction(char * lexeme) {
	LogDebug("[Flex] CloseParenthesisPatternAction: '%s'.", lexeme);
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token OpenParenthesisPatternAction(char * lexeme) {
	LogDebug("[Flex] OpenParenthesisPatternAction: '%s'.", lexeme);
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}


token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}
