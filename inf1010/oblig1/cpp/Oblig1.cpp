#include <stdio.h>
#include "Mus.h"
#include "Rotte.h"
#include "Bol.h"
#include "Katt.h"

int main(int argc, const char *argv[]){
    // Lager to bol for mus og rotte
    Bol<Mus> musebol;
    Bol<Rotte> rottebol;

    // Lager en katt, Tom
    Katt Tom("Tom");

    // Katten drar ut og jakter #1
    Tom.jakt(musebol, rottebol);

    // Lager ei rotte, Ronny
    Rotte Ronny("Ronny");
    Rotte *pRonny;
    pRonny = &Ronny;

    // Setter rotta Ronny inn i rottebolet
    rottebol.sett_inn(pRonny);

    // Katten drar ut og jakter #2
    Tom.jakt(musebol, rottebol);

    // Lager ei mus, Jerry
    Mus Jerry("Jerry");
    Mus *pJerry;
    pJerry = &Jerry;

    // Setter musa Jerry inn i musebolet
    musebol.sett_inn(pJerry);

    // Lager ei ny mus, Arne
    Mus Arne("Arne");
    Mus *pArne;
    pArne = &Arne;

    // Prover aa sette musa Arne inn i musebolet
    musebol.sett_inn(pArne);

    // Katten drar ut og jakter #3
    Tom.jakt(musebol, rottebol);

    return 0;
}
