#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <random>
using namespace std;

// ========================================
// STEP 1: Abstract Base Class and Interfaces - SOLUTION
// ========================================

class Entity {
protected:
    string name;
    int health;
    pair<int, int> position;  // x, y coordinates
    int maxHealth;
    
public:
    Entity(const string& entityName, int hp, int x, int y) 
        : name(entityName), health(hp), position(x, y), maxHealth(hp) {
        cout << "Entity created: " << name << " at position (" << x << "," << y << ")" << endl;
    }
    
    // Virtual destructor - ESSENTIAL for polymorphism
    virtual ~Entity() {
        cout << "Entity destroyed: " << name << endl;
    }
    
    // Pure virtual function - makes this an abstract class
    virtual void interact() const = 0;
    
    // Pure virtual function for type identification
    virtual string getType() const = 0;
    
    // Virtual function with default implementation
    virtual void displayInfo() const {
        cout << getType() << ": " << name << endl;
        cout << "  Health: " << health << "/" << maxHealth << endl;
        cout << "  Position: (" << position.first << "," << position.second << ")" << endl;
    }
    
    // Virtual function for movement
    virtual void move(int deltaX, int deltaY) {
        position.first += deltaX;
        position.second += deltaY;
        cout << name << " moves to (" << position.first << "," << position.second << ")" << endl;
    }
    
    // Common utility methods
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        cout << name << " takes " << damage << " damage. Health: " << health << endl;
    }
    
    void heal(int amount) {
        health = min(health + amount, maxHealth);
        cout << name << " heals " << amount << " HP. Health: " << health << endl;
    }
    
    bool isAlive() const { return health > 0; }
    
    // Getters
    string getName() const { return name; }
    int getHealth() const { return health; }
    pair<int, int> getPosition() const { return position; }
    
    // Calculate distance to another entity
    double distanceTo(const Entity& other) const {
        int dx = position.first - other.position.first;
        int dy = position.second - other.position.second;
        return sqrt(dx*dx + dy*dy);
    }
};

class Player : public Entity {
private:
    int level;
    int experience;
    int attackPower;
    string playerClass;
    
public:
    Player(const string& name, int x, int y, const string& charClass = "Warrior") 
        : Entity(name, 100, x, y), level(1), experience(0), attackPower(20), playerClass(charClass) {
        cout << "Player " << name << " (" << charClass << ") joins the adventure!" << endl;
    }
    
    ~Player() {
        cout << "Player " << name << " has left the game" << endl;
    }
    
    void interact() const override {
        cout << "🗡️ " << name << " the " << playerClass << " (Level " << level << ") looks around for adventure!" << endl;
        cout << "   \"I'm ready for anything!\"" << endl;
    }
    
    string getType() const override {
        return "Player (" + playerClass + ")";
    }
    
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "  Level: " << level << endl;
        cout << "  Experience: " << experience << endl;
        cout << "  Attack Power: " << attackPower << endl;
        cout << "  Class: " << playerClass << endl;
    }
    
    void levelUp() {
        level++;
        attackPower += 5;
        maxHealth += 20;
        health = maxHealth;
        cout << "🎉 " << name << " levels up to level " << level << "!" << endl;
    }
    
    void gainExperience(int exp) {
        experience += exp;
        cout << name << " gains " << exp << " experience (Total: " << experience << ")" << endl;
        if (experience >= level * 100) {
            levelUp();
        }
    }
    
    void attack(Entity& target) {
        if (!isAlive()) {
            cout << name << " cannot attack - defeated!" << endl;
            return;
        }
        
        cout << "⚔️ " << name << " attacks " << target.getName() << " for " << attackPower << " damage!" << endl;
        target.takeDamage(attackPower);
        
        if (!target.isAlive()) {
            gainExperience(50);
        }
    }
    
    // Getters
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    int getAttackPower() const { return attackPower; }
    string getPlayerClass() const { return playerClass; }
};

