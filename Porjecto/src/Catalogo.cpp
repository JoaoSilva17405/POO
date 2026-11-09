#include "Catalogo.h"
#include "Uteis.h"

// Único sítio do projeto que inclui as classes concretas.
#include "MCombustao.h"
#include "MEletrico.h"
#include "MInducao.h"

using namespace std;

Catalogo::Catalogo()
{
    // Uma linha por tipo concreto. Para acrescentar um tipo: criar a classe e
    // acrescentar aqui uma linha.
    Registar_Motor(new MCombustao());
    Registar_Motor(new MEletrico());
    Registar_Motor(new MInducao());
}

Catalogo::~Catalogo()
{
    for (list<Motor *>::iterator it = LM_Prototipos.begin(); it != LM_Prototipos.end(); ++it)
        delete *it;
}

string Catalogo::Normalizar(const string &tag)
{
    return Uteis::Maiusculas(Uteis::Trim(tag));
}

Motor *Catalogo::Prototipo_Motor(const string &tag) const
{
    string procurada = Normalizar(tag);
    for (list<Motor *>::const_iterator it = LM_Prototipos.begin(); it != LM_Prototipos.end(); ++it)
        if ((*it)->QuemEs() == procurada)
            return *it;
    return nullptr;
}

void Catalogo::Registar_Motor(Motor *prototipo)
{
    if (!prototipo)
        return;
    Motor *anterior = Prototipo_Motor(prototipo->QuemEs());
    if (anterior)
    {
        // Retirar da lista antes de libertar, para não ficar um ponteiro pendurado.
        LM_Prototipos.remove(anterior);
        delete anterior;
    }
    LM_Prototipos.push_back(prototipo);
}

Motor *Catalogo::CriarMotor(const string &tag) const
{
    Motor *prototipo = Prototipo_Motor(tag);
    if (!prototipo)
        return nullptr;
    return prototipo->Clone();
}

list<string> Catalogo::Tipos_Motor() const
{
    list<string> tipos;
    for (list<Motor *>::const_iterator it = LM_Prototipos.begin(); it != LM_Prototipos.end(); ++it)
        tipos.push_back((*it)->QuemEs());
    return tipos;
}
