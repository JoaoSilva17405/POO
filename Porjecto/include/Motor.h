#ifndef MOTOR_H
#define MOTOR_H

#include "Dispositivo.h"
#include "Enumerados.h"

class ConfigMotor;

/**
 * @brief Base abstrata dos motores (MCombustao, MEletrico, MInducao, …).
 *
 * Um motor tem um estado (STOP, START, RUN; AVARIADO vem da flag da base), uma
 * temperatura e um consumo por hora. É o próprio motor que classifica a sua temperatura,
 * usando a configuração do seu tipo, que recebe quando entra na Fábrica
 * (`Add` → `Atribuir_Config`), como o `Add` do Aquário fazia `Atribuir_Aquario`.
 *
 * Em cada ciclo (gancho `Funcionar`): STOP arrefece; START passa a RUN; RUN trabalha
 * (`RUN()`: aquece, conta tempo e consumo) e o motor avalia-se (`ESTOU_QUENTE`), como o
 * peixe que decide sozinho que está gordo e pede para sair (Ficha 5).
 *
 * Uma subclasse concreta só escreve `QuemEs`, `Clone` e `Aquecimento`, e pode redefinir
 * RUN, START, RESTART, STOP e ESTOU_QUENTE (enunciado).
 */
class Motor : public Dispositivo
{
    /** @brief Estado atual (STOP, START ou RUN); a avaria é a flag da base. */
    ESTADO_MOTOR ESTADO;
    /** @brief Temperatura atual. */
    double TEMPERATURA;
    /** @brief Consumo por hora de trabalho. */
    double CONSUMO_HORA;
    /** @brief Minutos que o motor já trabalhou (para o ranking dos mais trabalhadores). */
    int MINUTOS_TRABALHO;
    /** @brief Consumo acumulado no dia (para o relatório). */
    double CONSUMO_DIA;
    /**
     * @brief Já avisou o gestor nesta subida a AMARELO (como o Ja_Avisei_Tenho_Fome da Ficha 5).
     *
     * Volta a falso quando a temperatura regressa ao VERDE.
     */
    bool Ja_Avisei_Amarelo;
    /**
     * @brief O XML trazia um estado válido (`ESTADO_ATUAL`), por exemplo um ficheiro gravado.
     *
     * Decide o Arranque_Inicial: um estado gravado é respeitado; um motor sem estado
     * (ficheiro de configuração) arranca com a fábrica.
     */
    bool ESTADO_GRAVADO;
    /** @brief Configuração do tipo de motor (não dono; a dona é a Fábrica). */
    const ConfigMotor *Ptr_Config;

    /**
     * @brief Configuração com os intervalos por omissão, usada enquanto o motor não tem
     * a do seu tipo (fora da fábrica). Evita usar Ptr_Config nulo.
     */
    static const ConfigMotor CONFIG_OMISSAO;

    /** @brief Temperatura ambiente: o motor nasce a esta temperatura e não arrefece abaixo dela. */
    static const double TEMP_AMBIENTE;

    /** @brief Configuração do tipo, ou a de omissão se ainda não tem nenhuma (nunca nula). */
    const ConfigMotor &Config() const;

protected:
    /** @brief Cria um motor parado, à temperatura ambiente e sem configuração. */
    Motor();

    /**
     * @brief Cópia usada pelo Clone() das subclasses.
     * @param outro Motor copiado.
     */
    Motor(const Motor &outro);

    /**
     * @brief Graus que o motor aquece em cada ciclo a trabalhar (ritmo próprio de cada tipo).
     *
     * Gancho puro: cada tipo concreto tem de o definir.
     */
    virtual double Aquecimento() const = 0;

    /** @brief Graus que o motor arrefece em cada ciclo parado (por omissão 5). */
    virtual double Arrefecimento() const;

    /**
     * @brief Máquina de estados do motor (gancho do Dispositivo::Ciclo).
     *
     * STOP arrefece; START passa a RUN; RUN chama RUN() e depois ESTOU_QUENTE().
     */
    virtual void Funcionar();

    /** @brief O motor só avaria quando está a trabalhar (RUN). */
    virtual bool Pode_Avariar() const;

    /** @brief A probabilidade de avaria é a do tipo de motor (configuração), não a do motor. */
    virtual double Probabilidade_Avaria() const;

