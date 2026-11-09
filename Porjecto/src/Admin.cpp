#include "Admin.h"

using namespace std;

Admin::Admin(int _id, const string &_nome) : User(_id, _nome)
{
}

Admin::~Admin()
{
}

bool Admin::Pode(ACAO acao) const
{
    // O Admin gere a fábrica por completo: pode todas as ações.
    (void)acao;
    return true;
}
