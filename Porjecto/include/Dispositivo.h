#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <iosfwd>
#include <string>

#include "Posicao.h"

class Fabrica;
class ElementoXML;
class Motor;

/**
 * @brief Base abstrata de motores e sensores (o "Peixe" da Ficha 5).
 *
 * Usa o padrão **Template Method**: os métodos públicos não virtuais (`Ciclo`, `LerXML`,
 * `Escreve`, `EscreveXML`) tratam o que é comum a todos os dispositivos e chamam ganchos
 * virtuais protegidos (`Funcionar`, `Pode_Avariar`, `LerCamposXML`, `Descricao_Especifica`,
 * `EscreveCamposXML`) que
 * cada subclasse redefine só para o que a distingue. Assim, a lógica comum (por exemplo,
 * a avaria) fica escrita uma única vez.
 *
 * O dispositivo conhece a Fábrica onde está (`Ptr_Fabrica`, não dono), atribuída no `Add`
 * da Fábrica, como o `Atribuir_Aquario` da Ficha 5. As subclasses nunca usam esse
 * ponteiro: falam com a Fábrica só pelos reencaminhamentos protegidos (`Avisar_…`), que
 * verificam o nulo num único sítio. Por isso um dispositivo pode ser testado sozinho.
 */
class Dispositivo
{
    /** @brief Identificador único do dispositivo na fábrica. */
    int ID;
    /** @brief Marca do dispositivo. */
    std::string MARCA;
    /** @brief Posição (X, Y) na fábrica. */
    Posicao POSICAO;
    /** @brief Probabilidade de avaria por ciclo, em % (lida tal como está no XML). */
    double PROB_AVARIA;
    /** @brief Número de avarias que o dispositivo já teve (para o ranking das marcas). */
    int AVARIAS;
    /**
     * @brief Está avariado: não funciona até ter manutenção.
     *
     * É a única fonte de verdade da avaria (como a flag de fome do peixe na Ficha 5): o
     * motor devolve o estado AVARIADO enquanto ela estiver ligada.
     */
    bool Avariado;
    /** @brief Fábrica onde o dispositivo está (não dono); nullptr se estiver fora de uma fábrica. */
    Fabrica *Ptr_Fabrica;

    /** @brief Número de dispositivos vivos (criados e ainda não destruídos), para os testes de memória. */
    static int N_VIVOS;

    /**
     * @brief Avisa a Fábrica de que este dispositivo avariou (Aviso_Avaria).
     *
     * Privado: só o Ciclo, aqui na base, trata a avaria.
     */
    void Avisar_Avaria();

protected:
    /** @brief Cria um dispositivo vazio; os dados vêm depois do XML (LerXML). */
    Dispositivo();

    /**
     * @brief Cópia usada pelo Clone() das subclasses (protótipos do Catálogo).
     *
     * A cópia fica fora de qualquer fábrica (Ptr_Fabrica a nullptr) e conta como mais um
     * dispositivo vivo.
     * @param outro Dispositivo copiado.
     */
    Dispositivo(const Dispositivo &outro);

    /**
     * @brief Gancho do LerXML: lê os campos próprios da subclasse.
     *
     * Por omissão não lê nada. A versão de uma subclasse deve chamar a da classe mãe.
     * @param elemento Elemento XML do dispositivo.
     */
    virtual void LerCamposXML(const ElementoXML &elemento);

    /**
     * @brief Gancho do Escreve: escreve a parte própria da subclasse, na mesma linha.
     *
     * Por omissão não escreve nada.
     * @param f Destino da escrita.
     */
    virtual void Descricao_Especifica(std::ostream &f) const;

    /**
     * @brief Gancho do EscreveXML: escreve as tags próprias da subclasse, dentro da tag do dispositivo.
     *
     * Por omissão não escreve nada. A versão de uma subclasse deve chamar a da classe mãe.
     * O texto livre tem de passar pelo Uteis::Escapar_XML.
     * @param f Destino da escrita.
     */
    virtual void EscreveCamposXML(std::ostream &f) const;

    /**
     * @brief Gancho do Ciclo: o que o dispositivo faz num ciclo em que não está avariado.
     *
     * Gancho puro: cada família (motor, sensor) tem a sua máquina de estados.
     */
    virtual void Funcionar() = 0;

    /**
     * @brief Gancho do Ciclo: diz se neste ciclo o dispositivo pode avariar.
     *
     * Por omissão, sempre. O motor só avaria quando está a trabalhar.
     */
    virtual bool Pode_Avariar() const;

    /**
     * @brief Gancho do Ciclo: probabilidade de avaria por ciclo, em %.
     *
     * Por omissão, a lida do XML do próprio dispositivo. O motor usa a do seu tipo
     * (configuração em `<DEFINICOES>`).
     */
    virtual double Probabilidade_Avaria() const;

    // --- Reencaminhamentos para a Fábrica (o único ponto de contacto das subclasses) ---

