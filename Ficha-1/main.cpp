// ============================================================================
// Programacao Orientada a Objetos - Ficha de Trabalho 1
// Engenharia Informatica - 2o Ano, 1o Semestre - Politecnico de Viseu
//
// Exercicio 1: classe Ordenacao (algoritmos de ordenacao + metricas em CSV)
// Exercicio 2: lista de strings (std::list) com as alineas a) ... k)
// Exercicio 3: classes XMLWriter e XMLReader
//
// Compilar:  g++ -std=c++11 -Wall main.cpp -o Ficha_1
// ============================================================================

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <ctime>

#include "Ordenacao.h"
#include "ListaStrings.h"
#include "XMLWriter.h"
#include "XMLReader.h"

using namespace std;

// ----------------------------------------------------------------------------
// Auxiliar: cria um vetor de inteiros aleatorios com N elementos.
// ----------------------------------------------------------------------------
int *GeraVetor(int N)
{
    int *V = new int[N];
    for (int i = 0; i < N; i++)
        V[i] = rand() % 100000;
    return V;
}

// ----------------------------------------------------------------------------
// EXERCICIO 1
// Corre todos os algoritmos para varios tamanhos de N. Cada algoritmo recebe
// uma COPIA dos mesmos dados (para uma comparacao justa) e grava as metricas
// no ficheiro CSV.
// ----------------------------------------------------------------------------
void Exercicio1()
{
    const string ficheiro = "FicheiroOutput.csv";
    // Recomeca o ficheiro do zero em cada execucao do exercicio.
    remove(ficheiro.c_str());

    int tamanhos[] = {100, 500, 1000, 5000, 10000};
    int nTamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    cout << "\n=== Exercicio 1: comparacao de algoritmos de ordenacao ===\n";
    cout << "A gravar metricas em \"" << ficheiro << "\" ...\n\n";

    for (int t = 0; t < nTamanhos; t++)
    {
        int N = tamanhos[t];
        int *base = GeraVetor(N);

        // Para cada metodo trabalhamos sobre uma copia igual do vetor base.
        int *V = new int[N];

        for (int i = 0; i < N; i++) V[i] = base[i];
        Ordenacao::BubbleSort(V, N, ficheiro);

        for (int i = 0; i < N; i++) V[i] = base[i];
        Ordenacao::SelectionSort(V, N, ficheiro);

        for (int i = 0; i < N; i++) V[i] = base[i];
        Ordenacao::InsertionSort(V, N, ficheiro);

        for (int i = 0; i < N; i++) V[i] = base[i];
        Ordenacao::ShellSort(V, N, ficheiro);

        for (int i = 0; i < N; i++) V[i] = base[i];
        Ordenacao::QuickSort(V, N, ficheiro);

        for (int i = 0; i < N; i++) V[i] = base[i];
        Ordenacao::MergeSort(V, N, ficheiro);

        cout << "  N = " << N << "  ->  concluido (6 algoritmos)\n";

        delete[] V;
        delete[] base;
    }

    cout << "\nConcluido. Abra \"" << ficheiro
         << "\" no Excel para gerar os graficos.\n";
}

