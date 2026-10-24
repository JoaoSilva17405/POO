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
    string COR;
    Aquario *Ptr_Aquario;

    bool Ja_Avisei_Tenho_Fome;
    bool Ja_Avisei_Quero_Sair;
    void EstouComFome(); //

    protected:
         // filhos de Peixe podem LER o peso
        virtual bool EstouGordo() ; // virtual caso queira diferentes condiçoes de saida para os peixes

    public:
        Peixe();
        Peixe(int _id, string _especie, int PESO = 10, string _cor = "Cinzento");
        virtual ~Peixe();
        void Atribuir_Aquario(Aquario *A) { Ptr_Aquario = A; }
        virtual void Show();
        virtual void Run();
        int GetID() { return ID;} // publico para aquario usar
        void Alimentar(int cal);
        int GetPeso() { return PESO; }
};

#endif // PEIXE_H
