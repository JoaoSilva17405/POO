#include "Peixe.h"
#include "Aquario.h"

Peixe::Peixe(int _id, string _especie, int PESO, string _cor)
{
    ID = _id;
    ESPECIE = _especie;
    this->PESO = PESO;
    Ptr_Aquario = NULL;
    Ja_Avisei_Tenho_Fome = false;
    Ja_Avisei_Quero_Sair = false;
    COR = _cor;
}

Peixe::~Peixe()
{
    //dtor
}
void Peixe::Alimentar(int cal)
{
    PESO += cal;
    Ja_Avisei_Tenho_Fome = false;
    if( EstouGordo() && !Ja_Avisei_Quero_Sair) // se depois de comer estiver acima de 50g
                                                //avisar Aquario que quer sair
    {
        Ja_Avisei_Quero_Sair = Ptr_Aquario->QueroSair(this);
    }
}

void Peixe::Show()
{
    cout << "Peixe: (" << ID << "," << ESPECIE << "," << PESO <<"," << COR <<")\n";
}
void Peixe::EstouComFome()
{
    if (!Ja_Avisei_Tenho_Fome )
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
        //PESO += 50; Errado viola encapsulamento, o peixe nao vai buscar a comida, informa que tem fome
        EstouComFome();
    }
    Show();
}

bool Peixe::EstouGordo()
{
    return PESO > 50 ;
}
