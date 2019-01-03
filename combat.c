#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "engine.h"
#include "helper.h"

int begin_battle();
void ai_turn();
void execute_move();
void choose_move();
void execute_held();
void choose_held();
void defend();
void initialise_entities();
void display_ui();
void get_order();
int compare();
void reset_round();

// Work on sizes of these global arrays
Entity blufor[];
Entity opfor[];
Entity *turn_order[];
char blufor_title[];
char opfor_title[];

int blufor_size;
int opfor_size;

int begin_battle(Entity _blufor[], Entity _opfor[], char *messages[]) {
    /*
        message[]:
            0: blufor title
            1: opfor title
            2: start message
            3: win message
            4: loss message
    */
    
    char buffer[1024];
    int valid;
    int end = 0;
    int b_alive = 1;
    int o_alive = 1;

    blufor_size = sizeof(*_blufor)/sizeof(_blufor[0]);
    opfor_size = sizeof(*_opfor)/sizeof(_opfor[0]);

    for (int b = 0; b < blufor_size; b++) {
        blufor[b] = _blufor[b];
    }
    for (int o = 0; o < opfor_size; o++) {
        opfor[o] = _opfor[o];
    }

    char start_message[1024];
    char win_message[1024];
    char loss_message[1024];
    strcpy(blufor_title, messages[0]);
    strcpy(opfor_title, messages[1]);
    strcpy(start_message, messages[2]);
    strcpy(win_message, messages[3]);
    strcpy(loss_message, messages[4]);

    display_ui();
    text_crawl(start_message, 50);
    while (!end) {
        for (int b = 0; b < blufor_size; b++) {
            if (blufor[b].alive) {
                valid = 0;
                while (!valid) {
                    snprintf(buffer, 1024, "What will %s do?\n", blufor[b].name);
                    text_crawl(buffer, 50);
                    text_crawl("1: Attack\n", 50);
                    text_crawl("2: Defend\n",  50);
                    text_crawl("3: Held\n", 50);
                    int sel = menu_choice(1, 3);
                    if (sel) {
                        valid = 1;
                        switch (sel) {
                            case 1:
                                choose_move(blufor[b]);
                                break;
                            case 2:
                                // Remember to reset defend values
                                defend(blufor[b]);
                                break;
                            case 3:
                                choose_held(blufor[b]);
                                break;
                        }
                    }
                    else {
                        text_crawl("Invalid choice!      \n", 100);
                        clear();
                        display_ui();
                    }
                }
            }
        }

        ai_turn();
        get_order();

        for (int t = 0; t < blufor_size + opfor_size; t++) {
            if ((*turn_order[t]).alive) {
                execute_move(turn_order[t]);
            }
        }

        for (int b = 0; b < blufor_size; b++) {
            if (blufor[b].alive) {
                b_alive = 1;
            }
        }  
        for (int o = 0; o < opfor_size; o++) {
            if (opfor[o].alive) {
                o_alive = 1;
            }
        }

        if (!b_alive || !o_alive) {
            end = 1;
        }
    }
    if (!b_alive && o_alive) {return 0 ;}
    else {return 1 ;}
}

void ai_turn() {
    int low_health_enemy; // Prioritise low health enemies
    int low_health_ally; // Prioritise healing low allies
    int has_low_health; // Prioritise defending self
    int has_healing; // Prioritise healing allies if needed
    int ally_has_healing; // Prioritise continuing to fight as allies can heal

    int high_health_enemy; // Prioritise on high health enemy
    int low_damage; // Prioritise damage on one enemy if many allies with low damage


    /*
        AI move priorities:

            Enemy has low hp and AI has high speed -> Target Enemy
            AI has low health and defended last turn -> Heal or call for heal
            AI has low health -> Heal if possible else call for heal if possible else defend
            Enemy has low hp -> Target Enemy
            AI's ally has low health -> Heal ally if possible
            Else Attack
            Defend

        Attack schema:
            If low damage attacks or high hp enemy:
                Focus fire with allies
            Else:
                Target randomly

            Use more powerful moves on high hp targets or where 1 hit KO likely
            Use less powerful attacks when sp and mp low.
            Defend when sp and mp low to restore.
    */
}