class Enemy : public Entity {
private:
    int damage;
    int aggroRange;
    string enemyType;
    bool isAggressive;
    
public:
    Enemy(const string& name, int hp, int x, int y, int dmg, const string& type = "Goblin") 
        : Entity(name, hp, x, y), damage(dmg), aggroRange(5), enemyType(type), isAggressive(true) {
        cout << "Enemy " << name << " (" << type << ") spawns with hostile intent!" << endl;
    }
    
    ~Enemy() {
        cout << "Enemy " << name << " has been vanquished" << endl;
    }
    
    void interact() const override {
        cout << "👹 " << name << " the " << enemyType << " snarls menacingly!" << endl;
        cout << "   \"Graahhh! You shall not pass!\"" << endl;
        if (isAggressive) {
            cout << "   " << name << " prepares to attack!" << endl;
        }
    }
    
    string getType() const override {
        return "Enemy (" + enemyType + ")";
    }
    
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "  Enemy Type: " << enemyType << endl;
        cout << "  Damage: " << damage << endl;
        cout << "  Aggro Range: " << aggroRange << endl;
        cout << "  Aggressive: " << (isAggressive ? "Yes" : "No") << endl;
    }
    
    void attack(Entity& target) {
        if (!isAlive()) return;
        
        cout << "💥 " << name << " strikes " << target.getName() << " for " << damage << " damage!" << endl;
        target.takeDamage(damage);
    }
    
    void patrol() {
        if (!isAlive()) return;
        
        // Random movement for patrol
        int dx = (rand() % 3) - 1; // -1, 0, or 1
        int dy = (rand() % 3) - 1;
        move(dx, dy);
        cout << name << " patrols the area..." << endl;
    }
    
    bool playerInRange(const Player& player) const {
        return distanceTo(player) <= aggroRange;
    }
    
    // Getters
    int getDamage() const { return damage; }
    int getAggroRange() const { return aggroRange; }
    string getEnemyType() const { return enemyType; }
    bool getIsAggressive() const { return isAggressive; }
};

class NPC : public Entity {
private:
    string dialogue;
    bool hasQuest;
    string questDescription;
    bool isQuestGiver;
    
public:
    NPC(const string& name, int x, int y, const string& dialog = "Hello, traveler!", bool quest = false) 
        : Entity(name, 50, x, y), dialogue(dialog), hasQuest(quest), isQuestGiver(quest) {
        if (hasQuest) {
            questDescription = "Find the lost artifact in the nearby dungeon.";
        }
        cout << "NPC " << name << " takes their position, ready to help travelers" << endl;
    }
    
    ~NPC() {
        cout << "NPC " << name << " bids farewell" << endl;
    }
    
    void interact() const override {
        cout << "💬 " << name << " greets you warmly." << endl;
        cout << "   \"" << dialogue << "\"" << endl;
        if (hasQuest && isQuestGiver) {
            cout << "   \"I have a quest for you, if you're interested!\"" << endl;
        }
    }
    
    string getType() const override {
        return hasQuest ? "NPC (Quest Giver)" : "NPC";
    }
    
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "  Dialogue: \"" << dialogue << "\"" << endl;
        cout << "  Has Quest: " << (hasQuest ? "Yes" : "No") << endl;
        if (hasQuest) {
            cout << "  Quest: " << questDescription << endl;
        }
    }
    
    void talk() const {
        cout << "💭 " << name << ": \"" << dialogue << "\"" << endl;
    }
    
    void giveQuest(Player& player) {
        if (hasQuest && isQuestGiver) {
            cout << "📜 " << name << " offers a quest to " << player.getName() << ":" << endl;
            cout << "    \"" << questDescription << "\"" << endl;
            cout << "    Reward: 200 experience points" << endl;
            isQuestGiver = false; // Quest can only be given once
        } else {
            cout << name << " has no quests available right now." << endl;
        }
    }
    
    // Getters
    string getDialogue() const { return dialogue; }
    bool getHasQuest() const { return hasQuest; }
    string getQuestDescription() const { return questDescription; }
};

// ========================================
// STEP 2: Extended Entity Types - SOLUTION
// ========================================

