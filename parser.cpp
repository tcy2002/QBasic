#include "parser.h"

#include <stack>
#include <sstream>
#include <iostream>

bool ExpParser::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '&';
}

bool ExpParser::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool ExpParser::isNumber(token &t) {
    if (t.empty() || t == "+" || t == "-")
        return false;
    if (!isDigit(t[0]) && t[0] != '+' && t[0] != '-')
        return false;
    for (auto c = t.begin() + 1; c != t.end(); c++)
        if (!isDigit(*c))
            return false;
    return true;
}

bool ExpParser::isName(token &t) {
    if (t.empty())
        return false;
    if (!isLetter(t[0]))
        return false;
    for (auto c = (t.begin() + 1); c < t.end(); c++)
        if (!isLetter(*c) && !isDigit(*c))
            return false;
    return true;
}

bool ExpParser::isOperator(token &t) {
    return t == "+" || t == "-" || t == "*" || t == "/"
            || t == "**" || t == "(" || t == ")";
}

bool ExpParser::isLever1(token &t) {
    return t == "+" || t == "-";
}

bool ExpParser::isLever2(token &t) {
    return t == "*" || t == "/";
}

bool ExpParser::isLever3(token &t) {
    return t == "**";
}

void ExpParser::preHandle(std::vector<token> &tokens) {
    int size = tokens.size();
    for (int i = 0; i < size; i++) {
        // illegal case: ()
        if (i < size - 1 && tokens[i] == "(" && tokens[i + 1] == ")")
            throw ParseException("illegal expression");
        // legal cases: -1+2 ; 1-(-2)
        if ((tokens[i] == "+" || tokens[i] == "-") &&
                (i == 0 || tokens[i - 1] == "(") &&
                (i < size - 1 && (isNumber(tokens[i + 1]) || isName(tokens[i + 1])))) {
            tokens.insert(tokens.begin() + i, "0"); // add constant 0 ahead
            i++;
        }
    }
}

ExpParser::ExpParser(std::vector<token> tokens):
    expression(nullptr) {
    preHandle(tokens);

    std::stack<std::string> operators;
    std::stack<Expression *> operands;

    std::string op;
    Expression *lhs, *rhs;

#define MERGE {\
    op = operators.top(); \
    if (op == "(") \
        throw ParseException("expected \")\" to match \"(\""); \
    operators.pop(); \
    if (operands.empty()) \
        throw ParseException("incomplete expression"); \
    rhs = operands.top(); \
    operands.pop(); \
    if (operands.empty()) \
        throw ParseException("incomplete expression"); \
    lhs = operands.top(); \
    operands.pop(); \
    operands.push(new CompoundExp(op, lhs, rhs)); \
}

    // push stack
    for (auto &t :tokens) {
        if (isNumber(t)) {
            int n = 0;
            std::istringstream ist(t);
            ist >> n;
            operands.push(new ConstantExp(n));
        } else if (isName(t)) {
            operands.push(new IdentifierExp(t));
        } else if (isOperator(t)) {
            if (t == "(" || isLever3(t)) {
                operators.push(t);
            } else if (isLever1(t)) {
                while (!operators.empty() && operators.top() != "(")
                    MERGE;
                operators.push(t);
            } else if (isLever2(t)) {
                while (!operators.empty() && operators.top() != "(" && !isLever1(operators.top()))
                    MERGE;
                operators.push(t);
            } else if (t == ")") {
                while (!operators.empty() && operators.top() != "(")
                    MERGE;
                if (operators.empty())
                    throw ParseException("expected \"(\" to match \")\"");
                operators.pop();
            }
        } else {
            throw ParseException("illegal expression");
        }
    }

    // pop stack
    while (!operators.empty())
        MERGE;

    // to avoid cases: (1)(2) ; 1+(2)3
    if (operands.size() != 1)
        throw ParseException("illegal expression");
    expression = operands.top();
}

bool StmtParser::isComparator(token &t) {
    return t == "=" || t == "<" || t == ">";
}

StmtParser::StmtParser(std::vector<token> tokens):
    statement(nullptr) {
    if (tokens.empty())
        return;

    // parsing to different statements
    if (tokens[0] == "REM") {
        std::string content;
        for (auto t = (tokens.begin() + 1); t < tokens.end(); t++)
            content += (*t + " ");
        statement = new RemStmt(content);
    } else if (tokens[0] == "LET") {
        if (tokens.size() < 4 || tokens[2] != "=")
            throw ParseException("incomplete statement, " + LetStmt::usage());
        if (!ExpParser::isName(tokens[1]))
            throw ParseException("illegal variable name");
        ExpParser parser({tokens.begin() + 3, tokens.end()});
        statement = new LetStmt(tokens[1], parser.expression);
    } else if (tokens[0] == "PRINT") {
        if (tokens.size() < 2)
            throw ParseException("incomplete statement, " + PrintStmt::usage());
        ExpParser parser({tokens.begin() + 1, tokens.end()});
        statement = new PrintStmt(parser.expression);
    } else if (tokens[0] == "INPUT") {
        if (tokens.size() < 2)
            throw ParseException("incomplete statement, " + InputStmt::usage());
        if (!ExpParser::isName(tokens[1]))
            throw ParseException("illegal variable name");
        statement = new InputStmt(tokens[1]);
    } else if (tokens[0] == "GOTO") {
        if (tokens.size() < 2)
            throw ParseException("incomplete statement, " + GotoStmt::usage());
        std::istringstream ist(tokens[1]);
        int n = 0;
        ist >> n;
        if (ist.fail() || n <= 0)
            throw ParseException("illegal line number");
        statement = new GotoStmt(n);
    } else if (tokens[0] == "IF") {
        if (tokens.size() < 6)
            throw ParseException("incomplete statement, " + IfStmt::usage());
        int op_i = 0, then_i = 0, size = tokens.size();
        for (int i = 1; i < size; i++) {
            if (op_i == 0 && isComparator(tokens[i]))
                op_i = i;
            if (then_i == 0 && tokens[i] == "THEN")
                then_i = i;
        }
        if (op_i == 0 || then_i == 0)
            throw ParseException("incomplete statement, " + IfStmt::usage());
        ExpParser parser({tokens.begin() + 1, tokens.begin() + op_i});
        ExpParser parser1({tokens.begin() + (op_i + 1), tokens.begin() + then_i});
        std::istringstream ist(tokens[then_i + 1]);
        int n = 0;
        ist >> n;
        if (ist.fail())
            throw ParseException("illegal line number");
        statement = new IfStmt(parser.expression, tokens[op_i], parser1.expression, n);
    } else if (tokens[0] == "END") {
        statement = new EndStmt();
    } else {
        throw ParseException("illegal statement");
    }
}

ParseException::ParseException(std::string err):
    err(err) {

}

std::string ParseException::what() throw() {
    return "parse error: " + err;
}
