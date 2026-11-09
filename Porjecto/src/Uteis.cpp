#include "Uteis.h"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <limits>

using namespace std;

// Código específico de Windows: só aqui (regra do projeto).
#include <conio.h>
#include <windows.h>

bool Uteis::VIDEOS_ATIVOS = true;

int Uteis::Aleatorio(int min, int max)
{
    if (max <= min)
        return min;
    return min + rand() % (max - min + 1);
}

double Uteis::Aleatorio_Real(double min, double max)
{
    if (max <= min)
        return min;
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

bool Uteis::Acontece(double percentagem)
{
    // rand()/(RAND_MAX+1.0) está em [0,1); vezes 100 fica em [0,100).
    // Assim percentagem 0 nunca acontece e 100 acontece sempre.
    return Aleatorio_Real(0.0, 100.0) < percentagem;
}

bool Uteis::TeclaPressionada()
{
    return _kbhit() != 0;
}

char Uteis::LerTecla()
{
    return static_cast<char>(_getch());
}

void Uteis::Esperar(int milissegundos)
{
    if (milissegundos <= 0)
        return;
    Sleep(milissegundos);
}

int Uteis::LerInteiro(const string &mensagem)
{
    int valor;
    cout << mensagem;
    while (!(cin >> valor))
    {
        // Entrada inválida: limpa o erro e o resto da linha e pede outra vez.
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Valor invalido. " << mensagem;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return valor;
}

string Uteis::LerTexto(const string &mensagem)
{
    string linha;
    cout << mensagem;
    getline(cin, linha);
    return Trim(linha);
}

string Uteis::Trim(const string &texto)
{
    const string brancos = " \t\r\n";
    string::size_type inicio = texto.find_first_not_of(brancos);
    if (inicio == string::npos)
        return "";
    string::size_type fim = texto.find_last_not_of(brancos);
    return texto.substr(inicio, fim - inicio + 1);
}

string Uteis::Maiusculas(const string &texto)
{
    string resultado = texto;
    for (string::size_type i = 0; i < resultado.size(); ++i)
        resultado[i] = static_cast<char>(toupper(static_cast<unsigned char>(resultado[i])));
    return resultado;
}

string Uteis::Escapar_XML(const string &texto)
{
    string resultado;
    for (string::size_type i = 0; i < texto.size(); ++i)
    {
        switch (texto[i])
        {
            case '&': resultado += "&amp;"; break;
            case '<': resultado += "&lt;"; break;
            case '>': resultado += "&gt;"; break;
            case '"': resultado += "&quot;"; break;
            case '\'': resultado += "&apos;"; break;
            default: resultado += texto[i]; break;
        }
    }
    return resultado;
}

bool Uteis::Substituir_Ficheiro(const string &temporario, const string &definitivo)
{
    // MOVEFILE_REPLACE_EXISTING: troca o conteúdo numa só operação, sem apagar antes.
    return MoveFileExA(temporario.c_str(), definitivo.c_str(), MOVEFILE_REPLACE_EXISTING) != 0;
}

bool Uteis::Abrir_Video(const string &ficheiro)
{
    if (!VIDEOS_ATIVOS || ficheiro.empty())
        return false;
    // "start" abre com o programa associado e não bloqueia a simulação.
    string comando = "start \"\" \"" + ficheiro + "\"";
    system(comando.c_str());
    return true;
}

void Uteis::Videos_Ativos(bool ativos)
{
    VIDEOS_ATIVOS = ativos;
}

void Uteis::Configurar_Consola()
{
    SetConsoleOutputCP(CP_UTF8);
}
