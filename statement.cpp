#include "statement.h"

#include <iostream>
#include <sstream>

RemStmt::RemStmt(std::string content):
    content(content) {

}

std::string RemStmt::toString() {
    return "REM " + content;
}

std::string RemStmt::toTree() {
    return "REM\n    " + content + "\n";
}

StatementType RemStmt::type() {
    return REM;
}

std::string RemStmt::usage() {
    return "usage: REM [content]";
}

std::string RemStmt::getContent() {
    return content;
}

LetStmt::LetStmt(std::string name, Expression *exp):
    name(name),
    exp(exp) {

}

LetStmt::~LetStmt() {
    delete exp;
}

std::string LetStmt::toString() {
    std::string exp_str = exp ? exp->toString() : "";
    return "LET " + name + " = " + exp_str;
}

std::string LetStmt::toTree() {
    std::string exp_tree = exp ? exp->toTree(1) : "\n";
    return "LET =\n    " + name + "\n" + exp_tree;
}

std::string LetStmt::usage() {
    return "usage: LET <varname> = <expression>";
}

StatementType LetStmt::type() {
    return LET;
}

std::string LetStmt::getIdentifierName() {
    return name;
}

Expression *LetStmt::getExpression() {
    return exp;
}

PrintStmt::PrintStmt(Expression *exp):
    exp(exp) {

}

PrintStmt::~PrintStmt() {
    delete exp;
}

std::string PrintStmt::toString() {
    std::string exp_str = exp ? exp->toString() : "";
    return "PRINT " + exp_str;
}

std::string PrintStmt::toTree() {
    std::string exp_tree = exp ? exp->toTree(1) : "\n";
    return "PRINT\n" + exp_tree;
}

std::string PrintStmt::usage() {
    return "usage: PRINT <expression>";
}

StatementType PrintStmt::type() {
    return PRINT;
}

Expression *PrintStmt::getExpression() {
    return exp;
}

InputStmt::InputStmt(std::string name):
    done(false),
    name(name) {

}

std::string InputStmt::toString() {
    return "INPUT " + name;
}

std::string InputStmt::toTree() {
    return "INPUT\n    " + name + "\n";
}

StatementType InputStmt::type() {
    return INPUT;
}

std::string InputStmt::usage() {
    return "usage: INPUT <varname>";
}

std::string InputStmt::getIdentifierName() {
    return name;
}

GotoStmt::GotoStmt(int number):
    number(number) {

}

std::string GotoStmt::toString() {
    std::ostringstream ost;
    ost << "GOTO " << number;
    return ost.str();
}

std::string GotoStmt::toTree() {
    std::ostringstream ost;
    ost << "GOTO\n    " << number << "\n";
    return ost.str();
}

StatementType GotoStmt::type() {
    return GOTO;
}

int GotoStmt::getLineNumber() {
    return number;
}

std::string GotoStmt::usage() {
    return "usage: GOTO <linenumber>";
}

IfStmt::IfStmt(Expression *exp, std::string op, Expression *exp1, int number):
    exp(exp),
    op(op),
    exp1(exp1),
    number(number) {

}

IfStmt::~IfStmt() {
    delete exp;
    delete exp1;
}

std::string IfStmt::toString() {
    std::ostringstream ost;
    std::string exp_str = exp ? exp->toString() : "";
    std::string exp1_str = exp1 ? exp1->toString() : "";
    ost << "IF " << exp_str << " " << op << " " << exp1_str << " THEN " << number;
    return ost.str();
}

std::string IfStmt::toTree() {
    std::string exp_tree = exp ? exp->toTree(1) : "\n";
    std::string exp1_tree = exp1 ? exp1->toTree(1) : "\n";
    std::ostringstream ost;
    ost << "IF THEN\n" << exp_tree << "    " << op << "\n"
        << exp1_tree << "    " << number << "\n";
    return ost.str();
}

std::string IfStmt::usage() {
    return "usage: IF <expression> <operator> <expression> THEN <linenumber>";
}

StatementType IfStmt::type() {
    return IFTHEN;
}

Expression *IfStmt::getExpression() {
    return exp;
}

std::string IfStmt::getOperator() {
    return op;
}

Expression *IfStmt::getExpression1() {
    return exp1;
}

int IfStmt::getLineNumber() {
    return number;
}

std::string EndStmt::toString() {
    return "END";
}

std::string EndStmt::toTree() {
    return "END\n";
}

StatementType EndStmt::type() {
    return END;
}

std::string EndStmt::usage() {
    return "usage: END";
}