    /**
     * @brief Lê os campos próprios do motor: CONSUMO_HORA e, quando existem, o estado
     * guardado (VALOR_TEMP_ATUAL, ESTADO_ATUAL).
     * @param elemento Elemento XML do motor.
     */
    virtual void LerCamposXML(const ElementoXML &elemento);

    /**
     * @brief Escreve o estado, a temperatura, a cor e o consumo por hora.
     * @param f Destino da escrita.
     */
    virtual void Descricao_Especifica(std::ostream &f) const;

    /**
     * @brief Escreve as tags que o Simulador.exe pede a um motor: VALOR_TEMP_ATUAL,
     * ESTADO_ATUAL e ESTADO_COR, mais o CONSUMO_HORA.
     *
     * A cor é calculada aqui (Get_Cor), com os intervalos do tipo: o simulador não a calcula.
     * @param f Destino da escrita.
     */
    virtual void EscreveCamposXML(std::ostream &f) const;

public:
    /** @brief Destrutor virtual. */
    virtual ~Motor();

    /**
     * @brief Cria uma cópia nova deste motor, do mesmo tipo concreto (padrão Protótipo).
     *
     * Usado pelo Catálogo para criar motores sem conhecer o tipo. Quem chama fica dono da cópia.
     */
    virtual Motor *Clone() const = 0;

    /**
     * @brief Liga o motor à configuração do seu tipo (chamado pela Fábrica no Add).
     * @param C Configuração do tipo (não dono).
     */
    void Atribuir_Config(const ConfigMotor *C) { Ptr_Config = C; }

    /**
     * @brief Começa um dia de trabalho: o consumo do dia volta a zero.
     *
     * Chamado pela Fábrica na abertura. Os minutos trabalhados não voltam a zero, porque
     * contam para o ranking de toda a simulação.
     */
    void Novo_Dia();

    /**
     * @brief Primeiro arranque depois do Load, com a fábrica aberta.
     *
     * O Load põe o relógio à hora de abertura, mas o evento de abertura só volta a
     * acontecer no dia seguinte. Por isso a Fábrica avisa cada motor de que abriu, e o
     * motor decide: se o ficheiro trazia o seu estado, mantém-no (foi gravado assim); se
     * não trazia, faz START (que ele próprio ignora se estiver avariado).
     */
    void Arranque_Inicial();

    // --- Operações do enunciado (virtuais: cada tipo pode redefinir) ---

    /**
     * @brief Um ciclo a trabalhar: aquece segundo o tipo, conta 10 minutos e o consumo.
     *
     * Chamado pela máquina de estados quando o motor está em RUN.
     */
    virtual void RUN();

    /**
     * @brief Arranca o motor: de STOP passa a START (e a RUN no ciclo seguinte).
     *
     * Não tem efeito num motor avariado nem num que já está a arrancar ou a trabalhar.
     */
    virtual void START();

    /** @brief Volta a ligar o motor: STOP seguido de START (sem efeito se estiver avariado). */
    virtual void RESTART();

    /** @brief Pára o motor (estado STOP). */
    virtual void STOP();

    /**
     * @brief O motor avalia a sua temperatura e decide (como o EstouGordo do peixe).
     *
     * VERDE repõe a flag do aviso; AMARELO avisa o gestor uma vez por subida; VERMELHO
     * pára o motor e avisa o gestor.
     * @return A cor da temperatura atual.
     */
    virtual COR_TEMP ESTOU_QUENTE();

    /**
     * @brief Estado atual do motor (usado pela Fábrica em Get_Estado e nas saídas).
     *
     * É AVARIADO enquanto a flag de avaria da base estiver ligada.
     */
    ESTADO_MOTOR Get_Estado() const;

    /** @brief Temperatura atual (usada pela Fábrica nas saídas). */
    double Get_Temperatura() const { return TEMPERATURA; }

    /**
     * @brief Cor da temperatura atual, segundo os intervalos do tipo.
     *
     * Sem configuração atribuída (motor fora da fábrica) usa os intervalos por omissão
     * (VERDE 0–80, AMARELO 81–100, VERMELHO a partir de 101).
     */
    COR_TEMP Get_Cor() const;

    /** @brief Minutos trabalhados (usado pela Fábrica no ranking dos mais trabalhadores). */
    int Get_Minutos_Trabalho() const { return MINUTOS_TRABALHO; }

    /** @brief Consumo acumulado no dia (usado pela Fábrica no relatório). */
    double Get_Consumo_Dia() const { return CONSUMO_DIA; }
};

#endif // MOTOR_H
