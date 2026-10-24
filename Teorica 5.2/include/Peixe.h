#ifndef PEIXE_H
#define PEIXE_H

#include <iostream>
using namespace std;
#include <string>
#include "..//Uteis.h"

class Aquario;

class Peixe
{
    int ID;
    string ESPECIE;
    int PESO;
    Aquario *Ptr_Aquario;

    bool Ja_Avisei_Tenho_Fome;
    void EstouComFome();

    public:
        Peixe();
        Peixe(int _id, string _especie, int PESO = 10);
        virtual ~Peixe();
        void Atribuir_Aquario(Aquario *A) { Ptr_Aquario = A; }
        virtual void Show();
        virtual void Run();

        void Alimentar(int cal);
};

#endif // PEIXE_H
