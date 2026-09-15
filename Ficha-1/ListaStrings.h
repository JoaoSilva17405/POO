#ifndef LISTASTRINGS_H
#define LISTASTRINGS_H

// ============================================================================
// Ficha 1 - Exercicio 2
// Trabalhar com a class list<string> da STL, usando o mais possivel os metodos
// que a propria std::list ja disponibiliza.
//
// Funcionalidades a) ... k) pedidas no enunciado.
// ============================================================================

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <iterator>

namespace Ex2
{
    // a) Declarar uma list de strings ->  std::list<std::string> lista;

    // b) Ler de um ficheiro de texto todas as palavras e coloca-las na lista.
    //    Usa o operador >> que ja separa por espacos/mudancas de linha.
    inline bool LerDeFicheiro(std::list<std::string> &lista, const std::string &ficheiro)
    {
        std::ifstream in(ficheiro.c_str());
        if (!in.is_open())
        {
            std::cout << "Nao foi possivel abrir o ficheiro: " << ficheiro << "\n";
            return false;
        }
        std::string palavra;
        while (in >> palavra)
            lista.push_back(palavra); // metodo da propria list
        return true;
    }

    // c) Contar todas as palavras.
    inline std::size_t ContarPalavras(const std::list<std::string> &lista)
    {
        return lista.size(); // metodo da propria list
    }

    // d) Listar as palavras que estao na lista.
    inline void Listar(const std::list<std::string> &lista)
    {
        std::cout << "Lista (" << lista.size() << " palavras): ";
        for (std::list<std::string>::const_iterator it = lista.begin();
             it != lista.end(); ++it)
            std::cout << *it << " ";
        std::cout << "\n";
    }

    // e) Listar ao contrario o conteudo da lista (reverse iterators).
    inline void ListarInverso(const std::list<std::string> &lista)
    {
        std::cout << "Lista ao contrario: ";
        for (std::list<std::string>::const_reverse_iterator it = lista.rbegin();
             it != lista.rend(); ++it)
            std::cout << *it << " ";
        std::cout << "\n";
    }

    // f) Eliminar uma palavra da lista (dada a palavra) -> list::remove.
    inline void EliminarPorValor(std::list<std::string> &lista, const std::string &palavra)
    {
        lista.remove(palavra); // remove TODAS as ocorrencias dessa palavra
    }

    // g) Eliminar uma palavra da lista (dada uma posicao) -> avanca iterador e erase.
    inline void EliminarPorPosicao(std::list<std::string> &lista, std::size_t pos)
    {
        if (pos >= lista.size())
        {
            std::cout << "Posicao invalida (" << pos << ").\n";
            return;
        }
        std::list<std::string>::iterator it = lista.begin();
        std::advance(it, pos);
        lista.erase(it); // metodo da propria list
    }

    // h) Verificar se existem palavras repetidas.
    inline bool ExistemRepetidas(const std::list<std::string> &lista)
    {
        std::set<std::string> vistas;
        for (std::list<std::string>::const_iterator it = lista.begin();
             it != lista.end(); ++it)
            if (!vistas.insert(*it).second) // insert falha => ja existia
                return true;
        return false;
    }

    // i) Contar o numero de palavras de tamanho inferior a 5.
    inline int ContarInferiores(const std::list<std::string> &lista, std::size_t tamanho = 5)
    {
        int cont = 0;
        for (std::list<std::string>::const_iterator it = lista.begin();
             it != lista.end(); ++it)
            if (it->size() < tamanho)
                cont++;
        return cont;
    }

    // j) Passar todas as palavras da lista para maiusculas.
    inline void ParaMaiusculas(std::list<std::string> &lista)
    {
        for (std::list<std::string>::iterator it = lista.begin();
             it != lista.end(); ++it)
            std::transform(it->begin(), it->end(), it->begin(), ::toupper);
    }

    // k) Gravar num ficheiro de texto as palavras de tamanho superior a 10.
    inline bool GravarGrandes(const std::list<std::string> &lista,
                              const std::string &ficheiro, std::size_t tamanho = 10)
    {
        std::ofstream out(ficheiro.c_str());
        if (!out.is_open())
            return false;
        for (std::list<std::string>::const_iterator it = lista.begin();
             it != lista.end(); ++it)
            if (it->size() > tamanho)
                out << *it << "\n";
        return true;
    }
}

#endif // LISTASTRINGS_H
