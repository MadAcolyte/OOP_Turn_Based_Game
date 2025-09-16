#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;


// ================================== GLOBAL SETTINGS =======================================

const vector<pair<string, double>> difficultyOptions = {
    {"Easy",   0.8},
    {"Normal", 1.0},
    {"Hard",   1.2}
};
int currentDifficultyIndex = 1;
int responseTimeMS = 2500;
const string filename = "save.txt";

#define WHITE 7
#define GREEN 10
#define RED 12

// ================================== HELPER FUNCTIONS =======================================


void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void clearScreen()
{
    system("cls");
}

int computeEffectiveDamage(int rawDamage, int targetDefense)
{
    int effDmg = rawDamage - (targetDefense / 2);
    if (effDmg < 1) return 1;
    else return effDmg;
}

int inputValidationFunction(const string& prompt, int min, int& max)
{
    int value;
    while (true)
    {
        cout << prompt << endl;
        if (min != max)
            cout << "min: " << min << " max: " << max;
        cout << ": ";
        cin >> value;
        if (cin.fail() || value < min || value > max)
        {
            cout << "Invalid input. Must be an integer between "
                << min << " and " << max << ".\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        else
        {
            cin.ignore(INT_MAX, '\n');
            return value;
        }
    }
}

int pickGreater(int a, int b)
{
    if (a > b) return a;
    else return b;
}

string difficultyToString()
{
    return difficultyOptions[currentDifficultyIndex].first;
}

double difficultyMultiplier()
{
    return difficultyOptions[currentDifficultyIndex].second;
}


//=================================== CLASS SECTION ============================================
class person {
protected:
    string name;
    struct stats {
        int healthPoints;
        int damage;
        int defense;
        int maxHealth;
        stats(int hp, int dmg, int def) : healthPoints(hp), damage(dmg), defense(def), maxHealth(100) {}
    };
    stats playerStats;
public:
    person() : name("Sahur"), playerStats(100, 10, 20) {};
    virtual ~person() {};

    // ============== Methods ================= 

    void setName(string a)
    {
        name = a;
    }

    void setStats(int hp, int dmg, int def)
    {
        playerStats.healthPoints = hp;
        playerStats.damage = dmg;
        playerStats.defense = def;
    }

    bool isAlive()
    {
        return (playerStats.healthPoints > 0);
    }

    string getName()
    {
        return name;
    }

    int getHP()
    {
        return playerStats.healthPoints;
    }

    const int getMaxHP()
    {
        return playerStats.maxHealth;
    }

    int getDamage()
    {
        return playerStats.damage;
    }

    int getDefense()
    {
        return playerStats.defense;
    }

    void heal()
    {
        playerStats.healthPoints += (playerStats.maxHealth - playerStats.healthPoints) * 30 / 100;
        if (playerStats.healthPoints > playerStats.maxHealth)
            playerStats.healthPoints = playerStats.maxHealth;
    }

    void operator -=(int rawDamage)
    {
        playerStats.healthPoints -= computeEffectiveDamage(rawDamage, playerStats.defense);
    }

    int attack()
    {
        return playerStats.damage;
    }
};

class player : public person {
private:
    struct equipment {
        int healingPotion;
        pair <string, int> weapon;
        pair <string, int> armor;
        pair <string, int> helmet;
        pair <string, int> boots;
        equipment(int he, pair <string, int> w, pair <string, int> a, pair <string, int> h, pair <string, int>  b)
            : healingPotion(he), weapon(w), armor(a), helmet(h), boots(b) {
        };
    };
    equipment playersEquipment;
public:
    player() : playersEquipment(5, { "basic_sword", 0 }, { "basic_armor", 0 }, { "basic_helmet", 0 }, { "basic_boots",0 }) {};
    ~player() {};

    void createPlayer()
    {
        int hp,
            dmg,
            def,
            remainingPoints = 98;

        // Request a name
        cout << "Please, enter name: ";
        getline(cin >> ws, name);
        if (name.size() > 15) name = "Hacker";
        cout << endl << "You have 100 points to distribute between your stats (health points, damage, defense)." << endl;

        // Distribute health points
        hp = inputValidationFunction("Please, enter health points: ", 1, remainingPoints);
        remainingPoints += 2;
        remainingPoints -= hp;

        // Distribute damage
        dmg = inputValidationFunction("Please, enter damage : ", 1, remainingPoints);
        remainingPoints -= dmg;

        // Automatic defense points distribution
        def = remainingPoints;
        cout << "Remaining " << remainingPoints << " points assigned to defense automatically." << endl;

        if (name == "god") setStats(999, 999, 999);
        else if (name == "10010bonus") setStats(18, 1818, 0);
        else if (name == "Longintas") setStats(7, 7, 7);
        else if (name == "NeedHealing!") setStats(1, 150, 1);
        else setStats(hp, dmg, def);
    }

    int getPotionAmount()
    {
        return playersEquipment.healingPotion;
    }

    void operator --()
    {
        playersEquipment.healingPotion--;
    }

    void operator ++()
    {
        playersEquipment.healingPotion++;
    }

    void updateEquipment(const string parameter, pair <string, int>& equipment)
    {
        if (parameter == "weapon")
        {
            playerStats.damage -= playersEquipment.weapon.second;
            equipment.second = pickGreater(equipment.second, playersEquipment.weapon.second);
            playersEquipment.weapon = equipment;
            playerStats.damage += equipment.second;
        }
        else if (parameter == "armor")
        {
            playerStats.defense -= playersEquipment.armor.second;
            equipment.second = pickGreater(equipment.second, playersEquipment.armor.second);
            playersEquipment.armor = equipment;
            playerStats.defense += equipment.second;
        }
        else if (parameter == "helmet")
        {
            playerStats.defense -= playersEquipment.helmet.second;
            equipment.second = pickGreater(equipment.second, playersEquipment.helmet.second);
            playersEquipment.helmet = equipment;
            playerStats.defense += equipment.second;
        }
        else if (parameter == "boots")
        {
            playerStats.defense -= playersEquipment.boots.second;
            equipment.second = pickGreater(equipment.second, playersEquipment.boots.second);
            playersEquipment.boots = equipment;
            playerStats.defense += equipment.second;
        }
        else return;
    }

    pair <string, int> getWeapon()
    {
        return playersEquipment.weapon;
    }

    pair <string, int> getArmor()
    {
        return playersEquipment.armor;
    }

    pair <string, int> getHelmet()
    {
        return playersEquipment.helmet;
    }
    pair <string, int> getBoots()
    {
        return playersEquipment.boots;
    }
};

class enemyNPC : public person {
private:
    int HP_start, HP_end, DMG_start, DMG_end, DEF_start, DEF_end, healingPotion;
public:
    enemyNPC() : HP_start(90), HP_end(120), DMG_start(15), DMG_end(25), DEF_start(10), DEF_end(15), healingPotion(6) {};
    ~enemyNPC() {};

    void createEnemy()
    {
        vector<string> adjectives = {
        "Dark", "Savage", "Ancient", "Elite", "Corrupted", "Feral", "Bloodthirsty",
        "Cursed", "Frost", "Venomous", "Shadow", "Fire", "Undead"
        };
        vector<string> baseNames = {
            "Goblin", "Orc", "Knight", "Wraith", "Zombie", "Beast", "Warrior", "Lich", "Giant",
            "Vampire", "Golem", "Troll", "Ogre"
        };
        name = adjectives[rand() % adjectives.size()] + " " + baseNames[rand() % baseNames.size()];

        int he = rand() % (HP_end - HP_start + 1) + HP_start;
        int da = rand() % (DMG_end - DMG_start + 1) + DMG_start;
        int de = rand() % (DEF_end - DEF_start + 1) + DEF_start;
        setStats(he * difficultyMultiplier(), da * difficultyMultiplier(), de * difficultyMultiplier());

        HP_start += 20;
        HP_end += 20;
        DMG_start += 10;
        DMG_end += 10;
        DEF_start += 5;
        DEF_end += 5;
    }

    void reduceHealingPotionAmount()
    {
        healingPotion--;
    }

    int getHealingPotionAmount()
    {
        return healingPotion;
    }


    void enemyAI(vector<player>& Hero)
    {
        // Phase 1: Look for kill shot opportunity.
        int killTargetIndex = -1;
        for (int i = 0; i < Hero.size(); i++) {
            if (!Hero[i].isAlive())
                continue;
            // Calculate effective damage against hero[i]
            int effDmg = computeEffectiveDamage(playerStats.damage, Hero[i].getDefense());
            if (Hero[i].getHP() <= effDmg) {
                killTargetIndex = i;
                break;
            }
        }
        if (killTargetIndex != -1)
        {
            setColor(RED);
            cout << name << " executes a kill shot on " << Hero[killTargetIndex].getName()
                << "!" << endl;
            Sleep(responseTimeMS);
            Hero[killTargetIndex] -= playerStats.damage;
            setColor(WHITE);
            return;
        }

        // Phase 2: If enemy is low on health, heal.
        if (playerStats.healthPoints <= playerStats.maxHealth / 2 && healingPotion > 0)
        {
            setColor(RED);
            cout << name << " chooses to heal." << endl;
            Sleep(responseTimeMS);
            heal();
            reduceHealingPotionAmount();
            setColor(WHITE);
            return;
        }

        // Phase 3: Otherwise attack the weakest hero.
        int minHP = INT_MAX;
        int targetIndex = -1;
        for (int i = 0; i < Hero.size(); i++)
        {
            if (!Hero[i].isAlive())
                continue;
            if (Hero[i].getHP() < minHP)
            {
                minHP = Hero[i].getHP();
                targetIndex = i;
            }
        }
        if (targetIndex != -1)
        {
            setColor(RED);
            cout << name << " attacks " << Hero[targetIndex].getName() << "! Damage given: " << computeEffectiveDamage(getDamage(), Hero[targetIndex].getDefense()) << endl;
            Sleep(responseTimeMS);
            Hero[targetIndex] -= playerStats.damage;
            setColor(WHITE);
        }

    }
};

// ===================================================== FUNCTION PROTOTYPE SECTION ============================================================
void clearScreen();
void mainMenu(bool& running, int& playerNum, int& npcNum, vector <player>& Hero, vector < enemyNPC>& enemies, int& selected, int& enemyIndex, bool& exitFlag, bool& gameIsOn);
void battleMenu(bool& gameIsOn, bool& running, int& playerNum, int& npcNum, vector <player>& Hero, vector < enemyNPC>& enemies, int& selected, int& enemyIndex, bool& exitFlag);
void drawMenu(int selectedIndex);
void setupNewGame(int& playerNum, int& npcNum, vector<player>& hero, vector <enemyNPC>& enemy);
void drawBattleUI(player& hero, enemyNPC& enemy);
void drawBattleMenu(int selectedIndex, int potionAmount);
int getHeroChoice(player& hero, enemyNPC& enemy, vector <player> heroes);
void grantRandomLoot(player& p);
void printHeroList(vector<player>& heroes);
bool saveGame(const string& filename, vector<player>& heroes, vector<enemyNPC>& enemies, int enemyIndex);
bool loadGame(const string& filename, vector<player>& heroes, vector<enemyNPC>& enemies, int& enemyIndex);
void chooseDifficulty();
void setSettings();

// ========================================================= MAIN FUNCTION SECTION =============================================================
int main()
{
    srand(time(0));
    bool gameIsOn = true;
    int playerNum, npcNum = 1;
    int selected = 0;
    bool running = true, exitFlag = false;
    vector<player> Hero;
    vector<enemyNPC> enemies;
    int enemyIndex = 0;

    // Main Menu Loop
MAIN_MENU:
    mainMenu(running, playerNum, npcNum, Hero, enemies, selected, enemyIndex, exitFlag, gameIsOn);
    // Battle Loop (Phase-based)
    battleMenu(gameIsOn, running, playerNum, npcNum, Hero, enemies, selected, enemyIndex, exitFlag);
    if (exitFlag) return 0;
    else goto MAIN_MENU;
    return 0;
}


// ========================================================= FUNCTION SECTION ===============================================================

void mainMenu(bool& running, int& playerNum, int& npcNum, vector <player>& Hero, vector < enemyNPC>& enemies, int& selected, int& enemyIndex, bool& exitFlag, bool& gameIsOn)
{
    while (running)
    {
        clearScreen();
        cout << "========================================================================" << endl;
        cout << "=== Welcome to the turn-based fighting game created by Edvin Macevic ===" << endl;
        cout << "=== Navigate through menu using 'w' and 's' or 'up' and 'down' arrow ===" << endl;
        cout << "========================================================================" << endl;
        cout << "=== About game:                                                      ===" << endl;
        cout << "=== You should create a hero, with stats: health, damage and defense ===" << endl;
        cout << "=== After that you'll be send to dungeon to fight different monsters ===" << endl;
        cout << "=== Heroes atack first, then the turn goes to the monster            ===" << endl;
        cout << "=== You'll have 2 options: atack or heal                             ===" << endl;
        cout << "=== Good luck and have a nice game!                                  ===" << endl;
        cout << "========================================================================" << endl << endl;
        cout << "======Main menu======" << endl << endl;
        drawMenu(selected);

        char input = _getch();
        switch (input) {
        case 'w':
        case 'W':
        case 72:
            selected = (selected - 1 + 4) % 4;
            break;
        case 's':
        case 'S':
        case 80:
            selected = (selected + 1) % 4;
            break;
        case '\r':
            switch (selected) {
            case 0:
                setupNewGame(playerNum, npcNum, Hero, enemies);
                running = false;
                break;
            case 1:
                cout << "Loading saved game..." << endl;
                if (loadGame(filename, Hero, enemies, enemyIndex))
                    cout << "Your game was successfuly loaded!" << endl;
                else
                {
                    cout << "No saves were found!";
                    Sleep(responseTimeMS);
                }
                running = false;
                break;
            case 2:
                setSettings();
                break;
            case 3:
                cout << "Exiting..." << endl;
                running = false;
                exitFlag = true;
                gameIsOn = false;
                return;
                break;
            }
            break;
        }

    }
}
void battleMenu(bool& gameIsOn, bool& running, int& playerNum, int& npcNum, vector <player>& Hero, vector < enemyNPC>& enemies, int& selected, int& enemyIndex, bool& exitFlag)
{
    while (gameIsOn)
    {

        // PHASE A: Each hero takes action.
        for (int i = 0; i < Hero.size(); i++)
        {
            if (!Hero[i].isAlive())
                continue;
            clearScreen();
            drawBattleUI(Hero[i], enemies[enemyIndex]);


            int choice = getHeroChoice(Hero[i], enemies[enemyIndex], Hero);
            if (choice == 0)
            {
                setColor(GREEN);
                enemies[enemyIndex] -= Hero[i].attack();
                cout << Hero[i].getName() << " hit " << enemies[enemyIndex].getName() << " with " << computeEffectiveDamage(Hero[i].attack(), enemies[enemyIndex].getDefense()) << " damage!" << endl;
                Sleep(responseTimeMS);
                setColor(WHITE);
            }
            else if (choice == 1 && Hero[i].getPotionAmount() > 0)
            {
                setColor(GREEN);
                Hero[i].heal();
                --Hero[i];
                cout << Hero[i].getName() << " healed himself by 30% of missing health!" << endl;
                Sleep(responseTimeMS);
                setColor(WHITE);
            }
            else if (choice == 2)
            {
                clearScreen();
                if (saveGame(filename, Hero, enemies, enemyIndex))
                {
                    cout << "Your game has been saved!" << endl;
                    running = true;
                    return;
                }
                else {
                    cout << "There was an error with saving your progress :-(" << endl;
                    return;
                }

            }
            else {
                cout << "You have no healing potions left. Choose another action." << endl;
                i--; // Repeat this hero's turn
                Sleep(responseTimeMS); // Small delay for message visibility
            }
            if (!enemies[enemyIndex].isAlive())
            {
                cout << enemies[enemyIndex].getName() << " was killed!";
                break;
            }
        }

        // PHASE B: Check if the current enemy is defeated.
        if (!enemies[enemyIndex].isAlive())
        {

            for (int i = 0; i < Hero.size(); i++)
            {
                if (!Hero[i].isAlive()) continue;
                grantRandomLoot(Hero[i]);
                Sleep(responseTimeMS);
            }
            enemyIndex++;
            if (enemyIndex >= (enemies.size()))
            {
                cout << "Congratulations! You have defeated all monsters!" << endl;
                gameIsOn = false;
                exitFlag = true;
                break;
            }
            // Start next round immediately with the next enemy.
            continue;
        }

        // PHASE C: The enemy takes its turn.
        enemies[enemyIndex].enemyAI(Hero);

        // PHASE D: Remove dead heroes.
        for (int i = Hero.size() - 1; i >= 0; i--)
        {
            if (!Hero[i].isAlive()) {
                cout << Hero[i].getName() << " was killed :-(" << endl;
                Hero.erase(Hero.begin() + i);
            }
        }
        if (Hero.empty())
        {
            cout << "Game is over. All heroes have fallen." << endl;
            gameIsOn = false;
            exitFlag = true;
        }
    }
}
void drawMenu(int selectedIndex)
{
    string options[] = { "Start New Game", "Load Saves", "Settings", "Exit" };
    for (int i = 0; i < 4; ++i)
    {
        if (i == selectedIndex)
            setColor(GREEN);
        else
            setColor(WHITE);

        cout << (i == selectedIndex ? " > " : "   ") << options[i] << endl;
    }
    setColor(WHITE); // Reset color
}

void setupNewGame(int& playerNum, int& npcNum, vector<player>& hero, vector <enemyNPC>& enemy)
{
    char input;
    int maxPlayerNum = 5;
    cout << endl << "Starting new game..." << endl;
    playerNum = inputValidationFunction("Before you start, please enter a number of players(max 5) : ", 1, maxPlayerNum);
    hero.resize(playerNum);
    npcNum = playerNum * 2;
    enemy.resize(npcNum);
    for (int i = 0; i < playerNum; i++)
    {
        hero[i] = player();
        cout << "Perfect, let's create hero number " << i + 1 << endl;
        hero[i].createPlayer();
    }
    for (int i = 0; i < npcNum; i++)
    {
        enemy[i] = enemyNPC();
        enemy[i].createEnemy();
    }
    cout << "<< Press any button to continue >> " << endl;
    input = _getch();
    clearScreen();
}

void drawBattleUI(player& hero, enemyNPC& enemy)
{
    cout << "======== HERO ========                              ======== ENEMY ========" << endl;

    cout << left << setw(55) << ("Name: " + hero.getName())
        << "Name: " << enemy.getName() << endl;

    cout << left << setw(55) << ("HP: " + to_string(hero.getHP()) + "/" + to_string(hero.getMaxHP()))
        << "HP: " << enemy.getHP() << endl;

    cout << left << setw(55) << ("DMG: " + to_string(hero.getDamage()))
        << "DMG: " << enemy.getDamage() << endl;

    cout << left << setw(55) << ("DEF: " + to_string(hero.getDefense()))
        << "DEF: " << enemy.getDefense() << endl;
}


void drawBattleMenu(int selectedIndex, int potionAmount)
{
    string options[] = { "Attack", "Heal (" + to_string(potionAmount) + ")", "Save and exit" };
    for (int i = 0; i < 3; ++i)
    {
        if (i == selectedIndex)
            setColor(GREEN);
        else
            setColor(WHITE);

        cout << (i == selectedIndex ? " > " : "   ") << options[i] << endl;
    }
    setColor(WHITE);
}


int getHeroChoice(player& hero, enemyNPC& enemy, vector <player> heroes)
{
    int selected = 0;
    const int totalOptions = 3;
    char input;

    while (true)
    {
        clearScreen();
        printHeroList(heroes);
        drawBattleUI(hero, enemy);
        cout << endl;
        drawBattleMenu(selected, hero.getPotionAmount());
        cout << endl << "Battle information:" << endl << endl;

        input = _getch();
        switch (input) {
        case 'w':
        case 'W':
        case 72:
            selected = (selected - 1 + totalOptions) % totalOptions;
            break;
        case 's':
        case 'S':
        case 80:
            selected = (selected + 1) % totalOptions;
            break;
        case '\r':
            return selected;
        }
    }
}

void grantRandomLoot(player& p) {
    int roll = rand() % 100 + 1;

    if (roll <= 30) {
        cout << p.getName() << " found a healing potion!" << endl;
        ++p;
        return;
    }

    // Determine rarity
    int rarityRoll = rand() % 100 + 1;
    int bonus = 0;
    vector<string> adjectives = {
                "Ancient", "Celestial", "Cursed", "Divine", "Doomforged", "Draconic",
                "Eternal", "Fallen", "Flamebound", "Frostwoven", "Gilded", "Haunted",
                "Infernal", "Ironclad", "Luminous", "Mystic", "Nightshade", "Obsidian",
                "Phantom", "Runed", "Sacred", "Savage", "Shadowsteel", "Soulbound",
                "Starlit", "Stormforged", "Titanic", "Twilight", "Venomous", "Voidborn"
    };
    string adj = adjectives[rand() % adjectives.size()];
    if (rarityRoll <= 50)
    {
        bonus = 4;
    }
    else if (rarityRoll <= 80)
    {
        bonus = 8;
    }
    else if (rarityRoll <= 95)
    {
        bonus = 12;
    }
    else bonus = 16;

    // Pick a type to drop
    int type = rand() % 4;
    pair <string, int> item;
    switch (type) {
    case 0:
        item = { adj + " Blade", bonus };
        p.updateEquipment("weapon", item);
        cout << p.getName() << " received a new weapon: " << item.first << " (+" << bonus << " DMG)" << endl;
        break;
    case 1:
        item = { adj + " Armor", bonus };
        p.updateEquipment("armor", item);
        cout << p.getName() << " received armor: " << item.first << " (+" << bonus << " DEF)" << endl;
        break;
    case 2:
        item = { adj + " Helmet", bonus };
        p.updateEquipment("helmet", item);
        cout << p.getName() << " received a helmet: " << item.first << " (+" << bonus << " DEF)" << endl;
        break;
    case 3:
        item = { adj + " Boots", bonus };
        p.updateEquipment("boots", item);
        cout << p.getName() << " received boots: " << item.first << " (+" << bonus << " DEF)" << endl;
        break;
    }
}

void printHeroList(vector<player>& heroes)
{
    cout << "Players in session: ";
    for (int i = 0; i < heroes.size(); i++)
    {
        cout << heroes[i].getName();
        if (i != heroes.size() - 1) cout << ", ";
    }
    cout << endl;
}

bool saveGame(const string& filename, vector<player>& heroes, vector<enemyNPC>& enemies, int enemyIndex)
{
    ofstream ofs(filename);
    if (!ofs) return false;

    // 1) Current enemy index (i.e. which enemy is next)
    ofs << enemyIndex << '\n';

    // 2) Heroes count & data
    ofs << heroes.size() << '\n';
    for (auto& h : heroes)
    {
        ofs << h.getName() << '\n';
        ofs << h.getHP() << ' ' << h.getDamage() - h.getWeapon().second << ' ' << h.getDefense() - h.getArmor().second - h.getHelmet().second - h.getBoots().second
            << ' ' << h.getPotionAmount() << '\n';
        // Equipment
        ofs << h.getWeapon().first << ' ' << h.getWeapon().second << '\n';
        ofs << h.getArmor().first << ' ' << h.getArmor().second << '\n';
        ofs << h.getHelmet().first << ' ' << h.getHelmet().second << '\n';
        ofs << h.getBoots().first << ' ' << h.getBoots().second << '\n';
    }

    // 3) Enemies count & data
    ofs << enemies.size() << '\n';
    for (auto& e : enemies)
    {
        ofs << e.getName() << '\n';
        ofs << e.getHP() << ' ' << e.getDamage() << ' ' << e.getDefense()
            << ' ' << e.getHealingPotionAmount() << '\n';
    }
    ofs << currentDifficultyIndex << endl;
    ofs << responseTimeMS << endl;
    return true;
}

bool loadGame(const string& filename, vector<player>& heroes, vector<enemyNPC>& enemies, int& enemyIndex)
{
    ifstream ifs(filename);
    if (!ifs) return false;

    int heroCount;
    ifs >> enemyIndex >> heroCount;
    if (ifs.fail()) return false;
    ifs.ignore(INT_MAX, '\n');


    heroes.clear();
    heroes.resize(heroCount);
    for (int i = 0; i < heroCount; ++i)
    {

        string name;
        getline(ifs, name);

        int hp, dmg, def, potions;
        ifs >> hp >> dmg >> def >> potions;
        ifs.ignore(INT_MAX, '\n');
        heroes[i].setName(name);
        heroes[i].setStats(hp, dmg, def);

        while (heroes[i].getPotionAmount() < potions) ++heroes[i];
        while (heroes[i].getPotionAmount() > potions) --heroes[i];

        unsigned int space;
        pair <string, int> item;
        getline(ifs, item.first);
        space = item.first.rfind(' ');
        item.second = stoi(item.first.substr(space + 1));
        item.first = item.first.substr(0, space);
        heroes[i].updateEquipment("weapon", item);
        getline(ifs, item.first);
        space = item.first.rfind(' ');
        item.second = stoi(item.first.substr(space + 1));
        item.first = item.first.substr(0, space);
        heroes[i].updateEquipment("armor", item);
        getline(ifs, item.first);
        space = item.first.rfind(' ');
        item.second = stoi(item.first.substr(space + 1));
        item.first = item.first.substr(0, space);
        heroes[i].updateEquipment("helmet", item);
        getline(ifs, item.first);
        space = item.first.rfind(' ');
        item.second = stoi(item.first.substr(space + 1));
        item.first = item.first.substr(0, space);
        heroes[i].updateEquipment("boots", item);
    }


    int enemyCount;
    ifs >> enemyCount;
    if (ifs.fail()) return false;
    ifs.ignore(INT_MAX, '\n');

    enemies.clear();
    enemies.resize(enemyCount);
    for (int i = 0; i < enemyCount; ++i)
    {

        string name;
        getline(ifs, name);


        int hp, dmg, def, potions;
        ifs >> hp >> dmg >> def >> potions;
        ifs.ignore(INT_MAX, '\n');
        enemies[i].setName(name);
        enemies[i].setStats(hp, dmg, def);

        while (enemies[i].getHealingPotionAmount() < potions)
            enemies[i].reduceHealingPotionAmount();
        while (enemies[i].getHealingPotionAmount() > potions)
            enemies[i].reduceHealingPotionAmount();
    }
    ifs >> currentDifficultyIndex;
    ifs >> responseTimeMS;
    return true;
}

void chooseDifficulty()
{
    int selected = currentDifficultyIndex;
    const int totalOptions = difficultyOptions.size();
    while (true) {
        clearScreen();
        cout << "=== Select Difficulty ===" << endl;
        for (int i = 0; i < totalOptions; i++) {
            setColor(i == selected ? GREEN : WHITE);
            cout << (i == selected ? " > " : "   ")
                << difficultyOptions[i].first << endl;
        }
        setColor(WHITE);

        char input = _getch();
        switch (input) {
        case 'w': case 'W': case 72:
            selected = (selected - 1 + totalOptions) % totalOptions;
            break;
        case 's': case 'S': case 80:
            selected = (selected + 1) % totalOptions;
            break;
        case '\r':
            currentDifficultyIndex = selected;
            return;
        }
    }
}

void setSettings() {
    int selected = 0, maximum = 10000;
    const int totalOptions = 3;
    while (true) {
        clearScreen();
        cout << "=== Settings ===" << endl;
        const string options[3] = {
            "Difficulty: " + difficultyToString(),
            "Response Time: " + to_string(responseTimeMS) + " ms",
            "Return to Main Menu"
        };
        for (int i = 0; i < totalOptions; i++) {
            setColor(i == selected ? GREEN : WHITE);
            cout << (i == selected ? " > " : "   ") << options[i] << endl;
        }
        setColor(WHITE);

        char input = _getch();
        switch (input) {
        case 'w': case 'W': case 72:
            selected = (selected - 1 + totalOptions) % totalOptions;
            break;
        case 's': case 'S': case 80:
            selected = (selected + 1) % totalOptions;
            break;
        case '\r':
            if (selected == 0) {
                chooseDifficulty();
            }
            else if (selected == 1) {
                int rt = inputValidationFunction(
                    "Enter response time in milliseconds (100-10000)",
                    100, maximum
                );
                responseTimeMS = rt;
            }
            else {
                return;
            }
            break;
        }
    }
}