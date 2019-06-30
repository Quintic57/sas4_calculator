#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "sas4_calculator.h"

Weapon** weapons;
Weapon* assault_rifles;
Weapon* disc_throwers;
Weapon* flame_throwers;
Weapon* lasers;
Weapon* lmgs;
Weapon* pistols;
Weapon* rocket_launchers;
Weapon* shotguns;
Weapon* smgs;
Weapon* sniper_rifles;

void init() {
    /*Init the data structures
    Note: If you set the big daddy array first, each element of that array will
    be a pointer to the place in memory where each sub-array is declared. But if
    you then initialize each of those sub-arrays by using malloc, it then points
    them to a different area in memory and big daddy array now has all of the
    wrong pointers. See whereas Java stores references, C stores addresses*/
    assault_rifles = calloc(NUM_ASSAULT_RIFLES, sizeof(Weapon));
    disc_throwers = calloc(NUM_DISC_THROWERS, sizeof(Weapon));
    flame_throwers = calloc(NUM_FLAME_THROWERS, sizeof(Weapon));
    lasers = calloc(NUM_LASERS, sizeof(Weapon));
    rocket_launchers = calloc(NUM_ROCKET_LAUNCHERS, sizeof(Weapon));
    lmgs = calloc(NUM_LMGS, sizeof(Weapon));
    pistols = calloc(NUM_PISTOLS, sizeof(Weapon));
    shotguns = calloc(NUM_SHOTGUNS, sizeof(Weapon));
    smgs = calloc(NUM_SMGS, sizeof(Weapon));
    sniper_rifles = calloc(NUM_SNIPER_RIFLES, sizeof(Weapon));

    // Big daddy weapons array: 
    weapons = calloc(NUM_WEAPON_CATEGORIES, sizeof(Weapon*));
    weapons[ASSAULT_RIFLES] = assault_rifles;
    weapons[DISC_THROWERS] = disc_throwers;
    weapons[FLAME_THROWERS] = flame_throwers;
    weapons[LASERS] = lasers;
    weapons[LMGS] = lmgs;
    weapons[PISTOLS] = pistols;
    weapons[ROCKET_LAUNCHERS] = rocket_launchers;
    weapons[SHOTGUNS] = shotguns;
    weapons[SMGS] = smgs;
    weapons[SNIPER_RIFLES] = sniper_rifles;

    /*Parse the CSVs and get that juicy data*/
    for (int category_num = 0; category_num < NUM_WEAPON_CATEGORIES; category_num++) {
        char category_buf[64];

        if (category_num == ASSAULT_RIFLES) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/assault_rifles.csv");
        } else if (category_num == DISC_THROWERS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/disc_throwers.csv");
        } else if (category_num == FLAME_THROWERS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/flamethrowers.csv");
        } else if (category_num == LASERS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/lasers.csv");
        } else if (category_num == LMGS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/lmgs.csv");
        } else if (category_num == PISTOLS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/pistols.csv");
        } else if (category_num == ROCKET_LAUNCHERS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/rocket_launchers.csv");
        } else if (category_num == SHOTGUNS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/shotguns.csv");
        } else if (category_num == SMGS) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/smgs.csv");
        } else if (category_num == SNIPER_RIFLES) {
            snprintf(category_buf, sizeof(category_buf), "%s",
                "../data/sniper_rifles.csv");
        }
        printf("%s\n", category_buf);
        FILE *fp = fopen(category_buf, "r");
        char buf[1024];

        if (!fp) {
            printf("Can't open file\n");
            return;
        }

        int i = 0;
        while (fgets(buf, 1024, fp)) {
            if (i != 0) {
                parse_csv_field(buf, weapons[category_num], i);
                // parse_csv_field(buf, assault_rifles, i);
            }
            i++;
        }

        fclose(fp);
    }
}

void parse_csv_field(char* row, Weapon* category, int i) {
    int attribute = 0;

    // printf("Row: %s\n", row);
    // printf("i is %i", i);
    for (char* field = strtok(row, ";"); field != NULL; field = strtok(NULL, ";")) {
        char* temp = strdup(field);

        // TODO: Think I can clean up some of these
        if (attribute == NAME) {
            // printf("%i\n", i);
            category[i - 1].name = temp;
        } else if (attribute == DMG) {
            double* dmg = parse_csv_array(temp);
            memcpy(category[i - 1].dmg, dmg, sizeof(category[i - 1].dmg));
            free(dmg);
        } else if (attribute == PIERCE) {
            double* pierce = parse_csv_array(temp);
            memcpy(category[i - 1].pierce, pierce, sizeof(category[i - 1].pierce));
            free(pierce);
        } else if (attribute == NUM_PELLETS) {
            sscanf(temp, "%hu", &category[i - 1].num_pellets);
        } else if (attribute == RATE_OF_FIRE) {
            parse_csv_string(temp, &category[i - 1].rate_of_fire);
        } else if (attribute == CAPACITY) {
            sscanf(temp, "%hu", &category[i - 1].capacity);
        } else if (attribute == RELOAD_TIME) {
            parse_csv_string(temp, &category[i - 1].reload_time);
        } else {
            printf("%s\n", "Attribute is invalid");
        }
        // printf("Attribute #: %i\n", attribute);
        attribute++;
    }
}

double* parse_csv_array(char* str) {
    char* end_char = strchr(str, ']');
    int array_size = end_char - str - 1;
    char temp_str[array_size + 1];
    memcpy(temp_str, str + 1, array_size);
    temp_str[array_size] = '\0';

    double* values = calloc(3, sizeof(double));

    /*Note: You can not use strtok twice, as it keeps a const char pointer to
    the string that you're tokening. Using it twice means the state of the first
    strtok is lost*/
    // int i = 0;
    // for (char* value = strtok(temp, ", "); value != NULL; value = strtok(NULL, ", ")) {
    //     sscanf(value, "%lf", &values[i]);
    //     // printf("%lf\n", values[i]);
    //     i++;
    // }

    int start_index = 0;
    int end_index = 0;
    int values_index = 0;
    while(end_index <= (int) strlen(temp_str)) {
        if (temp_str[end_index] == ',' || temp_str[end_index] == '\0') {
            char cur[end_index - start_index];
            memcpy(&cur, temp_str + start_index, end_index - start_index);

            sscanf(cur, "%lf", &values[values_index]);
            start_index = end_index + 2;
            values_index++;
        }
        end_index++;
    }

    return values;
}

void parse_csv_string(char* str, double* value) {
    bool done = false;
    char* temp;
    for (int i = 0; !done && i < sizeof(str); i++) {
        if (str[i] == ' ') {
            memcpy(temp, str, i);
            done = true;
        }
    }

    sscanf(temp, "%lf", value);
}

int main() {
    init();

    /***Testing***/
    // NAME
    printf("%s\n", assault_rifles[1].name);
    // DMG
    for (int i = 0; i < 3; i++) {
        printf("%lf\n", assault_rifles[1].dmg[i]);
    }
    // PIERCE
    for (int i = 0; i < 3; i++) {
        printf("%lf\n", assault_rifles[1].pierce[i]);
    }
    // NUM_PELLETS
    printf("%hu\n", assault_rifles[1].num_pellets);
    // ROF
    printf("%lf\n", assault_rifles[1].rate_of_fire);
    // Capacity
    printf("%hu\n", assault_rifles[1].capacity);
    // Reload Time
    printf("%lf\n", assault_rifles[1].reload_time);

    return 0;
}