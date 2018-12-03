//#include <iostream>
//#include <stack>
//#include <queue>
//#include <sstream>
//#include <cmath>
//
//using namespace std;
//
//template<typename T>
//T safePop(stack<T>& argStack)
//{
//    if(argStack.empty())
//        throw invalid_argument("Attempted to pop empty stack");
//    T value = argStack.top();
//    argStack.pop();
//
//    return value;
//}
//
//
//class Token
//{
//};
//
//class NumberToken: public Token
//{
//public:
//    const int value = 0;
//
//    NumberToken(int value)
//    : value(value)
//    {}
//
////	friend ostream& operator<<(ostream& os, const NumberToken& token);
//};
//
//enum BinaryOperator
//{
//    Add,
//    Subtract,
//    Multiply,
//    Divide,
//    Power
//};
//
//class BinaryMathOperatorToken : public Token
//{
//public:
//    const BinaryOperator operatorType;
//
//    BinaryMathOperatorToken(BinaryOperator operatorType)
//    : operatorType(operatorType)
//    {}
//
//    void stackAction(stack<double>& execStack)
//    {
//        double num2 = safePop(execStack);
//        double num1 = safePop(execStack);
//
//        switch(this->operatorType)
//        {
//            case BinaryOperator::Add:
//                execStack.push(num1 + num2);
//                break;
//            case BinaryOperator::Subtract:
//                execStack.push(num1 - num2);
//                break;
//            case BinaryOperator::Multiply:
//                execStack.push(num1 * num2);
//                break;
//            case BinaryOperator::Divide:
//                execStack.push(num1 / num2);
//                break;
//            case BinaryOperator::Power:
//                execStack.push(pow(num1, num2));
//                break;
//        }
//    }
//};
//
////ostream& operator<<(ostream& os, const NumberToken& token)
////{
////    os << token.value;
////    return os;
////}
//
//double executePolishNotation(queue<Token*> tokens)
//{
//    stack<double> execStack;
//    while(!tokens.empty())
//    {
//        tokens.front()->stackAction(execStack);
//        tokens.pop();
//    }
//    if(execStack.size() == 0)
//        throw invalid_argument("Result stack empty!");
//    else if(execStack.size() > 1)
//        throw invalid_argument("Result stack contains more than one value!");
//
//    return execStack.top();
//}
//
//queue<Token*> parsePolishNotation(const string& input)
//{
//    queue<Token*> tokens;
//    istringstream inputStream(input);
//
//    while(!inputStream.eof())
//    {
//        char peekChar = inputStream.peek();
//        switch(peekChar)
//        {
//            case '+':
//                tokens.push(new BinaryMathOperatorToken(BinaryOperator::Add));
//                inputStream.ignore();
//                break;
//            case '-':
//                tokens.push(new BinaryMathOperatorToken(BinaryOperator::Subtract));
//                inputStream.ignore();
//                break;
//            case '*':
//                tokens.push(new BinaryMathOperatorToken(BinaryOperator::Multiply));
//                inputStream.ignore();
//                break;
//            case '/':
//                tokens.push(new BinaryMathOperatorToken(BinaryOperator::Divide));
//                inputStream.ignore();
//                break;
//            case '^':
//                tokens.push(new BinaryMathOperatorToken(BinaryOperator::Power));
//                inputStream.ignore();
//                break;
//        }
//        if(isdigit(peekChar))
//        {
//            int num;
//            inputStream >> num;
//            tokens.push(new NumberToken(num));
//        }
//        else if(iswspace(peekChar))
//        {
//            do
//            {
//                inputStream.ignore();
//            }
//            while(iswspace(peekChar = inputStream.peek()));
//        }
//    }
//
//    return tokens;
//}
//
//queue<Token*> convertNotationInfixToPolish(queue<Token*> infixTokens)
//{
//    queue<Token*> polishTokens;
//    stack<Token*> operators;
//    while(!infixTokens.empty())
//    {
//		Token* nextToken = infixTokens.front();
//		infixTokens.pop();
//
//        switch(typeid(nextToken))
//        {
//			case typeid(NumberToken*):
//				polishTokens.push(nextToken);
//				break;
//			case typeid(BinaryMathOperatorToken*):
//
//        }
//    }
//}
//
//int main2()
//{
////    queue<Token*> tokenStack;
////    tokenStack.push(new NumberToken(50));
////    tokenStack.push(new NumberToken(10));
////    tokenStack.push(new MathOperatorToken(MathOperator::Divide));
////    cout << executePolishNotation(tokenStack) << endl;
//
//    string test = "10 5    ^5/";
//    cout << executePolishNotation(parsePolishNotation(test)) << endl;
//
//	return EXIT_SUCCESS;
//}
//
