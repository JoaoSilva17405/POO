#ifndef AQUARIO_H
#define AQUARIO_H

#include <iostream>
using namespace std;
#include <string>
#include <list>

#include "Peixe.h"


class Aquario
{
    list<Peixe *> LP;
    list<Peixe *> LP_Fome;
    list<Peixe *> LP_Assar;
    public:
        Aquario();
        virtual ~Aquario();

        void Add(Peixe *P);
        void Run();
        void Alimentar();
        bool Remover(int id);
        Peixe *Pertence(int id);

        bool Preciso_de_Comer(Peixe *P);
};

#endif // AQUARIO_H
