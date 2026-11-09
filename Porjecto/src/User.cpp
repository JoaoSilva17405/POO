#include "User.h"

using namespace std;

User::User(int _id, const string &_nome)
{
    ID = _id;
    NOME = _nome;
}

User::~User()
{
}

bool User::Pode(ACAO acao) const
{
    // Sem perfil, nenhuma permissão. O parâmetro só é usado nas subclasses.
    (void)acao;
    return false;
}