class Merchant : public Entity {
private:
    int gold;
    vector<string> inventory;
    vector<int> prices;
    
public:
    Merchant(const string& name, int x, int y, int startingGold = 500) 
        : Entity(name, 75, x, y), gold(startingGold) {
        // Initialize inventory
        inventory = {"Health Potion", "Sword", "Shield", "Magic Scroll"};
        prices = {50, 200, 150, 100};
        cout << "Merchant " << name << " sets up shop with " << gold << " gold" << endl;
    }
    
    ~Merchant() {
        cout << "Merchant " << name << " packs up and leaves" << endl;
    }
    
    void interact() const override {
        cout << "💰 " << name << " the merchant greets you with a smile." << endl;
        cout << "   \"Welcome to my shop! Take a look at my wares!\"" << endl;
        cout << "   \"I have " << inventory.size() << " items for sale.\"" << endl;
    }
    
    string getType() const override {
        return "Merchant";
    }
    
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "  Gold: " << gold << endl;
        cout << "  Inventory Items: " << inventory.size() << endl;
    }
    
    void showInventory() const {
        cout << "🛍️ " << name << "'s Shop:" << endl;
        for (size_t i = 0; i < inventory.size(); i++) {
            cout << "   " << (i+1) << ". " << inventory[i] << " - " << prices[i] << " gold" << endl;
        }
    }
    
    bool sellItem(const string& item, Player& buyer, int playerGold) {
        for (size_t i = 0; i < inventory.size(); i++) {
            if (inventory[i] == item) {
                if (playerGold >= prices[i]) {
                    cout << "💸 " << buyer.getName() << " buys " << item << " for " << prices[i] << " gold" << endl;
                    gold += prices[i];
                    inventory.erase(inventory.begin() + i);
                    prices.erase(prices.begin() + i);
                    return true;
                } else {
                    cout << "❌ Not enough gold to buy " << item << endl;
                    return false;
                }
            }
        }
        cout << "❌ " << item << " not found in inventory" << endl;
        return false;
    }
    
    int getGold() const { return gold; }
    const vector<string>& getInventory() const { return inventory; }
};

class Guardian : public Entity {
private:
    int protectionRadius;
    int alertLevel;
    string guardDuty;
    bool isOnDuty;
    
public:
    Guardian(const string& name, int x, int y, const string& duty = "Protect the village") 
        : Entity(name, 150, x, y), protectionRadius(10), alertLevel(0), guardDuty(duty), isOnDuty(true) {
        cout << "Guardian " << name << " takes watch to " << duty << endl;
    }
    
    ~Guardian() {
        cout << "Guardian " << name << " ends their watch" << endl;
    }
    
    void interact() const override {
        cout << "🛡️ " << name << " the Guardian stands vigilant." << endl;
        cout << "   \"I am here to " << guardDuty << ".\"" << endl;
        if (isOnDuty) {
            cout << "   \"Stay safe, traveler. Danger lurks nearby.\"" << endl;
        }
    }
    
    string getType() const override {
        return "Guardian";
    }
    
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "  Duty: " << guardDuty << endl;
        cout << "  Protection Radius: " << protectionRadius << endl;
        cout << "  Alert Level: " << alertLevel << "/10" << endl;
        cout << "  On Duty: " << (isOnDuty ? "Yes" : "No") << endl;
    }
    
    void protect(const Entity& protectedEntity) {
        double distance = distanceTo(protectedEntity);
        if (distance <= protectionRadius) {
            cout << "🛡️ " << name << " protects " << protectedEntity.getName() << endl;
            cout << "   \"You are under my protection!\"" << endl;
        } else {
            cout << name << " is too far away to protect " << protectedEntity.getName() << endl;
        }
    }
    
    void alert(int level) {
        alertLevel = min(10, alertLevel + level);
        cout << "🚨 " << name << " raises alert level to " << alertLevel << endl;
        if (alertLevel >= 8) {
            cout << "   \"DANGER! All citizens take cover!\"" << endl;
        }
    }
    
    int getProtectionRadius() const { return protectionRadius; }
    int getAlertLevel() const { return alertLevel; }
};

