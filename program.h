#ifndef PROGRAM_H
#define PROGRAM_H

#include <map>

#include "exp.h"
#include "statement.h"

/*
 * Type: ProgramState
 * -----------------
 * This enumerated type is used to describe different
 * states of this program.
 */

enum ProgramState { BEGIN, RUNNING, INPUTTING };

/*
 * Class: Program
 * -----------------
 * This class is a background program, which maintains
 * the states and order of statements.
 */

class Program {

private:

    std::map<int, Statement *> stmts;
    EvaluationContext context;

    /* program counter: current line number that is under execution */
    int pc;

public:

    Program();
    ~Program();
    std::string toString();
    std::string toTree();

    /* programing method */

    // insert a new statement
    void insert(int line, Statement *stmt);
    // directly execute a statement
    ProgramState step(std::string &out, Statement *stmt);
    // execute a statement forward
    ProgramState step(std::string &out, bool skip = false);

    /* set the value of a variable directly or during runtime */
    void setVariable(std::string name, int val);

};

/*
 * Class: RuntimeException
 * -----------------
 * This is a customized exception class used to
 * handle errors during runtime.
 */

class RuntimeException {

private:

    std::string err;

public:

    RuntimeException(std::string err);
    std::string what() throw();

};

#endif // PROGRAM_H
