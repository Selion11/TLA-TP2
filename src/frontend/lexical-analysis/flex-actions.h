#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Patrones terminales del lenguaje diseñado.
token CreatePatternAction(char * lexeme);
token NodePatternAction(char * lexeme);
token TextPatternAction(char * lexeme);
token UnionPatternAction(char * lexeme);
token WithPatternAction(char * lexeme);
token BackgroundPatternAction(char * lexeme);
token BorderPatternAction(char * lexeme);
token ColorPatternAction(char * lexeme);
token RedPatternAction(char * lexeme);
token GreenPatternAction(char * lexeme);
token BluePatternAction(char * lexeme);
token PurplePatternAction(char * lexeme);
token PinkPatternAction(char * lexeme);
token SemmicolonPatternAction(char * lexeme);
token ArrowPatternAction(char * lexeme);
token CommaPatternAction(char * lexeme);
token CloseParenthesisPatternAction(char * lexeme);
token OpenParenthesisPatternAction(char * lexeme);
token NamePatternAction(char * lexeme, const int length);
token StringPatternAction(char * lexeme, const int length);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
