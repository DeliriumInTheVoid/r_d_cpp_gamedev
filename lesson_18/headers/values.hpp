#pragma once

class range
{
public:
    range(const int min_value, const int max_value);

public:
    int generate_value() const;

private:
    int min_;
    int max_;
};


class chanced_range
{
public:
    chanced_range(const int chance, range&& value);

public:
    int get_value(const int chance_increase = 0) const;

private:
    int chance_;
    range value_;
    range random_value_{ 0, 100 };
};

class chanced_value
{
public:
    chanced_value(const int chance, const float value);

public:
    float get_value() const;

private:
    int chance_;
    float value_;
    range random_value_{ 0, 100 };
};
