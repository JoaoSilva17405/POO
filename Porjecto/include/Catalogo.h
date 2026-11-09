#ifndef CATALOGO_H
#define CATALOGO_H

#include <list>
#include <string>

class Motor;

/**
 * @brief Registo de protótipos: o **único sítio** que enumera os tipos concretos de dispositivo.
 *
 * O construtor regista um protótipo de cada classe concreta (uma linha por tipo). Para
 * criar um dispositivo a partir de uma tag XML, o Catálogo procura o protótipo com esse
 * `QuemEs()` e devolve um `Clone()` dele. Assim, a Fábrica e o Load nunca fazem `if`
 * sobre a tag, e um tipo novo é **uma classe nova + uma linha no construtor do Catálogo**.
 *
 * Os registos estão separados por categoria (motores; os sensores chegam numa fase
 * seguinte), para cada criação devolver logo o tipo certo.
 *
 * É dono dos protótipos (liberta-os no destrutor) e não é copiável. Os protótipos nunca
 * entram na simulação.
 */
class Catalogo
{
    /** @brief Protótipos de motores (dono). */
    std::list<Motor *> LM_Prototipos;

    /** @brief Protótipo de motor com esta tag, ou nullptr. */
    Motor *Prototipo_Motor(const std::string &tag) const;

    /** @brief Tag normalizada: sem espaços nas pontas e em maiúsculas. */
    static std::string Normalizar(const std::string &tag);

public:
    /** @brief Regista um protótipo de cada tipo concreto (uma linha por tipo). */
    Catalogo();

    /** @brief Liberta todos os protótipos. */
    ~Catalogo();

    /** @brief Cópia proibida: os protótipos seriam libertados duas vezes. */
    Catalogo(const Catalogo &) = delete;

    /** @brief Atribuição proibida, pela mesma razão. */
    Catalogo &operator=(const Catalogo &) = delete;

    /**
     * @brief Regista um protótipo de motor, com a chave `QuemEs()`.
     *
     * Se a tag já estiver registada, o protótipo anterior é substituído e libertado.
     * Usado pelo construtor e pelos testes (que registam tipos definidos só nos testes).
     * @param prototipo Protótipo (o Catálogo fica dono; nullptr é ignorado).
     */
    void Registar_Motor(Motor *prototipo);

    /**
     * @brief Cria um motor novo do tipo com esta tag (chamado pela Fábrica no Load e no menu).
     * @param tag Tag XML do tipo (sem distinguir maiúsculas/minúsculas).
     * @return Motor novo (o chamador fica dono) ou nullptr se o tipo não existir.
     */
    Motor *CriarMotor(const std::string &tag) const;

    /** @brief Tags dos tipos de motor registados, pela ordem de registo. */
    std::list<std::string> Tipos_Motor() const;
};

#endif // CATALOGO_H
