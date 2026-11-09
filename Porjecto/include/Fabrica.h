#ifndef FABRICA_H
#define FABRICA_H

#include <iostream>
#include <list>
#include <string>

#include "Catalogo.h"
#include "Enumerados.h"
#include "Relogio.h"

class User;
class Dispositivo;
class Motor;
class ConfigMotor;
class ElementoXML;
class Posicao;

/**
 * @brief A fábrica: dona de tudo, carrega a configuração e trata os pedidos (o "Aquário" da Ficha 5).
 *
 * É dona dos motores, das configurações de cada tipo de motor e do utilizador atual, e
 * tem um Catálogo de tipos e um Relógio por valor. Não conhece nenhum tipo concreto de
 * dispositivo: cria-os através do Catálogo e trata-os pela classe base.
 *
 * O Relógio só diz o que aconteceu no horário (abertura, fecho); quem reage é a Fábrica,
 * que pára os motores no fecho e os arranca na abertura.
 *
 * **Interface pública:** só os métodos que o enunciado atribui à Fábrica (com o mesmo
 * nome e assinatura), mais o `Run` e os métodos de colaboração aprovados pelo docente.
 * Nesta fase existem o construtor, `Load`, `Add(Motor*)`, `Listar`, o `Run` e os métodos
 * de colaboração `Ciclo`, `Notificar_Gestor`, `Aviso_Aquecimento`,
 * `Aviso_Sobreaquecimento` e `Aviso_Avaria`. Tudo o resto é privado.
 *
 * Não é copiável: copiar a fábrica faria dois donos dos mesmos motores.
 */
class Fabrica
{
    /** @brief Nome da empresa (DEFINICOES/NOME_EMPRESA). */
    std::string NOME_EMPRESA;
    /**
     * @brief Relógio simulado: dia, hora e o horário (DEFINICOES/HORA_INICIO e HORA_FECHO).
     *
     * As horas de abertura e fecho ficam só no relógio, para haver uma única fonte.
     */
    Relogio RELOGIO;
    /** @brief Distância de vizinhança dos avisos de humidade (DEFINICOES/VIZINHANCA_AVISO). */
    int VIZINHANCA_AVISO;
    /** @brief Largura da fábrica: X válido de 0 a LARGURA (DEFINICOES/DIMENSAO_FABRICA). */
    int LARGURA;
    /** @brief Altura da fábrica: Y válido de 0 a ALTURA (DEFINICOES/DIMENSAO_FABRICA). */
    int ALTURA;

    /** @brief Utilizador atual (dono). As suas permissões decidem o que o menu deixa fazer. */
    User *UTILIZADOR;
    /** @brief O utilizador escolheu "Sair do programa" no menu: o Run termina. */
    bool TERMINAR;
    /** @brief Motores da fábrica (dono). */
    std::list<Motor *> LM;
    /** @brief Configurações de cada tipo de motor (dono; libertadas depois dos motores). */
    std::list<ConfigMotor *> LC;
    /** @brief Catálogo de tipos: cria os dispositivos a partir das tags XML. */
    Catalogo CATALOGO;

    /** @brief Histórico dos avisos ao gestor, já com a marca do tempo (para o relatório e o HTML). */
    std::list<std::string> LAvisos;

    /** @brief Ficheiro de log onde cada aviso ao gestor é acrescentado. */
    static const std::string FICHEIRO_LOG;
    /** @brief Ficheiro (nome fixo) que o Simulador.exe lê; é reescrito em cada ciclo. */
    static const std::string FICHEIRO_SIMULADOR;
    /** @brief Espera entre ciclos no Run, em milissegundos. */
    static const int ESPERA_CICLO_MS = 1000;

    /** @brief Cópia proibida: a fábrica é dona de ponteiros. */
    Fabrica(const Fabrica &) = delete;
    /** @brief Atribuição proibida, pela mesma razão. */
    Fabrica &operator=(const Fabrica &) = delete;

    /** @brief Repõe as definições com os valores por omissão. */
    void Repor_Definicoes();

    /** @brief Liberta os motores e depois as configurações, e repõe as definições. */
    void Limpar();

    /**
     * @brief Lê a secção DEFINICOES: dados da empresa e a configuração de cada tipo de motor.
     * @param definicoes Elemento `<DEFINICOES>` (pode ser inválido: ficam os valores por omissão).
     */
    void Ler_Definicoes(const ElementoXML &definicoes);

    /**
     * @brief Lê a secção MOTORES: cria cada motor pelo Catálogo e adiciona-o com Add.
     *
     * Tipos desconhecidos são ignorados com um aviso. Um motor rejeitado pelo Add é
     * libertado de imediato.
     * @param motores Elemento `<MOTORES>`.
     */
    void Ler_Motores(const ElementoXML &motores);

