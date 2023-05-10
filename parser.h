#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "tokenizer.h"
#include "exp.h"
#include "statement.h"

/*
 * Class: ExpParser
 * -----------------
 * This class is a parser for expressions.
 */

class ExpParser {

public:

    /* some tools for parsing symbols */

    // decide whether a char is a letter or a digit
    inline static bool isLetter(char c);
    inline static bool isDigit(char c);

    // decide whether a token is a constant number,
    // a variable or an operator
    inline static bool isNumber(token &t);
    inline static bool isName(token &t);
    inline static bool isOperator(token &t);

    // decide an operator's precedence level
    inline static bool isLever1(token &t);
    inline static bool isLever2(token &t);
    inline static bool isLever3(token &t);

    // handle explicit signed numbers
    static void preHandle(std::vector<token> &tokens);

public:

    ExpParser(std::vector<token> tokens);

    Expression *expression;

};

/*
 * Class: StmtParser
 * -----------------
 * This class is a parser for statements.
 * Need to use ExpParser if it has child expressions.
 */

class StmtParser {

public:

    // decide whether a token is an comparation operator: <, > and =
    inline static bool isComparator(token &t);

public:

    StmtParser(std::vector<token> tokens);

    Statement *statement;

};

/*
 * Class: ParseException
 * -----------------
 * This is a customized exception class used to
 * handle errors during parsing.
 */

class ParseException {

private:

    std::string err;

public:

    ParseException(std::string err);
    std::string what() throw();

};

#endif // PARSER_H
