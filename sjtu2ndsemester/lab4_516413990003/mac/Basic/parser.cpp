/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"

using namespace std;


/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */



/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      error("parseExp: Found extra token: " + scanner.nextToken());
   }
   return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, a identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stoi(token));
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses in expression");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}

Statement * parseStatement (TokenScanner & scanner) {
	string token = scanner.nextToken();
	if (token=="REM") return new RemStmt (scanner);
	else if (token=="PRINT") return new PrintStmt (scanner);
	else if (token=="INPUT") return new InputStmt (scanner);
	else if (token=="LET")	return new LetStmt(scanner);
	else if (token=="GOTO") return new GotoStmt(scanner);
	else if (token=="IF") return new IfStmt(scanner);
	else if (token=="END") return new EndStmt(scanner);

}