    /**
     * @brief Configuração de um tipo de motor.
     *
     * Se o XML não a definiu, cria uma com os valores por omissão e avisa.
     * @param tipo Tag do tipo de motor.
     */
    ConfigMotor *Obter_Config(const std::string &tipo);

    /**
     * @brief Pesquisa um motor por ID (como o Pertence da Ficha 5).
     * @return O motor, ou nullptr se não existir.
     */
    Motor *Pertence_Motor(int id) const;

    /** @brief Diz se uma posição está dentro da fábrica. */
    bool Dentro_Fabrica(const Posicao &p) const;

    /**
     * @brief Mostra um aviso no ecrã.
     *
     * Usado para os problemas de configuração encontrados no Load. Os avisos da simulação
     * vão para o gestor (Notificar_Gestor), que também os guarda no histórico e no log.
     */
    void Avisar(const std::string &mensagem) const;

    /** @brief Marca do tempo dos avisos e do resumo: o dia e a hora simulados (por exemplo, "Dia 1 08:10"). */
    std::string Marca_Tempo() const;

    /**
     * @brief Reage a um evento do relógio (o relógio não conhece motores).
     *
     * No fecho, STOP a todos os motores. Na abertura, `Novo_Dia` a todos (o consumo do dia
     * volta a zero) e START a todos; o próprio motor ignora o START se estiver avariado.
     * Os dois eventos são avisados ao gestor, para ficarem no histórico e no log.
     * @param evento O que o relógio devolveu no Avancar.
     */
    void Reagir_Relogio(EVENTO_RELOGIO evento);

    /** @brief Número de motores num estado (para o resumo de cada ciclo). */
    int Contar_Estado(ESTADO_MOTOR estado) const;

    /**
     * @brief Mostra no ecrã uma linha de resumo do ciclo: motores por estado e avisos novos.
     * @param avisos_novos Avisos ao gestor feitos durante o ciclo.
     */
    void Mostrar_Resumo(int avisos_novos) const;

    /**
     * @brief Escreve o estado da fábrica em XML, no formato do Simulador.exe (o de `ExemploParaSimuladorXML.xml`).
     *
     * A Fábrica escreve o "envelope" (`DADOS`, `DEFINICOES` com os dados da empresa e a
     * configuração de cada tipo, `MOTORES`, `SENSORES`) e cada motor escreve-se a si
     * próprio (`EscreveXML`), como no `Aquario::GravarXML` da Ficha 5. O formato é o que o
     * Load lê, por isso o ficheiro pode ser carregado de novo.
     * @param f Destino da escrita.
     */
    void Escreve_XML(std::ostream &f) const;

    /**
     * @brief Grava o XML da fábrica num ficheiro, sem que um leitor o apanhe a meio da escrita.
     *
     * Escreve primeiro num ficheiro temporário (`ficheiro` + ".tmp"), fecha-o e só depois o
     * troca pelo definitivo (Uteis::Substituir_Ficheiro). Assim o Simulador.exe, que lê o
     * ficheiro de tempos a tempos, vê sempre um XML completo.
     * @param ficheiro Ficheiro definitivo.
     * @return Verdadeiro se o ficheiro ficou gravado.
     */
    bool Gravar_XML(const std::string &ficheiro) const;

    /** @brief Descrição curta de um dispositivo para os avisos (por exemplo, "MCOMBUSTAO 100 (Roy)"). */
    static std::string Identificar(const Dispositivo *d);

    // ------------------------------------------------------------ menu e permissões

    /**
     * @brief Único ponto de autorização: pergunta ao utilizador atual se pode fazer a ação.
     *
     * Cada operação do menu chama-o antes de fazer alguma coisa. A Fábrica não faz `if`
     * sobre o perfil: é o utilizador que responde (`User::Pode`, virtual). Sem permissão,
     * mostra "Sem permissao" e a operação não acontece.
     * @param acao Ação pedida.
     * @return Verdadeiro se a ação é permitida.
     */
    bool Autorizado(ACAO acao) const;

    /**
     * @brief Mostra as opções do menu e devolve a escolhida (como o `menu()` da Ficha 5).
     *
     * Só lê a opção; quem a executa é o Executa_Menu_Acoes.
     */
    int menu() const;

    /**
     * @brief Menu da simulação parada: repete o menu e despacha cada opção para o seu método.
     *
     * Como o `Executa_Menu_Acoes` da Ficha 5: o `switch` só despacha (todos os `case` com
     * `break`); a leitura de dados fica nos métodos `XxxMenu` e a lógica nos métodos da
     * Fábrica. Termina com "Voltar" (a simulação retoma) ou "Sair do programa".
     */
    void Executa_Menu_Acoes();

    /**
     * @brief Opção "Listar": pergunta se é para o ecrã ou para um ficheiro e chama o Listar.
     *
     * Precisa da permissão LISTAR. O Listar não lê do teclado: quem lê é este método
     * (como o `PertenceMenu` da Ficha 5).
     */
    void ListarMenu();

public:
    /**
     * @brief Cria uma fábrica vazia para o utilizador atual (enunciado: `Fabrica(USER *ut)`).
     *
     * A configuração vem depois, com Load.
     * @param ut Utilizador atual (a Fábrica fica dona dele).
     */
    Fabrica(User *ut);

