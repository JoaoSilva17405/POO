#ifndef ENUMERADOS_H_INCLUDED
#define ENUMERADOS_H_INCLUDED

#include <string>

/**
 * @file Enumerados.h
 * @brief Enumerados do projeto e o único sítio onde são convertidos de e para texto.
 *
 * Usa-se `enum class` (C++11) porque os nomes do enunciado repetem-se: `RUN` é um estado
 * do motor, uma ação de utilizador e um método do Motor (`RUN()`). Com `enum class` cada
 * valor escreve-se com o nome do tipo (`ESTADO_MOTOR::RUN`) e não há colisões.
 */

/** @brief Estado de um motor (enunciado), mais INEXISTENTE para IDs que não existem. */
enum class ESTADO_MOTOR
{
    STOP,       ///< Parado.
    START,      ///< A arrancar (passa a RUN no ciclo seguinte).
    RUN,        ///< A trabalhar.
    AVARIADO,   ///< Avariado até ter manutenção.
    INEXISTENTE ///< Devolvido por Get_Estado quando o ID não existe.
};

/** @brief Classe de temperatura de um motor (enunciado). */
enum class COR_TEMP
{
    VERDE,   ///< A funcionar bem.
    AMARELO, ///< A aquecer: avisa o gestor.
    VERMELHO ///< Muito quente: avisa o gestor e pára.
};

/** @brief Ações que um utilizador pode ter permissão para fazer (enunciado). */
enum class ACAO
{
    ADD,       ///< Adicionar.
    REMOVER,   ///< Remover.
    LISTAR,    ///< Listar e consultar.
    RUN,       ///< Correr a simulação.
    MANUTENCAO ///< Fazer manutenção.
};

/**
 * @brief Conversões dos enumerados de e para texto, num único sítio.
 *
 * O XML, o HTML, as listagens e o Load usam estas funções. Assim, acrescentar um valor a
 * um enumerado só obriga a mudar este ficheiro, e não há `switch` de conversão
 * espalhados pelo código.
 */
class Conversoes
{
public:
    /** @brief Texto de um estado (por exemplo, "RUN"). */
    static std::string Texto(ESTADO_MOTOR estado);

    /** @brief Texto de uma cor (por exemplo, "AMARELO"). */
    static std::string Texto(COR_TEMP cor);

    /** @brief Texto de uma ação (por exemplo, "MANUTENCAO"). */
    static std::string Texto(ACAO acao);

    /**
     * @brief Converte texto num estado, ignorando maiúsculas/minúsculas e espaços.
     * @param texto Texto lido (por exemplo, do XML).
     * @param omissao Valor devolvido se o texto não for reconhecido.
     */
    static ESTADO_MOTOR Para_Estado(const std::string &texto, ESTADO_MOTOR omissao);

    /**
     * @brief Converte texto numa cor, ignorando maiúsculas/minúsculas e espaços.
     * @param texto Texto lido.
     * @param omissao Valor devolvido se o texto não for reconhecido.
     */
    static COR_TEMP Para_Cor(const std::string &texto, COR_TEMP omissao);
};

#endif // ENUMERADOS_H_INCLUDED
