#include <iostream>
#include <string>
using namespace std;
// TODO: Implement Character base class
class Character {
private:
    string name;  // Only Character class can modify name    
protected:
    int health; // Derived classes need access for combat
    int speed;  // Derived classes use for abilities    
public:
    // Constructor with member initialization
    Character(const string& n, int h, int s) : name(n), health(h), speed(s) {
        cout << "Character " << name << " created" << endl;
    }    
    // Virtual destructor - essential for inheritance
    virtual ~Character() {
        cout << "Character " << name << " destroyed" << endl;
    }    
    // Virtual method for polymorphism
    virtual void displayStats() const {
        cout << "Name: " << name << ", Health: " << health << ", Speed: " << speed << endl;
    }    
    // Controlled access to private members
    string getName() const { return name; }    
    // Validation methods for protected members
    void setHealth(int h) {
        health = (h >= 0) ? h : 0;
    }    
    int getHealth() const { return health; }
};
// TODO: Implement Warrior derived class
class Warrior : public Character {
private:
    int strength;    
public:
    Warrior(const string& name, int health, int speed, int str)
        : Character(name, health, speed), strength(str) {
        cout << "Warrior " << getName() << " created with strength " << strength << endl;
    }    
    ~Warrior() {
        cout << "Warrior " << getName() << " destroyed" << endl;
    }    
    void displayStats() const override {
        Character::displayStats();
        cout << "Health: " << health<< ", Class: Warrior" << endl;
        cout << "Speed: " << speed<< ", Class: Warrior" << endl;
        cout << "Strength: " << strength << ", Class: Warrior" << endl;
    }    
    void attack() {
        int damage = strength + (speed / 2);
        cout << getName() << " attacks for " << damage << " damage!" << endl;
    }
};
// TODO: Implement Mage derived class
class Mage : public Character {
private:
    int mana;    
public:
    Mage(const string& name, int health, int speed, int m)
        : Character(name, health, speed), mana(m) {
        cout << "Mage " << getName() << " created with " << mana << " mana" << endl;
    }    
    ~Mage() {
        cout << "Mage " << getName() << " destroyed" << endl;
    }    
    void displayStats() const override {
        Character::displayStats();
        cout << "Mana: " << mana << ", Class: Mage" << endl;
    }    
    void castSpell() {
        if (mana >= 10) {
            int damage = mana / 2 + speed;
            mana -= 10;
            cout << getName() << " casts spell for " << damage << " damage!" << endl;
        } else {
            cout << getName() << " has insufficient mana!" << endl;
        }
    }
};

int main() {
  Warrior w("Aragorn", 100, 20, 30);
  Mage m("Gandalf", 80, 15, 50);

  return 0;
}