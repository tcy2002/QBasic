#include "exp.h"
#include "program.h"
#include <sstream>
#include <iostream>

ConstantExp::ConstantExp(int val):
    value(val) {

}

int ConstantExp::eval(EvaluationContext &) {
    return value;
}

std::string ConstantExp::toString() {
    std::ostringstream ost;
    ost << value;
    return ost.str();
}

std::string ConstantExp::toTree(int level) {
    std::string prefix(level * 4, ' ');
    return prefix + toString() + "\n";
}

ExpressionType ConstantExp::type() {
    return CONSTANT;
}

int ConstantExp::getConstantValue() {
    return value;
}

IdentifierExp::IdentifierExp(std::string name):
    name(name) {

}

int IdentifierExp::eval(EvaluationContext &context) {
    if (!context.isDefined(name)) {
        throw RuntimeException("`" + name + "` is not declared");
    }
    return context.getValue(name);
}

std::string IdentifierExp::toString() {
    return name;
}

std::string IdentifierExp::toTree(int level) {
    std::string prefix(level * 4, ' ');
    return prefix + name + "\n";
}

ExpressionType IdentifierExp::type() {
    return IDENTIFIER;
}

std::string IdentifierExp::getIdentifierName() {
    return name;
}

CompoundExp::CompoundExp(std::string op, Expression *lhs, Expression *rhs):
    op(op),
    lhs(lhs),
    rhs(rhs) {

}

CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

int CompoundExp::eval(EvaluationContext &context) {
    int right = rhs->eval(context);
    int left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
       if (right == 0)
           throw RuntimeException("divide by zero");
       return left / right;
    }
    if (op == "**") {
        if (right < 0) return 0;
        int ret = 1;
        while (right-- > 0) ret *= left;
        return ret;
    }
    throw RuntimeException("illegal operator in expression");
    return 0;
}

std::string CompoundExp::toString() {
    std::string ret = "(";
    if (lhs)
        ret += lhs->toString();
    ret += " ";
    ret += op;
    ret += " ";
    if (rhs)
        ret += rhs->toString();
    return ret + ")";
}

std::string CompoundExp::toTree(int level) {
    std::string prefix(level * 4, ' ');
    std::string ret = prefix + op + "\n";
    if (lhs)
        ret += lhs->toTree(level + 1);
    if (rhs)
        ret += rhs->toTree(level + 1);
    return ret;
}

ExpressionType CompoundExp::type() {
    return COMPOUND;
}

std::string CompoundExp::getOperator() {
    return op;
}

Expression *CompoundExp::getLHS() {
    return lhs;
}

Expression *CompoundExp::getRHS() {
    return rhs;
}

void EvaluationContext::setValue(std::string var, int value) {
    symbolTable[var] = value;
}

int EvaluationContext::getValue(std::string var) {
    return symbolTable[var];
}

bool EvaluationContext::isDefined(std::string var) {
    return symbolTable.find(var) != symbolTable.end();
}
