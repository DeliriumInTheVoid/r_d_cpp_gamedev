#pragma once

#include <string>


class Munchkin;

class MonsterPolicy
{
public:
    virtual ~MonsterPolicy() = default;

public:
    virtual void apply(Munchkin* munchkin) = 0;

    virtual std::string getFullInfo() = 0;
};