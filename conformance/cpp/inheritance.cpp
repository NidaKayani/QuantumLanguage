#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Animal {
protected:
    string name;
public:
    Animal(const string& n) : name(n) { cout << "Animal(" << n << ")" << endl; }
    virtual ~Animal() { cout << "~Animal(" << name << ")" << endl; }
    virtual string sound() const = 0;
    virtual void speak() const { cout << name << " says " << sound() << endl; }
    string getName() const { return name; }
};

class Dog : public Animal {
public:
    Dog(const string& n) : Animal(n) { cout << "Dog()" << endl; }
    ~Dog() { cout << "~Dog()" << endl; }
    string sound() const override { return "Woof"; }
};

class Cat : public Animal {
public:
    Cat(const string& n) : Animal(n) {}
    string sound() const override { return "Meow"; }
    void speak() const override { cout << "The cat " << name << " purrs: " << sound() << endl; }
};

class Puppy : public Dog {
public:
    Puppy(const string& n) : Dog(n) {}
    string sound() const override { return "Yip"; }
};

int main() {
    vector<Animal*> zoo;
    zoo.push_back(new Dog("Rex"));
    zoo.push_back(new Cat("Tom"));
    zoo.push_back(new Puppy("Bit"));
    for (Animal* a : zoo) a->speak();
    for (Animal* a : zoo) {
        if (Dog* d = dynamic_cast<Dog*>(a)) cout << d->getName() << " is a dog" << endl;
    }
    for (Animal* a : zoo) delete a;
    return 0;
}
