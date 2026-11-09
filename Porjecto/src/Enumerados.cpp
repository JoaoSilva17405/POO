#include "Enumerados.h"
#include "Uteis.h"

using namespace std;

string Conversoes::Texto(ESTADO_MOTOR estado)
{
    switch (estado)
    {
        case ESTADO_MOTOR::STOP: return "STOP";
        case ESTADO_MOTOR::START: return "START";
        case ESTADO_MOTOR::RUN: return "RUN";
        case ESTADO_MOTOR::AVARIADO: return "AVARIADO";
        case ESTADO_MOTOR::INEXISTENTE: return "INEXISTENTE";
    }
    return "?";
}

string Conversoes::Texto(COR_TEMP cor)
{
    switch (cor)
    {
        case COR_TEMP::VERDE: return "VERDE";
        case COR_TEMP::AMARELO: return "AMARELO";
        case COR_TEMP::VERMELHO: return "VERMELHO";
    }
    return "?";
}

string Conversoes::Texto(ACAO acao)
{
    switch (acao)
    {
        case ACAO::ADD: return "ADD";
        case ACAO::REMOVER: return "REMOVER";
        case ACAO::LISTAR: return "LISTAR";
        case ACAO::RUN: return "RUN";
        case ACAO::MANUTENCAO: return "MANUTENCAO";
    }
    return "?";
}

ESTADO_MOTOR Conversoes::Para_Estado(const string &texto, ESTADO_MOTOR omissao)
{
    // Percorre os valores e compara com o texto de cada um: a tabela de nomes fica só
    // no Texto(), não é repetida aqui.
    const ESTADO_MOTOR todos[] = {ESTADO_MOTOR::STOP, ESTADO_MOTOR::START, ESTADO_MOTOR::RUN,
                                  ESTADO_MOTOR::AVARIADO, ESTADO_MOTOR::INEXISTENTE};
    string procurado = Uteis::Maiusculas(Uteis::Trim(texto));
    for (int i = 0; i < 5; ++i)
        if (Texto(todos[i]) == procurado)
            return todos[i];
    return omissao;
}

COR_TEMP Conversoes::Para_Cor(const string &texto, COR_TEMP omissao)
{
    const COR_TEMP todas[] = {COR_TEMP::VERDE, COR_TEMP::AMARELO, COR_TEMP::VERMELHO};
    string procurado = Uteis::Maiusculas(Uteis::Trim(texto));
    for (int i = 0; i < 3; ++i)
        if (Texto(todas[i]) == procurado)
            return todas[i];
    return omissao;
}
