#include "Motor.h"
#include "ConfigMotor.h"
#include "ElementoXML.h"
#include "Relogio.h"

#include <ostream>

using namespace std;

// Tipo "" porque não pertence a nenhum tipo de motor: só serve quando falta a configuração.
const ConfigMotor Motor::CONFIG_OMISSAO("");

const double Motor::TEMP_AMBIENTE = 20;

Motor::Motor()
    : Dispositivo()
{
    ESTADO = ESTADO_MOTOR::STOP;
    TEMPERATURA = TEMP_AMBIENTE;
    CONSUMO_HORA = 0;
    MINUTOS_TRABALHO = 0;
    CONSUMO_DIA = 0;
    Ja_Avisei_Amarelo = false;
    ESTADO_GRAVADO = false;
    Ptr_Config = nullptr;
}

Motor::Motor(const Motor &outro)
    : Dispositivo(outro)
{
    ESTADO = outro.ESTADO;
    TEMPERATURA = outro.TEMPERATURA;
    CONSUMO_HORA = outro.CONSUMO_HORA;
    MINUTOS_TRABALHO = outro.MINUTOS_TRABALHO;
    CONSUMO_DIA = outro.CONSUMO_DIA;
    Ja_Avisei_Amarelo = outro.Ja_Avisei_Amarelo;
    ESTADO_GRAVADO = outro.ESTADO_GRAVADO;
    // A configuração é atribuída pela Fábrica quando a cópia entrar nela.
    Ptr_Config = nullptr;
}

Motor::~Motor()
{
}

const ConfigMotor &Motor::Config() const
{
    // Sem configuração (motor ainda fora da fábrica) não se usa um ponteiro nulo: usam-se
    // os intervalos por omissão, os mesmos que a Fábrica dá a um tipo que o XML não define.
    if (!Ptr_Config)
        return CONFIG_OMISSAO;
    return *Ptr_Config;
}

void Motor::LerCamposXML(const ElementoXML &elemento)
{
    Dispositivo::LerCamposXML(elemento);
    CONSUMO_HORA = elemento.Real("CONSUMO_HORA", CONSUMO_HORA);

    // Campos de estado: só existem num ficheiro gravado (por exemplo, o do simulador).
    TEMPERATURA = elemento.Real("VALOR_TEMP_ATUAL", TEMPERATURA);
    // Um estado que falta ou não se reconhece conta como "sem estado gravado".
    string estado = elemento.Texto("ESTADO_ATUAL");
    ESTADO_GRAVADO = Conversoes::Para_Estado(estado, ESTADO_MOTOR::INEXISTENTE) != ESTADO_MOTOR::INEXISTENTE;
    ESTADO = Conversoes::Para_Estado(estado, ESTADO);
    // AVARIADO não é um valor deste atributo (é a flag da base, que só a manutenção
    // limpa) e INEXISTENTE não é um estado real: um motor assim fica parado.
    if (ESTADO == ESTADO_MOTOR::AVARIADO || ESTADO == ESTADO_MOTOR::INEXISTENTE)
        ESTADO = ESTADO_MOTOR::STOP;
    // ESTADO_COR é ignorado de propósito: a cor é calculada pelo próprio motor.
}

void Motor::Descricao_Especifica(ostream &f) const
{
    f << " | " << Conversoes::Texto(Get_Estado()) << " | " << TEMPERATURA << " graus ("
      << Conversoes::Texto(Get_Cor()) << ") | " << CONSUMO_HORA << "/h | " << MINUTOS_TRABALHO
      << " min | consumo do dia: " << CONSUMO_DIA;
}

void Motor::EscreveCamposXML(ostream &f) const
{
    Dispositivo::EscreveCamposXML(f);
    // Textos do estado e da cor só pelas Conversoes: os mesmos que o LerCamposXML aceita.
    f << "\t\t\t<VALOR_TEMP_ATUAL>" << TEMPERATURA << "</VALOR_TEMP_ATUAL>" << endl;
    f << "\t\t\t<ESTADO_ATUAL>" << Conversoes::Texto(Get_Estado()) << "</ESTADO_ATUAL>" << endl;
    f << "\t\t\t<ESTADO_COR>" << Conversoes::Texto(Get_Cor()) << "</ESTADO_COR>" << endl;
    f << "\t\t\t<CONSUMO_HORA>" << CONSUMO_HORA << "</CONSUMO_HORA>" << endl;
}

