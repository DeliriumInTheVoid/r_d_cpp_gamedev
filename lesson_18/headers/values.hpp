#pragma once

class diapason_value
{
public:
    diapason_value(const int min_value, const int max_value);

public:
    int generate_value() const;

private:
    int min_;
    int max_;
};


class chanced_diapason_value
{
public:
    chanced_diapason_value(const int chance, diapason_value&& value);

public:
    int get_value(const int chance_increase = 0) const;

private:
    int chance_;
    diapason_value value_;
    diapason_value random_value_{ 0, 100 };
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
    diapason_value random_value_{ 0, 100 };
};
