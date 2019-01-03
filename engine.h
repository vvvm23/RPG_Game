#include <stdio.h>
#include <string.h>

typedef struct Weapon Weapon;
typedef struct Armour Armour;
typedef struct Consumable Consumable;
typedef struct Modifier Modifier;
typedef struct Item Item;

typedef struct Move Move;
typedef struct Creature Creature;
typedef struct Entity Entity;

/* Items and substructures */
typedef struct Weapon {
    unsigned int attack_modifier;
} Weapon;

typedef struct Armour {
    float defence_modifier;
} Armour;

typedef struct Consumable {
    unsigned int attack : 1;

    unsigned int hp;
    unsigned int mp;
    unsigned int sp;
    void (*effect_function)(Entity);
} Consumable;

typedef struct Modifier {
    float damage_modifer;
    float defence_modifier;
    void (*effect_function)(Entity);
} Modifier;

typedef struct Item {
    unsigned int id;
    char name[20];

    Weapon weapon;
    Armour armour;
    Consumable Consumable;
    Modifier modifier;
} Item;

/* Moves */
typedef struct Move {
    unsigned int id;
    char name[20];
    char message[50];

    unsigned int attack : 1;

    unsigned int c_mp;
    unsigned int c_sp;

    unsigned int hp;
    unsigned int mp;
    unsigned int sp;
   
    void (*effect_function)(Entity*);
} Move;

/* Entities and substructures */
typedef struct Creature {
    unsigned int id;
    char name[20];

    unsigned int max_hp;
    unsigned int max_mp;
    unsigned int max_sp;
    unsigned int base_speed;
    float base_accuracy;

    Move default_moveset[4];
    Item default_weapon;
    Item default_armour;
    Item default_held;
    Item default_inventory[10];

    void (*ability_function)(Entity*);
} Creature;

typedef struct Entity {
    unsigned int id;
    Creature creature;
    char name[20];
    
    unsigned int alive : 1;
    unsigned int status : 3;
    /*
        0: None
        1: Poison {-5hp, -5mp, -10sp}
        2: Paralysis {Lose 2 turns}
        3: Psychosis {Lose 1 turn then -50% accuracy, -50mp, -30sp}
        4: Burning {-10hp, -30% accuracy}
        5: Terrified {Lose 1 turn, -20mp}
        6: Stunned {Lose 1 turn, -20sp}
        7: Crippled {Lose 2 turns, -15sp, -15% accuracy}
    */

    unsigned int hp;
    unsigned int mp;
    unsigned int sp;
    unsigned int speed;
    float accuracy;
    int defend;
    int cons_defend;

    Move moves[4];
    Item equipped_weapon;
    Item equipped_armour;
    Item equipped_held;
    Item inventory[10];

    Move selected_move;
    Entity *target;
} Entity;