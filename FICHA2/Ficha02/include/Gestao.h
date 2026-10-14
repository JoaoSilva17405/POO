#ifndef GESTAO_H
#define GESTAO_H

#include <string>
#include <list>
#include <map>
#include "Pessoa.h"
using namespace std;

class Gestao
{
    public:
        Gestao();
        virtual ~Gestao();

        // O Gestao e "dono" dos ponteiros: uma copia faria delete duas vezes
        Gestao(const Gestao &) = delete;
        Gestao &operator=(const Gestao &) = delete;

        bool AddPessoa(string _nome, string _cidade, int _idade);   // 3a
        bool AddPessoa(Pessoa *P);                                  // 3b
        bool LerFicheiro(string nome_ficheiro);                     // 3c
        void Mostrar();                                             // 3d
        bool Remover(Pessoa *P);                                    // 3e
        bool Remover(string _nome);                                 // 3f
        bool GravarFicheiro(string ficheiro);                       // 3g
        Pessoa *PesquisarPessoa(string _nome);                      // 3h
        string GetCidadeMaisHabitantes();                           // 3i
        void OrdenarPorIdade();                                     // 3j

    protected:

    private:
        // chave = nome da cidade, valor = lista de pessoas dessa cidade
        map<string, list<Pessoa *>> DADOS;
};

#endif // GESTAO_H