    /**
     * @brief Envia uma mensagem ao gestor da Fábrica (Notificar_Gestor).
     * @param mensagem Texto do aviso.
     * @return Falso se o dispositivo não estiver numa fábrica.
     */
    bool Avisar_Gestor(const std::string &mensagem);

    /**
     * @brief Avisa a Fábrica de que um motor entrou em AMARELO (Aviso_Aquecimento).
     *
     * Como o `Ptr_Aquario->Preciso_de_Comer(this)` da Ficha 5: o valor devolvido serve
     * para a flag "já avisei".
     * @param m O motor que aqueceu (o próprio, `this`).
     * @return Verdadeiro se a Fábrica recebeu o aviso; falso fora de uma fábrica.
     */
    bool Avisar_Aquecimento(Motor *m);

    /**
     * @brief Avisa a Fábrica de que um motor chegou a VERMELHO e parou (Aviso_Sobreaquecimento).
     * @param m O motor que sobreaqueceu (o próprio, `this`).
     * @return Verdadeiro se a Fábrica recebeu o aviso; falso fora de uma fábrica.
     */
    bool Avisar_Sobreaquecimento(Motor *m);

public:
    /** @brief Destrutor virtual: apagar um Motor através de um Dispositivo* chama o destrutor certo. */
    virtual ~Dispositivo();

    /** @brief Atribuição proibida: um dispositivo não se copia por cima de outro. */
    Dispositivo &operator=(const Dispositivo &) = delete;

    /**
     * @brief Liga o dispositivo à Fábrica (chamado pela Fábrica no Add).
     * @param F Fábrica onde o dispositivo passa a estar (não dono).
     */
    void Atribuir_Fabrica(Fabrica *F) { Ptr_Fabrica = F; }

    /**
     * @brief Um ciclo de vida do dispositivo (Template Method), como o `Peixe::Run` da Ficha 5.
     *
     * Se estiver avariado, não faz nada. Se puder avariar (`Pode_Avariar`), sorteia a
     * avaria com `Probabilidade_Avaria`: se avariar, liga a flag, conta a avaria e avisa a
     * Fábrica. Caso contrário chama o gancho `Funcionar`. Chamado pela Fábrica em cada
     * ciclo. O nome evita a confusão com o `RUN()` do motor (enunciado).
     */
    void Ciclo();

    /**
     * @brief Lê o dispositivo de um elemento XML (Template Method).
     *
     * Lê os campos comuns por nome (ID, MARCA, POSICAO, PROB_AVARIA), mantendo o valor
     * atual nos que faltam, e depois chama o gancho LerCamposXML da subclasse.
     * Chamado pela Fábrica no Load.
     * @param elemento Elemento XML do dispositivo (por exemplo, `<MCOMBUSTAO>`).
     */
    void LerXML(const ElementoXML &elemento);

    /**
     * @brief Escreve uma linha com o dispositivo (Template Method), no ecrã ou num ficheiro.
     *
     * Escreve a parte comum e acrescenta Descricao_Especifica. Chamado pela Fábrica no Listar.
     * @param f Destino da escrita.
     */
    void Escreve(std::ostream &f) const;

    /**
     * @brief Escreve o dispositivo em XML (Template Method), no formato do Simulador.exe.
     *
     * Abre a tag `QuemEs()`, escreve os campos comuns (ID, MARCA, POSICAO), chama o gancho
     * EscreveCamposXML da subclasse e fecha a tag. Como no `Aquario::GravarXML` da Ficha 5,
     * a Fábrica só abre o ficheiro e pede a cada dispositivo que se escreva.
     * Chamado pela Fábrica ao gravar o XML do simulador.
     * @param f Destino da escrita (ficheiro ou, nos testes, um ostringstream).
     */
    void EscreveXML(std::ostream &f) const;

    /** @brief Tipo do dispositivo, que é também a sua tag XML (por exemplo, "MCOMBUSTAO"). */
    virtual std::string QuemEs() const = 0;

    /** @brief ID do dispositivo (usado pela Fábrica na pesquisa por ID). */
    int GetID() const { return ID; }

    /** @brief Marca do dispositivo (usada pela Fábrica nas listagens e rankings). */
    const std::string &GetMarca() const { return MARCA; }

    /** @brief Posição do dispositivo (usada pela Fábrica para verificar se está dentro dela). */
    const Posicao &GetPosicao() const { return POSICAO; }

    /** @brief Número de avarias já tidas (usado pela Fábrica no ranking das marcas). */
    int Get_Avarias() const { return AVARIAS; }

    /** @brief Diz se está avariado (usado pelo Motor no estado e pela Fábrica na abertura). */
    bool Esta_Avariado() const { return Avariado; }

    /** @brief Número de dispositivos vivos; tem de voltar a 0 quando tudo é libertado (testes de memória). */
    static int Contador_Vivos() { return N_VIVOS; }
};

#endif // DISPOSITIVO_H
