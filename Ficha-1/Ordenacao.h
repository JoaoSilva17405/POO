#ifndef ORDENACAO_H
#define ORDENACAO_H

// ============================================================================
// Ficha 1 - Exercicio 1
// Classe Ordenacao: reune varios algoritmos de ordenacao (todos ESTATICOS).
//
// Observacao da ficha: os metodos sao pensados para vetores de inteiros, mas a
// implementacao esta feita com TEMPLATES para ser facilmente generalizavel a
// qualquer tipo T que suporte os operadores de comparacao ( <, > ).
//
// Cada metodo:
//   - conta o numero de TROCAS efetuadas;
//   - mede o TEMPO de execucao (segundos);
//   - grava uma linha no ficheiro CSV: Metodo;N;NTrocas;Tempo
// ============================================================================

#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

class Ordenacao
{
private:
    // ---- Troca dois elementos e incrementa o contador de trocas ------------
    template <class T>
    static void Troca(T &a, T &b, long long &nTrocas)
    {
        T tmp = a;
        a = b;
        b = tmp;
        nTrocas++;
    }

    // ---- Grava uma linha de resultados no ficheiro CSV (modo append) --------
    static void GravaCSV(const std::string &ficheiro, const std::string &metodo,
                         int N, long long nTrocas, double tempo)
    {
        // Verifica se o ficheiro ja tem conteudo (para so escrever o cabecalho
        // uma unica vez, na primeira gravacao).
        bool temConteudo = false;
        {
            std::ifstream in(ficheiro.c_str());
            if (in.good() && in.peek() != std::ifstream::traits_type::eof())
                temConteudo = true;
        }

        std::ofstream out(ficheiro.c_str(), std::ios::app);
        if (!out.is_open())
            return;

        if (!temConteudo)
            out << "Metodo;N;NTrocas;Tempo\n";

        out << std::fixed << std::setprecision(6);
        out << metodo << ";" << N << ";" << nTrocas << ";" << tempo << "\n";
    }

    // ---- Auxiliares recursivos do QuickSort e MergeSort --------------------
    template <class T>
    static void QuickRec(T *V, int esq, int dir, long long &nTrocas)
    {
        int i = esq, j = dir;
        T pivot = V[(esq + dir) / 2];
        while (i <= j)
        {
            while (V[i] < pivot) i++;
            while (V[j] > pivot) j--;
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

    template <class T>
    static void Merge(T *V, int esq, int meio, int dir, long long &nTrocas)
    {
        int n1 = meio - esq + 1;
        int n2 = dir - meio;
        T *L = new T[n1];
        T *R = new T[n2];
        for (int a = 0; a < n1; a++) L[a] = V[esq + a];
        for (int b = 0; b < n2; b++) R[b] = V[meio + 1 + b];

        int i = 0, j = 0, k = esq;
        while (i < n1 && j < n2)
        {
            if (L[i] <= R[j]) V[k++] = L[i++];
            else              V[k++] = R[j++];
            nTrocas++; // conta cada colocacao (movimento) de um elemento
        }
        while (i < n1) { V[k++] = L[i++]; nTrocas++; }
        while (j < n2) { V[k++] = R[j++]; nTrocas++; }

        delete[] L;
        delete[] R;
    }

    template <class T>
    static void MergeRec(T *V, int esq, int dir, long long &nTrocas)
    {
        if (esq < dir)
        {
            int meio = esq + (dir - esq) / 2;
            MergeRec(V, esq, meio, nTrocas);
            MergeRec(V, meio + 1, dir, nTrocas);
            Merge(V, esq, meio, dir, nTrocas);
        }
    }

public:
    // ======================= BUBBLE SORT ====================================
    template <class T>
    static void BubbleSort(T *V, int N, const std::string &ficheiro_output)
    {
        long long nTrocas = 0;
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N - 1; i++)
        {
            bool trocou = false;
            for (int j = 0; j < N - 1 - i; j++)
                if (V[j] > V[j + 1])
                {
                    Troca(V[j], V[j + 1], nTrocas);
                    trocou = true;
                }
            if (!trocou) break; // ja esta ordenado
        }

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();
        GravaCSV(ficheiro_output, "Bubble", N, nTrocas, tempo);
    }

    // ======================= SELECTION SORT =================================
    template <class T>
    static void SelectionSort(T *V, int N, const std::string &ficheiro_output)
    {
        long long nTrocas = 0;
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N - 1; i++)
        {
            int menor = i;
            for (int j = i + 1; j < N; j++)
                if (V[j] < V[menor])
                    menor = j;
            if (menor != i)
                Troca(V[i], V[menor], nTrocas);
        }

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();
        GravaCSV(ficheiro_output, "Selection", N, nTrocas, tempo);
    }

    // ======================= INSERTION SORT =================================
    template <class T>
    static void InsertionSort(T *V, int N, const std::string &ficheiro_output)
    {
        long long nTrocas = 0;
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 1; i < N; i++)
        {
            T chave = V[i];
            int j = i - 1;
            while (j >= 0 && V[j] > chave)
            {
                V[j + 1] = V[j]; // deslocamento
                nTrocas++;
                j--;
            }
            V[j + 1] = chave;
        }

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();
        GravaCSV(ficheiro_output, "Insertion", N, nTrocas, tempo);
    }

    // ======================= SHELL SORT =====================================
    template <class T>
    static void ShellSort(T *V, int N, const std::string &ficheiro_output)
    {
        long long nTrocas = 0;
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int gap = N / 2; gap > 0; gap /= 2)
            for (int i = gap; i < N; i++)
            {
                T chave = V[i];
                int j = i;
                while (j >= gap && V[j - gap] > chave)
                {
                    V[j] = V[j - gap];
                    nTrocas++;
                    j -= gap;
                }
                V[j] = chave;
            }

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();
        GravaCSV(ficheiro_output, "Shell", N, nTrocas, tempo);
    }

    // ======================= QUICK SORT =====================================
    template <class T>
    static void QuickSort(T *V, int N, const std::string &ficheiro_output)
    {
        long long nTrocas = 0;
        auto inicio = std::chrono::high_resolution_clock::now();

        if (N > 1)
            QuickRec(V, 0, N - 1, nTrocas);

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();
        GravaCSV(ficheiro_output, "Quick", N, nTrocas, tempo);
    }

    // ======================= MERGE SORT =====================================
    template <class T>
    static void MergeSort(T *V, int N, const std::string &ficheiro_output)
    {
        long long nTrocas = 0;
        auto inicio = std::chrono::high_resolution_clock::now();

        if (N > 1)
            MergeRec(V, 0, N - 1, nTrocas);

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();
        GravaCSV(ficheiro_output, "Merge", N, nTrocas, tempo);
    }
};

#endif // ORDENACAO_H
