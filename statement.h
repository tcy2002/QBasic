#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>

#include "exp.h"

/*
 * Type: StatementType
 * --------------------
 * This enumerated type is used to differentiate the 7 different
 * statement types: REM, LET, PRINT, INPUT, IF, GOTO, END.
 */

enum StatementType { REM, LET, PRINT, INPUT, GOTO, IFTHEN, END };

/*
 * Class: Statement
 * -----------------
 * This class is used to represent one command type.
 * Statement itself is an abstract class.  Every Statement object
 * is therefore created using one of the 7 concrete subclasses:
 * RemStmt, LetStmt, PrintStmt, InputStmt, GotoStmt, IfStmt and EndStmt.
 */

class Statement {

public:

    Statement() {}
    virtual ~Statement() {}
    virtual std::string toString() = 0;
    virtual std::string toTree() = 0;
    virtual StatementType type() = 0;

/* Getter methods for convenience */

    virtual std::string getContent() {return "";}
    virtual std::string getIdentifierName() {return "";}
    virtual Expression * getExpression() {return nullptr;}
    virtual int getLineNumber() {return -1;}
    virtual std::string getOperator() {return "";}
    virtual Expression * getExpression1() {return nullptr;}

};

/*
 * Class: RemStmt
 * ------------------
 * This subclass represents a REM statement.
 */

class RemStmt: public Statement {

public:

    RemStmt(std::string content);
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

    virtual std::string getContent() override;

private:

    std::string content;

};

/*
 * Class: LetStmt
 * ------------------
 * This subclass represents a LET statement.
 */

class LetStmt: public Statement {

public:

    LetStmt(std::string name, Expression *exp);
    ~LetStmt();
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

    virtual std::string getIdentifierName() override;
    virtual Expression * getExpression() override;

private:

    std::string name;
    Expression *exp;

};

/*
 * Class: PrintStmt
 * ------------------
 * This subclass represents a PRINT statement.
 */

class PrintStmt: public Statement {

public:

    PrintStmt(Expression *exp);
    ~PrintStmt();
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

    virtual Expression * getExpression() override;

private:

    Expression *exp;

};

/*
 * Class: InputStmt
 * ------------------
 * This subclass represents a INPUT statement.
 */

class InputStmt: public Statement {

public:

    // whether this variable is set
    bool done;

    InputStmt(std::string name);
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

    virtual std::string getIdentifierName() override;

private:

    std::string name;

};

/*
 * Class: GotoStmt
 * ------------------
 * This subclass represents a GOTO statement.
 */

class GotoStmt: public Statement {

public:

    GotoStmt(int number);
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

    virtual int getLineNumber() override;

private:

    int number;

};

/*
 * Class: IfStmt
 * ------------------
 * This subclass represents a IFTHEN statement.
 */

class IfStmt: public Statement {

public:

    IfStmt(Expression *exp, std::string op, Expression *exp1, int number);
    ~IfStmt();
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

    virtual Expression * getExpression() override;
    virtual std::string getOperator() override;
    virtual Expression * getExpression1() override;
    virtual int getLineNumber() override;

private:

    Expression *exp;
    std::string op;
    Expression *exp1;
    int number;

};

/*
 * Class: EndStmt
 * ------------------
 * This subclass represents a END statement.
 */

class EndStmt: public Statement {

public:

    EndStmt() {}
    static std::string usage();

    virtual std::string toString() override;
    virtual std::string toTree() override;
    virtual StatementType type() override;

};

#endif // STATEMENT_H
