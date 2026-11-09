#include "Dispositivo.h"
#include "ElementoXML.h"
#include "Fabrica.h"
#include "Uteis.h"

#include <ostream>

using namespace std;

int Dispositivo::N_VIVOS = 0;

Dispositivo::Dispositivo()
{
    ID = 0;
    MARCA = "";
    POSICAO = Posicao(0, 0);
    PROB_AVARIA = 0;
    AVARIAS = 0;
    Avariado = false;
    Ptr_Fabrica = nullptr;
    ++N_VIVOS;
}

Dispositivo::Dispositivo(const Dispositivo &outro)
{
    ID = outro.ID;
    MARCA = outro.MARCA;
    POSICAO = outro.POSICAO;
    PROB_AVARIA = outro.PROB_AVARIA;
    AVARIAS = outro.AVARIAS;
    Avariado = outro.Avariado;
    // A cópia ainda não está em nenhuma fábrica: só o Add da Fábrica a liga.
    Ptr_Fabrica = nullptr;
    ++N_VIVOS;
}

Dispositivo::~Dispositivo()
{
    --N_VIVOS;
}

void Dispositivo::LerXML(const ElementoXML &elemento)
{
    // Campos comuns, sempre por nome: a ordem no ficheiro não interessa e os que faltam
    // mantêm o valor atual (o de omissão).
    ID = elemento.Inteiro("ID", ID);
    MARCA = elemento.Texto("MARCA", MARCA);
    double x = POSICAO.GetX();
    double y = POSICAO.GetY();
    if (elemento.Par("POSICAO", x, y))
        POSICAO = Posicao(static_cast<int>(x), static_cast<int>(y));
    PROB_AVARIA = elemento.Real("PROB_AVARIA", PROB_AVARIA);

    // Gancho: cada subclasse lê só o que a distingue.
    LerCamposXML(elemento);
}

void Dispositivo::Escreve(ostream &f) const
{
    f << QuemEs() << " " << ID << " | " << MARCA << " | (" << POSICAO.GetX() << "," << POSICAO.GetY()
      << ")";
    Descricao_Especifica(f);
    f << " | avarias: " << AVARIAS << endl;
}

void Dispositivo::EscreveXML(ostream &f) const
{
    // A tag é o próprio QuemEs(): o mesmo nome que o Load usa para criar o dispositivo,
    // por isso o ficheiro escrito pode ser carregado de novo.
    f << "\t\t<" << QuemEs() << ">" << endl;
    f << "\t\t\t<ID>" << ID << "</ID>" << endl;
    // A marca é texto livre: um '&' ou um '<' tornariam o XML inválido.
    f << "\t\t\t<MARCA>" << Uteis::Escapar_XML(MARCA) << "</MARCA>" << endl;
    f << "\t\t\t<POSICAO>" << POSICAO.GetX() << "," << POSICAO.GetY() << "</POSICAO>" << endl;
    // Gancho: cada subclasse acrescenta só as suas tags.
    EscreveCamposXML(f);
    f << "\t\t</" << QuemEs() << ">" << endl;
}

void Dispositivo::Ciclo()
{
    // Avariado não faz nada: só a manutenção o põe de novo a funcionar.
    if (Avariado)
        return;
    if (Pode_Avariar() && Uteis::Acontece(Probabilidade_Avaria()))
    {
        Avariado = true;
        ++AVARIAS;
        Avisar_Avaria();
        return;
    }
    // Gancho: cada família faz o seu ciclo (o motor aquece, o sensor lê, ...).
    Funcionar();
}

bool Dispositivo::Pode_Avariar() const
{
    return true;
}

double Dispositivo::Probabilidade_Avaria() const
{
    return PROB_AVARIA;
}

// Os reencaminhamentos verificam o nulo num só sítio: fora de uma fábrica (por exemplo,
// num teste) o dispositivo continua a funcionar, só não há a quem avisar.

bool Dispositivo::Avisar_Gestor(const string &mensagem)
{
    if (!Ptr_Fabrica)
        return false;
    return Ptr_Fabrica->Notificar_Gestor(mensagem);
}

bool Dispositivo::Avisar_Aquecimento(Motor *m)
{
    if (!Ptr_Fabrica)
        return false;
    return Ptr_Fabrica->Aviso_Aquecimento(m);
}

bool Dispositivo::Avisar_Sobreaquecimento(Motor *m)
{
    if (!Ptr_Fabrica)
        return false;
    return Ptr_Fabrica->Aviso_Sobreaquecimento(m);
}

void Dispositivo::Avisar_Avaria()
{
    if (Ptr_Fabrica)
        Ptr_Fabrica->Aviso_Avaria(this);
}

void Dispositivo::LerCamposXML(const ElementoXML &)
{
    // Por omissão não há campos próprios.
}

void Dispositivo::Descricao_Especifica(ostream &) const
{
    // Por omissão não há nada de específico a escrever.
}

void Dispositivo::EscreveCamposXML(ostream &) const
{
    // Por omissão não há tags próprias.
}
