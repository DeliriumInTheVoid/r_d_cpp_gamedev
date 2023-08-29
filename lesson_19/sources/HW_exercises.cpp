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
    //if present at least one virtual function virtual destructor must be declared
    virtual ~Base(){}

    virtual void print() const {
        std::cout << "Base" << std::endl;
    }
};

class Derived1 : public Base {
public:
    ~Derived1(){}

    //override for virtual function
    void print() const override {
        std::cout << "Derived1" << std::endl;
    }
};

class Derived2 : public Base {
public:
    ~Derived2() {}

    //override for virtual function
    void print() const override {
        std::cout << "Derived2" << std::endl;
    }
};



//CASE 3
class Animal
{
public:
    //if present at least one virtual function virtual destructor must be declared
    virtual ~Animal(){}

    virtual void eat()
    {
        std::cout << "Animal eat method\n";
    }

    virtual void sleep() const
    {
        std::cout << "Animal sleep method\n";
    }
};

class Dog : public Animal
{
public:

    ~Dog(){}

    void eat() override
    {
        eatCallsCounter++;
        std::cout << "Dog eating\n" << eatCallsCounter;
    }

    //override for virtual function can be present.
    //If not, there is no reason to make it virtual for current hierarchy
    void sleep() const override
    {
        std::cout << "Dog sleep method\n";
    }

private:
    unsigned eatCallsCounter = 0;
};

class Cat : public Animal
{
public:
    ~Cat(){}

    //override for virtual function can be present.
    //If not, there is no reason to make it virtual for current hierarchy
    void sleep() const override
    {
        std::cout << "Cat sleep method\n";
    }

    //override for virtual function
    void eat() override
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
        //static_cast for safe conversion
        //https://en.cppreference.com/w/cpp/string/byte/tolower
        //ch = std::tolower(ch);
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
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
    // down casting issue. static_cast instead of dynamic_cast.
    // A is base class for B and A does not contain functions created for B.
    // B* b = static_cast<B*>(a); 
    B* b = new B();

    b->print("sdf");
    b->print();


    //CASE 2
    Derived1 d1;
    d1.print();

    Base* bp = &d1;
    bp->print();

    // down casting issue
    //Derived2* dp2 = dynamic_cast<Derived2*>(bp);
    Derived2* d2 = new Derived2();
    Base* dp2 = d2;
    dp2->print();


    //CASE 3
    std::string petChoice;
    std::cout << "Choose your pet:(Dog/Cat):";
    std::cin >> petChoice;

    Animal* animal = createAnimal(petChoice);
    //dynamic_cast<T>() must be used for down casting and runtime casting
    //Dog* dog = static_cast<Dog*>(animal);
    Dog* dog = dynamic_cast<Dog*>(animal);
    if (dog != nullptr)
    {
        dog->eat();
    }
}

