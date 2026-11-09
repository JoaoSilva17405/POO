#ifndef CONFIGMOTOR_H
#define CONFIGMOTOR_H

#include <iosfwd>
#include <string>

#include "Enumerados.h"

class ElementoXML;

/**
 * @brief Configuração de um tipo de motor: intervalos de temperatura e probabilidade de avaria.
 *
 * Vem da secção `<DEFINICOES>` do XML (por exemplo, `<MCOMBUSTAO>`). Há uma por tipo de
 * motor; a dona é a Fábrica e os motores desse tipo apontam para ela (sem serem donos).
 */
class ConfigMotor
{
    /** @brief Tag do tipo de motor a que se aplica (por exemplo, "MCOMBUSTAO"). */
    std::string TIPO;
    /** @brief Início da zona VERDE. */
    double VERDE_MIN;
    /** @brief Fim da zona VERDE. */
    double VERDE_MAX;
    /** @brief Início da zona AMARELO (temperatura de aviso). */
    double AMARELO_MIN;
    /** @brief Fim da zona AMARELO. */
    double AMARELO_MAX;
    /** @brief Início da zona VERMELHO (temperatura de paragem). */
    double VERMELHO_MIN;
    /** @brief Fim da zona VERMELHO. */
    double VERMELHO_MAX;
    /** @brief Probabilidade de avaria por ciclo, em % (lida exatamente como está no XML). */
    double PROB_AVARIA;

public:
    /**
     * @brief Cria uma configuração com valores por omissão.
     *
     * VERDE 0–80, AMARELO 81–100, VERMELHO 101–250 e probabilidade de avaria 1%. É usada
     * quando o XML não define o tipo.
     * @param _tipo Tag do tipo de motor.
     */
    explicit ConfigMotor(const std::string &_tipo);

    /**
     * @brief Lê a configuração de um elemento como `<MCOMBUSTAO>` das definições.
     *
     * Cada campo é procurado por nome; os que faltam mantêm o valor que tinham.
     * @param elemento Elemento com VERDE, AMARELO, VERMELHO e PROB_AVARIA.
     */
    void LerXML(const ElementoXML &elemento);

    /** @brief Tag do tipo de motor (usada pela Fábrica para encontrar a configuração). */
    const std::string &GetTipo() const { return TIPO; }

    /**
     * @brief Classifica uma temperatura numa cor.
     *
     * A partir do início do VERMELHO é VERMELHO; a partir do início do AMARELO é AMARELO;
     * abaixo disso é VERDE. Assim não há buracos entre zonas (por exemplo, 80.5).
     * @param temperatura Temperatura do motor.
     */
    COR_TEMP Classificar(double temperatura) const;

    /** @brief Temperatura de aviso: início da zona AMARELO. */
    double Temp_Aviso() const { return AMARELO_MIN; }

    /** @brief Temperatura de paragem: início da zona VERMELHO. */
    double Temp_Paragem() const { return VERMELHO_MIN; }

    /** @brief Probabilidade de avaria por ciclo, em %. */
    double Prob_Avaria() const { return PROB_AVARIA; }

    /** @brief Temperatura aleatória dentro da zona VERDE (usada na manutenção). */
    double Temp_Verde_Aleatoria() const;

    /**
     * @brief Escreve a configuração (uma linha) para o ecrã ou para um ficheiro.
     * @param f Destino da escrita.
     */
    void Escreve(std::ostream &f) const;

    /**
     * @brief Escreve a configuração em XML, como na secção `<DEFINICOES>` (por exemplo, `<MCOMBUSTAO>`).
     *
     * O formato é o que o LerXML lê, por isso o ficheiro do simulador pode ser carregado de
     * novo. Chamado pela Fábrica ao gravar o XML do simulador.
     * @param f Destino da escrita.
     */
    void EscreveXML(std::ostream &f) const;
};

#endif // CONFIGMOTOR_H
