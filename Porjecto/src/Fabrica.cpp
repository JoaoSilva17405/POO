#include "Fabrica.h"

#include "ConfigMotor.h"
#include "ElementoXML.h"
#include "Motor.h"
#include "Posicao.h"
#include "User.h"
#include "Uteis.h"

// Além do ElementoXML, só o Load conhece a biblioteca de XML (para abrir o documento).
#include "tinyxml2.h"

#include <fstream>
#include <sstream>

using namespace std;

const string Fabrica::FICHEIRO_LOG = "Avisos.log";
const string Fabrica::FICHEIRO_SIMULADOR = "Simulador.xml";

Fabrica::Fabrica(User *ut)
{
    UTILIZADOR = ut;
    TERMINAR = false;
    Repor_Definicoes();
}

Fabrica::~Fabrica()
{
    // Como no ~Aquario: a lista só guarda ponteiros. Sem estes delete, os motores, as
    // configurações e o utilizador ficavam na memória sem dono (fuga de memória).
    Limpar();
    delete UTILIZADOR;
}

void Fabrica::Repor_Definicoes()
{
    NOME_EMPRESA = "Fabrica";
    RELOGIO = Relogio(); // horário por omissão, no dia 1 à hora de abertura
    VIZINHANCA_AVISO = 10;
    LARGURA = 100;
    ALTURA = 100;
}

void Fabrica::Limpar()
{
    // Primeiro os motores e só depois as configurações, porque os motores apontam para elas.
    for (list<Motor *>::iterator it = LM.begin(); it != LM.end(); ++it)
        delete *it;
    LM.clear();
    for (list<ConfigMotor *>::iterator it = LC.begin(); it != LC.end(); ++it)
        delete *it;
    LC.clear();
    Repor_Definicoes();
}

bool Fabrica::Load(const string &ficheiro)
{
    // Fase 1: abrir e validar o ficheiro. Se falhar, a fábrica fica como estava.
    tinyxml2::XMLDocument documento;
    if (documento.LoadFile(ficheiro.c_str()) != tinyxml2::XML_SUCCESS)
    {
        Avisar("Nao foi possivel carregar '" + ficheiro + "': " + documento.ErrorStr());
        return false;
    }
    ElementoXML raiz(documento.RootElement());
    if (!raiz.Valido())
    {
        Avisar("O ficheiro '" + ficheiro + "' nao tem elemento raiz.");
        return false;
    }

    // Fase 2: o ficheiro é válido, por isso substitui o estado anterior.
    Limpar();

    // Fase 3: as definições primeiro, porque criam as configurações dos tipos de motor
    // que o Add atribui a cada motor. A ordem das secções no ficheiro não interessa.
    Ler_Definicoes(raiz.Filho("DEFINICOES"));
    Ler_Motores(raiz.Filho("MOTORES"));

    // Fase 4: o Load põe o relógio no dia 1, à hora de abertura, mas o evento de abertura
    // só volta a acontecer no dia seguinte. Sem isto, os motores sem estado gravado
    // ficavam parados todo o primeiro dia. Cada motor decide se arranca.
    if (RELOGIO.Aberto())
        for (list<Motor *>::iterator it = LM.begin(); it != LM.end(); ++it)
            (*it)->Arranque_Inicial();
    return true;
}

