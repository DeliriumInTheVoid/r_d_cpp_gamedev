#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

#include "HWs.hpp"

unsigned int countDivisibleBy(const std::vector<int>& vec, int number)
{
    return std::count_if(vec.begin(), vec.end(), [number](int element) {
        return element % number == 0;
     });
}

//#TODO: Find out what's wrong
std::function<std::string()> makeLambda()
{
    const std::string val = "string";
    // "val" it's "makeLambda()" scope variable and lives in a stack frame of the function.
    // in case of passing "val" as reference,
    // lambda will return some memory trash or other variable memory
    // which leads to the UB
    return [val]// or return [=] {return val;}
    {
        return val;
    };
}//after this line function stack frame will be destroyed with "val" variable.
