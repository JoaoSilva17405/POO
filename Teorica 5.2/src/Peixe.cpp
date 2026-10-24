#include "Peixe.h"
#include "Aquario.h"

Peixe::Peixe(int _id, string _especie, int PESO)
{
    ID = _id;
    ESPECIE = _especie;
    this->PESO = PESO;
    Ptr_Aquario = NULL;
    Ja_Avisei_Tenho_Fome = false;
}

Peixe::~Peixe()
{
    //dtor
}
void Peixe::Alimentar(int cal)
{
    PESO += cal;
    Ja_Avisei_Tenho_Fome = false;
}

void Peixe::Show()
{
    cout << "Peixe: (" << ID << "," << ESPECIE << "," << PESO << ")\n";
}
void Peixe::EstouComFome()
{
    if (!Ja_Avisei_Tenho_Fome)
    {
        bool ret = Ptr_Aquario->Preciso_de_Comer(this);
        Ja_Avisei_Tenho_Fome = ret;
    }
}
void Peixe::Run()
{
    PESO -= Uteis::Aleatorio(1, 5);
    if (PESO < 20)
    {
        cout << "Preciso de Comer: " << ID << endl;
        //PESO += 50;
        EstouComFome();
    }
    Show();
}
