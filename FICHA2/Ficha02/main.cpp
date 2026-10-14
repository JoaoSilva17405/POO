#include <iostream>
#include "Gestao.h"
using namespace std;

int main()
{
    Gestao G;

    cout << "===== 3c LerFicheiro =====" << endl;
    if (!G.LerFicheiro("pessoas.txt"))
        return 1;
    G.Mostrar();

    cout << endl << "===== 3a / 3b AddPessoa =====" << endl;
    cout << "Add Ana Silva (Viseu, 40): " << G.AddPessoa("Ana Silva", "Viseu", 40) << endl;
    cout << "Add Rui Costa (Braga, 19): " << G.AddPessoa(new Pessoa("Rui Costa", "Braga", 19)) << endl;
    cout << "Add Luis Miguel repetido:  " << G.AddPessoa("Luis Miguel", "Porto", 50) << endl;

    cout << endl << "===== 3h PesquisarPessoa =====" << endl;
    Pessoa *P = G.PesquisarPessoa("Pedro Miguel");
    if (P) P->Show();   // idade em falta no ficheiro -> 18
    cout << "Pesquisar 'Ninguem': " << (G.PesquisarPessoa("Ninguem") ? "encontrou" : "nao encontrou") << endl;

    cout << endl << "===== 3i GetCidadeMaisHabitantes =====" << endl;
    cout << G.GetCidadeMaisHabitantes() << endl;

    cout << endl << "===== 3e / 3f Remover =====" << endl;
    cout << "Remover(\"Jose Francisco\"): " << G.Remover("Jose Francisco") << endl; // Lisboa fica vazia
    cout << "Remover(Pesquisar(\"Rui Costa\")): " << G.Remover(G.PesquisarPessoa("Rui Costa")) << endl;
    cout << "Remover(\"Ninguem\"): " << G.Remover("Ninguem") << endl;

    cout << endl << "===== 3j OrdenarPorIdade =====" << endl;
    G.OrdenarPorIdade();
    G.Mostrar();

    cout << endl << "===== 3g GravarFicheiro =====" << endl;
    cout << "Gravar pessoas_out.txt: " << G.GravarFicheiro("pessoas_out.txt") << endl;

    return 0;
}
