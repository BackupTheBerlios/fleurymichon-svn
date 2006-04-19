#include <iostream>

#define CMDSMAX 32 //taille maxi d'une commande ou d'un argument

struct arguments
{
    char arg[CMDSMAX];
    struct arguments *next;
};

struct commande
{
    char com[CMDSMAX];
    int size;
    struct arguments *args;
};

struct commande * decompcom(char* s);
void freecmd(struct commande *cmd);
