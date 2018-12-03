#include <iostream>
#include <stack>
#include <queue>
#include <sstream>
#include <typeinfo>
#include <map>

#include "tokens.h"
#include "utils.h"

using namespace std;



double executePolishNotation(queue<Token*> tokens)
{
    stack<double> execStack;
    while(!tokens.empty())
    {
        tokens.front()->stackAction(execStack);
        tokens.pop();
    }
    if(execStack.size() == 0)
        throw invalid_argument("Result stack empty!");
    else if(execStack.size() > 1)
        throw invalid_argument("Result stack contains more than one value!");

    return execStack.top();
}

queue<Token*> parseTokenSequence(const string& input, bool handleUnaryOperators = false)
{
    queue<Token*> tokens;
    istringstream inputStream(input);
    bool expressionStart = true;

    while(!inputStream.eof())
    {
        char peekChar = inputStream.peek();
        switch(peekChar)
        {
            case '+':
                if(handleUnaryOperators && expressionStart)
					tokens.push(new UnaryOperatorToken(UnaryOperator::Positive));
				else
					tokens.push(new BinaryOperatorToken(BinaryOperator::Add));

                inputStream.ignore();
				expressionStart = false;
                break;
            case '-':
                if(handleUnaryOperators && expressionStart)
					tokens.push(new UnaryOperatorToken(UnaryOperator::Negative));
				else
					tokens.push(new BinaryOperatorToken(BinaryOperator::Subtract));

                inputStream.ignore();

				expressionStart = false;
                break;
            case '*':
                tokens.push(new BinaryOperatorToken(BinaryOperator::Multiply));
                inputStream.ignore();

            	expressionStart = true;
                break;
            case '/':
                tokens.push(new BinaryOperatorToken(BinaryOperator::Divide));
                inputStream.ignore();

            	expressionStart = true;
                break;
            case '^':
                tokens.push(new BinaryOperatorToken(BinaryOperator::Power));
                inputStream.ignore();

            	expressionStart = true;
                break;
			case 'n':
				tokens.push(new UnaryOperatorToken(UnaryOperator::Negative));
				inputStream.ignore();

            	expressionStart = false;
				break;
			case '(':
				tokens.push(new MiscToken(MiscTokenType::LeftParenthesis));
				inputStream.ignore();

				expressionStart = true;
				break;
			case ')':
				tokens.push(new MiscToken(MiscTokenType::RightParenthesis));
				inputStream.ignore();

				expressionStart = false;
				break;
        }
        if(isdigit(peekChar))
        {
            int num;
            inputStream >> num;
            tokens.push(new NumberToken(num));
            expressionStart = false;
        }
        else if(iswspace(peekChar))
        {
            do
            {
                inputStream.ignore();
            }
            while(iswspace(peekChar = inputStream.peek()));
        }
    }

    return tokens;
}

queue<Token*> convertNotationInfixToPolish(queue<Token*> infixTokens)
{
    queue<Token*> polishTokens;
    stack<OperatorToken*> operators;
    while(!infixTokens.empty())
    {
		Token* nextToken = infixTokens.front();
//		cout << *nextToken << ' ';
		infixTokens.pop();

		if(NumberToken* numToken = dynamic_cast<NumberToken*>(nextToken))
		{
			polishTokens.push(numToken);
		}
		else if(UnaryOperatorToken* operatorToken = dynamic_cast<UnaryOperatorToken*>(nextToken))
		{
			operators.push(operatorToken);
		}
		else if(BinaryOperatorToken* operatorToken = dynamic_cast<BinaryOperatorToken*>(nextToken))
		{
			while(!operators.empty() && (operatorToken->getPrecedence() < operators.top()->getPrecedence() ||
					(operatorToken->getPrecedence() == operators.top()->getPrecedence() &&
						operatorToken->getAssociativity() == Associativity::Left)))
			{
				polishTokens.push(operators.top());
				operators.pop();
			}
			operators.push(operatorToken);
		}
		else if(MiscToken* miscToken = dynamic_cast<MiscToken*>(nextToken))
		{
			if(miscToken->type == MiscTokenType::LeftParenthesis)
			{
				operators.push(miscToken);
			}
			else //right parenthesis
			{
				while(!operators.empty())
				{
					if(MiscToken* miscToken2 = dynamic_cast<MiscToken*>(operators.top()))
					{
                        if(miscToken2->type == MiscTokenType::LeftParenthesis)
							break;
					}

					Token* operatorToken = safePop(operators);
					polishTokens.push(operatorToken);
				}

				if(operators.empty())
					throw invalid_argument("Mismatched parenthesis!");
				operators.pop();
			}
		}
    }

    while(!operators.empty())
	{
		OperatorToken* operatorToken = operators.top();
		if(MiscToken* miscToken = dynamic_cast<MiscToken*>(operatorToken))
		{
			throw invalid_argument("Mismatched parenthesis!");
		}
		polishTokens.push(operatorToken);
		operators.pop();
	}
	return polishTokens;
}

int main()
{
//    queue<Token*> tokenStack;
//    tokenStack.push(new NumberToken(50));
//    tokenStack.push(new NumberToken(10));
//    tokenStack.push(new MathOperatorToken(MathOperator::Divide));
//    cout << executePolishNotation(tokenStack) << endl;

//    string test = "4 1 n ^";
//    cout << executePolishNotation(parseTokenSequence(test)) << endl;

    string test2 = "-1 + (5+3) / (2 ^-1)   -1";
    cout << test2 << endl << endl;

    queue<Token*> sequence = parseTokenSequence(test2, true);
    print(sequence);
    cout << endl << endl;

	sequence = convertNotationInfixToPolish(sequence);
	print(sequence);
	cout << endl << endl;

    cout << executePolishNotation(sequence) << endl;

	return EXIT_SUCCESS;
}
