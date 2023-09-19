#pragma once

#include <variant>

#include "debug_macroses.hpp"


template <class T, int SmallBufferSize = 3>
class inplace_array
{
public:
    inplace_array();

    inplace_array(const std::size_t size);

    inplace_array(const inplace_array& other);

    ~inplace_array() = default;

public:
    inplace_array& operator=(const inplace_array& other);

    T& operator[](std::size_t index);

    void set_size(const std::size_t new_size);

    std::size_t get_size() const;

    void clear();

    void push_back(T element);

    void pop_back();

    T& back() const;

    void reserve(const std::size_t reserved_space);

    std::size_t get_capacity() const;

    void shrink_to_fit();

    bool operator==(const inplace_array& other);

private:
    bool small_buffer_used() const;

    T* get_buffer_prt();

    const T* get_buffer_prt() const;

private:
    struct small_buffer
    {
        T data[SmallBufferSize]{};
    };

    struct buffer
    {
        buffer() = default;

        explicit buffer(const std::size_t size) : data{ new T[size] }
        {
        }

        ~buffer()
        {
            delete[] data;
            data = nullptr;
        }

        buffer(buffer&& other) noexcept
        {
            data = other.data;
            other.data = nullptr;
        }

        buffer& operator=(buffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            data = other.data;
            other.data = nullptr;
            return *this;
        }

        T* data{ nullptr };
    };

private:
    static constexpr std::size_t extend_step_size = 10;

private:
    size_t size_ = 0;
    size_t capacity_ = SmallBufferSize;
    std::variant<small_buffer, buffer> buffer_ {};
};

template <class T, int SmallBufferSize>
inplace_array<T, SmallBufferSize>::inplace_array() : inplace_array{ 0 }
{
}

template <class T, int SmallBufferSize>
inplace_array<T, SmallBufferSize>::inplace_array(const std::size_t size)
{
    capacity_ = size > SmallBufferSize ? size : SmallBufferSize;
    size_ = size;

    if (size > SmallBufferSize)
    {
        buffer_.template emplace<buffer>(std::move(buffer{ capacity_ }));
    }

    memset(get_buffer_prt(), T{}, sizeof(T) * capacity_);
}

template <class T, int SmallBufferSize>
inplace_array<T, SmallBufferSize>::inplace_array(const inplace_array& other)
    :size_{ other.size_ }, capacity_{ other.capacity_ }
{
    const T* other_buffer = other.get_buffer_prt();

    if (!other.small_buffer_used())
    {
        buffer_.template emplace<buffer>(std::move(buffer{ capacity_ }));
    }

    std::copy(other_buffer, other_buffer + size_, get_buffer_prt());
}

template <class T, int SmallBufferSize>
inplace_array<T, SmallBufferSize>& inplace_array<T, SmallBufferSize>::operator=(const inplace_array& other)
{
    if (this == &other)
    {
        return *this;
    }

    size_ = other.size_;
    capacity_ = other.capacity_;

    const T* other_buffer = other.get_buffer_prt();

    if (!other.small_buffer_used())
    {
        buffer_.template emplace<buffer>(std::move(buffer{ capacity_ }));
    }

    std::copy(other_buffer, other_buffer + size_, get_buffer_prt());

    return *this;
}

template <class T, int SmallBufferSize>
T& inplace_array<T, SmallBufferSize>::operator[](std::size_t index)
{
    ARR_OUT_OF_RANGE_GUARD(size_, index)
        return get_buffer_prt()[index];
}

template <class T, int SmallBufferSize>
void inplace_array<T, SmallBufferSize>::set_size(const std::size_t new_size)
{
    capacity_ = new_size > SmallBufferSize ? new_size : SmallBufferSize;

    const size_t copy_size = size_ <= capacity_ ? size_ : capacity_;

    if (small_buffer_used() && capacity_ <= SmallBufferSize)
    {
        if (new_size < SmallBufferSize)
        {
            memset(get_buffer_prt() + new_size, T{}, sizeof(T) * SmallBufferSize - new_size);
        }
    }
    else
    {
        buffer tmp_buffer = buffer{ capacity_ };
        memset(tmp_buffer.data, T{}, sizeof(T) * capacity_);

        T* buffer_ptr = get_buffer_prt();
        std::copy(buffer_ptr, buffer_ptr + copy_size, tmp_buffer.data);
        buffer_.template emplace<buffer>(std::move(tmp_buffer));
    }

    size_ = new_size;
}

