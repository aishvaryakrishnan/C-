#include <iostream>
#include <string.h>
#include <math.h>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

class Expression
{
public:
  bool validate(const std::string &expression) const
  {
    // opting to consider empty expression as valid
    if (expression.empty())
    {
        return true;
    }

    std::stack<char> s;
    for (auto &ch : expression)
    {
        // if char we are reading is an open bracket:
        // add to stack and continue
        if  (brackets.find(ch) != brackets.end())
        {
            s.push(ch);
        }
        else
        {
          // char we are reading is a closed bracket
          // so if top of stack is a matching bracket then
          // remove the matching bracket and continue
          // else, we have an imbalanced expression
          if (s.empty() || brackets.find(s.top())->second != ch)
            {
                return false;
            }
            else
            {
                s.pop();
            }
        }
    }

    // if at the end of processing stack has stray brackets
    // the expression is imbalanced
    if (!s.empty())
    {
        return false;
    }

    return true;
  };

private:
  static std::unordered_map<char, char> brackets;
};

std::unordered_map<char, char> Expression::brackets = {{'{','}'}, {'(',')'}, {'[',']'}};;

int main(const int argc, const char **argv)
{
  std::string expression;
  std::cout << "enter an expression" << std::endl;
  std::getline(std::cin, expression);

  Expression e;
  std::cout << (e.validate(expression) ? "valid" : "invalid") << std::endl;
  
  return 0;
}