    /** @brief Liberta os motores, as configurações e o utilizador. */
    virtual ~Fabrica();

    /**
     * @brief Carrega a configuração da fábrica de um ficheiro XML (enunciado).
     *
     * Se o ficheiro não existir ou estiver mal formado, mostra uma mensagem e a fábrica
     * fica como estava. Se for válido, limpa o estado anterior, lê as definições primeiro
     * (criam as configurações dos tipos) e depois os motores. Por fim, como a fábrica fica
     * aberta (dia 1, à hora de abertura), pede a cada motor o Arranque_Inicial: os que não
     * trazem estado gravado arrancam; os outros mantêm o estado do ficheiro.
     * @param ficheiro Caminho do ficheiro XML.
     * @return Verdadeiro se o ficheiro foi carregado.
     */
    bool Load(const std::string &ficheiro);

    /**
     * @brief Adiciona um motor à fábrica (enunciado).
     *
     * Rejeita um ID repetido ou uma posição fora da fábrica. Se aceitar, liga o motor à
     * Fábrica e à configuração do seu tipo e fica dona dele.
     * @param m Motor a adicionar.
     * @return Verdadeiro se foi adicionado. Se for falso, o motor continua a pertencer a
     * quem chamou, que o deve libertar.
     */
    bool Add(Motor *m);

    /**
     * @brief Lista o estado atual da fábrica, no ecrã ou num ficheiro (enunciado).
     * @param f Destino da escrita (por omissão, o ecrã).
     */
    void Listar(std::ostream &f = std::cout);

    // --- Exceção aceite (não está no enunciado) ---

    /**
     * @brief Corre a simulação continuamente: Ciclo, verifica a tecla e espera.
     *
     * Como o `Aquario::Run` da Ficha 5, mas com espera sem ocupar o processador. A tecla
     * 'M' (maiúscula ou minúscula) pára a simulação e mostra o menu; ao voltar do menu, a
     * simulação continua. Sem permissão RUN, a simulação não corre e só o menu está
     * disponível. Termina quando se escolhe "Sair do programa".
     * @note Chamado pelo `main`.
     */
    void Run();

    // --- Interface de colaboração (não está no enunciado; aprovada pelo docente) ---

    /**
     * @brief Uma iteração da simulação, sem teclado nem espera.
     *
     * Avança o relógio 10 minutos e reage ao evento (fecho: STOP a todos; abertura:
     * Novo_Dia e START aos não avariados). Com a fábrica aberta, chama o Ciclo de cada
     * motor (cada um decide sobre si). Depois reescreve o XML do Simulador.exe e mostra o
     * resumo do ciclo, com o dia e a hora.
     * @note Chamado pelo `Run` e pelos testes (que nunca usam o `Run`).
     */
    void Ciclo();

    /**
     * @brief Aviso ao gestor: mostra no ecrã, junta ao histórico e acrescenta ao log, com o dia e a hora simulados.
     * @param mensagem Texto do aviso.
     * @return Verdadeiro se o aviso foi registado (também ficou no log).
     * @note Chamado pelo `Dispositivo` (reencaminhamento protegido `Avisar_Gestor`) e pelos
     * outros avisos desta classe.
     */
    bool Notificar_Gestor(const std::string &mensagem);

    /**
     * @brief Um motor entrou em AMARELO e avisa o gestor (como o `Preciso_de_Comer` da Ficha 5).
     * @param m O motor que aqueceu (não dono).
     * @return Verdadeiro se o aviso foi feito; o motor guarda-o na flag "já avisei".
     * @note Chamado pelo `Motor::ESTOU_QUENTE`, através do reencaminhamento protegido
     * `Dispositivo::Avisar_Aquecimento`.
     */
    bool Aviso_Aquecimento(Motor *m);

    /**
     * @brief Um motor chegou a VERMELHO, já parou sozinho, e avisa o gestor.
     * @param m O motor que sobreaqueceu (não dono).
     * @return Verdadeiro se o aviso foi feito.
     * @note Chamado pelo `Motor::ESTOU_QUENTE`, através do reencaminhamento protegido
     * `Dispositivo::Avisar_Sobreaquecimento`.
     */
    bool Aviso_Sobreaquecimento(Motor *m);

    /**
     * @brief Um dispositivo avariou e avisa o gestor (para mandar fazer manutenção).
     * @param d O dispositivo avariado (não dono).
     * @return Verdadeiro se o aviso foi feito.
     * @note Chamado pelo `Dispositivo::Ciclo`, quando sorteia uma avaria.
     */
    bool Aviso_Avaria(Dispositivo *d);
};

#endif // FABRICA_H
