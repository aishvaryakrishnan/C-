
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
  Expression(const std::string &e) : expression(e) { }
  bool validate() const
  {
    // opting to consider empty expression as valid
    if (expression.empty())
    {
        return true;
    }

    std::stack<char> s;
    for (auto &ch : expression)
    {
        // catch situations when char is not a bracket
        if (open_brackets.find(ch) != open_brackets.end() && close_brackets.find(ch) != close_brackets.end() )
        {
            return false;
        } 
        if (s.empty())
        {
            // we always want an open bracket first
            if (open_brackets.find(ch) != open_brackets.end())
            {
                s.push(ch);
            }
            else
            {
                return false;
            }
        }
        // if char we are reading is a close bracket:
        // and, top of stack is a matching bracket then
        // remove the matching bracket and continue
        // else, we have an imbalanced expression
        else if  (close_brackets.find(ch) != close_brackets.end())
        {
            auto brackets_val = brackets.find(s.top());
            if (brackets_val == brackets.end())
            {
                return false;
            }
            else
            {
                s.pop();
            }
        }
        // if we are reading an open bracket, 
        // add to stack and continue
        else
        {
            s.push(ch);
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
  std::string expression;
  std::unordered_map<char, char> brackets {{'{','}'}, {'(',')'}, {'[',']'}};
  std::unordered_set<char> open_brackets {'{', '[', '('};
  std::unordered_set<char> close_brackets {'}', ']', ')'};
};

int main(const int argc, const char **argv)
{
  std::string expression;
  std::cout << "enter an expression" << std::endl;
  std::getline(std::cin, expression);

  Expression e(expression);
  std::cout << (e.validate() ? "valid" : "invalid") << std::endl;

  return 0;
}
