#include <iostream>
#include <string>
#include <algorithm>

//CASE 1
class A
{
public:
    A()
    {
        std::cout << "A()" << std::endl;
    }

    ~A()
    {
        std::cout << "~A()" << std::endl;
    }
};

class B : public A
{
public:
    B()
    {
        std::cout << "B()" << std::endl;
    }

    ~B()
    {
        std::cout << "~B()" << std::endl;
    }

    void print() const
    {
        std::cout << my_string << std::endl;
    }

    void print(const std::string& text) const
    {
        std::cout << text << std::endl;
    }

private:
    std::string my_string;
};


//CASE 2
class Base {
public:
    virtual void print() {
        std::cout << "Base" << std::endl;
    }
};

class Derived1 : public Base {
public:
    void print() {
        std::cout << "Derived1" << std::endl;
    }
};

class Derived2 : public Base {
public:
    void print() {
        std::cout << "Derived2" << std::endl;
    }
};



//CASE 3
class Animal
{
public:
    virtual void eat() {};

    virtual void sleep()
    {
        std::cout << "Animal sleep method\n";
    }
};

class Dog : public Animal
{
public:
    void eat() override
    {
        eatCallsCounter++;
        std::cout << "Dog eating\n" << eatCallsCounter;
    }

private:
    int eatCallsCounter = 0;
};

class Cat : public Animal
{
public:
    void eat() const
    {
        std::cout << "Cat eating\n";
    }
};

void toLower(std::string& str)
{
    //ranged-based for loop
    //read: iterate every char of the string from left to right and execute something inside with it
    //similar to for (int i = 0; i < str.size(); i++) { str[i]... }
    for (char& ch : str)
    {
        ch = std::tolower(ch);
    }
}

Animal* createAnimal(std::string pet)
{
    toLower(pet);

    if (pet == "dog")  return new Dog;
    if (pet == "cat") return new Cat;

    return new Animal;
}


int main()
{
    //CASE 1
    A* a = new A();
    B* b = static_cast<B*>(a);

    b->print("sdf");
    b->print();


    //CASE 2
    Derived1 d1;
    d1.print();

    Base* bp = dynamic_cast<Base*>(&d1);
    bp->print();

    Derived2* dp2 = dynamic_cast<Derived2*>(bp);
    dp2->print();


    //CASE 3
    std::string petChoice;
    std::cout << "Choose your pet:(Dog/Cat):";
    std::cin >> petChoice;

    Animal* animal = createAnimal(petChoice);
    Dog* dog = static_cast<Dog*>(animal);
    if (dog != nullptr)
    {
        dog->eat();
    }
}