void Fabrica::Ler_Definicoes(const ElementoXML &definicoes)
{
    // Cada campo é procurado por nome; se faltar, fica o valor por omissão.
    NOME_EMPRESA = definicoes.Texto("NOME_EMPRESA", NOME_EMPRESA);
    int hora_inicio = definicoes.Inteiro("HORA_INICIO", RELOGIO.Get_Hora_Inicio());
    int hora_fecho = definicoes.Inteiro("HORA_FECHO", RELOGIO.Get_Hora_Fecho());
    if (!Relogio::Hora_Valida(hora_inicio) || !Relogio::Hora_Valida(hora_fecho))
    {
        Avisar("Horario invalido no XML (as horas vao de 0 a 23): usada a hora por omissao.");
        if (!Relogio::Hora_Valida(hora_inicio))
            hora_inicio = RELOGIO.Get_Hora_Inicio();
        if (!Relogio::Hora_Valida(hora_fecho))
            hora_fecho = RELOGIO.Get_Hora_Fecho();
    }
    // Um relógio novo com o horário lido: a simulação recomeça no dia 1, à abertura.
    RELOGIO = Relogio(hora_inicio, hora_fecho);
    VIZINHANCA_AVISO = definicoes.Inteiro("VIZINHANCA_AVISO", VIZINHANCA_AVISO);
    double largura = LARGURA;
    double altura = ALTURA;
    if (definicoes.Par("DIMENSAO_FABRICA", largura, altura))
    {
        LARGURA = static_cast<int>(largura);
        ALTURA = static_cast<int>(altura);
    }

    // Uma configuração por cada tipo de motor do Catálogo que o XML define. Os tipos vêm
    // do Catálogo: a Fábrica não tem nomes de tipos escritos no código.
    list<string> tipos = CATALOGO.Tipos_Motor();
    for (list<string>::iterator it = tipos.begin(); it != tipos.end(); ++it)
    {
        ElementoXML tipo = definicoes.Filho(*it);
        if (tipo.Valido())
        {
            ConfigMotor *config = new ConfigMotor(*it);
            config->LerXML(tipo);
            LC.push_back(config);
        }
    }
}

void Fabrica::Ler_Motores(const ElementoXML &motores)
{
    list<ElementoXML> filhos = motores.Filhos();
    for (list<ElementoXML>::iterator it = filhos.begin(); it != filhos.end(); ++it)
    {
        // O Catálogo cria o motor certo a partir da tag: não há if sobre o tipo.
        Motor *m = CATALOGO.CriarMotor(it->Nome());
        if (!m)
        {
            Avisar("Tipo de motor desconhecido ignorado: " + it->Nome());
            continue;
        }
        m->LerXML(*it);
        if (!Add(m))
        {
            ostringstream msg;
            msg << m->QuemEs() << " " << m->GetID()
                << " rejeitado (ID repetido ou posicao fora da fabrica).";
            Avisar(msg.str());
            delete m; // rejeitado: ninguém ficou com ele, por isso é libertado já
        }
    }
}

ConfigMotor *Fabrica::Obter_Config(const string &tipo)
{
    for (list<ConfigMotor *>::iterator it = LC.begin(); it != LC.end(); ++it)
        if ((*it)->GetTipo() == tipo)
            return *it;

    // O XML não definiu este tipo: usa valores por omissão, para o motor nunca ficar
    // num estado inválido.
    Avisar("Sem configuracao para " + tipo + " no XML: usados os valores por omissao.");
    ConfigMotor *config = new ConfigMotor(tipo);
    LC.push_back(config);
    return config;
}

Motor *Fabrica::Pertence_Motor(int id) const
{
    for (list<Motor *>::const_iterator it = LM.begin(); it != LM.end(); ++it)
        if ((*it)->GetID() == id)
            return *it;
    return nullptr;
}

bool Fabrica::Dentro_Fabrica(const Posicao &p) const
{
    return p.GetX() >= 0 && p.GetX() <= LARGURA && p.GetY() >= 0 && p.GetY() <= ALTURA;
}

bool Fabrica::Add(Motor *m)
{
    if (!m)
        return false;
    if (Pertence_Motor(m->GetID()) != nullptr)
        return false;
    if (!Dentro_Fabrica(m->GetPosicao()))
        return false;

    LM.push_back(m);
    // Ligação nos dois sentidos, como o Atribuir_Aquario do Add da Ficha 5.
    m->Atribuir_Fabrica(this);
    m->Atribuir_Config(Obter_Config(m->QuemEs()));
    return true;
}

void Fabrica::Avisar(const string &mensagem) const
{
    cout << "[AVISO] " << mensagem << endl;
}

