#ifndef ORDENACAO_H
#define ORDENACAO_H

// ============================================================================
// Classe Ordenacao da Ficha 1, ADAPTADA para a Ficha 2.
//
// Adaptacoes:
//   1. Na Ficha 1 os vetores eram de int (int *V). Aqui sao vetores de
//      ponteiros para Pessoa (Pessoa **V) e ordena-se pela IDADE.
//   2. Na Ficha 1 comparava-se com V[i] < V[j]. Com Pessoa* isso compararia
//      ENDERECOS de memoria, por isso toda a comparacao passa pelo metodo
//      privado Menor(a, b). Para ordenar por outro criterio (ex: nome) basta
//      alterar o Menor.
//   3. Retirada a gravacao em CSV e a medicao de tempo (eram para comparar
//      algoritmos na Ficha 1, aqui nao interessam). Os metodos devolvem o
//      numero de trocas.
// ============================================================================

#include "Pessoa.h"

class Ordenacao
{
    public:
        static int BubbleSort(Pessoa **V, int N);
        static int SelectionSort(Pessoa **V, int N);
        static int InsertionSort(Pessoa **V, int N);
        static int QuickSort(Pessoa **V, int N);

    private:
        static bool Menor(Pessoa *a, Pessoa *b);
        static void Troca(Pessoa *&a, Pessoa *&b, int &nTrocas);
        static void QuickRec(Pessoa **V, int esq, int dir, int &nTrocas);
};

#endif // ORDENACAO_H
