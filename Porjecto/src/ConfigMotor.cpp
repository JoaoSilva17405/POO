#include "ConfigMotor.h"
#include "ElementoXML.h"
#include "Uteis.h"

#include <ostream>

using namespace std;

ConfigMotor::ConfigMotor(const string &_tipo)
{
    TIPO = _tipo;
    VERDE_MIN = 0;
    VERDE_MAX = 80;
    AMARELO_MIN = 81;
    AMARELO_MAX = 100;
    VERMELHO_MIN = 101;
    VERMELHO_MAX = 250;
    PROB_AVARIA = 1;
}

void ConfigMotor::LerXML(const ElementoXML &elemento)
{
    // Os campos em falta ficam com o valor que já tinham (os de omissão).
    elemento.Par("VERDE", VERDE_MIN, VERDE_MAX);
    elemento.Par("AMARELO", AMARELO_MIN, AMARELO_MAX);
    elemento.Par("VERMELHO", VERMELHO_MIN, VERMELHO_MAX);
    // Lida exatamente como está no ficheiro, sem arredondar nem limitar (decisão do PRD).
    PROB_AVARIA = elemento.Real("PROB_AVARIA", PROB_AVARIA);
}

COR_TEMP ConfigMotor::Classificar(double temperatura) const
{
    if (temperatura >= VERMELHO_MIN)
        return COR_TEMP::VERMELHO;
    if (temperatura >= AMARELO_MIN)
        return COR_TEMP::AMARELO;
    return COR_TEMP::VERDE;
}

double ConfigMotor::Temp_Verde_Aleatoria() const
{
    return Uteis::Aleatorio_Real(VERDE_MIN, VERDE_MAX);
}

void ConfigMotor::Escreve(ostream &f) const
{
    f << TIPO << ": VERDE " << VERDE_MIN << "-" << VERDE_MAX << ", AMARELO " << AMARELO_MIN << "-"
      << AMARELO_MAX << ", VERMELHO " << VERMELHO_MIN << "-" << VERMELHO_MAX << ", avaria "
      << PROB_AVARIA << "%" << endl;
}

void ConfigMotor::EscreveXML(ostream &f) const
{
    f << "\t\t<" << TIPO << ">" << endl;
    f << "\t\t\t<VERDE>" << VERDE_MIN << "," << VERDE_MAX << "</VERDE>" << endl;
    f << "\t\t\t<AMARELO>" << AMARELO_MIN << "," << AMARELO_MAX << "</AMARELO>" << endl;
    f << "\t\t\t<VERMELHO>" << VERMELHO_MIN << "," << VERMELHO_MAX << "</VERMELHO>" << endl;
    f << "\t\t\t<PROB_AVARIA>" << PROB_AVARIA << "</PROB_AVARIA>" << endl;
    f << "\t\t</" << TIPO << ">" << endl;
}
