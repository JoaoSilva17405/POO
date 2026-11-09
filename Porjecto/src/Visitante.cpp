#include "Visitante.h"

using namespace std;

Visitante::Visitante(int _id, const string &_nome) : User(_id, _nome)
{
}

Visitante::~Visitante()
{
}

bool Visitante::Pode(ACAO acao) const
{
    // Só vê o estado da fábrica.
    return acao == ACAO::LISTAR;
}
