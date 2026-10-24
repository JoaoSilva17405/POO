#include "Tubarao.h"

Tubarao::Tubarao()
: Peixe(-1, "??", 123)
{
    GRAU_PERIGO = 0;
}
Tubarao::Tubarao(int _id, string _especie, int PESO, int gp)
: Peixe(_id, _especie, PESO)
{
    GRAU_PERIGO = gp;
}

Tubarao::~Tubarao()
{
    //dtor
}

void Tubarao::Show()
{
    cout << "Grau de perigo do Tubarao: " << GRAU_PERIGO;
    Peixe::Show();
}
