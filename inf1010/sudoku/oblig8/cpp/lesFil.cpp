#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#include "Rute.h"
#include "Brett.h"



void lesFil(const char *filnavn){
    ifstream infile(filnavn);
    string line;

    int n = 6;

    while (getline(infile, line)) {
        istringstream iss(line);
        for (int i = 0; i < n; i++) {
            Rute rute(line[i]);

            rute.print_verdi();
            cout << "  ";
        }
        cout << "\n";
    }
}

int main(int argc, const char *argv[]) {
    char filnavn[] = "sudokubrett.txt";
    lesFil(filnavn);

    return 0;
}

