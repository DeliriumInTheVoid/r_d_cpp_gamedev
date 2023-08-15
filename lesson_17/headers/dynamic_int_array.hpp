#pragma once
#include <cstdlib>


class dynamic_int_array
{
public:
    //default c-tor
    dynamic_int_array();

    //c-tor with size
    dynamic_int_array(std::size_t size);

    //copy c-tor
    dynamic_int_array(const dynamic_int_array& other);

    //free allocated memory
    ~dynamic_int_array();

    // copy assignment operator
    dynamic_int_array& operator=(const dynamic_int_array& other);

    int& operator[](std::size_t index);

    //delete previous memory, create new inner array with updated size
    void set_size(std::size_t new_size);
    std::size_t get_size() const;

    //Clear internal memory, set size to 0
    void clear();

    // Allocate new memory for new element
    // Copy old content to new inner array
    // insert element at the last index
    // update size
    void push_back(int element);

private:
    static constexpr std::size_t extend_step_size = 10;
    static constexpr std::size_t initial_buffer_size = 5;

private:
    std::size_t size_;
    std::size_t max_size_;
    int* dynamic_buffer_{ nullptr };
};
