#include <string>
#include <algorithm>
#include <sstream>

#include "AbilityAction.h"


std::string getAbilityActionString(const AbilityAction action, const float value)
{
    using enum AbilityAction;

    std::ostringstream out;
    out.precision(1);
    out << std::fixed << value;

    switch (action)
    {
    case Add:
        return "+" + std::move(out).str();
        break;
    case Multiply:
        return "x" + std::move(out).str();
        break;
    case Subtract:
        return "-" + std::move(out).str();
        break;
    case Divide:
        return "%" + std::move(out).str();
        break;
    case None:
    default:
        return "";
    }
}


AbilityAction getAbilityAction(std::string action)
{
    std::ranges::transform(action, action.begin(), toupper);

    if (action == "MULTIPLY")
    {
        return AbilityAction::Multiply;
    }

    if (action == "ADD")
    {
        return AbilityAction::Add;
    }

    if (action == "DIVIDE")
    {
        return AbilityAction::Divide;
    }

    if (action == "SUBTRACT")
    {
        return AbilityAction::Subtract;
    }

    return AbilityAction::None;
}