#include "Aquario.h"

Aquario::Aquario()
{
    //ctor
}

Aquario::~Aquario()
{
    //dtor
}

void Aquario::Add(Peixe *P)
{
    LP.push_front(P);
    P->Atribuir_Aquario(this);
}
void Aquario::Run()
{
    while (true)
    {
        for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        {
            (*it)->Run();
        }
        cout << "Esfomeados: " << LP_Fome.size() << endl;
        cout << "------------------------------" << endl;
        if (LP_Fome.size() >= 2)
        {
            Alimentar();
        }
        Uteis::wait_segundos(2);
    }
}

bool Aquario::Preciso_de_Comer(Peixe *P)
{
    LP_Fome.push_front(P);
    return true;
}
void Aquario::Alimentar()
{
    // percorrer a LP e alimentar cada um
    cout << "Alimentacao a DEcorrer....." << endl;
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
    {
        (*it)->Alimentar(Uteis::Aleatorio(20, 50));
    }
    LP_Fome.clear();
}
bool Aquario::Remover(int id)
{
    return true;
}
Peixe *Aquario::Pertence(int id)
{
    return NULL;
}
