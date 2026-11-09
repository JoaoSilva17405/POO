#ifndef VISITANTE_H
#define VISITANTE_H

#include <string>

#include "User.h"

/**
 * @brief Perfil Visitante: só lista e consulta; não altera nada na fábrica.
 */
class Visitante : public User
{
public:
    /**
     * @brief Cria um utilizador com o perfil Visitante.
     * @param _id Identificador.
     * @param _nome Nome.
     */
    Visitante(int _id, const std::string &_nome);

    /** @brief Destrutor. */
    virtual ~Visitante();

    /** @brief Perfil, para as listagens: "VISITANTE". */
    virtual std::string QuemEs() const { return "VISITANTE"; }

    /**
     * @brief Só pode LISTAR.
     * @param acao Ação pedida.
     */
    virtual bool Pode(ACAO acao) const;
};

#endif // VISITANTE_H
