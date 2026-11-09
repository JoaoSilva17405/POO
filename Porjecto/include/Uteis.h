#ifndef UTEIS_H_INCLUDED
#define UTEIS_H_INCLUDED

#include <string>

/**
 * @brief Classe utilitária só com métodos estáticos (como o Uteis da Ficha 5).
 *
 * Junta os números aleatórios, o teclado, a espera, a leitura validada de dados, o
 * tratamento de texto e a abertura de vídeos. **Todo o código específico de Windows**
 * (`conio.h`, `windows.h`, `system("start ...")`) está no Uteis.cpp e em mais nenhum
 * sítio do projeto. O programa é só para Windows.
 */
class Uteis
{
    /** @brief Se for falso, Abrir_Video não abre nada (os testes desligam os vídeos). */
    static bool VIDEOS_ATIVOS;

public:
    /**
     * @brief Inteiro aleatório no intervalo fechado [min, max].
     * @param min Limite inferior.
     * @param max Limite superior (se for menor do que min, devolve min).
     */
    static int Aleatorio(int min, int max);

    /**
     * @brief Real aleatório no intervalo [min, max].
     * @param min Limite inferior.
     * @param max Limite superior.
     */
    static double Aleatorio_Real(double min, double max);

    /**
     * @brief Sorteia um acontecimento com uma dada probabilidade em percentagem.
     *
     * Compara um real aleatório em [0, 100) com @p percentagem. Assim, 100 acontece
     * sempre, 0 nunca, e 0.01 acontece em 0,01% das vezes (ver o PRD, "Probabilidade de
     * avaria").
     * @param percentagem Probabilidade na escala de 0 a 100.
     * @return Verdadeiro se o acontecimento ocorreu.
     */
    static bool Acontece(double percentagem);

    /** @brief Diz se há uma tecla à espera de ser lida, sem bloquear. */
    static bool TeclaPressionada();

    /** @brief Lê uma tecla já pressionada, sem esperar pelo Enter. */
    static char LerTecla();

    /**
     * @brief Espera um tempo sem ocupar o processador.
     *
     * Corrige o erro da Ficha 5, em que o `wait` fazia espera ativa.
     * @param milissegundos Tempo de espera.
     */
    static void Esperar(int milissegundos);

    /**
     * @brief Mostra uma mensagem e lê um inteiro, repetindo até o valor ser válido.
     * @param mensagem Texto a mostrar ao utilizador.
     */
    static int LerInteiro(const std::string &mensagem);

    /**
     * @brief Mostra uma mensagem e lê uma linha de texto (já sem espaços nas pontas).
     * @param mensagem Texto a mostrar ao utilizador.
     */
    static std::string LerTexto(const std::string &mensagem);

    /** @brief Devolve o texto sem espaços, tabs e mudanças de linha no início e no fim. */
    static std::string Trim(const std::string &texto);

    /** @brief Devolve o texto em maiúsculas (só letras sem acento). */
    static std::string Maiusculas(const std::string &texto);

    /**
     * @brief Troca os caracteres especiais de XML/HTML (`& < > " '`) pelas entidades.
     * @param texto Texto a escrever dentro de um ficheiro XML ou HTML.
     */
    static std::string Escapar_XML(const std::string &texto);

    /**
     * @brief Substitui um ficheiro por outro de uma só vez (o temporário passa a definitivo).
     *
     * Usa a troca do Windows (`MoveFileEx` com substituição), que nunca deixa o destino
     * meio escrito nem inexistente: quem o lê (o Simulador.exe) vê o ficheiro antigo ou o
     * novo, completo. O `rename` da biblioteca padrão falha no Windows quando o destino
     * existe, e apagar antes de mudar o nome deixava um instante sem ficheiro.
     * @param temporario Ficheiro já escrito e fechado.
     * @param definitivo Ficheiro a substituir (é criado se não existir).
     * @return Verdadeiro se a troca foi feita.
     */
    static bool Substituir_Ficheiro(const std::string &temporario, const std::string &definitivo);

    /**
     * @brief Abre um vídeo com o leitor predefinido do Windows (sem esperar que acabe).
     * @param ficheiro Caminho do vídeo.
     * @return Falso se os vídeos estiverem desligados ou o nome for vazio.
     */
    static bool Abrir_Video(const std::string &ficheiro);

    /**
     * @brief Liga ou desliga a abertura de vídeos.
     * @param ativos Falso nos testes, para não abrir janelas.
     */
    static void Videos_Ativos(bool ativos);

    /** @brief Põe a consola do Windows a mostrar UTF-8 (acentos). */
    static void Configurar_Consola();
};

#endif // UTEIS_H_INCLUDED
