#ifndef MINDUCAO_H
#define MINDUCAO_H

#include "Motor.h"

/**
 * @brief Motor de indução (tag XML `MINDUCAO`).
 *
 * Como o MCombustao (e o Tubarao da Ficha 5), só escreve o que o distingue: o nome do
 * tipo, a cópia e o ritmo de aquecimento. Foi acrescentado sem mexer na Fábrica: esta
 * classe e uma linha no Catálogo.
 */
class MInducao : public Motor
{
protected:
    /** @brief Aquece 3 graus por ciclo a trabalhar. */
    virtual double Aquecimento() const;

public:
    /** @brief Cria um motor de indução vazio (os dados vêm do XML ou do menu). */
    MInducao();

    /** @brief Destrutor. */
    virtual ~MInducao();

    /** @brief Tag XML deste tipo: o nome do tipo só existe aqui. */
    virtual std::string QuemEs() const { return "MINDUCAO"; }

    /** @brief Cópia nova deste motor (o chamador fica dono). */
    virtual Motor *Clone() const;
};

#endif // MINDUCAO_H
