#pragma once

#include <iostream>
#include <algorithm>


template <class T>
class dynamic_array
{
public:
    //default c-tor
    dynamic_array();

    //c-tor with size
    dynamic_array(std::size_t size);

    //copy c-tor
    dynamic_array(const dynamic_array& other);

    //free allocated memory
    ~dynamic_array();

    // copy assignment operator
    dynamic_array& operator=(const dynamic_array& other);

    T& operator[](std::size_t index);

    bool operator==(const dynamic_array& other) const;

    //delete previous memory, create new inner array with updated size
    void set_size(std::size_t new_size);
    std::size_t get_size() const;

    //Clear internal memory, set size to 0
    void clear();

    // Allocate new memory for new element
    // Copy old content to new inner array
    // insert element at the last index
    // update size
    void push_back(T element);

    void pop_back();

    T& back() const;

    void reserve(const std::size_t reserved_space);

    std::size_t get_capacity() const;

    void shrink_to_fit();

private:
    void extend_buffer(std::size_t capacity, bool update_size = false, std::size_t new_size = 0);
    void check_size_guard() const;
    void check_index_guard(std::size_t index) const;

private:
    static constexpr std::size_t extend_step_size = 10;
    static constexpr std::size_t initial_buffer_size = 5;

private:
    std::size_t size_;
    std::size_t capacity_;
    T* dynamic_buffer_{ nullptr };
};


//default c-tor
template <class T>
dynamic_array<T>::dynamic_array() : dynamic_array{ 0 }
{
}

//c-tor with size
template <class T>
dynamic_array<T>::dynamic_array(const std::size_t size) : size_{ size }
{
    capacity_ = size > initial_buffer_size ? size : initial_buffer_size;
    dynamic_buffer_ = new T[capacity_] {};
}

//copy c-tor
template <class T>
dynamic_array<T>::dynamic_array(const dynamic_array& other) :
    size_{ other.size_ }, capacity_{ other.capacity_ }
{
    dynamic_buffer_ = new T[capacity_];
    std::copy_n(other.dynamic_buffer_, size_, dynamic_buffer_);
}

//free allocated memory
template <class T>
dynamic_array<T>::~dynamic_array()
{
    delete[] dynamic_buffer_;
    dynamic_buffer_ = nullptr;
}

// copy assignment operator
template <class T>
dynamic_array<T>& dynamic_array<T>::operator=(const dynamic_array<T>& other)
{
    if (this != &other)
    {
        delete[] dynamic_buffer_;
        dynamic_buffer_ = nullptr;

        capacity_ = other.capacity_;
        size_ = other.size_;

        dynamic_buffer_ = new T[capacity_];
        std::copy_n(other.dynamic_buffer_, size_, dynamic_buffer_);
    }

    return *this;
}

template <class T>
T& dynamic_array<T>::operator[](const std::size_t index)
{
    check_index_guard(index);
    return dynamic_buffer_[index];
}

template <class T>
bool dynamic_array<T>::operator==(const dynamic_array<T>& other) const
{
    if (this == &other)
    {
        return true;
    }

    if (size_ != other.get_size())
    {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i)
    {
        if (this[i] != other[i])
        {
            return false;
        }
    }

    return true;
}

//delete previous memory, create new inner array with updated size
template <class T>
void dynamic_array<T>::set_size(const std::size_t new_size)
{
    extend_buffer(
        new_size > initial_buffer_size ? new_size : initial_buffer_size,
        true,
        new_size);
}

template <class T>
std::size_t dynamic_array<T>::get_size() const
{
    return size_;
}

//Clear internal memory, set size to 0
template <class T>
void dynamic_array<T>::clear()
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
template <class T>
void dynamic_array<T>::push_back(const T element)
{
    if (size_ >= capacity_)
    {
        extend_buffer(size_ + extend_step_size);
    }

    dynamic_buffer_[size_] = element;

    ++size_;
}

template <class T>
void dynamic_array<T>::pop_back()
{
    check_size_guard();
    dynamic_buffer_[size_] = T{};
    --size_;
}

template <class T>
T& dynamic_array<T>::back() const
{
    check_size_guard();

    return dynamic_buffer_[size_ - 1];
}

template <class T>
void dynamic_array<T>::reserve(const std::size_t reserved_space)
{
    if (capacity_ >= reserved_space)
    {
        return;
    }

    extend_buffer(reserved_space);
}

template <class T>
std::size_t dynamic_array<T>::get_capacity() const
{
    return capacity_;
}

template <class T>
void dynamic_array<T>::shrink_to_fit()
{
    if (capacity_ > size_)
    {
        set_size(size_);
    }
}

template <class T>
void dynamic_array<T>::extend_buffer(std::size_t capacity, bool update_size, std::size_t new_size)
{
    capacity_ = capacity;
    T* extended_buffer = new T[capacity_];

	size_t copy_size = size_;
    if (update_size)
    {
        memset(extended_buffer, T{}, sizeof(T) * capacity_);
        copy_size = size_ <= capacity_ ? size_ : capacity_;
        size_ = new_size;
    }
    std::copy_n(dynamic_buffer_, copy_size, extended_buffer);

    delete[] dynamic_buffer_;
    dynamic_buffer_ = extended_buffer;
}

template <class T>
void dynamic_array<T>::check_size_guard() const
{
    if (size_ <= 0)
    {
        std::cerr << "Error! dynamic_array::back(): array is empty" << std::endl;
    }
}

template <class T>
void dynamic_array<T>::check_index_guard(std::size_t index) const
{
    if (index >= size_)
    {
        std::cerr << "dynamic_array: error. operator[] index: " << index << " is out of range!";
    }
}
