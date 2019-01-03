#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "engine.h"

void display_ui(char blufor_title[], char opfor_title[], Entity blufor[], Entity opfor[],
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
    clear();

    Creature test_blufor;
    test_blufor.max_hp = 100;
    test_blufor.max_mp = 100;
    test_blufor.max_sp = 120;

    Entity e_test_blufor;
    strcpy(e_test_blufor.name, "Laura's Peasants.");
    e_test_blufor.hp = 100;
    e_test_blufor.mp = 100;
    e_test_blufor.sp = 120;
    e_test_blufor.creature = test_blufor;
    e_test_blufor.alive = 1;

    Creature test_opfor;
    test_opfor.max_hp = 1;
    test_opfor.max_mp = 0;
    test_opfor.max_sp = 1000;

    Entity e_test_opfor;
    strcpy(e_test_opfor.name, "Robbie");
    e_test_opfor.hp = 1;
    e_test_opfor.mp = 0;
    e_test_opfor.sp = 1000;
    e_test_opfor.creature = test_opfor;
    e_test_opfor.alive = 1;

    Entity blufor[] = {e_test_blufor, e_test_blufor, e_test_blufor, e_test_blufor, e_test_blufor, e_test_blufor};
    Entity opfor[] = {e_test_opfor};

    display_ui("Team Laura", "Team Coffee", blufor, opfor, 6, 1, "You were challenged by Robbie!");

    if (1) {
        printf("1 is true\n");
    }
    if (0) {
        printf("0 is true\n");
    }

    char buffer[1024];
    snprintf(buffer, 1024, "this is a test to see if buffer makes text_crawl sssslllooowww\n");
    text_crawl(buffer, 50);

    printf("Length of blufor is %ld\n", sizeof(blufor)/sizeof(blufor[0]));
    return 0;
}