// ========================================
// STEP 3: Advanced Entity - Dragon
// ========================================

class Dragon : public Entity {
private:
    int fireBreathDamage;
    int treasureHoard;
    bool isAngry;
    
public:
    Dragon(const string& name, int x, int y) 
        : Entity(name, 300, x, y), fireBreathDamage(50), treasureHoard(1000), isAngry(false) {
        cout << "🐉 Ancient Dragon " << name << " awakens from slumber!" << endl;
    }
    
    ~Dragon() {
        cout << "🐉 Dragon " << name << " returns to eternal rest" << endl;
    }
    
    void interact() const override {
        cout << "🐉 " << name << " the Ancient Dragon regards you with burning eyes." << endl;
        if (isAngry) {
            cout << "   \"ROAAAAR! You dare disturb my slumber?!\"" << endl;
        } else {
            cout << "   \"Mortal... speak quickly before I lose patience.\"" << endl;
        }
    }
    
    string getType() const override {
        return "Dragon (Ancient)";
    }
    
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "  Fire Breath Damage: " << fireBreathDamage << endl;
        cout << "  Treasure Hoard: " << treasureHoard << " gold" << endl;
        cout << "  Angry: " << (isAngry ? "Yes" : "No") << endl;
    }
    
    void breatheFire(Entity& target) {
        cout << "🔥 " << name << " breathes scorching fire at " << target.getName() << "!" << endl;
        target.takeDamage(fireBreathDamage);
    }
    
    void getAngry() {
        isAngry = true;
        fireBreathDamage += 25;
        cout << "😡 " << name << " becomes enraged! Fire breath power increases!" << endl;
    }
};

// ========================================
// POLYMORPHIC SYSTEM FUNCTIONS - SOLUTION
// ========================================

void runGameWorld(const vector<unique_ptr<Entity>>& entities) {
    cout << "\n🌍 === GAME WORLD SIMULATION === 🌍" << endl;
    cout << "Entities in the world: " << entities.size() << endl;
    
    for (const auto& entity : entities) {
        cout << "\n--- " << entity->getType() << " ---" << endl;
        entity->interact();
        cout << endl;
    }
    
    cout << "🌍 === SIMULATION COMPLETE === 🌍" << endl;
}

void enhancedInteraction(Entity* entity) {
    cout << "\n🔍 === ENHANCED INTERACTION === 🔍" << endl;
    
    // Polymorphic call
    entity->interact();
    entity->displayInfo();
    
    cout << "\nRTTI Analysis:" << endl;
    cout << "TypeID: " << typeid(*entity).name() << endl;
    
    // Dynamic casting to identify specific types
    if (auto* player = dynamic_cast<Player*>(entity)) {
        cout << "🗡️ Player-specific operations:" << endl;
        cout << "   Level: " << player->getLevel() << endl;
        cout << "   Class: " << player->getPlayerClass() << endl;
    }
    else if (auto* enemy = dynamic_cast<Enemy*>(entity)) {
        cout << "👹 Enemy-specific operations:" << endl;
        cout << "   Enemy type: " << enemy->getEnemyType() << endl;
        cout << "   Damage potential: " << enemy->getDamage() << endl;
    }
    else if (auto* merchant = dynamic_cast<Merchant*>(entity)) {
        cout << "💰 Merchant-specific operations:" << endl;
        merchant->showInventory();
    }
    else if (auto* guardian = dynamic_cast<Guardian*>(entity)) {
        cout << "🛡️ Guardian-specific operations:" << endl;
        cout << "   Alert level: " << guardian->getAlertLevel() << "/10" << endl;
    }
    else if (auto* dragon = dynamic_cast<Dragon*>(entity)) {
        cout << "🐉 Dragon-specific operations:" << endl;
        cout << "   Ancient and powerful beyond measure!" << endl;
    }
    else if (auto* npc = dynamic_cast<NPC*>(entity)) {
        cout << "💬 NPC-specific operations:" << endl;
        cout << "   Has quest: " << (npc->getHasQuest() ? "Yes" : "No") << endl;
    }
    
    cout << "🔍 === INTERACTION COMPLETE === 🔍" << endl;
}

