#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sas4_calculator.h"

Weapon assault_rifles[17];
Weapon disc_throwers[1];
Weapon flamethrowers[1];
Weapon lasers[1];
Weapon launchers[8];
Weapon lmgs[12];
Weapon pistols[11];
Weapon shotguns[5];
Weapon smgs[10];
Weapon sniper_rifles[4];

void init() {
    /*Parse the CSVs and get that juicy data*/
    FILE *fp = fopen("../data/assault_rifles.csv", "r");
    char buf[1024];

    if (!fp) {
        printf("Can't open file\n");
        return;
    }

    int i = 0;
    while (fgets(buf, 1024, fp)) {
        if (i != 0) {
            int attribute = 0;

            // printf("%s\n", buf);
            // printf("i is %i", i);
            for (char* field = strtok(buf, ";"); field != NULL; field = strtok(NULL, ";")) {
                // printf("%s\n", field);
                char* temp = strdup(field);

                if (attribute == NAME) {
                    assault_rifles[i - 1].name = temp;
                } else if (attribute == DMG) {
                    double* dmg = parse_csv_array(temp);
                    memcpy(assault_rifles[i - 1].dmg, dmg, sizeof(assault_rifles[i - 1].dmg));
                    free(dmg);
                } else if (attribute == PIERCE) {
                    double* pierce = parse_csv_array(temp);
                    memcpy(assault_rifles[i - 1].pierce, pierce, sizeof(assault_rifles[i - 1].pierce));
                    free(pierce);
                } else if (attribute == NUM_PELLETS) {
                    sscanf(temp, "%hu", &assault_rifles[i - 1].num_pellets);
                } else if (attribute == RATE_OF_FIRE) {
                    
                } else if (attribute == CAPACITY) {
                    
                } else if (attribute == RELOAD_TIME) {
                    
                } else {
                    printf("%s\n", "Attribute is invalid");
                }
                // printf("Attribute #: %i\n", attribute);
                attribute++;
            }
        }
        i++;
    }

    fclose(fp);
}

// void parse_csv_field(char* row, Weapon* category) {

// }

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
    return 0;
}