#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <iostream>
using namespace std;

class Pessoa
{
    public:
        Pessoa();
        Pessoa(string _nome, string _cidade, int _idade = 18);
        virtual ~Pessoa();

        // Getters (usados pelo Gestao para pesquisar / ordenar / gravar)
        string GetNome() const   { return NOME; }
        string GetCidade() const { return CIDADE; }
        int    GetIdade() const  { return IDADE; }

        // Setters
        void SetNome(string _nome)     { NOME = _nome; }
        void SetCidade(string _cidade) { CIDADE = _cidade; }
        void SetIdade(int _idade)      { IDADE = _idade; }

        // Escreve a pessoa no formato do ficheiro: Cidade;Nome;Idade
        void Gravar(ostream &f) const;
        void Show() const;

    protected:

    private:
        string NOME;
        string CIDADE;
        int IDADE;
};

#endif // PESSOA_H