void simulateGameEvents(vector<unique_ptr<Entity>>& entities) {
    cout << "\n⚔️ === GAME EVENT SIMULATION === ⚔️" << endl;
    
    // Find entities of specific types
    Player* player = nullptr;
    Enemy* enemy = nullptr;
    Merchant* merchant = nullptr;
    NPC* npc = nullptr;
    
    for (const auto& entity : entities) {
        if (!player) player = dynamic_cast<Player*>(entity.get());
        if (!enemy) enemy = dynamic_cast<Enemy*>(entity.get());
        if (!merchant) merchant = dynamic_cast<Merchant*>(entity.get());
        if (!npc) npc = dynamic_cast<NPC*>(entity.get());
    }
    
    // Combat simulation
    if (player && enemy && player->isAlive() && enemy->isAlive()) {
        cout << "\n🥊 Combat Encounter:" << endl;
        player->attack(*enemy);
        if (enemy->isAlive()) {
            enemy->attack(*player);
        }
    }
    
    // Trading simulation
    if (player && merchant) {
        cout << "\n🛒 Trading Encounter:" << endl;
        merchant->showInventory();
        // Simulate player buying something
        merchant->sellItem("Health Potion", *player, 100);
    }
    
    // Quest giving simulation
    if (player && npc) {
        cout << "\n📜 Quest Encounter:" << endl;
        npc->giveQuest(*player);
    }
    
    cout << "⚔️ === EVENTS COMPLETE === ⚔️" << endl;
}

// ========================================
// MAIN FUNCTION - COMPREHENSIVE DEMONSTRATION
// ========================================

int main() {
    cout << "=== Polymorphic System Architecture Implementation - SOLUTION ===" << endl;
    
    // ========================================
    // STEP 1: Basic Entity Creation and Testing
    // ========================================
    cout << "\n--- Step 1: Basic Polymorphism Testing ---" << endl;
    
    Player hero("Aragorn", 0, 0, "Ranger");
    Enemy orc("Ugluk", 80, 5, 3, 15, "Orc");
    NPC villager("Farmer Joe", -2, -1, "The harvest was good this year!", true);
    
    cout << "\nDirect method calls:" << endl;
    hero.interact();
    orc.interact();
    villager.interact();
    
    // ========================================
    // STEP 2: Polymorphic Container System
    // ========================================
    cout << "\n--- Step 2: Polymorphic Container System ---" << endl;
    
    // Create game world with diverse entities
    vector<unique_ptr<Entity>> gameWorld;
    gameWorld.push_back(make_unique<Player>("Legolas", 1, 1, "Elf Archer"));
    gameWorld.push_back(make_unique<Enemy>("Goblin Scout", 40, 8, 2, 12, "Goblin"));
    gameWorld.push_back(make_unique<NPC>("Village Elder", -5, 0, "Welcome to our village, brave one!", true));
    gameWorld.push_back(make_unique<Merchant>("Trader Tom", -1, 5, 750));
    gameWorld.push_back(make_unique<Guardian>("Sir Gallant", 0, -3, "Defend the innocent"));
    gameWorld.push_back(make_unique<Dragon>("Smaug", 10, 10));
    
    // Demonstrate polymorphic behavior
    runGameWorld(gameWorld);
    
    // ========================================
    // STEP 3: RTTI and Dynamic Binding
    // ========================================
    cout << "\n--- Step 3: RTTI and Dynamic Binding ---" << endl;
    
    // Test enhanced interactions
    for (const auto& entity : gameWorld) {
        enhancedInteraction(entity.get());
        cout << "\n" << string(50, '-') << endl;
    }
    
    // Simulate complex game events
    simulateGameEvents(gameWorld);
    
    // ========================================
    // ADVANCED: Open/Closed Principle Demonstration
    // ========================================
    cout << "\n--- Advanced: Open/Closed Principle Demonstration ---" << endl;
    
    cout << "Adding new Dragon entity to existing system..." << endl;
    gameWorld.push_back(make_unique<Dragon>("Bahamut", 15, 15));
    
    cout << "New entity integrates seamlessly with existing polymorphic functions:" << endl;
    enhancedInteraction(gameWorld.back().get());
    
    // ========================================
    // ARCHITECTURE ANALYSIS
    // ========================================
    cout << "\n--- Architecture Analysis ---" << endl;
    
    cout << "\nPolymorphic System Statistics:" << endl;
    cout << "• Total entities in world: " << gameWorld.size() << endl;
    
    // Count entity types using RTTI
    int playerCount = 0, enemyCount = 0, npcCount = 0, merchantCount = 0, guardianCount = 0, dragonCount = 0;
    
    for (const auto& entity : gameWorld) {
        if (dynamic_cast<Player*>(entity.get())) playerCount++;
        else if (dynamic_cast<Enemy*>(entity.get())) enemyCount++;
        else if (dynamic_cast<NPC*>(entity.get())) npcCount++;
        else if (dynamic_cast<Merchant*>(entity.get())) merchantCount++;
        else if (dynamic_cast<Guardian*>(entity.get())) guardianCount++;
        else if (dynamic_cast<Dragon*>(entity.get())) dragonCount++;
    }
    
    cout << "• Players: " << playerCount << endl;
    cout << "• Enemies: " << enemyCount << endl;
    cout << "• NPCs: " << npcCount << endl;
    cout << "• Merchants: " << merchantCount << endl;
    cout << "• Guardians: " << guardianCount << endl;
    cout << "• Dragons: " << dragonCount << endl;
    
    cout << "\nArchitecture Principles Demonstrated:" << endl;
    cout << "✓ Abstract base class with pure virtual functions" << endl;
    cout << "✓ Polymorphic behavior through inheritance" << endl;
    cout << "✓ Dynamic binding with virtual function dispatch" << endl;
    cout << "✓ RTTI for runtime type identification and safe casting" << endl;
    cout << "✓ Open/Closed Principle - extensible without core modification" << endl;
    cout << "✓ Smart pointers for automatic memory management" << endl;
    cout << "✓ Container-based polymorphism for scalable systems" << endl;
    
    cout << "\n🎮 Game world simulation complete! All entities managed polymorphically. 🎮" << endl;
    
    return 0;
}

