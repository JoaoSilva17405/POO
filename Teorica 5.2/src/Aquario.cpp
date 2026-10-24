#include "Aquario.h"

Aquario::Aquario()
{
    //ctor
}

Aquario::~Aquario()

//Quando um objeto Aquario é destruído, as listas LP, LP_Fome e LP_Assar
//são destruídas automaticamente. Mas essas listas guardam só ponteiros.
//Destruir a lista apaga as "moradas", não os peixes.
//Os peixes ficam na memória sem ninguém que aponte para eles.(fuga de memoria)
//E se um peixe tiver um destrutor com trabalho a fazer (como o ~Tubarao), esse código nunca correria.
{
    //dtor
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        delete *it;
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
            RemoverParaAssar();
        }
        Uteis::wait_segundos(2);
    }
}

bool Aquario::Preciso_de_Comer(Peixe *P) //atualiza lista de peixes com fome
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
    //RemoverParaAssar();

}
bool Aquario:: QueroSair ( Peixe * P)
{
    LP_Assar.push_back(P);
    return true;
}

bool Aquario::RemoverParaAssar() //precorrer lista de Peixes
{
    for(list<Peixe*>::iterator it = LP_Assar.begin(); it != LP_Assar.end(); ++it )
    {
        cout << "O peixe "<< (*it)->GetID() << ", com o peso " << (*it)->GetPeso() <<  "(g), saiu do aquario!" << endl;
        LP.remove(*it);
        delete *it;
    }
    LP_Assar.clear(); // no fim de remover da lista dos Peixes, limpar lista LP_ASSAR

    return true;
}


Peixe *Aquario::Pertence(int id) //pesquisa por ID
{
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        if ((*it)->GetID() == id) //chama metodo GetID no Peixe para o qual aponta it
            return *it;
    return NULL;
}

bool Aquario::RemoverID(int id)
{
    Peixe *P = Pertence(id); //Pesquisa porID, e verifica se existe e tenta eliminar de todas as listas
    if (P == NULL)
        return false;            // não existe nenhum peixe com esse número
    LP.remove(P);
    LP_Fome.remove(P);           // não deixar ponteiros para um peixe apagado
    LP_Assar.remove(P);
    delete P;
    return true;
}
