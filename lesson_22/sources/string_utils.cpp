#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
#include <stack>

#include "string_utils.hpp"


unsigned int unique_words_count(const std::string& line)
{
    std::istringstream iss{ line };
    std::set<std::string> unique_words;

    std::string word;
    while (iss >> word)
    {
        unique_words.insert(word);
    }

    return static_cast<unsigned int>(unique_words.size());
}

std::string most_occurred_word(const std::string& line)
{
    std::unordered_map<std::string, unsigned> word_count;

    std::istringstream iss{ line };
    std::string word;

    std::string most_common_word;
    unsigned max_count{ 0 };

    while (iss >> word)
    {
        const unsigned current_count = ++word_count[word];

        if(current_count > max_count)
        {
            max_count = current_count;
            most_common_word = word;
        }
    }

    return most_common_word;
}

bool is_brackets_balanced(const std::string& str)
{
    std::stack<char> brackets_stack;

    for (char ch : str)
    {
        if (ch == '{' || ch == '[' || ch == '(')
        {
            brackets_stack.push(ch);
        }
        else if (ch == '}' || ch == ']' || ch == ')')
        {
            if (brackets_stack.empty())
            {
                return false;
            }

            const char open_bracket = brackets_stack.top();
            brackets_stack.pop();

            if ((ch == '}' && open_bracket != '{') ||
                (ch == ']' && open_bracket != '[') ||
                (ch == ')' && open_bracket != '('))
            {
                return false;
            }
        }
    }

    return brackets_stack.empty();
}
