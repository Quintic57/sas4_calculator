typedef struct Weapon {
    char* name;
	double dmg[3];
    double pierce[3];
    unsigned short int num_pellets;
	double rate_of_fire;
    unsigned short int capacity;
    double reload_time;
} Weapon;

#define NUM_WEAPON_CATEGORIES 10
#define NUM_ASSAULT_RIFLES 17
#define NUM_DISC_THROWERS 1
#define NUM_FLAME_THROWERS 1
#define NUM_LASERS 1
#define NUM_LMGS 12
#define NUM_PISTOLS 11
#define NUM_ROCKET_LAUNCHERS 8
#define NUM_SHOTGUNS 5
#define NUM_SMGS 10
#define NUM_SNIPER_RIFLES 4

#define ASSAULT_RIFLES 0
#define DISC_THROWERS 1
#define FLAME_THROWERS 2
#define LASERS 3
#define LMGS 4
#define PISTOLS 5
#define ROCKET_LAUNCHERS 6
#define SHOTGUNS 7
#define SMGS 8
#define SNIPER_RIFLES 9

#define NORMAL 0
#define RED 1
#define BLACK 2

#define NAME 0
#define DMG 1
#define PIERCE 2
#define NUM_PELLETS 3
#define RATE_OF_FIRE 4
#define CAPACITY 5
#define RELOAD_TIME 6

void init();
double* parse_csv_array(char* str);
void parse_csv_field(char* row, Weapon* category, int i);
void parse_csv_string(char* str, double* value);