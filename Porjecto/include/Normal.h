#ifndef NORMAL_H
#define NORMAL_H

#include <string>

#include "User.h"

/**
 * @brief Perfil Normal: adiciona, lista, corre a simulação e faz manutenção, mas não remove.
 */
class Normal : public User
{
public:
    /**
     * @brief Cria um utilizador com o perfil Normal.
     * @param _id Identificador.
     * @param _nome Nome.
     */
    Normal(int _id, const std::string &_nome);

    /** @brief Destrutor. */
    virtual ~Normal();

    /** @brief Perfil, para as listagens: "NORMAL". */
    virtual std::string QuemEs() const { return "NORMAL"; }

    /**
     * @brief Pode tudo menos REMOVER.
     * @param acao Ação pedida.
     */
    virtual bool Pode(ACAO acao) const;
};

#endif // NORMAL_H
