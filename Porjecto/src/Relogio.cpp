#include "Relogio.h"

#include <iomanip>
#include <sstream>

using namespace std;

Relogio::Relogio(int _hora_inicio, int _hora_fecho)
{
    // Uma hora inválida daria um horário em que os eventos nunca acontecem.
    HORA_INICIO = Hora_Valida(_hora_inicio) ? _hora_inicio : HORA_INICIO_OMISSAO;
    HORA_FECHO = Hora_Valida(_hora_fecho) ? _hora_fecho : HORA_FECHO_OMISSAO;
    DIA = 1;
    // Começa à hora de abertura: a fábrica está aberta desde o primeiro ciclo.
    MINUTO = HORA_INICIO * 60;
}

bool Relogio::Hora_Valida(int hora)
{
    return hora >= 0 && hora <= 23;
}

EVENTO_RELOGIO Relogio::Avancar()
{
    MINUTO += MINUTOS_CICLO;
    if (MINUTO >= MINUTOS_DIA)
    {
        MINUTO -= MINUTOS_DIA;
        ++DIA;
    }

    // Com as duas horas iguais a fábrica nunca fecha, por isso não há eventos.
    if (HORA_INICIO == HORA_FECHO)
        return EVENTO_RELOGIO::NENHUM;
    // As horas são inteiras e 10 divide 60: o relógio passa exatamente por cada hora,
    // por isso a igualdade acontece uma vez por dia e o evento nunca se repete.
    if (MINUTO == HORA_FECHO * 60)
        return EVENTO_RELOGIO::FECHO;
    if (MINUTO == HORA_INICIO * 60)
        return EVENTO_RELOGIO::ABERTURA;
    return EVENTO_RELOGIO::NENHUM;
}

bool Relogio::Aberto() const
{
    int abertura = HORA_INICIO * 60;
    int fecho = HORA_FECHO * 60;
    if (abertura == fecho)
        return true;
    if (abertura < fecho)
        return MINUTO >= abertura && MINUTO < fecho;
    // Horário noturno: aberto da abertura até à meia-noite e da meia-noite até ao fecho.
    return MINUTO >= abertura || MINUTO < fecho;
}

string Relogio::Hora() const
{
    ostringstream texto;
    texto << setfill('0') << setw(2) << MINUTO / 60 << ":" << setw(2) << MINUTO % 60;
    return texto.str();
}

string Relogio::Data_Hora() const
{
    ostringstream texto;
    texto << "Dia " << DIA << " " << Hora();
    return texto.str();
}
