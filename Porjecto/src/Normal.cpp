#include "Normal.h"

using namespace std;

Normal::Normal(int _id, const string &_nome) : User(_id, _nome)
{
}

Normal::~Normal()
{
}

bool Normal::Pode(ACAO acao) const
{
    // Opera a fábrica, mas não pode apagar equipamento.
    return acao != ACAO::REMOVER;
}