// ----------------------------------------------------------------------------
// EXERCICIO 2
// Demonstra todas as alineas pedidas sobre uma std::list<string>.
// ----------------------------------------------------------------------------
void Exercicio2()
{
    cout << "\n=== Exercicio 2: lista de strings (std::list) ===\n";

    // a) Declarar uma list de strings.
    list<string> lista;

    // b) Ler as palavras de um ficheiro de texto.
    if (!Ex2::LerDeFicheiro(lista, "palavras.txt"))
    {
        cout << "Crie o ficheiro \"palavras.txt\" na pasta do executavel.\n";
        return;
    }

    // c) Contar todas as palavras.
    cout << "\nc) Total de palavras: " << Ex2::ContarPalavras(lista) << "\n";

    // d) Listar as palavras.
    cout << "\nd) ";
    Ex2::Listar(lista);

    // e) Listar ao contrario.
    cout << "\ne) ";
    Ex2::ListarInverso(lista);

    // f) Eliminar uma palavra dada (todas as ocorrencias de "repetida").
    cout << "\nf) Eliminar a palavra \"repetida\":\n   ";
    Ex2::EliminarPorValor(lista, "repetida");
    Ex2::Listar(lista);

    // g) Eliminar a palavra na posicao 0.
    cout << "\ng) Eliminar a palavra na posicao 0:\n   ";
    Ex2::EliminarPorPosicao(lista, 0);
    Ex2::Listar(lista);

    // h) Verificar se existem palavras repetidas.
    cout << "\nh) Existem palavras repetidas? "
         << (Ex2::ExistemRepetidas(lista) ? "SIM" : "NAO") << "\n";

    // i) Contar palavras de tamanho inferior a 5.
    cout << "\ni) Palavras com menos de 5 letras: "
         << Ex2::ContarInferiores(lista, 5) << "\n";

    // j) Passar todas as palavras para maiusculas.
    cout << "\nj) Todas em maiusculas:\n   ";
    Ex2::ParaMaiusculas(lista);
    Ex2::Listar(lista);

    // k) Gravar as palavras de tamanho superior a 10.
    if (Ex2::GravarGrandes(lista, "palavras_grandes.txt", 10))
        cout << "\nk) Palavras com mais de 10 letras gravadas em \"palavras_grandes.txt\".\n";
}

// ----------------------------------------------------------------------------
// EXERCICIO 3
// Grava um XML com o XMLWriter e depois le-o de volta com o XMLReader.
// ----------------------------------------------------------------------------
void Exercicio3()
{
    cout << "\n=== Exercicio 3: XMLWriter e XMLReader ===\n";

    const string ficheiro = "FicheiroDados.xml";

    // ---- Escrita (exemplo do enunciado) ----
    XMLWriter XX;
    XX.WriteStartDocument(ficheiro);
    XX.WriteStartElement("DADOS");

    XX.WriteStartElement("PESSOA");
    XX.WriteElementString("NOME", "Jose Miguel");
    XX.WriteElementString("IDADE", "18");
    XX.WriteEndElement(); // fecha PESSOA

    XX.WriteStartElement("PESSOA");
    XX.WriteElementString("NOME", "Pedrito De Portugal");
    XX.WriteElementString("IDADE", "65");
    XX.WriteEndElement(); // fecha PESSOA

    XX.WriteEndElement();  // fecha DADOS
    XX.WriteEndDocument(); // fecha o documento

    cout << "XML gravado em \"" << ficheiro << "\".\n\n";

    // ---- Leitura ----
    cout << "Leitura do XML com o XMLReader:\n";
    XMLReader RD;
    if (!RD.Open(ficheiro))
        return;

    string ultimoElemento;
    while (RD.Read())
    {
        switch (RD.NodeType())
        {
            case XMLReader::INICIO_ELEMENTO:
                ultimoElemento = RD.Name();
                break;
            case XMLReader::TEXTO:
                if (!RD.Value().empty())
                    cout << "  " << ultimoElemento << " = " << RD.Value() << "\n";
                break;
            case XMLReader::FIM_ELEMENTO:
            default:
                break;
        }
    }
}

// ----------------------------------------------------------------------------
// Menu principal
// ----------------------------------------------------------------------------
int main()
{
    srand((unsigned)time(0));

    int opcao = -1;
    do
    {
        cout << "\n============ Ficha 1 - POO ============\n";
        cout << "1 - Exercicio 1 (Ordenacao -> CSV)\n";
        cout << "2 - Exercicio 2 (Lista de strings)\n";
        cout << "3 - Exercicio 3 (XMLWriter / XMLReader)\n";
        cout << "0 - Sair\n";
        cout << "Opcao: ";

        if (!(cin >> opcao))
            break; // fim de input (EOF) -> sai

        switch (opcao)
        {
            case 1: Exercicio1(); break;
            case 2: Exercicio2(); break;
            case 3: Exercicio3(); break;
            case 0: cout << "A sair...\n"; break;
            default: cout << "Opcao invalida.\n"; break;
        }
    } while (opcao != 0);

    return 0;
}
