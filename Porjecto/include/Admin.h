#ifndef ADMIN_H
#define ADMIN_H

#include <string>

#include "User.h"

/**
 * @brief Perfil Admin: adiciona, remove, lista, corre a simulação e faz manutenção (tudo).
 */
class Admin : public User
{
public:
    /**
     * @brief Cria um utilizador com o perfil Admin.
     * @param _id Identificador.
     * @param _nome Nome.
     */
    Admin(int _id, const std::string &_nome);

    /** @brief Destrutor. */
    virtual ~Admin();

    /** @brief Perfil, para as listagens: "ADMIN". */
    virtual std::string QuemEs() const { return "ADMIN"; }

    /**
     * @brief Pode todas as ações.
     * @param acao Ação pedida.
     */
    virtual bool Pode(ACAO acao) const;
};

#endif // ADMIN_H