void Fabrica::Listar(ostream &f)
{
    f << "=== Fabrica: " << NOME_EMPRESA << " ===" << endl;
    if (UTILIZADOR)
        f << "Utilizador: " << UTILIZADOR->GetNome() << " (" << UTILIZADOR->QuemEs() << ")" << endl;
    f << "Agora: " << RELOGIO.Data_Hora() << " (" << (RELOGIO.Aberto() ? "aberta" : "fechada") << ")"
      << endl;
    f << "Horario: " << RELOGIO.Get_Hora_Inicio() << "h - " << RELOGIO.Get_Hora_Fecho() << "h | Dimensao: " << LARGURA << " x "
      << ALTURA << " | Vizinhanca de aviso: " << VIZINHANCA_AVISO << endl;

    f << "Configuracoes dos motores:" << endl;
    for (list<ConfigMotor *>::iterator it = LC.begin(); it != LC.end(); ++it)
    {
        f << "  ";
        (*it)->Escreve(f);
    }

    f << "Motores (" << LM.size() << "):" << endl;
    if (LM.empty())
        f << "  (nenhum)" << endl;
    for (list<Motor *>::iterator it = LM.begin(); it != LM.end(); ++it)
    {
        f << "  ";
        (*it)->Escreve(f);
    }

    f << "Avisos ao gestor (" << LAvisos.size() << "):" << endl;
    for (list<string>::iterator it = LAvisos.begin(); it != LAvisos.end(); ++it)
        f << "  " << *it << endl;
}

// ---------------------------------------------------------------- simulação

void Fabrica::Run()
{
    // Como o Aquario::Run da Ficha 5: ciclo, tecla, espera. A espera usa o Uteis::Esperar,
    // que não ocupa o processador (o wait da Ficha 5 fazia espera ativa).
    cout << "Simulacao a correr. Carregue em 'M' para parar e abrir o menu." << endl;
    while (!TERMINAR)
    {
        // Correr a simulação também é uma operação com permissão. Sem ela, a simulação
        // fica parada e o utilizador só tem o menu (por exemplo, para listar).
        if (!Autorizado(ACAO::RUN))
        {
            Executa_Menu_Acoes();
            continue;
        }

        Ciclo();
        if (Uteis::TeclaPressionada())
        {
            char tecla = Uteis::LerTecla();
            // Enquanto o menu está aberto não há ciclos: a simulação está parada.
            if (tecla == 'M' || tecla == 'm')
                Executa_Menu_Acoes();
        }
        if (!TERMINAR)
            Uteis::Esperar(ESPERA_CICLO_MS);
    }
    cout << "Simulacao terminada." << endl;
}

void Fabrica::Ciclo()
{
    size_t avisos_antes = LAvisos.size();

    // 1. O relógio avança e diz o que aconteceu; a Fábrica é que reage (o relógio não
    //    conhece motores).
    Reagir_Relogio(RELOGIO.Avancar());

    // 2. A Fábrica só pede a cada motor que viva um ciclo: é ele que decide se aquece,
    //    avaria ou pára, e avisa pelos métodos de colaboração (como o Peixe::Run).
    //    Fora de horas os motores não trabalham.
    if (RELOGIO.Aberto())
        for (list<Motor *>::iterator it = LM.begin(); it != LM.end(); ++it)
            (*it)->Ciclo();

    // Depois de todos os motores viverem o ciclo, o simulador gráfico recebe o estado novo.
    if (!Gravar_XML(FICHEIRO_SIMULADOR))
        Avisar("Nao foi possivel gravar o XML do simulador (" + FICHEIRO_SIMULADOR + ").");

    Mostrar_Resumo(static_cast<int>(LAvisos.size() - avisos_antes));
}

void Fabrica::Reagir_Relogio(EVENTO_RELOGIO evento)
{
    if (evento == EVENTO_RELOGIO::FECHO)
    {
        for (list<Motor *>::iterator it = LM.begin(); it != LM.end(); ++it)
            (*it)->STOP();
        Notificar_Gestor("Fecho da fabrica: todos os motores parados.");
    }
    else if (evento == EVENTO_RELOGIO::ABERTURA)
    {
        // A Fábrica não verifica se o motor está avariado: pede o START e é o motor que
        // decide (avariado, ignora-o até à manutenção), como o peixe decide sobre si.
        for (list<Motor *>::iterator it = LM.begin(); it != LM.end(); ++it)
        {
            (*it)->Novo_Dia();
            (*it)->START();
        }
        Notificar_Gestor("Abertura da fabrica: novo dia de trabalho, motores a arrancar.");
    }
}

