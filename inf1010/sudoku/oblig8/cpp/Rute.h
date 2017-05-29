#ifndef RUTE_H
#define RUTE_H

class Rute {
public:
    Rute(int verdi);
    void print_verdi();

private:
    int verdi;
};

Rute::Rute(int verdi) {
    verdi = verdi;
}

void Rute::print_verdi() {
    cout << verdi;
}

#endif
