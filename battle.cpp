#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>
#include <limits>

using namespace std;

// 🔹 Input Validation
int getValidatedChoice(int min, int max) {
    int choice;

    while (true) {
        cout << " Please select an option (" << min << "-" << max << "): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid input! Enter numbers only.\n";
            continue;
        }

        if (choice < min || choice > max) {
            cout << " Invalid choice! Try again.\n";
            continue;
        }

        return choice;
    }
}

// 🔹 Base Class
class Character {
protected:
    int health;
    int attackPower;

public:
    Character(int h, int a) : health(h), attackPower(a) {}

    int getHealth() { return health; }
    void setHealth(int h) { health = h; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
    }

    virtual int attack() {
        return attackPower;
    }

    virtual ~Character() {}
};

// 🔹 Player Class
class Player : public Character {
private:
    int healCount;

public:
    Player() : Character(100, 20), healCount(2) {}

    int getHealCount() { return healCount; }
    void setHealCount(int h) { healCount = h; }

    int specialAttack() {
        int chance = rand() % 2;
        if (chance == 1) {
            cout << " Special Attack Success!\n";
            return 2 * attackPower;
        }
        cout << " Special Attack Failed!\n";
        return 0;
    }

    int defend(int damage) {
        return damage / 2;
    }

    void useHeal() {
        if (healCount > 0) {
            heal(20);
            healCount--;
            cout << " Healed! Remaining heals: " << healCount << endl;
        } else {
            cout << " No heals left!\n";
        }
    }
};

// 🔹 Enemy Base
class Enemy : public Character {
public:
    Enemy(int h, int a) : Character(h, a) {}
};

class NewbieEnemy : public Enemy {
public:
    NewbieEnemy() : Enemy(80, 10) {}
};

class MediumEnemy : public Enemy {
public:
    MediumEnemy() : Enemy(100, 15) {}

    int attack() override {
        return 10 + rand() % 11;
    }
};

class BossEnemy : public Enemy {
public:
    BossEnemy() : Enemy(150, 20) {}

    int attack() override {
        if (rand() % 5 == 0) {
            cout << " Critical Hit!\n";
            return 2 * attackPower;
        }
        return attackPower;
    }
};

// 🔹 Enemy Factory
unique_ptr<Enemy> createEnemy(int level) {
    if (level == 1) return make_unique<NewbieEnemy>();
    if (level == 2) return make_unique<MediumEnemy>();
    return make_unique<BossEnemy>();
}

// 🔹 Save Game
void saveGame(Player &p, int level) {
    ofstream file("save.txt");

    if (!file.is_open()) {
        cout << " Error saving game!\n";
        return;
    }

    file << p.getHealth() << endl;
    file << p.getHealCount() << endl;
    file << level << endl;

    file.close();

    cout << " Game Saved Successfully\n";
}

// 🔹 Load Game
bool loadGame(Player &p, int &level) {
    ifstream file("save.txt");

    if (!file.is_open()) {
        return false;
    }

    int h, hc;

    if (!(file >> h >> hc >> level)) {
        cout << " Corrupted save file!\n";
        file.close();
        return false;
    }

    p.setHealth(h);
    p.setHealCount(hc);

    file.close();

    cout << " Game Loaded Successfully\n";
    return true;
}

// 🔹 Battle System
bool battle(Player &player, Enemy* enemy) {

    while (player.getHealth() > 0 && enemy->getHealth() > 0) {

        cout << "\n====================================\n";
        cout << "Player HP: " << player.getHealth()
             << " | Heals: " << player.getHealCount() << endl;
        cout << "Enemy  HP: " << enemy->getHealth() << endl;
        cout << "====================================\n";

        cout << "1. Attack\n";
        cout << "2. Defend\n";
        cout << "3. Special Attack\n";
        cout << "4. Heal\n";
        cout << "5. Quit Battle\n";

        int choice = getValidatedChoice(1, 5);
        bool defendMode = false;

        switch (choice) {
        case 1:
            enemy->takeDamage(player.attack());
            break;
        case 2:
            defendMode = true;
            break;
        case 3:
            enemy->takeDamage(player.specialAttack());
            break;
        case 4:
            player.useHeal();
            break;
        case 5:
            cout << " You exited the battle.\n";
            return false;
        }

        // Enemy Turn
        if (enemy->getHealth() > 0) {
            int dmg = enemy->attack();
            if (defendMode) dmg = player.defend(dmg);

            cout << " Enemy deals " << dmg << " damage\n";
            player.takeDamage(dmg);
        }
    }

    if (player.getHealth() <= 0) {
        cout << " You Lost!\n";
        return false;
    }

    cout << " Enemy Defeated!\n";
    return true;
}

// 🔹 Creative Mode
void creativeMode(Player &player) {
    while (player.getHealth() > 0) {

        cout << "\n===== SELECT ENEMY =====\n";
        cout << "1. Newbie Enemy\n";
        cout << "2. Medium Enemy\n";
        cout << "3. Boss Enemy\n";

        int level = getValidatedChoice(1, 3);

        auto enemy = createEnemy(level);

        if (!battle(player, enemy.get())) break;

        // 🔹 Auto Save (level same as selected)
        saveGame(player, level);
    }
}

// 🔹 Manual Mode
void manualMode(Player &player, int startLevel) {
    for (int level = startLevel; level <= 3; level++) {

        cout << "\n=========== LEVEL " << level << " ===========\n";

        auto enemy = createEnemy(level);

        if (!battle(player, enemy.get())) {
        cout << " Game Over or Quit\n";
        return;
    }

    player.heal(10);

    // 🔹 Auto Save after each battle
    if (level < 3)
        saveGame(player, level + 1);
    else
        saveGame(player, 3);
    }

    cout << " You Won the Game!\n";
}

// 🔹 Main
int main() {
    srand(time(0));

    Player player;
    int level = 1;

    cout << "\n========= BATTLE ARENA =========\n";
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. Quit\n";

    int choice = getValidatedChoice(1, 3);

    if (choice == 3) return 0;

    if (choice == 2) {
        if (!loadGame(player, level)) {
            cout << " No valid save found. Starting new game.\n";
            level = 1;
        }
    }

    cout << "\n========= SELECT MODE =========\n";
    cout << "1. Creative Mode\n";
    cout << "2. Manual Mode\n";

    int mode = getValidatedChoice(1, 2);

    if (mode == 1)
        creativeMode(player);
    else
        manualMode(player, level);

    cout << "\n Thanks for playing!\n";
    return 0;
}