void execute_move(Entity * source) {
    // Maybe rework so there is local copy of source and then set to pointer

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
    char buffer[1024];
    int in;
    int valid = 0;
    while (!valid) {
        text_crawl("Select Move:\n", 50);
        for (int m = 0; m < sizeof((*source).moves)/sizeof((*source).moves[0]); m++) {
            snprintf(buffer, 1024, "%d: %s\n",
                    m+1, (*source).moves[m].name);
            text_crawl(buffer, 50);
        }
        //printf("\r  "); ?
        in = (int) fgetc(stdin); // careful casting from char directly;
        if (in < 1 || in > sizeof((*source).moves)/sizeof((*source).moves[0])) {
            text_crawl("\nInvalid input!\n", 50);
        }
        else if ((*source).moves[in - 1].c_mp >= (*source).mp || \
                (*source).moves[in - 1].c_sp >= (*source).sp) {
            text_crawl("\nNot enough sp or mp to execute move!\n", 50);
        }
        else {
            valid = 1;
        }
    }
    
    (*source).selected_move = (*source).moves[in - 1];

    // This system allows for targetting of downed enemies. todo: fix.
    valid = 0;
    while (!valid) {
        text_crawl("\nSelect Target:\n", 50);
        if ((*source).selected_move.attack) {
            for (int o = 0; o < sizeof(*opfor)/sizeof(opfor[0]); o++) {
                snprintf(buffer, 1024, "%d: %s\n",
                        o+1, opfor[o].name);
                text_crawl(buffer, 50);
            }

            in = (int) fgetc(stdin);
            if (in < 1 || in > sizeof(*opfor)/sizeof(opfor[0])) {
                text_crawl("\nInvalid input", 50);
            }
            else {
                (*source).target = &opfor[in - 1];
                valid = 1;
            }
        }
        else {
            for (int b = 0; b < sizeof(*blufor)/sizeof(blufor[0]); b++) {
                snprintf(buffer, 1024, "%d: %s\n",
                        b+1, blufor[b].name);
                text_crawl(buffer, 50);
            }

            in = (int) fgetc(stdin);
            if (in < 1 || in > sizeof(*blufor)/sizeof(blufor[0])) {
                text_crawl("\nInvalid input", 50);
            }
            else {
                (*source).target = &blufor[in - 1];
                valid = 1;
            }            
        }
    }
}

// Maybe allow each entity to hold one item that can be used in a battle
// inventory allows to swap out held item between fights.
void execute_held(Entity * source) {

}

void choose_held(Entity * source) {

}

// Blocks one attack and restores mp, sp. Chance of success falls off through repeated use
void defend(Entity * source) {
    const float base_defend = 1.0;
    float defend_chance = base_defend - exp((*source).cons_defend) * 0.1;
    if (defend_chance < 0) {
        (*source).defend = 1;
    }
    else {
        float defend_try = random();
        if (defend_try <= defend_chance) {
            (*source).defend = 0;
        }
        else {
            (*source).defend = 1;
        }
    }
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

void display_ui() {
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

void get_order() {
    int highest = 0;
    Entity combined[blufor_size + opfor_size];

    for (int b = 0; b < blufor_size; b++) {
        combined[b] = blufor[b];
    }

    for (int o = blufor_size; o < opfor_size + blufor_size; o++) {
        combined[o] = opfor[o - blufor_size];
    }

    qsort(combined, blufor_size + opfor_size, sizeof(Entity), compare);
    for (int c = 0; c < blufor_size + blufor_size; c++) {
        turn_order[c] = &combined[c];
    }
}

int compare(const void *a, const void * b) {
    Entity *entityA = (Entity *)a;
    Entity *entityB = (Entity *)b;

    return (entityA->speed - entityB->speed);
}

void reset_round() {

}

int main() {
    
    return 0;
}