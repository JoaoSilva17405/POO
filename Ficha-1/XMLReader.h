#ifndef XMLREADER_H
#define XMLREADER_H

// ============================================================================
// Ficha 1 - Exercicio 3
// Classe XMLReader: le um ficheiro XML no do formato gerado pelo XMLWriter e
// permite percorre-lo no a no (a semelhanca do XmlReader do .NET).
//
// Read()      -> avanca para o proximo no; devolve false quando termina.
// NodeType()  -> tipo do no atual (Inicio de elemento, Fim de elemento, Texto).
// Name()      -> nome do elemento atual.
// Value()     -> valor (texto) do no atual.
// ============================================================================

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class XMLReader
{
public:
    enum TipoNo { NENHUM, INICIO_ELEMENTO, FIM_ELEMENTO, TEXTO };

private:
    std::string conteudo; // todo o ficheiro em memoria
    std::size_t pos;      // posicao de leitura atual

    TipoNo tipo;
    std::string nome;
    std::string valor;

    // Salta espacos, mudancas de linha e tabs.
    void SaltaEspacos()
    {
        while (pos < conteudo.size() &&
               (conteudo[pos] == ' ' || conteudo[pos] == '\n' ||
                conteudo[pos] == '\r' || conteudo[pos] == '\t'))
            pos++;
    }

public:
    XMLReader() : pos(0), tipo(NENHUM) {}

    ~XMLReader() {}

    // Abre e carrega o ficheiro XML para memoria.
    bool Open(const std::string &ficheiro)
    {
        std::ifstream in(ficheiro.c_str());
        if (!in.is_open())
        {
            std::cout << "Nao foi possivel abrir o ficheiro XML: " << ficheiro << "\n";
            return false;
        }
        std::stringstream ss;
        ss << in.rdbuf();
        conteudo = ss.str();
        pos = 0;
        tipo = NENHUM;

        // Ignora a declaracao <?xml ... ?> se existir.
        SaltaEspacos();
        if (conteudo.compare(pos, 2, "<?") == 0)
        {
            std::size_t fim = conteudo.find("?>", pos);
            if (fim != std::string::npos)
                pos = fim + 2;
        }
        return true;
    }

    // Avanca para o proximo no. Devolve false quando nao ha mais nada.
    bool Read()
    {
        SaltaEspacos();
        if (pos >= conteudo.size())
            return false;

        if (conteudo[pos] == '<')
        {
            // E uma tag (inicio ou fim de elemento).
            std::size_t fim = conteudo.find('>', pos);
            if (fim == std::string::npos)
                return false;

            std::string tag = conteudo.substr(pos + 1, fim - pos - 1);
            pos = fim + 1;

            if (!tag.empty() && tag[0] == '/')
            {
                tipo = FIM_ELEMENTO;
                nome = tag.substr(1);
                valor = "";
            }
            else
            {
                tipo = INICIO_ELEMENTO;
                nome = tag;
                valor = "";
            }
            return true;
        }
        else
        {
            // E texto (ate ao proximo '<').
            std::size_t fim = conteudo.find('<', pos);
            if (fim == std::string::npos)
                fim = conteudo.size();
            tipo = TEXTO;
            nome = "";
            valor = conteudo.substr(pos, fim - pos);
            pos = fim;
            return true;
        }
    }

    TipoNo NodeType() const { return tipo; }
    std::string Name() const { return nome; }
    std::string Value() const { return valor; }
};

#endif // XMLREADER_H
