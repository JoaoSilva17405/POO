#include <iostream>
#include <cstdio>

using namespace std;

// Imprime, para uma dada "linha" (limiar de 9 a 1), um '*' ou '-' por
// digito do numero, da esquerda para a direita. Como nao pode usar
// arrays/vetores, percorremos os digitos com recursao (primeiro os mais
// significativos, atraves de n / 10, e so depois imprimimos o digito atual).
void ImprimeLinhaConsole(int n, int linha)
{
    if (n >= 10)
        ImprimeLinhaConsole(n / 10, linha);

    int digito = n % 10;
    cout << (digito >= linha ? '*' : '-') << " ";
}

void ImprimeLinhaFicheiro(FILE *f, int n, int linha)
{
    if (n >= 10)
        ImprimeLinhaFicheiro(f, n / 10, linha);

    int digito = n % 10;
    fprintf(f, "%c ", digito >= linha ? '*' : '-');
}

void ImprimeNumeroConsole(int n)
{
    if (n >= 10)
        ImprimeNumeroConsole(n / 10);

    cout << n % 10 << " ";
}

void ImprimeNumeroFicheiro(FILE *f, int n)
{
    if (n >= 10)
        ImprimeNumeroFicheiro(f, n / 10);

    fprintf(f, "%d ", n % 10);
}

void Histograma(int n)
{
    FILE *f = fopen("histograma.txt", "w");
    if (f == NULL)
    {
        cout << "Erro ao abrir o ficheiro!" << endl;
        return;
    }

    for (int linha = 9; linha >= 1; linha--)
    {
        ImprimeLinhaConsole(n, linha);
        cout << endl;

        ImprimeLinhaFicheiro(f, n, linha);
        fprintf(f, "\n");
    }

    ImprimeNumeroConsole(n);
    cout << endl;

    ImprimeNumeroFicheiro(f, n);
    fprintf(f, "\n");

    fclose(f);
}

int main()
{
    int N;
    cout << "Introduza um numero inteiro: ";
    cin >> N;

    if (N < 0)
        N = -N;

    Histograma(N);

    return 0;
}
