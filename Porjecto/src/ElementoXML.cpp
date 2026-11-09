#include "ElementoXML.h"

#include "tinyxml2.h"

#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace tinyxml2;

ElementoXML::ElementoXML(const XMLElement *elemento)
{
    Ptr_Elemento = elemento;
}

bool ElementoXML::Valido() const
{
    return Ptr_Elemento != nullptr;
}

string ElementoXML::Nome() const
{
    if (!Ptr_Elemento)
        return "";
    return Ptr_Elemento->Name();
}

const char *ElementoXML::Texto_Filho(const string &nome) const
{
    if (!Ptr_Elemento)
        return nullptr;
    // Procura pelo nome, em qualquer posição: a ordem dos campos não interessa.
    const XMLElement *filho = Ptr_Elemento->FirstChildElement(nome.c_str());
    if (!filho)
        return nullptr;
    return filho->GetText(); // nullptr se o campo estiver vazio
}

string ElementoXML::Texto(const string &nome, const string &omissao) const
{
    const char *texto = Texto_Filho(nome);
    if (!texto)
        return omissao;
    return texto;
}

int ElementoXML::Inteiro(const string &nome, int omissao) const
{
    const char *texto = Texto_Filho(nome);
    if (!texto)
        return omissao;
    // atoi ignora os espaços no início e pára no primeiro caráter que não é algarismo,
    // por isso "85%" dá 85.
    return atoi(texto);
}

double ElementoXML::Real(const string &nome, double omissao) const
{
    const char *texto = Texto_Filho(nome);
    if (!texto)
        return omissao;
    return atof(texto);
}

bool ElementoXML::Par(const string &nome, double &a, double &b) const
{
    const char *texto = Texto_Filho(nome);
    if (!texto)
        return false;
    // %lf salta os espaços, por isso "10,20" e "0, 80" são ambos lidos.
    return sscanf(texto, "%lf,%lf", &a, &b) == 2;
}

ElementoXML ElementoXML::Filho(const string &nome) const
{
    if (!Ptr_Elemento)
        return ElementoXML(nullptr);
    return ElementoXML(Ptr_Elemento->FirstChildElement(nome.c_str()));
}

list<ElementoXML> ElementoXML::Filhos() const
{
    list<ElementoXML> filhos;
    if (!Ptr_Elemento)
        return filhos;
    for (const XMLElement *f = Ptr_Elemento->FirstChildElement(); f != nullptr; f = f->NextSiblingElement())
        filhos.push_back(ElementoXML(f));
    return filhos;
}
