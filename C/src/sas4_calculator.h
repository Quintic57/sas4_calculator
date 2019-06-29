typedef struct Weapon {
    char* name;
	double dmg[3];
    double pierce[3];
    unsigned short int num_pellets;
	unsigned int rate_of_fire;
    unsigned short int capacity;
    double reload_time;
} Weapon;

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