/*
SOLUTION HIGHLIGHTS:

STEP 1: Abstract Base Class Design
✓ Pure virtual functions make Entity abstract
✓ Virtual destructor ensures proper cleanup
✓ Protected members allow derived class access
✓ Common functionality shared across all entities
✓ Complete Player, Enemy, and NPC implementations

STEP 2: Polymorphic System Architecture
✓ Container-based polymorphism with smart pointers
✓ Extended entity types (Merchant, Guardian, Dragon)
✓ Demonstrates Open/Closed Principle
✓ Runtime method resolution through virtual dispatch
✓ Automatic memory management with unique_ptr

STEP 3: Dynamic Binding and RTTI
✓ Safe downcasting with dynamic_cast
✓ Type identification using typeid operator
✓ Complex entity interaction system
✓ Runtime type analysis and statistics
✓ Practical RTTI usage in game scenarios

ADVANCED FEATURES:
✓ Complex entity interactions (combat, trading, quests)
✓ Multi-level inheritance hierarchy
✓ Professional game architecture patterns
✓ Statistical analysis using RTTI
✓ Extensible design supporting new entity types

DESIGN PATTERNS DEMONSTRATED:
✓ Strategy Pattern (different interaction behaviors)
✓ Template Method Pattern (common entity lifecycle)
✓ Factory Pattern potential (entity creation)
✓ Observer Pattern potential (entity interactions)

REAL-WORLD APPLICATIONS:
✓ Game engine entity systems
✓ Plugin architectures with common interfaces
✓ GUI framework widget hierarchies
✓ Business logic with different process types
✓ Media processing with various formats

This solution demonstrates professional-level polymorphic
architecture suitable for complex systems requiring runtime
flexibility, extensibility, and maintainable code organization.
*/