// ---------------------------------------------------------------- ciclo

void Motor::Funcionar()
{
    // A máquina de estados é do motor: a Fábrica só lhe pede um ciclo.
    if (ESTADO == ESTADO_MOTOR::STOP)
    {
        TEMPERATURA -= Arrefecimento();
        if (TEMPERATURA < TEMP_AMBIENTE)
            TEMPERATURA = TEMP_AMBIENTE;
        // Parado não se chama o ESTOU_QUENTE (repetiria o aviso de VERMELHO enquanto
        // arrefece), mas se já voltou ao VERDE a próxima subida tem de ser avisada.
        if (Get_Cor() == COR_TEMP::VERDE)
            Ja_Avisei_Amarelo = false;
    }
    else if (ESTADO == ESTADO_MOTOR::START)
        ESTADO = ESTADO_MOTOR::RUN; // o arranque dura um ciclo
    else if (ESTADO == ESTADO_MOTOR::RUN)
    {
        RUN();
        ESTOU_QUENTE(); // depois de trabalhar, o motor avalia-se e decide
    }
}

bool Motor::Pode_Avariar() const
{
    return ESTADO == ESTADO_MOTOR::RUN;
}

double Motor::Probabilidade_Avaria() const
{
    return Config().Prob_Avaria();
}

double Motor::Arrefecimento() const
{
    return 5;
}

// ---------------------------------------------------------------- enunciado

void Motor::RUN()
{
    TEMPERATURA += Aquecimento(); // ritmo próprio de cada tipo (gancho)
    // Os minutos de cada ciclo vêm do relógio: há um só sítio que diz quanto vale um ciclo.
    MINUTOS_TRABALHO += Relogio::MINUTOS_CICLO;
    CONSUMO_DIA += CONSUMO_HORA * Relogio::MINUTOS_CICLO / 60.0;
}

void Motor::Novo_Dia()
{
    CONSUMO_DIA = 0;
}

void Motor::Arranque_Inicial()
{
    // Quem decide é o motor (como o peixe decide sobre si): a Fábrica só diz que abriu.
    if (!ESTADO_GRAVADO)
        START();
}

void Motor::START()
{
    // Um motor avariado só volta a trabalhar depois da manutenção.
    if (Esta_Avariado())
        return;
    if (ESTADO == ESTADO_MOTOR::STOP)
        ESTADO = ESTADO_MOTOR::START;
}

void Motor::RESTART()
{
    STOP();
    START();
}

void Motor::STOP()
{
    ESTADO = ESTADO_MOTOR::STOP;
}

COR_TEMP Motor::ESTOU_QUENTE()
{
    COR_TEMP cor = Get_Cor();
    if (cor == COR_TEMP::VERDE)
        Ja_Avisei_Amarelo = false; // arrefeceu: a próxima subida volta a ser avisada
    else if (cor == COR_TEMP::AMARELO)
    {
        // Avisa só uma vez por subida. Como na Ficha 5, a flag fica com o que a Fábrica
        // respondeu: fora de uma fábrica ninguém recebeu o aviso, por isso não fica marcado.
        if (!Ja_Avisei_Amarelo)
            Ja_Avisei_Amarelo = Avisar_Aquecimento(this);
    }
    else
    {
        // O motor protege-se sozinho e depois informa; a Fábrica não decide por ele.
        STOP();
        Avisar_Sobreaquecimento(this);
    }
    return cor;
}

// ---------------------------------------------------------------- consultas

ESTADO_MOTOR Motor::Get_Estado() const
{
    // Uma só fonte de verdade para a avaria: a flag da base.
    if (Esta_Avariado())
        return ESTADO_MOTOR::AVARIADO;
    return ESTADO;
}

COR_TEMP Motor::Get_Cor() const
{
    return Config().Classificar(TEMPERATURA);
}
