#include <iostream>
#include <algorithm>

#include "../headers/dynamic_int_array.hpp"


//default c-tor
dynamic_int_array::dynamic_int_array() : dynamic_int_array{ 0 }
{
}

//c-tor with size
dynamic_int_array::dynamic_int_array(const std::size_t size) : size_{ size }
{
    capacity_ = size > initial_buffer_size ? size : initial_buffer_size;
    dynamic_buffer_ = new int[capacity_]{0};
}

//copy c-tor
dynamic_int_array::dynamic_int_array(const dynamic_int_array& other) :
    size_{ other.size_ }, capacity_{ other.capacity_ }
{
    dynamic_buffer_ = new int[capacity_];
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

        capacity_ = other.capacity_;
        size_ = other.size_;

        dynamic_buffer_ = new int[capacity_];
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
    capacity_ = new_size > initial_buffer_size ? new_size : initial_buffer_size;

    int* tmp_buffer = new int[capacity_];

    memset(tmp_buffer, 0, sizeof(int) * capacity_);

    const size_t copy_size = size_ <= capacity_ ? size_ : capacity_;
    std::copy_n(dynamic_buffer_, copy_size, tmp_buffer);

    size_ = new_size;

    delete[] dynamic_buffer_;

    dynamic_buffer_ = tmp_buffer;
}

std::size_t dynamic_int_array::get_size() const
{
    return size_;
}

//Clear internal memory, set size to 0
void dynamic_int_array::clear()
{
    delete[] dynamic_buffer_;
    dynamic_buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}


// Allocate new memory for new element
// Copy old content to new inner array
// insert element at the last index
// update size
void dynamic_int_array::push_back(const int element)
{
    if (size_ >= capacity_)
    {
        capacity_ = size_ + extend_step_size;
        int* extended_buffer = new int[capacity_];

        std::copy_n(dynamic_buffer_, size_, extended_buffer);

        delete[] dynamic_buffer_;
        dynamic_buffer_ = extended_buffer;
    }

    dynamic_buffer_[size_] = element;

    ++size_;
}
