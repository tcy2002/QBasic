#include "program.h"

#include <iostream>
#include <sstream>

Program::Program():
    pc(0) {

}

Program::~Program() {
    for (auto &stmt : stmts)
        delete stmt.second;
}

std::string Program::toString() {
    std::ostringstream ost;
    for (auto &stmt : stmts)
        ost << (pc == stmt.first ? "*" : " ") << stmt.first << " " << stmt.second->toString() << "\n";
    return ost.str();
}

std::string Program::toTree() {
    std::ostringstream ost;
    for (auto &stmt : stmts)
        ost << (pc == stmt.first ? "*" : " ") << stmt.first << " " << stmt.second->toTree();
    return ost.str();
}

void Program::insert(int line, Statement *stmt) {
    if (stmts.count(line) != 0) // remove the old line at first
        stmts.erase(line);
    if (stmt != nullptr) // insert a new line if needed
        stmts.emplace(line, stmt);
    if (!stmts.empty()) // next line to be executed
        pc = stmts.begin()->first;
}

ProgramState Program::step(std::string &out, Statement *stmt) {
    out.clear();
    std::ostringstream ost;

    switch (stmt->type()) {
    case LET:
        context.setValue(stmt->getIdentifierName(), stmt->getExpression()->eval(context));
        return RUNNING;
    case PRINT:
        ost << stmt->getExpression()->eval(context);
        out.assign(ost.str());
        return RUNNING;
    case INPUT:
        context.setValue(stmt->getIdentifierName(), 0);
        out.assign(stmt->getIdentifierName());
        return INPUTTING;
    default:
        throw RuntimeException("unknown statement");
    }
}

ProgramState Program::step(std::string &out, bool skip) {
    if (stmts.empty())
        return BEGIN;

    if (!skip) {
        out.clear();
        std::ostringstream ost;
        std::string op;
        int val, val1;
        auto stmt = stmts.find(pc);

        switch (stmt->second->type()) {
        case REM:
            break;
        case LET:
            context.setValue(stmt->second->getIdentifierName(),
                             stmt->second->getExpression()->eval(context));
            break;
        case PRINT:
            ost << stmt->second->getExpression()->eval(context);
            out.assign(ost.str());
            break;
        case INPUT:
            out.assign(stmt->second->getIdentifierName());
            return INPUTTING;
        case IFTHEN:
            op = stmt->second->getOperator();
            val = stmt->second->getExpression()->eval(context);
            val1 = stmt->second->getExpression1()->eval(context);
            if ((op == "<" && val < val1) ||
                    (op == ">" && val > val1) ||
                    (op == "=" && val == val1)) {
                stmt = stmts.find(stmt->second->getLineNumber());
                if (stmt == stmts.end())
                    throw RuntimeException("no matching line number");
                pc = stmt->first;
                return RUNNING;
            }
            break;
        case GOTO:
            stmt = stmts.find(stmt->second->getLineNumber());
            if (stmt == stmts.end())
                throw RuntimeException("no matching line number");
            pc = stmt->first;
            return RUNNING;
        case END:
            pc = stmts.begin()->first;
            return BEGIN;
        default:
            throw RuntimeException("unknown statement");
        }
    }

    if (pc == stmts.rbegin()->first) {
        pc = stmts.begin()->first;
        return BEGIN;
    }
    pc = (++stmts.find(pc))->first;
    return RUNNING;
}

void Program::setVariable(std::string name, int val) {
    context.setValue(name, val);
}

RuntimeException::RuntimeException(std::string err):
    err(err) {

}

std::string RuntimeException::what() throw() {
    return "runtime error: " + err;
}
