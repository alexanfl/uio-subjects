#ifndef BOL_H
#define BOL_H

template <class T>
class Bol{
public:
    Bol(){
        bebodd = 0;
        pBeboer = NULL;
    }
   

    int sjekk_bebodd();
    void sett_inn(T* A);
    T *hent_ut();
    void fjern();

private:
    int bebodd;
    T *pBeboer;
};

template <class T>
int Bol<T>::sjekk_bebodd(){
    /* if (bebodd) { */
    /*     cout << "Bolet er bebodd av "; */
    /*     (*pBeboer).print_navn(); */
    /*     cout << "\n"; */
    /* } */
    /* else { */
    /*     printf("Det er ingen dyr i bolet.\n"); */
    /* } */
    return bebodd;
}

template <class T>
void Bol<T>::sett_inn(T* A){
    sjekk_bebodd();

    if (!pBeboer) {
        pBeboer = A;
        bebodd = 1;
    }
    else {
        cout << "Bolet er allerede fullt.\n";
    }
}

template <class T>
void Bol<T>::fjern(){
    if (bebodd) {
        /* (*pBeboer).print_navn(); */
        /* cout << " flytter ut av bolet.\n"; */ 
        pBeboer = NULL;
        bebodd = 0;
    }
}

template <class T>
T *Bol<T>::hent_ut(){
    if (bebodd) {
        return pBeboer;
    }
}

#endif
