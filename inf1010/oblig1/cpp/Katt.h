#ifndef KATT_H
#define KATT_H

class Katt{
public:
    Katt (const char*);
    void print_navn();
    void jakt(Bol<Mus> bol1, Bol<Rotte> bol2);

private:
    const char* navn;
};

Katt::Katt(const char* a){
    navn = a;
}

void Katt::print_navn(){
    cout << navn;
}

void Katt::jakt(Bol<Mus> bol1, Bol<Rotte> bol2){
    if (bol1.sjekk_bebodd() && (bol1.hent_ut())->print_tilstand()) {
        printf("Katten %s gjorde et angrep paa musa ", navn);
        (bol1.hent_ut())->print_navn();
        cout << ".\n";
        (bol1.hent_ut())->angrep();
    }
    else if (bol2.sjekk_bebodd() && (bol2.hent_ut())->print_tilstand()) {
        printf("Katten %s gjorde et angrep paa rotta ", navn);
        (bol2.hent_ut())->print_navn();
        cout << ".\n";
        (bol2.hent_ut())->angrep();
    }
    else{
        cout << "Katten Tom fant ingen gnagere. Jakten avsluttes.\n";
    }
}

#endif
