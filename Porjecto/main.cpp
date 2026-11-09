/**
 * @file main.cpp
 * @brief Programa principal do Simulador de Gestão de Fábrica.
 *
 * Pede o nome e o perfil do utilizador, cria a Fábrica, carrega o XML de configuração e
 * corre a simulação. Durante a simulação, a tecla 'M' abre o menu; o programa acaba
 * quando se escolhe "Sair do programa". A memória é libertada pelo destrutor da Fábrica.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "Admin.h"
#include "Fabrica.h"
#include "Normal.h"
#include "User.h"
#include "Uteis.h"
#include "Visitante.h"

using namespace std;

/**
 * @brief Pede o perfil e cria o utilizador desse perfil.
 *
 * É o único sítio que conhece os perfis concretos: a partir daqui a Fábrica só vê um
 * User e pergunta-lhe o que pode fazer (User::Pode).
 * @param nome Nome do utilizador.
 * @return O utilizador criado com new (quem o recebe fica dono dele).
 */
static User *Criar_Utilizador(const string &nome)
{
    while (true)
    {
        int perfil = Uteis::LerInteiro("Perfil (1 - Admin, 2 - Normal, 3 - Visitante): ");
        switch (perfil)
        {
            case 1: return new Admin(1, nome);
            case 2: return new Normal(1, nome);
            case 3: return new Visitante(1, nome);
            default: cout << "Perfil invalido." << endl; break;
        }
    }
}

/** @brief Ponto de entrada do programa. */
int main()
{
    Uteis::Configurar_Consola();
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "=== Simulador de Gestao de Fabrica ===" << endl;
    string nome = Uteis::LerTexto("Nome do utilizador: ");
    if (nome.empty())
        nome = "Anonimo";

    // A Fábrica fica dona do utilizador e liberta-o no destrutor.
    Fabrica F(Criar_Utilizador(nome));

    string ficheiro = Uteis::LerTexto("Ficheiro XML de configuracao [Empresa1XML.xml]: ");
    if (ficheiro.empty())
        ficheiro = "Empresa1XML.xml";
    if (!F.Load(ficheiro))
        return 1;

    F.Listar();
    F.Run();
    // Ao sair do main, o destrutor da Fábrica liberta motores, configurações e utilizador.
    return 0;
}
