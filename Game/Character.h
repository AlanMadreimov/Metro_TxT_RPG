#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>
#include <vector>

class Skill;  // Предварительное объявление

class Character {
protected:
    std::string name;
    int level;
    int hp, maxHp;
    int mp, maxMp;
    int attack;
    int defense;
    int speed;

public:
    Character(std::string name, int hp, int mp, int atk, int def, int spd);
    virtual ~Character() = default;

    // Базовые методы
    bool isAlive() const { return hp > 0; }
    void takeDamage(int damage);
    void heal(int amount);

    // Боевые действия
    virtual void attackTarget(Character& target);
    virtual void useSkill(Skill& skill, Character& target);

    // Геттеры
    std::string getName() const { return name; }
    int getHp() const { return hp; }
    int getSpeed() const { return speed; }
};



#endif //CHARACTER_H
