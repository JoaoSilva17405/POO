#ifndef TUBARAO_H
#define TUBARAO_H

#include "Peixe.h"


class Tubarao : public Peixe
{
    int GRAU_PERIGO;
    public:
        Tubarao();
        Tubarao(int _id, string _especie, int PESO, int gp = 100);
        virtual ~Tubarao();

};

#endif // TUBARAO_H
