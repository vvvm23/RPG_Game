#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clear() {
    system("@cls||clear");
}

void milli_sleep(int time) {
    struct timespec ts;
    ts.tv_sec = time / 1000;
    ts.tv_nsec = (time % 1000) * 1000000;
    nanosleep(&ts, NULL); 
}

void text_crawl(char text[], int speed) {
    for (int c = 0; c < strlen(text); c++) {
        printf("%c", text[c]);
        fflush(stdout);
        milli_sleep(speed);
    }
}

void multi_line(char text[], int nb_lines) {
    for (int l = 0; l < nb_lines; l++) {
        printf("%s\n", text);
    }
}

void play_audio() {

}

char get_char() {
    return fgetc(stdin);
}