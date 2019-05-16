#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <typeinfo>

using namespace std;
class Expression
{
public:

    virtual int interpreter(map<string, int>& var) = 0;
    virtual ~Expression(){};
};


class VarExpression : public Expression
{
    string key;
public:
    VarExpression(string key)
    {
        this->key = key;
    }


    int interpreter(map<string, int>& var)
    {

        return var[key];
    }

};

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

    int interpreter(map<string, int>& var)
    {
        return left->interpreter(var) / right ->interpreter(var);
    }
};

  class doubleExpression : public SymbolExpression
{
public:
    doubleExpression(Expression* left, Expression* right): SymbolExpression(left,right)
    {
    }

 
    int interpreter(map<string, int>& var)
    {
      

        return left->interpreter(var) + right ->interpreter(var)+left ->interpreter(var) ;
    }




};

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
            case '+':  
   
                left = stkExp.top();
                stkExp.pop();

               
                right = new VarExpression(expStr.substr(++i,1));
               
                stkExp.push(new AddExpression(left, right));
                
                 if(expStr.substr(i,2)=="++"){
                   right = new VarExpression(expStr.substr(i,2));
                  stkExp.push(new doubleExpression(left, right));
                }

                break;

            case '-':
               
                left = stkExp.top();
                stkExp.pop();

                
                right = new VarExpression(expStr.substr(++i,1));

            
                stkExp.push(new SubExpression(left, right));

                break;

            case '*':
              
                left = stkExp.top();
                stkExp.pop();

               
                right = new VarExpression(expStr.substr(++i,1));

               
                stkExp.push(new MulExpression(left, right));

                break;

            case '/':
                
                left = stkExp.top();
                stkExp.pop();

               
                right = new VarExpression(expStr.substr(++i,1));

                
                stkExp.push(new DivExpression(left, right));

                break;
          
        

            default:
                stkExp.push(new VarExpression(expStr.substr(i,1)));
            }
        }

        
        if(!stkExp.empty())
        {
            expression = stkExp.top();
            stkExp.pop();
        }
    }

    void deltree(Expression* expression)
    {
        SymbolExpression* branch = dynamic_cast<SymbolExpression*>(expression);
        
        if (branch == NULL)
        {
            delete expression;
        }
        else  
        {
            
            deltree(branch->getLeft());

            
            deltree(branch->getRight());

            
            delete expression;
        }
    }

    ~Calculator()
    {
        deltree(expression);
        expression = NULL;
    }

   
    int run(map<string, int>& var)
    {
        return (expression == NULL) ? 0 : expression->interpreter(var);
    }
};
int main()
{
    string expStr = "a+b+c"; 
    string expStr2 = "a*b/c";
    map<string, int> var; 
    var["a"] = 10;
    var["b"] = 20;
    var["c"] = 10;
    Calculator cal(expStr);
    Calculator cal2(expStr2);
    

    cout << expStr << " = " << cal.run(var) << endl;
    cout << expStr2 << " = " << cal2.run(var) << endl;
    return 0;
}
