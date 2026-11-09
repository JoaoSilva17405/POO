#ifndef MELETRICO_H
#define MELETRICO_H

#include "Motor.h"

/**
 * @brief Motor elétrico (tag XML `MELETRICO`).
 *
 * Como o MCombustao (e o Tubarao da Ficha 5), só escreve o que o distingue: o nome do
 * tipo, a cópia e o ritmo de aquecimento. Foi acrescentado sem mexer na Fábrica: esta
 * classe e uma linha no Catálogo.
 */
class MEletrico : public Motor
{
protected:
    /** @brief Aquece 5 graus por ciclo a trabalhar. */
    virtual double Aquecimento() const;

public:
    /** @brief Cria um motor elétrico vazio (os dados vêm do XML ou do menu). */
    MEletrico();

    /** @brief Destrutor. */
    virtual ~MEletrico();

    /** @brief Tag XML deste tipo: o nome do tipo só existe aqui. */
    virtual std::string QuemEs() const { return "MELETRICO"; }

    /** @brief Cópia nova deste motor (o chamador fica dono). */
    virtual Motor *Clone() const;
};

#endif // MELETRICO_H
