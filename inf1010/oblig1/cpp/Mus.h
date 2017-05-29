#ifndef MUS_H
#define MUS_H

#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

class Mus {
public:
    Mus (const char*);
    void endre_tilstand();
    int print_tilstand();
    void print_navn();
    void angrep();

private:
    int tilstand;
    const char* navn;
};

Mus::Mus(const char* a){
    navn = a;
    tilstand = 1;
}

void Mus::endre_tilstand(){
    tilstand = 0;
    printf("Musa %s gikk fra aa vaere levende til aa vaere dod.\n", navn);
}

void Mus::angrep(){
    endre_tilstand();
}

void Mus::print_navn(){
    cout << navn;
}

int Mus::print_tilstand(){
    /* if (tilstand) { */
    /*     cout << "Levende\n"; */
    /* } */
    /* else { */
    /*     cout << "Dod\n"; */
    /* } */
    return tilstand;
}

#endif
