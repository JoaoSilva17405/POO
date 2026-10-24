#include <iostream>
using namespace std;
#include <fstream>
#include "Uteis.h"
#include "Aquario.h"

void F1()
{
    for (int i = 0; i < 10; i++)
        cout << "Ola pessoal" <<endl;

    cout << "odsfdsfdskfo" << endl;
}
void F1_ficheiro()
{
    ofstream F("ola.txt");
    for (int i = 0; i < 10; i++)
        F << "Ola pessoal" <<endl;

    F << "odsfdsfdskfo" << endl;
    F.close();
}

void F1_Escrita(ostream &fsaida)
{
    for (int i = 0; i < 10; i++)
        fsaida << "Ola pessoal" <<endl;

    fsaida << "odsfdsfdskfo" << endl;
}

/*
void Falunos_Fazerem(const Aquario &pt)
{

}
*/
int main()
{
    cout << "Aquario" << endl;
/*
    F1_Escrita(cout);
    ofstream F("dados.txt");
    F1_Escrita(F);
    F.close();
    return 0;
*/
    srand(time(NULL));
    Aquario AQ;

    int NP = Uteis::Aleatorio(2, 6);
    for (int i = 0; i < NP; i++)
    {
        Peixe *P = new Peixe(10+i, "Sardinha", Uteis::Aleatorio(20, 50));
        AQ.Add(P);
    }

    AQ.Run();


    return 0;
}




    //system("Motoserra_1.mp3 > null");
    //system("Tubarao.mp3 > null");
