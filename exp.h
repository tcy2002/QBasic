/*
 * File: exp.h
 * -----------
 * This interface defines a class hierarchy for arithmetic expressions.
 */

#ifndef _exp_h
#define _exp_h

#include <string>
#include <map>

/* Forward reference */

class EvaluationContext;

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression itself is an abstract class.  Every Expression object
 * is therefore created using one of the three concrete subclasses:
 * ConstantExp, IdentifierExp, or CompoundExp.
 */

class Expression {

public:

   Expression() {}
   virtual ~Expression() {}
   virtual int eval(EvaluationContext & context) = 0;
   virtual std::string toString() = 0;
   virtual std::string toTree(int level) = 0;
   virtual ExpressionType type() = 0;

   /* Getter methods for convenience */
   virtual int getConstantValue() {return 0;}
   virtual std::string getIdentifierName() {return "";}
   virtual std::string getOperator() {return "";}
   virtual Expression *getLHS() {return nullptr;}
   virtual Expression *getRHS() {return nullptr;}

};

/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp: public Expression {

public:

   ConstantExp(int val);

   virtual int eval(EvaluationContext & context) override;
   virtual std::string toString() override;
   virtual std::string toTree(int level) override;
   virtual ExpressionType type() override;

   virtual int getConstantValue() override;

private:

   int value;

};

/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents a expression corresponding to a variable.
 */

class IdentifierExp: public Expression {

public:

   IdentifierExp(std::string name);

   virtual int eval(EvaluationContext & context) override;
   virtual std::string toString() override;
   virtual std::string toTree(int level) override;
   virtual ExpressionType type() override;

   virtual std::string getIdentifierName() override;

private:

   std::string name;

};

/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression.
 */

class CompoundExp: public Expression {

public:

   CompoundExp(std::string op, Expression *lhs, Expression *rhs);
   virtual ~CompoundExp();

   virtual int eval(EvaluationContext & context) override;
   virtual std::string toString() override;
   virtual std::string toTree(int level) override;
   virtual ExpressionType type() override;

   virtual std::string getOperator() override;
   virtual Expression *getLHS() override;
   virtual Expression *getRHS() override;

private:

   std::string op;
   Expression *lhs, *rhs;

};

/*
 * Class: EvaluationContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 */

class EvaluationContext {

public:

   void setValue(std::string var, int value);
   int getValue(std::string var);
   bool isDefined(std::string var);

private:

   std::map<std::string, int> symbolTable;

};

#endif
