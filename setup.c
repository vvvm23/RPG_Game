#include <stdio.h>
#include <string.h>
#include "engine.h"

static Creature creature_list[3];
static Move move_list[10];
static Item item_list[10];

void regeneration(Entity *entity) {
    if (entity->hp + 10 < entity->creature.max_hp) {
        entity->hp += 10;
    }
}

void define_creatures() {
    for (int _ = 0; _ < 3; _++) {
        creature_list[_].id = _;
        strcpy(creature_list[_].name, "Creature %_");
        
        creature_list[_].max_hp = 100 - _;
        creature_list[_].max_mp = 100 - _;
        creature_list[_].max_sp = 100 - _;
        creature_list[_].base_accuracy = 1.0;

        creature_list[_].default_moveset[0] = move_list[_];
        creature_list[_].default_weapon = item_list[0];
        creature_list[_].default_armour = item_list[1];
        creature_list[_].default_held = item_list[2];

        creature_list[_].ability_function = regeneration;
    }
}

void define_moves() {
    move_list[0].id = 0;
    strcpy(move_list[0].message, "#NAME is channeling the spirit of mother russia!");
    strcpy(move_list[0].name, "Patriotic Spirit");
    move_list[0].attack = 0;
    move_list[0].hp = 0;
    move_list[0].mp = 20;
    move_list[0].sp = 30;
    move_list[0].effect_function = regeneration;
}

void define_items() {
    printf("%s: %s\n", move_list[0].name, move_list[0].message);
}

int main() {
    define_creatures();
    define_moves();
    define_items();

    return 0;
}