string Fabrica::Marca_Tempo() const
{
    return RELOGIO.Data_Hora();
}

int Fabrica::Contar_Estado(ESTADO_MOTOR estado) const
{
    int n = 0;
    for (list<Motor *>::const_iterator it = LM.begin(); it != LM.end(); ++it)
        if ((*it)->Get_Estado() == estado)
            ++n;
    return n;
}

void Fabrica::Mostrar_Resumo(int avisos_novos) const
{
    const ESTADO_MOTOR estados[] = {ESTADO_MOTOR::RUN, ESTADO_MOTOR::START, ESTADO_MOTOR::STOP,
                                    ESTADO_MOTOR::AVARIADO};
    cout << Marca_Tempo();
    for (int i = 0; i < 4; ++i)
        cout << " | " << Conversoes::Texto(estados[i]) << ": " << Contar_Estado(estados[i]);
    cout << " | avisos novos: " << avisos_novos << endl;
}

// ---------------------------------------------------------------- XML do simulador

void Fabrica::Escreve_XML(ostream &f) const
{
    f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    f << "<DADOS>" << endl;

    f << "\t<DEFINICOES>" << endl;
    // O nome da empresa é texto livre: tem de ser escapado.
    f << "\t\t<NOME_EMPRESA>" << Uteis::Escapar_XML(NOME_EMPRESA) << "</NOME_EMPRESA>" << endl;
    f << "\t\t<HORA_INICIO>" << RELOGIO.Get_Hora_Inicio() << "</HORA_INICIO>" << endl;
    f << "\t\t<HORA_FECHO>" << RELOGIO.Get_Hora_Fecho() << "</HORA_FECHO>" << endl;
    f << "\t\t<VIZINHANCA_AVISO>" << VIZINHANCA_AVISO << "</VIZINHANCA_AVISO>" << endl;
    f << "\t\t<DIMENSAO_FABRICA>" << LARGURA << "," << ALTURA << "</DIMENSAO_FABRICA>" << endl;
    // Cada configuração escreve a sua tag: a Fábrica não tem nomes de tipos no código.
    for (list<ConfigMotor *>::const_iterator it = LC.begin(); it != LC.end(); ++it)
        (*it)->EscreveXML(f);
    f << "\t</DEFINICOES>" << endl;

    f << "\t<MOTORES>" << endl;
    // Como no Aquario::GravarXML: a Fábrica percorre a lista e cada um escreve-se a si próprio.
    for (list<Motor *>::const_iterator it = LM.begin(); it != LM.end(); ++it)
        (*it)->EscreveXML(f);
    f << "\t</MOTORES>" << endl;

    // Os sensores chegam numa fase seguinte; a secção já existe para o formato estar completo.
    f << "\t<SENSORES>" << endl;
    f << "\t</SENSORES>" << endl;

    f << "</DADOS>" << endl;
}

bool Fabrica::Gravar_XML(const string &ficheiro) const
{
    // Escreve-se num temporário e só no fim se troca pelo definitivo: o Simulador.exe lê
    // o ficheiro quando quer e nunca pode apanhar um XML a meio da escrita.
    string temporario = ficheiro + ".tmp";
    ofstream f(temporario.c_str());
    if (!f.is_open())
        return false;
    Escreve_XML(f);
    f.close(); // tem de estar fechado (e escrito no disco) antes da troca
    if (f.fail())
        return false;
    return Uteis::Substituir_Ficheiro(temporario, ficheiro);
}

// ---------------------------------------------------------------- menu e permissões

bool Fabrica::Autorizado(ACAO acao) const
{
    // Um único sítio decide a autorização, e quem responde é o próprio utilizador (Pode é
    // virtual em cada perfil): nada de if sobre o perfil aqui.
    if (UTILIZADOR && UTILIZADOR->Pode(acao))
        return true;
    cout << "Sem permissao: ";
    if (UTILIZADOR)
        cout << "o perfil " << UTILIZADOR->QuemEs() << " nao pode fazer ";
    else
        cout << "nao ha utilizador para fazer ";
    cout << Conversoes::Texto(acao) << "." << endl;
    return false;
}

