#include "Character.h"
#include "Skill.h"
#include <iostream>

Character::Character(std::string name, int hp, int mp, int atk, int def, int spd)
    : name(name), maxHp(hp), hp(hp), maxMp(mp), mp(mp),
      attack(atk), defense(def), speed(spd), level(1) {}

void Character::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
    std::cout << name << " получил " << damage << " урона! Осталось HP: " << hp << "\n";
}

void Character::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
    std::cout << name << " восстановил " << amount << " HP.\n";
}

void Character::attackTarget(Character& target) {
    int damage = attack - target.defense / 2;
    if (damage < 1) damage = 1;
    target.takeDamage(damage);
}

void Character::useSkill(Skill& skill, Character& target) {
    if (mp >= skill.getMpCost()) {
        mp -= skill.getMpCost();
        skill.apply(*this, target);
    } else {
        std::cout << "Не хватает MP!\n";
    }
}