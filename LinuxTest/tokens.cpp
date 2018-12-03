#include "tokens.h"
#include "utils.h"
#include <cmath>
#include <iostream>


ostream& operator<<(ostream& os, const Token& token)
{
    token.print(os);
    return os;
}



NumberToken::NumberToken(int value)
: value(value)
{}

void NumberToken::stackAction(stack<double>& execStack) const
{
    execStack.push((double)value);
}

void NumberToken::print(ostream& os) const
{
	os << value;
}



BinaryOperatorToken::BinaryOperatorToken(BinaryOperator operatorType)
: operatorType(operatorType)
{}

void BinaryOperatorToken::stackAction(stack<double>& execStack) const
{
	double num2 = safePop(execStack);
	double num1 = safePop(execStack);

	switch(this->operatorType)
	{
	case BinaryOperator::Add:
		execStack.push(num1 + num2);
		break;
	case BinaryOperator::Subtract:
		execStack.push(num1 - num2);
		break;
	case BinaryOperator::Multiply:
		execStack.push(num1 * num2);
		break;
	case BinaryOperator::Divide:
		execStack.push(num1 / num2);
		break;
	case BinaryOperator::Power:
		execStack.push(pow(num1, num2));
		break;
	}
}

int BinaryOperatorToken::getPrecedence() const
{
	switch(operatorType)
	{
	case BinaryOperator::Add:
	case BinaryOperator::Subtract:
		return 3;
	case BinaryOperator::Multiply:
	case BinaryOperator::Divide:
		return 4;
	case BinaryOperator::Power:
		return 5;
	}
	return -1;
}

Associativity BinaryOperatorToken::getAssociativity() const
{
	switch(operatorType)
	{
	case BinaryOperator::Add:
	case BinaryOperator::Subtract:
	case BinaryOperator::Multiply:
	case BinaryOperator::Divide:
		return Associativity::Left;
	default:
		return Associativity::Right;
	}
}

void BinaryOperatorToken::print(ostream& os) const
{
	switch(operatorType)
	{
	case BinaryOperator::Add:
		os << '+';
		return;
	case BinaryOperator::Subtract:
		os << '-';
		return;
	case BinaryOperator::Multiply:
		os << '*';
		return;
	case BinaryOperator::Divide:
		os << '/';
		return;
	default:
		os << '^';
		return;
	}
}



UnaryOperatorToken::UnaryOperatorToken(UnaryOperator operatorType)
: operatorType(operatorType)
{}

void UnaryOperatorToken::stackAction(stack<double>& execStack) const
{
	double num = safePop(execStack);
	if(operatorType == UnaryOperator::Negative)
		execStack.push(-num);
}

int UnaryOperatorToken::getPrecedence() const
{
	return 6;
}

void UnaryOperatorToken::print(ostream& os) const
{
	if(operatorType == UnaryOperator::Negative)
		os << "negate";
	else
		os << "positivate(?)";
}



MiscToken::MiscToken(MiscTokenType type)
: type(type)
{}

void MiscToken::print(ostream& os) const
{
	if(type == MiscTokenType::LeftParenthesis)
		os << '(';
	else
		os << ')';
}