int Fabrica::menu() const
{
    cout << endl << "===== MENU (simulacao parada em " << Marca_Tempo() << ") =====" << endl;
    if (UTILIZADOR)
        cout << "Utilizador: " << UTILIZADOR->GetNome() << " (" << UTILIZADOR->QuemEs() << ")" << endl;
    cout << "1 - Listar (ecra ou ficheiro)" << endl;
    cout << "2 - Sair do programa" << endl;
    cout << "0 - Voltar a simulacao" << endl;
    return Uteis::LerInteiro("Escolha a opcao: ");
}

void Fabrica::Executa_Menu_Acoes()
{
    // O menu repete-se até o utilizador voltar à simulação ou sair do programa. Ao
    // contrário do switch da Ficha 5, todos os case têm break e o 0 não cai no default.
    bool no_menu = true;
    while (no_menu)
    {
        int op = menu();
        switch (op)
        {
            case 1: ListarMenu(); break;
            case 2:
                // Só se marca o fim: o Run termina e a memória é libertada pelo
                // destrutor da Fábrica, quando o main acaba.
                TERMINAR = true;
                no_menu = false;
                break;
            case 0: no_menu = false; break;
            default: cout << "Opcao invalida." << endl; break;
        }
    }
}

void Fabrica::ListarMenu()
{
    if (!Autorizado(ACAO::LISTAR))
        return;
    int destino = Uteis::LerInteiro("Listar para: 1 - Ecra, 2 - Ficheiro: ");
    if (destino == 1)
    {
        Listar(cout);
        return;
    }
    if (destino != 2)
    {
        cout << "Opcao invalida." << endl;
        return;
    }
    string ficheiro = Uteis::LerTexto("Nome do ficheiro [Listagem.txt]: ");
    if (ficheiro.empty())
        ficheiro = "Listagem.txt";
    // O mesmo Listar escreve no ecrã ou no ficheiro, porque recebe um ostream &.
    ofstream f(ficheiro.c_str());
    if (!f.is_open())
    {
        cout << "Nao foi possivel abrir '" << ficheiro << "'." << endl;
        return;
    }
    Listar(f);
    cout << "Listagem gravada em " << ficheiro << endl;
}

// ---------------------------------------------------------------- avisos ao gestor

string Fabrica::Identificar(const Dispositivo *d)
{
    ostringstream texto;
    texto << d->QuemEs() << " " << d->GetID() << " (" << d->GetMarca() << ")";
    return texto.str();
}

bool Fabrica::Notificar_Gestor(const string &mensagem)
{
    string linha = "[" + Marca_Tempo() + "] " + mensagem;
    cout << "[GESTOR] " << linha << endl;
    LAvisos.push_back(linha);

    // O log é aberto em modo de acrescentar: fica o registo de todas as execuções.
    ofstream f(FICHEIRO_LOG.c_str(), ios::app);
    if (!f.is_open())
        return false;
    f << linha << endl;
    return true;
}

bool Fabrica::Aviso_Aquecimento(Motor *m)
{
    if (!m)
        return false;
    ostringstream msg;
    msg << "Motor " << Identificar(m) << " a aquecer: " << m->Get_Temperatura() << " graus (AMARELO).";
    return Notificar_Gestor(msg.str());
}

bool Fabrica::Aviso_Sobreaquecimento(Motor *m)
{
    if (!m)
        return false;
    ostringstream msg;
    msg << "Motor " << Identificar(m) << " sobreaquecido: " << m->Get_Temperatura()
        << " graus (VERMELHO). O motor parou.";
    return Notificar_Gestor(msg.str());
}

bool Fabrica::Aviso_Avaria(Dispositivo *d)
{
    if (!d)
        return false;
    ostringstream msg;
    msg << "AVARIA: " << Identificar(d) << " avariou (avarias: " << d->Get_Avarias()
        << "). Precisa de manutencao.";
    return Notificar_Gestor(msg.str());
}
