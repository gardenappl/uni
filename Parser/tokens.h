#ifndef TOKENS_H
#define TOKENS_H

#include <stack>
#include <iostream>

using namespace std;

class Token
{
public:
	virtual void stackAction(stack<double>& execStack) const = 0;

	friend ostream& operator<<(ostream& os, const Token& token);

protected:
	virtual void print(ostream& os) const = 0; //required for derived classes
};



class NumberToken: public Token
{
public:
    const int value;

    NumberToken(int value);

	void stackAction(stack<double>& execStack) const;
	void print(ostream& os) const;
};

class OperatorToken : public Token
{
public:
	virtual int getPrecedence() const = 0;
};

enum BinaryOperator
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Power
};

enum Associativity
{
	Left,
	Right
};

class BinaryOperatorToken : public OperatorToken
{
public:
    const BinaryOperator operatorType;

    BinaryOperatorToken(BinaryOperator operatorType);

	void stackAction(stack<double>& execStack) const;
	int getPrecedence() const;
	Associativity getAssociativity() const;

protected:
	void print(ostream& os) const;
};

enum UnaryOperator
{
	Negative,
	Positive
};

class UnaryOperatorToken : public OperatorToken
{
public:
	const UnaryOperator operatorType;

	UnaryOperatorToken(UnaryOperator operatorType);

	void stackAction(stack<double>& execStack) const;
	int getPrecedence() const;

protected:
	void print(ostream& os) const;
};

enum MiscTokenType
{
	LeftParenthesis,
	RightParenthesis
};

class MiscToken : public OperatorToken
{
public:
	const MiscTokenType type;

	MiscToken(MiscTokenType type);

	void stackAction(stack<double>& execStack) const { /* no-op */ }
	int getPrecedence() const { return 0; }

protected:
	void print(ostream& os) const;
};


#endif // TOKENS_H
