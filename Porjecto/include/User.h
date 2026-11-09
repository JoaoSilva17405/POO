#ifndef USER_H
#define USER_H

#include <string>

#include "Enumerados.h"

/**
 * @brief Utilizador da fábrica (ID, NOME), como no enunciado.
 *
 * É a base dos perfis Admin, Normal e Visitante. Cada perfil responde a `Pode(acao)` com a
 * sua linha da tabela de permissões: a Fábrica pergunta ao utilizador se pode, em vez de
 * fazer um `if` sobre o perfil (como o peixe que decide sobre si na Ficha 5).
 *
 * Um User sem perfil não tem nenhuma permissão. O Add(User*) tem uma issue própria; esta
 * classe já está pronta para haver vários utilizadores.
 */
class User
{
    /** @brief Identificador do utilizador. */
    int ID;
    /** @brief Nome do utilizador. */
    std::string NOME;

public:
    /**
     * @brief Cria um utilizador.
     * @param _id Identificador.
     * @param _nome Nome.
     */
    User(int _id, const std::string &_nome);

    /** @brief Destrutor virtual: a Fábrica liberta o utilizador pela classe base. */
    virtual ~User();

    /** @brief ID do utilizador (usado pela Fábrica). */
    int GetID() const { return ID; }

    /** @brief Nome do utilizador (usado pela Fábrica nas listagens). */
    const std::string &GetNome() const { return NOME; }

    /** @brief Perfil do utilizador, para as listagens. */
    virtual std::string QuemEs() const { return "USER"; }

    /**
     * @brief Diz se este utilizador pode fazer uma ação (tabela de permissões do enunciado).
     *
     * Cada perfil redefine-o. Um User sem perfil não pode fazer nada: na dúvida, recusa.
     * @param acao Ação pedida.
     * @return Verdadeiro se a ação é permitida.
     */
    virtual bool Pode(ACAO acao) const;
};

#endif // USER_H
