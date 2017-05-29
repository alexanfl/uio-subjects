#ifndef ROTTE_H
#define ROTTE_H

#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

class Rotte {
public:
    Rotte (const char*);
    void endre_tilstand();
    int print_tilstand();
    void print_navn();
    void angrep();

private:
    int tilstand;
    const char* navn;
};

Rotte::Rotte(const char* a){
    navn = a;
    tilstand = 2.;
}

void Rotte::endre_tilstand(){
    if (tilstand == 2.) {
        tilstand--;
        printf("Rotta %s gikk fra aa vaere levende til aa vaere skadet.\n", navn);
    }
    else if (tilstand == 1.) {
        tilstand--;
        printf("Rotta %s gikk fra aa vaere skadet til aa vaere dod.\n", navn); 
    }
}

void Rotte::angrep(){
    endre_tilstand();
}

void Rotte::print_navn(){
    cout << navn;
}

int Rotte::print_tilstand(){
    /* if (tilstand == 2.) { */
    /*     cout << "Levende\n"; */
    /* } */
    /* else if (tilstand == 1.) { */
    /*     cout << "Skadet\n"; */
    /* } */
    /* else if (tilstand == 0.) { */
    /*     cout << "Dod\n"; */
    /* } */

    return tilstand;
}


#endif
