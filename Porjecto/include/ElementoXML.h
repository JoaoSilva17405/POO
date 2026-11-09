#ifndef ELEMENTOXML_H
#define ELEMENTOXML_H

#include <list>
#include <string>

// Declaração antecipada: quem inclui este cabeçalho não fica a depender da biblioteca.
namespace tinyxml2
{
class XMLElement;
}

/**
 * @brief Leitura simples de um elemento XML (por cima do TinyXML-2).
 *
 * Cada campo é procurado **pelo nome** e tem um valor por omissão. Por isso a ordem dos
 * campos não interessa, os que faltam ficam com o valor por omissão e os que estão a
 * mais são ignorados.
 *
 * Não é dono de nada: o elemento pertence ao documento XML aberto no Load.
 */
class ElementoXML
{
    /** @brief Elemento lido (não dono); nullptr se não existir. */
    const tinyxml2::XMLElement *Ptr_Elemento;

    /** @brief Texto do filho com este nome, ou nullptr se o filho não existir ou estiver vazio. */
    const char *Texto_Filho(const std::string &nome) const;

public:
    /**
     * @brief Cria a leitura de um elemento.
     * @param elemento Elemento do documento (nullptr = elemento inexistente).
     */
    explicit ElementoXML(const tinyxml2::XMLElement *elemento);

    /** @brief Diz se o elemento existe. */
    bool Valido() const;

    /** @brief Nome da tag (por exemplo, "MCOMBUSTAO"). */
    std::string Nome() const;

    /**
     * @brief Texto de um campo.
     * @param nome Nome do campo.
     * @param omissao Devolvido se o campo não existir.
     */
    std::string Texto(const std::string &nome, const std::string &omissao = "") const;

    /**
     * @brief Inteiro de um campo (`85%` dá 85).
     * @param nome Nome do campo.
     * @param omissao Devolvido se o campo não existir.
     */
    int Inteiro(const std::string &nome, int omissao) const;

    /**
     * @brief Real de um campo, tal como está escrito (`0.01` dá 0.01).
     * @param nome Nome do campo.
     * @param omissao Devolvido se o campo não existir.
     */
    double Real(const std::string &nome, double omissao) const;

    /**
     * @brief Lê um par "a,b" (por exemplo, `10,20` ou `0, 80`).
     * @param nome Nome do campo.
     * @param a Recebe o primeiro valor.
     * @param b Recebe o segundo valor.
     * @return Verdadeiro se leu os dois valores.
     */
    bool Par(const std::string &nome, double &a, double &b) const;

    /** @brief Filho com este nome (inválido se não existir). */
    ElementoXML Filho(const std::string &nome) const;

    /** @brief Todos os filhos, pela ordem do ficheiro. */
    std::list<ElementoXML> Filhos() const;
};

#endif // ELEMENTOXML_H
