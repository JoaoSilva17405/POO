#ifndef XMLWRITER_H
#define XMLWRITER_H

// ============================================================================
// Ficha 1 - Exercicio 3
// Classe XMLWriter: grava ficheiros XML de forma incremental, respeitando a
// indentacao e o fecho correto (por ordem) dos elementos abertos.
//
// Usa uma pilha (std::stack) para saber qual o elemento a fechar em
// WriteEndElement() e para calcular a indentacao.
// ============================================================================

#include <string>
#include <fstream>
#include <stack>
#include <iostream>

class XMLWriter
{
private:
    std::ofstream out;          // ficheiro de saida
    std::stack<std::string> elementos; // elementos atualmente abertos

    // Escreve a indentacao (2 espacos por nivel de profundidade).
    void Indenta()
    {
        for (std::size_t i = 0; i < elementos.size(); i++)
            out << "  ";
    }

public:
    XMLWriter() {}

    ~XMLWriter()
    {
        // Garante que o ficheiro fica fechado mesmo que se esqueca WriteEndDocument.
        if (out.is_open())
            out.close();
    }

    // Abre o documento/ficheiro e escreve o cabecalho XML.
    void WriteStartDocument(const std::string &ficheiro)
    {
        out.open(ficheiro.c_str());
        if (!out.is_open())
        {
            std::cout << "Nao foi possivel criar o ficheiro XML: " << ficheiro << "\n";
            return;
        }
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    }

    // Fecha o documento (fecha eventuais elementos ainda abertos).
    void WriteEndDocument()
    {
        while (!elementos.empty())
            WriteEndElement();
        if (out.is_open())
            out.close();
    }

    // Abre um elemento:  <el>
    void WriteStartElement(const std::string &el)
    {
        Indenta();
        out << "<" << el << ">\n";
        elementos.push(el);
    }

    // Fecha o ultimo elemento aberto:  </el>
    void WriteEndElement()
    {
        if (elementos.empty())
            return;
        std::string el = elementos.top();
        elementos.pop();
        Indenta();
        out << "</" << el << ">\n";
    }

    // Escreve um elemento completo numa linha:  <el>valor</el>
    void WriteElementString(const std::string &el, const std::string &valor)
    {
        Indenta();
        out << "<" << el << ">" << valor << "</" << el << ">\n";
    }
};

#endif // XMLWRITER_H
