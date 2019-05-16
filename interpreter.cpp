#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <typeinfo>

using namespace std;
//*******************************************抽象表达式类***********************************
class Expression
{
public:
    //解析公式和数值，其中var中的key是公式中的参数，value值是具体的数字
    //如a = 100; b = 20; c = 40
    virtual int interpreter(map<string, int>& var) = 0;
    virtual ~Expression(){};
};


//变量解析器（终结符表达式）
class VarExpression : public Expression
{
    string key;
public:
    VarExpression(string key)
    {
        this->key = key;
    }

    //从map中取出变量的值
    int interpreter(map<string, int>& var)
    {

        return var[key];
    }

};
//**********抽象运算符号解析器***********************
//抽象运算符号解析器
class SymbolExpression : public Expression
{
protected:
    Expression* left;
    Expression* right;
public:
    SymbolExpression(Expression* left, Expression* right)
    {
        this -> left = left;
        this -> right = right;
    }

    Expression* getLeft()
    {
        return left;
    }
    Expression* getRight()
    {
        return right;
    }
};


class AddExpression : public SymbolExpression
{
public:
    AddExpression(Expression* left, Expression* right): SymbolExpression(left,right)
    {
    }

  
    int interpreter(map<string, int>& var)
    {
        return left->interpreter(var) + right ->interpreter(var);
    }

};


class SubExpression : public SymbolExpression
{
public:
    SubExpression(Expression* left, Expression* right): SymbolExpression(left,right)
    {
    }

   
    int interpreter(map<string, int>& var)
    {
        return left->interpreter(var) - right ->interpreter(var);
    }

  

};

class MulExpression : public SymbolExpression
{
public:
    MulExpression(Expression* left, Expression* right): SymbolExpression(left,right)
    {
    }

    //把左右两个表达式运算的结果相减
    int interpreter(map<string, int>& var)
    {
        return left->interpreter(var) * right ->interpreter(var);
    }

};

class DivExpression : public SymbolExpression
{
public:
    DivExpression(Expression* left, Expression* right): SymbolExpression(left,right)
    {
    }

    //把左右两个表达式运算的结果相减
    int interpreter(map<string, int>& var)
    {
        return left->interpreter(var) / right ->interpreter(var);
    }


};

//*********************************解析器封装类***************************************
//解析器封装类，这个类是根据迪米特法则进行封装，目的是让Client只与直接朋友打交道，相当于Facade
class Calculator
{
private:
    Expression* expression;
public:
    Calculator(string expStr)
    {
        expression = NULL;

        stack<Expression*> stkExp;

        Expression* left  = NULL;
        Expression* right = NULL;

 
        for(unsigned int i = 0; i< expStr.length(); i++)
        {
            switch(expStr[i])
            {
            case '+':  //加法
                //1.先从栈中取出左操作数
                left = stkExp.top();
                stkExp.pop();

                //2.从表达式中取出+号后面的右操作数，并生成终结符解析对象
                right = new VarExpression(expStr.substr(++i,1));

                //3.将左右操作数相加，并把结果放入栈中
                stkExp.push(new AddExpression(left, right));

                break;

            case '-':
                //1.先从栈中取出左操作数
                left = stkExp.top();
                stkExp.pop();

                //2.从表达式中取出+号后面的右操作数，并生成终结符解析对象
                right = new VarExpression(expStr.substr(++i,1));

                //3.将左右操作数相减，并把结果放入栈中
                stkExp.push(new SubExpression(left, right));

                break;

            case '*':
                //1.先从栈中取出左操作数
                left = stkExp.top();
                stkExp.pop();

                //2.从表达式中取出+号后面的右操作数，并生成终结符解析对象
                right = new VarExpression(expStr.substr(++i,1));

                //3.将左右操作数相减，并把结果放入栈中
                stkExp.push(new MulExpression(left, right));

                break;

            case '/':
                //1.先从栈中取出左操作数
                left = stkExp.top();
                stkExp.pop();

                //2.从表达式中取出+号后面的右操作数，并生成终结符解析对象
                right = new VarExpression(expStr.substr(++i,1));

                //3.将左右操作数相减，并把结果放入栈中
                stkExp.push(new DivExpression(left, right));

                break;

            default:
                //如果是变量（终结符）：如a+b+c中的a\b\c，
                //则直接生成对应的变量解析器对象
                stkExp.push(new VarExpression(expStr.substr(i,1)));
            }
        }

        //栈中保存的就是最终语法树的根结点（本例为SuuExpression对象）
        if(!stkExp.empty())
        {
            expression = stkExp.top();
            stkExp.pop();
        }
    }

    void deltree(Expression* expression)
    {
        SymbolExpression* branch = dynamic_cast<SymbolExpression*>(expression);
        //叶子结点
        if (branch == NULL)
        {
            delete expression;
        }
        else  //分支结点
        {
            //左子树
            deltree(branch->getLeft());

            //右子树
            deltree(branch->getRight());

            //结点
            delete expression;
        }
    }

    ~Calculator()
    {
        deltree(expression);
        expression = NULL;
    }

    //开始运算
    int run(map<string, int>& var)
    {
        return (expression == NULL) ? 0 : expression->interpreter(var);
    }
};
int main()
{
    string expStr = "a+b-c*a/b"; //为简化处理，这里必须是合法的表达式

    map<string, int> var;   //相当于Interpreter模式中的Context
    var["a"] = 100;
    var["b"] = 20;
    var["c"] = 40;
    Calculator cal(expStr);

    cout <<"运算结果为：" << expStr << " = " << cal.run(var) << endl;
    return 0;
}