template <class T, int SmallBufferSize>
std::size_t inplace_array<T, SmallBufferSize>::get_size() const
{
    return size_;
}

template <class T, int SmallBufferSize>
void inplace_array<T, SmallBufferSize>::clear()
{
    size_ = 0;
    capacity_ = 0;

    if (!small_buffer_used())
    {
        buffer_.template emplace<buffer>(std::move(buffer{}));
    }
    else
    {
        memset(get_buffer_prt(), T{}, sizeof(T) * SmallBufferSize);
    }
}

template <class T, int SmallBufferSize>
void inplace_array<T, SmallBufferSize>::push_back(T element)
{
    if (size_ >= capacity_)
    {
        const size_t new_capacity = size_ + extend_step_size;

        buffer new_buffer = buffer{ new_capacity };
        const T* buffer_ptr = get_buffer_prt();
        std::copy(buffer_ptr, buffer_ptr + size_, new_buffer.data);
        buffer_.template emplace<buffer>(std::move(new_buffer));

        capacity_ = new_capacity;
    }
    get_buffer_prt()[size_++] = element;
}

template <class T, int SmallBufferSize>
void inplace_array<T, SmallBufferSize>::pop_back()
{
    ARR_OUT_OF_RANGE_GUARD(size_, size_ - 1)
    --size_;
}

template <class T, int SmallBufferSize>
T& inplace_array<T, SmallBufferSize>::back() const
{
    std::size_t index = size_ - 1;
    ARR_OUT_OF_RANGE_GUARD(size_, index);
    return get_buffer_prt()[index];
}

template <class T, int SmallBufferSize>
void inplace_array<T, SmallBufferSize>::reserve(const std::size_t reserved_space)
{
    if (reserved_space > capacity_ && reserved_space > SmallBufferSize)
    {
        buffer new_buffer = buffer{ reserved_space };
        if (size_ > 0)
        {
            const T* buffer_ptr = get_buffer_prt();
            std::copy(buffer_ptr, buffer_ptr + size_, new_buffer.data);
        }
        buffer_.template emplace<buffer>(std::move(new_buffer));

        capacity_ = reserved_space;
    }
}

template <class T, int SmallBufferSize>
std::size_t inplace_array<T, SmallBufferSize>::get_capacity() const
{
    return capacity_;
}

template <class T, int SmallBufferSize>
void inplace_array<T, SmallBufferSize>::shrink_to_fit()
{
    if (capacity_ > size_)
    {
        set_size(size_);
    }
}

template <class T, int SmallBufferSize>
bool inplace_array<T, SmallBufferSize>::operator==(const inplace_array& other)
{
    if (this == &other)
    {
        return true;
    }

    if (size_ != other.size_)
    {
        return false;
    }

    const T* buffer_ptr = get_buffer_prt();
    const T* other_buffer_ptr = other.get_buffer_prt();

    for (std::size_t i = 0; i < size_; ++i)
    {
        if (buffer_ptr[i] != other_buffer_ptr[i]) {
            return false;
        }
    }
    return true;
}

template <class T, int SmallBufferSize>
bool inplace_array<T, SmallBufferSize>::small_buffer_used() const
{
    return std::holds_alternative<small_buffer>(buffer_);
}

template <class T, int SmallBufferSize>
T* inplace_array<T, SmallBufferSize>::get_buffer_prt()
{
    if (small_buffer_used()) {
        return std::get<small_buffer>(buffer_).data;
    }
    return std::get<buffer>(buffer_).data;
}

template <class T, int SmallBufferSize>
const T* inplace_array<T, SmallBufferSize>::get_buffer_prt() const
{
    if (small_buffer_used()) {
        return std::get<small_buffer>(buffer_).data;
    }
    return std::get<buffer>(buffer_).data;
}
