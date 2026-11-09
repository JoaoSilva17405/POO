#ifndef RELOGIO_H
#define RELOGIO_H

#include <string>

/** @brief O que aconteceu no horário da fábrica ao avançar o relógio. */
enum class EVENTO_RELOGIO
{
    NENHUM,   ///< Nada mudou no horário.
    ABERTURA, ///< Chegou a hora de abertura: a fábrica abriu.
    FECHO     ///< Chegou a hora de fecho: a fábrica fechou.
};

/**
 * @brief Relógio simulado da fábrica: dia, hora e horário de abertura e fecho.
 *
 * Cada ciclo de simulação vale 10 minutos (`Avancar`). O relógio só diz **o que
 * aconteceu** (abertura, fecho ou nada) e se a fábrica está aberta: não conhece motores.
 * É a Fábrica que reage ao evento (pára ou arranca os motores), como o Aquário da Ficha 5
 * reage aos pedidos dos peixes em vez de os peixes mexerem nas listas dele.
 *
 * A simulação começa no dia 1, à hora de abertura, com a fábrica aberta. Assim os motores
 * trabalham logo no primeiro ciclo, sem esperar pela abertura.
 *
 * Horário noturno (abertura depois do fecho, por exemplo 22h–6h) é suportado: a fábrica
 * está aberta da abertura até à meia-noite e da meia-noite até ao fecho. Com as duas horas
 * iguais, a fábrica está sempre aberta e não há eventos.
 *
 * É um valor (sem ponteiros): a Fábrica guarda-o por valor e pode copiá-lo.
 */
class Relogio
{
    /** @brief Dia simulado (começa em 1). */
    int DIA;
    /** @brief Minuto do dia simulado (0 a 1439). */
    int MINUTO;
    /** @brief Hora de abertura (0 a 23). */
    int HORA_INICIO;
    /** @brief Hora de fecho (0 a 23). */
    int HORA_FECHO;

    /** @brief Minutos de um dia. */
    static const int MINUTOS_DIA = 24 * 60;

    /** @brief Hora de abertura usada quando a hora pedida é inválida. */
    static const int HORA_INICIO_OMISSAO = 8;
    /** @brief Hora de fecho usada quando a hora pedida é inválida. */
    static const int HORA_FECHO_OMISSAO = 16;

    /** @brief Hora atual formatada, por exemplo "08:10" (usada pelo Data_Hora). */
    std::string Hora() const;

public:
    /** @brief Minutos simulados de cada ciclo (10, como pede o enunciado). */
    static const int MINUTOS_CICLO = 10;

    /**
     * @brief Cria o relógio no dia 1, à hora de abertura.
     *
     * Uma hora fora de 0–23 é trocada pela de omissão (8h para a abertura, 16h para o
     * fecho). Quem lê o XML deve validar antes (Hora_Valida) para poder avisar.
     * @param _hora_inicio Hora de abertura.
     * @param _hora_fecho Hora de fecho.
     */
    Relogio(int _hora_inicio = HORA_INICIO_OMISSAO, int _hora_fecho = HORA_FECHO_OMISSAO);

    /**
     * @brief Avança 10 minutos (um ciclo) e diz o que aconteceu no horário.
     *
     * À meia-noite muda de dia. Cada evento acontece uma só vez por dia, no ciclo em que
     * o relógio chega exatamente à hora de abertura ou de fecho.
     * @return ABERTURA, FECHO ou NENHUM.
     */
    EVENTO_RELOGIO Avancar();

    /** @brief Diz se, na hora atual, a fábrica está aberta (usado pela Fábrica no Ciclo). */
    bool Aberto() const;

    /** @brief Hora de abertura (usada pela Fábrica nas saídas). */
    int Get_Hora_Inicio() const { return HORA_INICIO; }

    /** @brief Hora de fecho (usada pela Fábrica nas saídas). */
    int Get_Hora_Fecho() const { return HORA_FECHO; }

    /** @brief Dia e hora atuais, por exemplo "Dia 1 08:10" (marca do tempo dos avisos e do resumo). */
    std::string Data_Hora() const;

    /** @brief Diz se uma hora é válida (0 a 23). Usado pela Fábrica ao ler o XML. */
    static bool Hora_Valida(int hora);
};

#endif // RELOGIO_H
