#include "Pessoa.h"

Pessoa::Pessoa()
{
    NOME = "";
    CIDADE = "";
    IDADE = 18;
}

Pessoa::Pessoa(string _nome, string _cidade, int _idade)
{
    NOME = _nome;
    CIDADE = _cidade;
    IDADE = _idade;
}

Pessoa::~Pessoa()
{
    //dtor
}

void Pessoa::Gravar(ostream &f) const
{
    f << CIDADE << ";" << NOME << ";" << IDADE << endl;
}

void Pessoa::Show() const
{
    cout << "  " << NOME << " (" << IDADE << " anos) - " << CIDADE << endl;
}
