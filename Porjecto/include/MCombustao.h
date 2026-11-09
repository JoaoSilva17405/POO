#ifndef MCOMBUSTAO_H
#define MCOMBUSTAO_H

#include "Motor.h"

/**
 * @brief Motor de combustão (tag XML `MCOMBUSTAO`).
 *
 * Como o Tubarao da Ficha 5, só escreve o que o distingue: o nome do tipo, a cópia e o
 * ritmo de aquecimento.
 */
class MCombustao : public Motor
{
protected:
    /** @brief Aquece 8 graus por ciclo a trabalhar. */
    virtual double Aquecimento() const;

public:
    /** @brief Cria um motor de combustão vazio (os dados vêm do XML ou do menu). */
    MCombustao();

    /** @brief Destrutor. */
    virtual ~MCombustao();

    /** @brief Tag XML deste tipo: o nome do tipo só existe aqui. */
    virtual std::string QuemEs() const { return "MCOMBUSTAO"; }

    /** @brief Cópia nova deste motor (o chamador fica dono). */
    virtual Motor *Clone() const;
};

#endif // MCOMBUSTAO_H
