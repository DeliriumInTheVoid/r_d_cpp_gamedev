#include <iostream>
#include <algorithm>

#include "../headers/dynamic_int_array.hpp"


//default c-tor
dynamic_int_array::dynamic_int_array() : dynamic_int_array{ 0 }
{
}

//c-tor with size
dynamic_int_array::dynamic_int_array(const std::size_t size) : size_{ 0 }
{
    max_size_ = size > initial_buffer_size ? size : initial_buffer_size;
    dynamic_buffer_ = new int[max_size_];
}

//copy c-tor
dynamic_int_array::dynamic_int_array(const dynamic_int_array& other) :
    size_{ other.size_ }, max_size_{ other.max_size_ }
{
    dynamic_buffer_ = new int[max_size_];
    std::copy_n(other.dynamic_buffer_, size_, dynamic_buffer_);
}

//free allocated memory
dynamic_int_array::~dynamic_int_array()
{
    delete[] dynamic_buffer_;
    dynamic_buffer_ = nullptr;
}

// copy assignment operator
dynamic_int_array& dynamic_int_array::operator=(const dynamic_int_array& other)
{
    if (this != &other)
    {
        delete[] dynamic_buffer_;
        dynamic_buffer_ = nullptr;

        max_size_ = other.max_size_;
        size_ = other.size_;

        dynamic_buffer_ = new int[max_size_];
        std::copy_n(other.dynamic_buffer_, size_, dynamic_buffer_);
    }

    return *this;
}

int& dynamic_int_array::operator[](const std::size_t index)
{
    if (index >= size_)
    {
        std::cerr << "dynamic_int_array: error. operator[] index: " << index << " is out of range!";
    }

    return dynamic_buffer_[index];
}

//delete previous memory, create new inner array with updated size
void dynamic_int_array::set_size(const std::size_t new_size)
{
    delete[] dynamic_buffer_;
    dynamic_buffer_ = nullptr;

    size_ = 0;
    max_size_ = new_size > initial_buffer_size ? new_size : initial_buffer_size;
    dynamic_buffer_ = new int[max_size_];
}

std::size_t dynamic_int_array::get_size() const
{
    return size_;
}

//Clear internal memory, set size to 0
void dynamic_int_array::clear()
{
    set_size(0);
}


// Allocate new memory for new element
// Copy old content to new inner array
// insert element at the last index
// update size
void dynamic_int_array::push_back(const int element)
{
    if (size_ >= max_size_)
    {
        max_size_ = size_ + extend_step_size;
        int* extended_buffer = new int[max_size_];

        std::copy_n(dynamic_buffer_, size_, extended_buffer);

        delete[] dynamic_buffer_;
        dynamic_buffer_ = extended_buffer;
    }

    dynamic_buffer_[size_] = element;

    ++size_;
}
