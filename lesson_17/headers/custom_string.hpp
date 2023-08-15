#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>


class StringCustom {
public:
    StringCustom();
    StringCustom& operator=(const char* c_str);
    void assign(const char* c_str);

    ~StringCustom() { delete m_str; }

    std::size_t getLen() const { return strlen(m_str); }
    std::size_t getSize() const { return getLen() + 1; }
private:
    char* m_str;

};

StringCustom::StringCustom()
{
    m_str = new char[1];
    m_str[0] = '\0';
}

StringCustom& StringCustom::operator=(const char* c_str)
{
    if (c_str == nullptr)
    {
        m_str = new char;
        *m_str = '\0';
    }
    else
    {
        const std::size_t size = strlen(c_str);
        m_str = new char[size];

        for (int i = 0; i < size; i++)
        {
            m_str[i] = c_str[i];
        }
    }

    return *this;
}

void StringCustom::assign(const char* c_str)
{
    m_str = new char[strlen(c_str) + 1];
    strcpy(m_str, c_str);
}


int* allocateArray(int size)
{
    int* arrayInt = new int[size];
    return arrayInt;
}

void clearArray(int* numbers)
{
    delete numbers;
}

int** allocateArrayOfArrays(int rows, int columns)
{
    int** arrayPtr = new int* [rows];

    for (int i = 0; i < rows; i++)
    {
        //Size of each individual array in this case may be different
        arrayPtr[i] = new int[columns];
    }

    return arrayPtr;
}

//columns is not needed
//allocated memory size for new[] is stored internally
void clearArrayOfArrays(int** arrayPtr, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        //remove each individual array in bigger container one by one
        delete[] arrayPtr[i];
    }
}
