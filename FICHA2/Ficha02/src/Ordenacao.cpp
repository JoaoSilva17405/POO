#include "Ordenacao.h"

// a deve ficar antes de b se for mais novo
bool Ordenacao::Menor(Pessoa *a, Pessoa *b)
{
    return a->GetIdade() < b->GetIdade();
}

// Troca os PONTEIROS (as Pessoas nao sao copiadas).
// Pessoa *&a = referencia para um ponteiro, para a troca ficar no vetor.
void Ordenacao::Troca(Pessoa *&a, Pessoa *&b, int &nTrocas)
{
    Pessoa *tmp = a;
    a = b;
    b = tmp;
    nTrocas++;
}

// ======================= BUBBLE SORT ========================================
int Ordenacao::BubbleSort(Pessoa **V, int N)
{
    int nTrocas = 0;
    for (int i = 0; i < N - 1; i++)
    {
        bool trocou = false;
        for (int j = 0; j < N - 1 - i; j++)
            if (Menor(V[j + 1], V[j]))     // era: V[j] > V[j + 1]
            {
                Troca(V[j], V[j + 1], nTrocas);
                trocou = true;
            }
        if (!trocou) break; // ja esta ordenado
    }
    return nTrocas;
}

// ======================= SELECTION SORT =====================================
int Ordenacao::SelectionSort(Pessoa **V, int N)
{
    int nTrocas = 0;
    for (int i = 0; i < N - 1; i++)
    {
        int m = i;
        for (int j = i + 1; j < N; j++)
            if (Menor(V[j], V[m]))         // era: V[j] < V[menor]
                m = j;
        if (m != i)
            Troca(V[i], V[m], nTrocas);
    }
    return nTrocas;
}

// ======================= INSERTION SORT =====================================
// Estavel: pessoas com a mesma idade mantem a ordem original.
int Ordenacao::InsertionSort(Pessoa **V, int N)
{
    int nTrocas = 0;
    for (int i = 1; i < N; i++)
    {
        Pessoa *chave = V[i];
        int j = i - 1;
        while (j >= 0 && Menor(chave, V[j]))   // era: V[j] > chave
        {
            V[j + 1] = V[j]; // deslocamento
            nTrocas++;
            j--;
        }
        V[j + 1] = chave;
    }
    return nTrocas;
}

// ======================= QUICK SORT =========================================
void Ordenacao::QuickRec(Pessoa **V, int esq, int dir, int &nTrocas)
{
    int i = esq, j = dir;
    Pessoa *pivot = V[(esq + dir) / 2];
    while (i <= j)
    {
        while (Menor(V[i], pivot)) i++;   // era: V[i] < pivot
        while (Menor(pivot, V[j])) j--;   // era: V[j] > pivot
        if (i <= j)
        {
            if (i != j)
                Troca(V[i], V[j], nTrocas);
            i++;
            j--;
        }
    }
    if (esq < j) QuickRec(V, esq, j, nTrocas);
    if (i < dir) QuickRec(V, i, dir, nTrocas);
}

int Ordenacao::QuickSort(Pessoa **V, int N)
{
    int nTrocas = 0;
    if (N > 1)
        QuickRec(V, 0, N - 1, nTrocas);
    return nTrocas;
}
