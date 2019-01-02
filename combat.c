#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "helper.h"

int begin_battle();
void execute_move();
void choose_move();
void initialise_entities();
void display_ui();

Entity blufor[];
Entity opfor[];
int begin_battle(Entity _blufor[], Entity _opfor[]) {
    for (int b = 0; b < sizeof(*_blufor)/sizeof(_blufor[0]); b++) {
        blufor[b] = _blufor[b];
    }
    for (int o = 0; o < sizeof(*_opfor)/sizeof(_opfor[0]); o++) {
        opfor[o] = _opfor[o];
    }
    return 1;
}

void execute_move(Entity * source) {
    char buffer[1024];
    Entity * target = (*source).target;

    (*source).mp -= (*source).selected_move.c_mp;
    (*source).sp -= (*source).selected_move.c_sp;
    text_crawl((*source).selected_move.message, 50);

    if ((*source).selected_move.attack) {
        int d_hp = (*source).selected_move.hp * (*source).equipped_weapon.weapon.attack_modifier * \
                    (*target).equipped_armour.armour.defence_modifier * (*target).equipped_held.modifier.defence_modifier;
        int d_mp = (*source).selected_move.mp * (*source).equipped_weapon.weapon.attack_modifier * \
                    (*target).equipped_armour.armour.defence_modifier * (*target).equipped_held.modifier.defence_modifier;
        int d_sp = (*source).selected_move.sp * (*source).equipped_weapon.weapon.attack_modifier * \
                    (*target).equipped_armour.armour.defence_modifier * (*target).equipped_held.modifier.defence_modifier;

        (*target).hp -= d_hp;
        (*target).mp -= d_mp;
        (*target).sp -= d_sp;

        if ((*target).mp < 0) {(*target).mp = 0;}
        if ((*target).sp < 0) {(*target).sp = 0;}

        if ((*target).hp <= 0) {
            (*target).hp = 0;
            snprintf(buffer, 1024, "%s dealt %dhp to %s and killed them!\n", \
                        (*source).name, d_hp, (*target).name);
            text_crawl(buffer, 50);
            (*target).alive = 0;
        }
        else {
            snprintf(buffer, 1024, "%s dealt %dhp to %s!\n", \
                    (*source).name, d_hp, (*target).name);
            text_crawl(buffer, 50);
            (*source).selected_move.effect_function(target);
        }
    }
    else {
        (*target).hp += (*source).selected_move.hp;
        (*target).mp += (*source).selected_move.mp;
        (*target).sp += (*source).selected_move.sp;

        if ((*target).hp > (*target).creature.max_hp) {(*target).hp = (*target).creature.max_hp;}
        if ((*target).mp > (*target).creature.max_mp) {(*target).mp = (*target).creature.max_mp;}
        if ((*target).sp > (*target).creature.max_sp) {(*target).sp = (*target).creature.max_sp;}

        if ((*source).selected_move.hp > 0) {
            snprintf(buffer, 1024, "%s restored %s's hp by %d hp\n", \
                        (*source).name, (*target).name, (*source).selected_move.hp);
            text_crawl(buffer, 50);
        }
        if ((*source).selected_move.mp > 0) {
            snprintf(buffer, 1024, "%s restored %s's mp by %d mp\n", \
                        (*source).name, (*target).name, (*source).selected_move.mp);
            text_crawl(buffer, 50);
        }
        if ((*source).selected_move.sp > 0) {
            snprintf(buffer, 1024, "%s restored %s's sp by %d sp\n", \
                        (*source).name, (*target).name, (*source).selected_move.sp);
            text_crawl(buffer, 50);
        }
        (*source).selected_move.effect_function(target);
    }
}

void choose_move(Entity * source) {
    
}

void initialise_entities(Entity entities[], Creature creature, int quantity) {
    for (int e = sizeof(*entities)/sizeof(entities[0]); e < quantity; e++) {
        entities[e].id = e;
        entities[e].creature = creature;
        strcpy(entities[e].name, creature.name);
        entities[e].alive = 1;
        entities[e].status = 0;
        entities[e].hp = creature.max_hp;
        entities[e].mp = creature.max_mp;
        entities[e].sp = creature.max_sp;
        entities[e].speed = creature.base_speed;
        entities[e].accuracy = creature.base_accuracy;
        entities[e].equipped_weapon = creature.default_weapon;
        entities[e].equipped_armour = creature.default_armour;
        entities[e].equipped_held = creature.default_held;

        for (int m = 0; m < 4; m++) {
            entities[e].moves[m] = creature.default_moveset[m];
        }
        for (int i = 0; i < 10; i++) {
            entities[e].inventory[i] = creature.default_inventory[i];
        }
    }
}

void display_ui(char blufor_title[], char opfor_title[],
                int blufor_size, int opfor_size,
                char message[]) {
    clear();
    printf("====================s=======================================================\n");
    printf("                     %s versus %s\n", blufor_title, opfor_title);
    printf("===========================================================================\n");        
    for (int _ = 0; _ < blufor_size; _++) {

        printf("%s: %d/%d hp    %d/%d sp    %d/%d mp", blufor[_].name,
                                                        blufor[_].hp, blufor[_].creature.max_hp,
                                                        blufor[_].sp, blufor[_].creature.max_sp, 
                                                        blufor[_].mp, blufor[_].creature.max_mp);
        if (!blufor[_].alive) {
            printf("    DOWN\n");
        }
        else {
            printf("\n");
        }
    }
    printf("\n============================================================================\n");
    for (int _ = 0; _ < opfor_size; _++) {
        printf("%s: %d/%d hp    %d/%d sp    %d/%d mp", opfor[_].name,
                                                        opfor[_].hp, opfor[_].creature.max_hp,
                                                        opfor[_].sp, opfor[_].creature.max_sp, 
                                                        opfor[_].mp, opfor[_].creature.max_mp);
        if (!opfor[_].alive) {
            printf("    DOWN\n");
        }
        else {
            printf("\n");
        }
    }
    printf("\n============================================================================\n");
}

int main() {
    
    return 0;
}