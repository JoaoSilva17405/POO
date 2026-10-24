# Ficha de Trabalho 5 — Explicação Detalhada da Resolução (Aquário e Peixes)

> Este documento explica a resolução proposta pelo professor na aula teórica para a
> Ficha 5. **Não assume conhecimento prévio**: cada conceito (classe, ponteiro, `this`,
> herança, polimorfismo, encapsulamento, inclusão circular, ...) é explicado antes de ser
> usado. Se já conheces algum, salta essa secção.
>
> **Atualização (24/10/2026):** o documento descreve agora a versão atual do projeto,
> que acrescenta à base do professor a **saída do peixe do aquário** e a **remoção por
> número de série**. Os nomes usados são os do código: `QueroSair`, `RemoverParaAssar`,
> `RemoverID`, `Pertence`, `EstouGordo`, `Ja_Avisei_Quero_Sair`, `GetID`, `GetPeso`. A
> secção 8 explica essa extensão e as decisões tomadas. Algumas sugestões da versão
> anterior deste documento foram substituídas pela abordagem escolhida.

Ficheiros da resolução:

| Ficheiro | O que contém |
|---|---|
| `main.cpp` | Função `main` — cria um aquário, põe lá peixes e arranca a simulação |
| `Uteis.h` | Classe utilitária com funções "soltas" (número aleatório, esperar) |
| `include/Peixe.h` + `src/Peixe.cpp` | Classe `Peixe` |
| `include/Aquario.h` + `src/Aquario.cpp` | Classe `Aquario` |
| `include/Tubarao.h` + `src/Tubarao.cpp` | Classe `Tubarao`, que **herda** de `Peixe` |

Índice:

0. [Conceitos base de C++ necessários](#0-conceitos-base-de-c-necessários)
1. [O que o enunciado pede (em resumo)](#1-o-que-o-enunciado-pede-em-resumo)
2. [As classes criadas, porquê, e as relações entre elas](#2-as-classes-criadas-porquê-e-as-relações-entre-elas)
3. [Encapsulamento — o tema central da ficha](#3-encapsulamento--o-tema-central-da-ficha)
4. [Inclusão circular e a solução com ponteiro](#4-inclusão-circular-e-a-solução-com-ponteiro)
5. [Herança (`Tubarao : public Peixe`)](#5-herança-tubarao--public-peixe)
6. [Polimorfismo (`virtual`)](#6-polimorfismo-virtual)
7. [Execução passo-a-passo do programa](#7-execução-passo-a-passo-do-programa)
8. [A saída do peixe e a remoção: a solução implementada](#8-a-saída-do-peixe-e-a-remoção-a-solução-implementada)
9. [Resumo: porque é que esta é a forma correta](#9-resumo-porque-é-que-esta-é-a-forma-correta)

---

## 0. Conceitos base de C++ necessários

### 0.1 Classe e objeto

Uma **classe** é um "molde": descreve que **dados** (atributos) e que **ações**
(métodos) um certo tipo de coisa tem. Um **objeto** é uma coisa concreta construída a
partir desse molde.

```cpp
class Peixe          // o molde
{
    int PESO;        // atributo (dado)
public:
    void Alimentar(int cal);   // método (ação)
};

Peixe nemo(...);     // um objeto concreto
Peixe dory(...);     // outro objeto, com o seu próprio PESO
```

Cada objeto tem a **sua própria cópia** dos atributos: o `PESO` do `nemo` é independente
do `PESO` da `dory`.

### 0.2 `private` e `public`

Dentro de uma classe, cada membro é:

- **`private`** — só pode ser usado **por código da própria classe** (os seus métodos).
  Ninguém de fora lhe toca. Numa `class`, **tudo é `private` por omissão** até aparecer
  outra palavra. Por isso em `Peixe.h`, `ID`, `ESPECIE`, `PESO`, `Ptr_Aquario`,
  `Ja_Avisei_Tenho_Fome` e `EstouComFome()` são privados — estão antes do `public:`.
- **`public`** — pode ser usado por qualquer código (o `main`, o `Aquario`, ...).
- **`protected`** — (explicado na secção 5) como `private`, mas as classes derivadas
  (filhas) também lhe podem aceder.

A parte `public` é a **interface** da classe: a lista de "botões" que o mundo exterior pode
carregar. A parte `private` é a **implementação**: o mecanismo interno, escondido.

### 0.3 Construtor e destrutor

- O **construtor** é um método com o mesmo nome da classe, sem tipo de retorno, chamado
  **automaticamente** quando o objeto nasce. Serve para pôr os atributos num estado válido.
- O **destrutor** chama-se `~NomeDaClasse()` e é chamado automaticamente quando o objeto
  morre (sai de âmbito, ou é feito `delete`).

```cpp
Peixe::Peixe(int _id, string _especie, int PESO)
{
    ID = _id;
    ESPECIE = _especie;
    this->PESO = PESO;     // ver 0.6 para o "this->"
    Ptr_Aquario = NULL;    // ainda não está em nenhum aquário
    Ja_Avisei_Tenho_Fome = false;
}
```

**Parâmetros com valor por omissão**: em `Peixe.h` está
`Peixe(int _id, string _especie, int PESO = 10);`. O `= 10` quer dizer que, se quem cria o
peixe não indicar o peso, é usado 10. `Peixe p(1, "Sardinha");` é o mesmo que
`Peixe p(1, "Sardinha", 10);`. O valor por omissão escreve-se **só no `.h`** (declaração),
não no `.cpp`.

### 0.4 Ponteiros, `new`, `->`

Um **ponteiro** é uma variável que guarda o **endereço de memória** de outra coisa — é
como guardar a morada de uma casa em vez da casa.

```cpp
Peixe *P;                         // P é um ponteiro para um Peixe (ainda não aponta para nada útil)
P = new Peixe(10, "Sardinha", 30);// new cria um Peixe na memória dinâmica e devolve o seu endereço
P->Show();                        // "->" = vai ao objeto apontado e chama Show()
                                  // (equivale a (*P).Show())
delete P;                         // destrói o objeto e liberta a memória
```

Pontos importantes:

- Todos os ponteiros têm **o mesmo tamanho** (ex.: 8 bytes num sistema de 64 bits),
  **independentemente** do tipo para que apontam. Um `Peixe*` e um `Aquario*` ocupam o
  mesmo espaço. **Isto é a chave da solução da inclusão circular** (secção 4).
- `NULL` (ou `nullptr` em C++ moderno) é o valor "não aponta para nada".
- Um objeto criado com `new` **vive até alguém fazer `delete`** — não morre sozinho quando a
  função termina. É por isso que os peixes criados no `for` do `main` continuam a existir
  depois do `for` acabar.
- Copiar um ponteiro **não copia o objeto**: dois ponteiros com o mesmo endereço apontam
  para **o mesmo** peixe. Se o aquário guarda o ponteiro e o peixe guarda um ponteiro para
  o aquário, estão os dois a falar com os objetos **originais**, não com cópias.

### 0.5 Referências (`&`)

Em `main.cpp` aparece `void F1_Escrita(ostream &fsaida)`. O `&` num parâmetro diz que a
função recebe **o próprio objeto** (não uma cópia). É parecido com um ponteiro, mas
usa-se com `.` em vez de `->` e não pode ser `NULL`.

### 0.6 O ponteiro `this`

Dentro de qualquer método, existe automaticamente um ponteiro chamado **`this`** que
aponta **para o objeto que está a executar o método**. Usos na resolução:

1. **Desambiguar nomes**: no construtor do `Peixe`, o parâmetro chama-se `PESO` tal como o
   atributo. `this->PESO = PESO;` significa "o atributo `PESO` deste objeto recebe o
   parâmetro `PESO`".
2. **Um objeto dar o seu próprio endereço a outro objeto**. É o uso mais importante aqui:
   - Em `Aquario::Add`: `P->Atribuir_Aquario(this);` → o aquário diz ao peixe "**eu** sou
     o teu aquário" (passa o seu próprio endereço).
   - Em `Peixe::EstouComFome`: `Ptr_Aquario->Preciso_de_Comer(this);` → o peixe diz ao
     aquário "**eu** (este peixe) preciso de comer".

### 0.7 `list` da STL e iteradores

`list<Peixe *> LP;` é uma **lista ligada** (da biblioteca padrão) que guarda elementos do tipo
`Peixe*` (ponteiros para peixes).

- `LP.push_front(P)` — acrescenta `P` **no início** da lista.
- `LP.size()` — número de elementos.
- `LP.clear()` — esvazia a lista (atenção: só remove os **ponteiros** da lista, **não**
  destrói os peixes).
- Para percorrer usa-se um **iterador**, que é como um "cursor" que anda pela lista:

```cpp
for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
{
    (*it)->Run();
}
```

  - `LP.begin()` — cursor no primeiro elemento; `LP.end()` — posição **depois** do último.
  - `*it` — o elemento onde o cursor está (aqui, um `Peixe*`).
  - `(*it)->Run()` — chama `Run()` no peixe apontado por esse elemento.

### 0.8 Membros `static` (classe `Uteis`)

```cpp
class Uteis
{
public:
    static int Aleatorio(int min, int max) { return min + rand() % (max - min + 1); }
    ...
};
```

Um método `static` **pertence à classe e não a um objeto**. Chama-se com
`Uteis::Aleatorio(2, 6)` sem criar nenhum objeto `Uteis`. A classe `Uteis` é só uma
"caixa de ferramentas" que agrupa funções genéricas.

- `rand() % (max - min + 1)` dá um número entre `0` e `max - min`; somando `min` fica
  entre `min` e `max`.
- `srand(time(NULL))` no `main` "baralha" o gerador com a hora atual, para os números não
  serem iguais em todas as execuções.
- `wait_segundos(2)` fica num ciclo até passarem 2 segundos (é uma "espera ativa" — gasta
  CPU, mas é simples).

### 0.9 Ficheiros `.h` e `.cpp`, compilação e o pré-processador

Isto é **essencial para perceber a inclusão circular**.

- O **`.h`** (header / cabeçalho) contém a **declaração** da classe: que atributos e que
  métodos existem. É a "ficha técnica".
- O **`.cpp`** contém a **definição** (implementação) dos métodos: o código que eles
  executam.

A compilação faz-se **um `.cpp` de cada vez**, de forma independente. Cada `.cpp` é
compilado sozinho; no fim, o *linker* junta tudo num executável.

Antes de compilar, corre o **pré-processador**, que trata das linhas começadas por `#`:

- `#include "Peixe.h"` → o pré-processador **copia e cola literalmente** o conteúdo de
  `Peixe.h` naquele sítio. Não há magia: é copy-paste de texto.
- **Include guards** (proteção contra inclusão repetida):

```cpp
#ifndef PEIXE_H     // "se PEIXE_H ainda NÃO estiver definido..."
#define PEIXE_H     // "...define-o agora..."
    ... conteúdo ...
#endif              // "...fim do bloco"
```

  Na primeira vez que `Peixe.h` é colado, `PEIXE_H` não existe → o conteúdo entra e
  `PEIXE_H` fica definido. Se o mesmo ficheiro for incluído uma segunda vez no mesmo
  `.cpp`, `PEIXE_H` já existe → o conteúdo inteiro é **ignorado**. Isto evita "a classe
  `Peixe` foi definida duas vezes".

**Regra do compilador que importa aqui**: o compilador lê o código **de cima para baixo**.
Para usar um nome (por ex. `Aquario`), esse nome tem de ter aparecido **antes**.

### 0.10 `ostream` — um primeiro exemplo de polimorfismo (está no `main.cpp`)

O professor deixou no `main.cpp` três funções `F1`, `F1_ficheiro` e `F1_Escrita`. A ideia:

```cpp
void F1_Escrita(ostream &fsaida)
{
    for (int i = 0; i < 10; i++)
        fsaida << "Ola pessoal" << endl;
    fsaida << "odsfdsfdskfo" << endl;
}
...
F1_Escrita(cout);           // escreve no ecrã
ofstream F("dados.txt");
F1_Escrita(F);              // escreve no ficheiro dados.txt
```

`cout` (ecrã) e `ofstream` (ficheiro) são tipos diferentes, mas **ambos são um `ostream`**
(o `ofstream` **herda** de `ostream`). Por isso a mesma função serve para os dois: em vez
de escrever `F1` (só ecrã) e `F1_ficheiro` (só ficheiro) com código duplicado, escreve-se
**uma** função que recebe "qualquer coisa onde se possa escrever". O ficheiro `dados.txt`
que está na pasta é precisamente o resultado disso.

Isto é **herança + polimorfismo** a funcionar na biblioteca padrão — exatamente a mesma
ideia que vamos aplicar a `Peixe`/`Tubarao` (secções 5 e 6). Guarda este exemplo na cabeça.

---

## 1. O que o enunciado pede (em resumo)

- Simular **peixes** dentro de um **aquário**, cada conceito na sua classe.
- **Peixe**: tem espécie, cor, peso, número de série. Pode ser alimentado (a comida soma
  ao peso). Se passar de 50 g e estiver num aquário, **o peixe decide** que quer sair e
  **pede** ao aquário para o tirar. A decisão pode **variar com a espécie**.
- **Aquário**: tem muitos peixes. Pode inserir um peixe, verificar se um peixe lá está
  (pelo número de série) e alimentar todos.
- Interação **bidirecional**:
  1. aquário → peixe: "toma comida";
  2. peixe → aquário: "quero ser removido".
- Cada classe num par `.h`/`.cpp` → isto **provoca de propósito** uma inclusão circular, que
  é preciso saber resolver.
- **Encapsulamento**: o aquário **não** pode manipular o interior do peixe como uma
  "marioneta transparente".

A resolução da aula implementa a **estrutura** deste problema (as classes, a ligação
bidirecional, a solução da inclusão circular, a herança e os métodos virtuais) com uma
pequena variação no cenário: em vez de "peixe com mais de 50 g pede para sair", o
professor simula **peixes que vão perdendo peso e avisam o aquário quando têm fome**. O
mecanismo é **exatamente o mesmo** que o enunciado pede (peixe → aquário a pedir algo,
aquário → peixe a dar comida).

Sobre essa base foi acrescentada a regra do enunciado: **depois de comer, se tiver mais
de 50 g, o peixe pede ao aquário para sair**, e o aquário retira-o. Foi acrescentada
também a pesquisa e a remoção por número de série. A secção 8 explica esta extensão.

---

## 2. As classes criadas, porquê, e as relações entre elas

### 2.1 Porque é que existem estas classes

Em Programação Orientada a Objetos, a regra base é: **cada conceito do problema com
identidade, estado e comportamento próprios torna-se uma classe**. Lê-se o enunciado e
procuram-se os "substantivos" que fazem coisas:

| Classe | Porquê existe | Estado (dados) | Comportamento |
|---|---|---|---|
| `Peixe` | O enunciado fala de peixes, cada um com dados próprios e decisões próprias | `ID`, `ESPECIE`, `PESO`, `COR`, `Ptr_Aquario`, `Ja_Avisei_Tenho_Fome`, `Ja_Avisei_Quero_Sair` | `Run`, `Show`, `Alimentar`, `EstouComFome`, `EstouGordo`, `GetID`, `GetPeso` |
| `Aquario` | É um conceito diferente: um **contentor** que gere vários peixes | listas `LP`, `LP_Fome`, `LP_Assar` | `Add`, `Run`, `Alimentar`, `Preciso_de_Comer`, `QueroSair`, `RemoverParaAssar`, `RemoverID`, `Pertence` |
| `Tubarao` | "Pode haver várias espécies de peixe e a decisão varia em função da espécie" → uma espécie com comportamento próprio é um **tipo especial de peixe** | o que herda de `Peixe` + `GRAU_PERIGO` | o que herda de `Peixe` (e pode redefinir métodos `virtual`) |
| `Uteis` | Funções genéricas (aleatório, esperar) que não pertencem a nenhum conceito do problema | nenhum | métodos `static` |

**Porque não meter tudo numa classe só?** Porque um aquário **não é** um peixe e um peixe
**não é** um aquário. Se misturássemos, cada alteração ao peixe obrigava a mexer no
código do aquário e vice-versa. Separar dá a cada classe **uma responsabilidade**
("princípio da responsabilidade única"):

- o `Peixe` sabe **ser peixe** (comer, engordar, ter fome, decidir);
- o `Aquario` sabe **gerir uma coleção de peixes** (guardar, percorrer, alimentar, remover).

### 2.2 As relações entre as classes

```
                     ┌──────────────────────────────────────┐
                     │               Aquario                │
                     │--------------------------------------│
                     │ - LP       : list<Peixe*>            │
                     │ - LP_Fome  : list<Peixe*>            │
                     │ - LP_Assar : list<Peixe*>            │
                     │--------------------------------------│
                     │ + Add(Peixe*)                        │
                     │ + Run()                              │
                     │ + Alimentar()                        │
                     │ + Preciso_de_Comer(Peixe*) : bool    │
                     │ + QueroSair(Peixe*) : bool           │
                     │ + RemoverParaAssar() : bool          │
                     │ + RemoverID(int) : bool              │
                     │ + Pertence(int) : Peixe*             │
                     └──────────────────────────────────────┘
                        ◆ 1                          ▲
                        │ "tem/possui"               │ "sabe em que aquário está"
                        │  0..* peixes               │  (ponteiro Ptr_Aquario, 0..1)
                        ▼                            │
                     ┌──────────────────────────────────────┐
                     │                Peixe                 │
                     │--------------------------------------│
                     │ - ID, ESPECIE, PESO, COR             │
                     │ - Ptr_Aquario : Aquario*             │
                     │ - Ja_Avisei_Tenho_Fome : bool        │
                     │ - Ja_Avisei_Quero_Sair : bool        │
                     │ - EstouComFome()                     │
                     │ # virtual EstouGordo() : bool        │
                     │--------------------------------------│
                     │ + Atribuir_Aquario(Aquario*)         │
                     │ + virtual Show()                     │
                     │ + virtual Run()                      │
                     │ + Alimentar(int)                     │
                     │ + GetID() : int                      │
                     │ + GetPeso() : int                    │
                     │ + virtual ~Peixe()                   │
                     └──────────────────────────────────────┘
                                       △
                                       │ "é um" (herança)
                                       │
                     ┌──────────────────────────────────────┐
                     │               Tubarao                │
                     │--------------------------------------│
                     │ - GRAU_PERIGO : int                  │
                     │--------------------------------------│
                     │ + virtual Show()   (redefinido)      │
                     └──────────────────────────────────────┘

     Legenda: - private   # protected   + public

     Uteis: usada por Peixe e Aquario (dependência simples: "usa")
```

As três relações:

1. **Aquario → Peixe: "tem" (composição/agregação, 1 para muitos).**
   O aquário guarda **muitos** peixes (`list<Peixe*> LP`). O enunciado diz: "quando um
   peixe é posto no aquário, este assume a sua posse e controlo total" — o aquário é o
   **dono** dos peixes que tem. É por isso que é o aquário que faz `delete` quando um
   peixe sai (`RemoverParaAssar`) ou é removido (`RemoverID`). Pela mesma razão, quando o
   aquário é destruído devia destruir os peixes que ainda tem; isto ainda está por fazer
   (secção 8.12).

2. **Peixe → Aquario: "conhece" (associação, 0 ou 1).**
   O peixe guarda um `Aquario *Ptr_Aquario`, para saber **a quem pedir coisas**. Não é
   dono do aquário — só o conhece. Pode ser `NULL` (peixe ainda fora de qualquer aquário,
   como o enunciado permite: "quando são criados os peixes não estão necessariamente em
   aquário nenhum").

   As relações 1 e 2 juntas são a **relação bidirecional** que a ficha quer treinar:
   o aquário chega aos peixes **e** os peixes chegam ao aquário.

3. **Tubarao → Peixe: "é um" (herança).**
   Um tubarão **é um** peixe: tem tudo o que um peixe tem, mais algo de próprio. Secção 5.

### 2.3 Porque é que o aquário guarda `Peixe*` (ponteiros) e não `Peixe` (objetos)

`list<Peixe *> LP;` em vez de `list<Peixe> LP;`. Há três razões, todas importantes:

1. **Identidade (não haver cópias).** Se a lista guardasse `Peixe` por valor, `Add` metia
   lá uma **cópia** do peixe. O peixe original (que tem `Ptr_Aquario`) e a cópia (que está
   na lista) passavam a ser dois peixes diferentes: alimentar um não alterava o outro, e o
   `this` que o peixe envia ao aquário não seria o endereço do peixe que está na lista.
   Com ponteiros, existe **um único peixe** e toda a gente fala com ele.
2. **Polimorfismo.** Uma `list<Peixe>` só consegue guardar **exatamente** `Peixe`s. Se
   lá metêssemos um `Tubarao`, ele era "cortado" e ficava só a parte `Peixe` (chama-se
   *object slicing*). Uma `list<Peixe*>` pode guardar ponteiros para `Peixe`, `Tubarao`, ou
   qualquer outra espécie derivada, e cada um continua a ser o que é (secção 6).
3. **Posse / tempo de vida.** Os peixes são criados com `new` no `main` e "entregues" ao
   aquário. O aquário passa a controlar quando morrem ("poder de vida e morte"), com
   `delete`.

### 2.4 Para que servem as três listas do aquário

- `LP` — **todos** os peixes do aquário.
- `LP_Fome` — peixes que **pediram** comida. Repara: o aquário não vai "verificar quem tem
  fome" olhando para o peso dos peixes (isso seria violar o encapsulamento); são os
  **peixes que se inscrevem** nesta lista, chamando `Preciso_de_Comer(this)`.
- `LP_Assar` — peixes que **pediram para sair**. Segue o mesmo padrão da `LP_Fome`: o
  peixe inscreve-se chamando `QueroSair(this)`, e o aquário trata os pedidos mais tarde,
  em `RemoverParaAssar()`. Secção 8.

---

## 3. Encapsulamento — o tema central da ficha

### 3.1 O que é

**Encapsulamento** é a ideia de que **cada objeto é responsável pelo seu próprio estado**.
Os dados ficam `private` e o mundo exterior só interage com o objeto através de métodos
públicos que exprimem **pedidos** ("alimenta-te", "mostra-te"), nunca através de
manipulação direta dos dados ("o teu peso passa a ser X").

Analogia: num restaurante, tu (o cliente) dizes ao empregado "quero um bife". Não entras
na cozinha para mexer nos tachos. Como o bife é feito é problema da cozinha. Se a cozinha
mudar de fogão, tu não tens de mudar a forma como pedes.

O enunciado diz isto com o exemplo da "marioneta transparente":

> "fazer o aquário incrementar o peso do peixe quando este é alimentado – **errado**: o peixe
> come; se transforma essa comida em peso, ou se tem um problema de metabolismo e deita
> tudo fora, **o peixe é que sabe, não o aquário**."

### 3.2 A versão ERRADA (marioneta) vs a versão CERTA (a do professor)

**Versão errada** — o aquário mexe nos dados do peixe:

```cpp
// Peixe.h  (ERRADO: dados públicos ou getters/setters "para tudo")
class Peixe {
public:
    int PESO;
    bool Ja_Avisei_Tenho_Fome;
};

// Aquario.cpp  (ERRADO)
void Aquario::Alimentar()
{
    for (...) {
        Peixe *p = *it;
        p->PESO += 30;                     // o aquário decide como o peixe engorda
        p->Ja_Avisei_Tenho_Fome = false;   // o aquário mexe no "estado de espírito" do peixe
        if (p->PESO > 50) {                // o aquário decide se o peixe sai
            LP.remove(p);
        }
    }
}
```

O aquário tem de **saber** como o peixe funciona por dentro: que tem um `PESO`, que a comida
soma ao peso, que existe uma flag de fome, que o limite é 50 g... O aquário virou o
"cérebro" de todos os peixes.

**Versão certa** — a da resolução do professor:

```cpp
// Aquario.cpp — o aquário só DIZ "toma comida" e passa ao próximo
void Aquario::Alimentar()
{
    cout << "Alimentacao a DEcorrer....." << endl;
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
    {
        (*it)->Alimentar(Uteis::Aleatorio(20, 50));   // "toma, come, faz o que entenderes"
    }
    LP_Fome.clear();
}

// Peixe.cpp — é o PEIXE que decide o que a comida lhe faz
void Peixe::Alimentar(int cal)
{
    PESO += cal;                    // decisão do peixe: converte comida em peso
    Ja_Avisei_Tenho_Fome = false;   // decisão do peixe: já não tem fome
    if( EstouGordo() && !Ja_Avisei_Quero_Sair) // decisão do peixe: quer um "quintal" maior?
    {
        Ja_Avisei_Quero_Sair = Ptr_Aquario->QueroSair(this);
    }
}
```

O aquário **não sabe** que o peixe tem um atributo `PESO`, nem as flags
`Ja_Avisei_Tenho_Fome` e `Ja_Avisei_Quero_Sair`, nem que o limite para sair são 50 g. Nem precisa: `PESO` é `private`, e o aquário nem conseguiria
tocar-lhe — o compilador dava erro. O `private` é o "cadeado" que **obriga** o código a
respeitar o encapsulamento.

### 3.3 O caminho inverso: o peixe pede, não mexe

O mesmo princípio aplica-se no sentido peixe → aquário. Quando o peixe tem fome, ele
**não** se mete na lista do aquário (`Ptr_Aquario->LP_Fome.push_front(this)` — nem
compilaria, `LP_Fome` é `private`). Ele **pede**:

```cpp
void Peixe::EstouComFome()
{
    if (!Ja_Avisei_Tenho_Fome)
    {
        bool ret = Ptr_Aquario->Preciso_de_Comer(this);   // "aquário, preciso de comer"
        Ja_Avisei_Tenho_Fome = ret;
    }
}

bool Aquario::Preciso_de_Comer(Peixe *P)
{
    LP_Fome.push_front(P);   // o aquário decide como regista o pedido
    return true;             // e responde "pedido aceite"
}
```

A saída do aquário funciona da mesma forma. O enunciado diz: "pede ao aquário para o tirar
(**não se tira a ele próprio** – o peixe não faz ideia como é que o aquário guarda os peixes
internamente)". O peixe não faz `Ptr_Aquario->LP.remove(this)`. Pede:

```cpp
// no Peixe::Alimentar
Ja_Avisei_Quero_Sair = Ptr_Aquario->QueroSair(this);   // "aquário, quero sair"

bool Aquario::QueroSair(Peixe *P)
{
    LP_Assar.push_back(P);   // o aquário regista o pedido à sua maneira
    return true;             // "pedido aceite"
}
```

Cada lado manda **mensagens** ao outro; ninguém mexe nas "tripas" do outro:

```
   Aquario                                      Peixe
      │                                           │
      │ ── Alimentar(cal) ──────────────────────▶ │  (o peixe decide o que fazer
      │                                           │   com a comida)
      │ ◀──────────────────────── QueroSair(this)─│  (se ficou com mais de 50 g)
      │ ── return true ─────────────────────────▶ │
      │                                           │
      │ ◀──────────────── Preciso_de_Comer(this)──│  (no Run, se tiver fome)
      │ ── return true ─────────────────────────▶ │
```

### 3.4 Outros pormenores de bom encapsulamento na resolução

- **`EstouComFome()` é `private`.** É um passo interno do peixe. Ninguém de fora deve poder
  "obrigar" um peixe a ter fome. Só o próprio `Run()` do peixe o chama, quando o peso
  baixa de 20.
- **A flag `Ja_Avisei_Tenho_Fome`** é estado interno do peixe que evita que ele peça
  comida em todos os ciclos (e apareça repetido em `LP_Fome`). O aquário não sabe que ela
  existe. Se o peixe não tivesse esta flag, o problema "pedidos repetidos" teria de ser
  resolvido pelo aquário, que ficaria a depender de um pormenor do peixe. A flag
  `Ja_Avisei_Quero_Sair` faz o mesmo para o pedido de saída (secção 8.6).
- **`EstouGordo()` é `protected` e `virtual`.** O critério de saída é uma decisão interna
  do peixe, por isso não é `public`. É `protected virtual` para que cada espécie derivada
  possa ter o seu próprio critério (secção 6).
- **`Atribuir_Aquario(Aquario *A)`** é o único "setter" do peixe, e existe por uma boa
  razão: o peixe não consegue descobrir sozinho em que aquário foi posto — é o aquário
  que lho diz, dentro do seu `Add`:

  ```cpp
  void Aquario::Add(Peixe *P)
  {
      LP.push_front(P);          // o aquário guarda o peixe (à maneira dele)
      P->Atribuir_Aquario(this); // e diz-lhe "agora pertences-me a mim"
  }
  ```

  Assim o `main` só faz `AQ.Add(P)` e a ligação nos **dois sentidos** fica sempre
  consistente. Se o `main` tivesse de fazer `AQ.Add(P)` **e** `P->Atribuir_Aquario(&AQ)`,
  bastava alguém esquecer-se de um dos dois para o programa ficar incoerente.
- **Getters só de leitura e só quando fazem falta.** Existem `GetID()` (o aquário precisa
  do número de série para `Pertence` e `RemoverID`) e `GetPeso()` (o aquário mostra o peso
  do peixe que sai). **Não há setters** como `SetPeso()`: com um setter, o aquário voltava
  a poder decidir o peso do peixe, e a marioneta voltava pela porta das traseiras.

  Mesmo um getter de leitura exige cuidado. Usar `GetPeso()` para **mostrar** o peso está
  bem. Usá-lo para **decidir** pelo peixe, por exemplo
  `if ((*it)->GetPeso() > 50) RemoverID(...)` no aquário, era voltar ao erro que o
  enunciado descreve ("Estás com mais de 50 gramas? Então sai").

### 3.5 Boas práticas resumidas

1. Atributos **sempre `private`**.
2. Métodos públicos exprimem **ações/pedidos** ("alimenta-te"), não acesso a dados
   ("muda o teu peso").
3. **Quem tem os dados é quem toma as decisões sobre eles** ("o peixe é que sabe").
4. Métodos auxiliares internos ficam `private` (`EstouComFome`) ou `protected`, se as
   classes derivadas precisarem de os redefinir (`EstouGordo`).
5. Só criar getters quando há uma necessidade real e, se possível, só de leitura (por ex.
   o aquário precisa do número de série para `Pertence(id)` → um `GetID()` faz
   sentido; um `SetID` não).
6. Um objeto **pede** a outro em vez de lhe mexer nos dados.

### 3.6 Como o encapsulamento protege o código de alterações futuras

Esta é a grande vantagem prática. Vamos imaginar mudanças que o professor (ou um
"cliente") podia pedir, e comparar o que acontece nas duas versões.

**Alteração A — "O metabolismo mudou: o peixe só aproveita 80 % da comida."**

- Versão certa: muda-se **uma linha**, em `Peixe::Alimentar`:
  `PESO += cal * 0.8;`. O `Aquario` não é tocado. A regra de saída também continua
  certa, porque `EstouGordo()` é chamado depois, já com o peso novo.
- Versão marioneta: é preciso procurar **todos** os sítios onde alguém faz `PESO += ...`
  (o aquário, o `main`, outras classes...). Se faltar um, há um bug.

**Alteração B — "Várias espécies: a sardinha sai a partir de 50 g, o tubarão nunca sai,
o peixe-palhaço sai a partir de 30 g."** (isto é literalmente o que o enunciado sugere:
"a decisão variava em função da espécie")

- Versão certa: cada espécie redefine `EstouGordo()` (herança + polimorfismo, secção 6).
  O código do aquário **continua igual**: só chama `(*it)->Alimentar(g)`. Pode até aparecer
  uma espécie nova daqui a um ano e o aquário não muda **uma linha**.
- Versão marioneta: o aquário fica com um monstro de `if`s:

  ```cpp
  if (p->ESPECIE == "Sardinha" && p->PESO > 50) ...
  else if (p->ESPECIE == "Palhaco" && p->PESO > 30) ...
  else if (p->ESPECIE == "Tubarao") ...
  ```

  e cada espécie nova obriga a editar o aquário. É isto que o enunciado quer dizer com
  "é considerada um erro (e dá mais trabalho a fazer)".

**Alteração C — "O peso passa a ser guardado em quilogramas (`double`) em vez de gramas."**

- Versão certa: muda-se o tipo de `PESO` e a conversão dentro de `Peixe::Alimentar`
  (`PESO += cal / 1000.0;`). Quem chama `Alimentar(30)` continua a passar gramas. Ninguém
  fora do `Peixe` dá conta.
- Versão marioneta: todo o código que fazia `p->PESO += 30` passa a estar errado
  (somaria 30 kg!) **sem o compilador avisar**.

**Alteração D — "O aquário passa a guardar os peixes num `vector` ou num `map` por número de
série, para procurar mais depressa."**

- Versão certa: muda-se só `Aquario.h`/`Aquario.cpp`. O peixe nunca soube que era uma
  `list`: só chama `Ptr_Aquario->Preciso_de_Comer(this)` e
  `Ptr_Aquario->QueroSair(this)`. **O enunciado diz exatamente isto**: "o peixe não faz ideia
  como é que o aquário guarda os peixes internamente".
- Versão em que o peixe "se tira a si próprio" (`Ptr_Aquario->LP.remove(this)`): o
  código do peixe partia, porque dependia de `LP` ser uma `list`.

**Conclusão**: com encapsulamento, cada alteração fica **contida dentro de uma única
classe**. Sem ele, cada alteração "espalha-se" pelo programa todo.

---

## 4. Inclusão circular e a solução com ponteiro

### 4.1 Porque é que o problema aparece

A relação é bidirecional:

- `Aquario` precisa de conhecer `Peixe` (tem uma `list<Peixe *>`, chama
  `P->Atribuir_Aquario`, `(*it)->Run()`, ...).
- `Peixe` precisa de conhecer `Aquario` (tem um `Aquario *Ptr_Aquario` e chama
  `Ptr_Aquario->Preciso_de_Comer(this)`).

A tentação natural é:

```cpp
// Aquario.h                         // Peixe.h
#include "Peixe.h"                   #include "Aquario.h"
class Aquario { ... Peixe ... };     class Peixe { ... Aquario ... };
```

O `A.h` inclui `B.h` e o `B.h` inclui `A.h`. Isto é a **inclusão circular**.

### 4.2 Porque é que não funciona — passo a passo

Lembra-te (0.9): `#include` é **copy-paste** e o compilador lê **de cima para baixo**.

**Caso 1: sem include guards.** Ao compilar `main.cpp` que tem `#include "Aquario.h"`:

```
main.cpp
 └─ cola Aquario.h
     └─ cola Peixe.h
         └─ cola Aquario.h  (2ª vez)
             └─ cola Peixe.h  (2ª vez)
                 └─ cola Aquario.h  (3ª vez)
                     └─ ... até ao infinito
```

O pré-processador entra num ciclo infinito (na prática, pára com um erro de "include
nested too deeply"). **Não funciona.**

**Caso 2: com include guards** (`#ifndef/#define/#endif`, ou `#pragma once`). Ao compilar
`main.cpp`:

1. `#include "Aquario.h"` → `AQUARIO_H` não está definido → define-o e começa a colar
   `Aquario.h`.
2. A primeira coisa em `Aquario.h` é `#include "Peixe.h"` → `PEIXE_H` não está definido →
   define-o e começa a colar `Peixe.h`.
3. A primeira coisa em `Peixe.h` é `#include "Aquario.h"` → **`AQUARIO_H` já está
   definido** (passo 1) → o conteúdo é **ignorado**.
4. Continua `Peixe.h`: `class Peixe { ... Aquario *Ptr_Aquario; ... };`
   → **ERRO**: `'Aquario' does not name a type`. O compilador ainda não viu a classe
   `Aquario` — ela só seria colada **depois** de `Peixe.h` terminar (o resto do passo 1).

O ficheiro que fica "por dentro" não compila porque lhe falta o outro. **Não funciona.**

Os include guards impedem o ciclo infinito, mas **não resolvem a dependência** — é
impossível que `Aquario` apareça antes de `Peixe` **e** `Peixe` apareça antes de `Aquario`.

### 4.3 A solução: declaração antecipada (*forward declaration*) + ponteiro

Um dos lados **desiste de incluir o outro** e apenas **anuncia que a classe existe**. Na
resolução, é o `Peixe.h`:

```cpp
// Peixe.h
#ifndef PEIXE_H
#define PEIXE_H

#include <iostream>
using namespace std;
#include <string>
#include "..//Uteis.h"

class Aquario;          // <-- DECLARAÇÃO ANTECIPADA: "existe uma classe chamada Aquario,
                        //     confia em mim, os pormenores vêm noutro sítio"
class Peixe
{
    ...
    Aquario *Ptr_Aquario;   // <-- só um PONTEIRO para Aquario
    ...
};
#endif
```

E o `Aquario.h` continua a fazer `#include "Peixe.h"` normalmente.

**Porque é que basta dizer `class Aquario;`?**

Para compilar `class Peixe { ... }`, o compilador precisa de saber **quantos bytes ocupa
um `Peixe`**, para depois conseguir criar peixes. Para isso precisa do tamanho de cada
atributo. O atributo é `Aquario *` — **um ponteiro**, e (ver 0.4) **todos os ponteiros
têm o mesmo tamanho**, seja para que tipo for. Logo o compilador sabe o tamanho do
`Peixe` **sem saber nada** sobre o interior do `Aquario`. Só precisa de saber que
`Aquario` é o nome de uma classe — e é isso que `class Aquario;` lhe diz.

Se em vez de ponteiro fosse um **objeto**, não funcionava:

```cpp
class Aquario;
class Peixe {
    Aquario A;   // ERRO: "field has incomplete type 'Aquario'"
};               // para saber o tamanho do Peixe, era preciso o tamanho do Aquario
```

É o que o enunciado diz: "No caso de apenas precisar de um ponteiro, basta mencionar que a
classe existe (se precisar de um objeto mesmo, então terá que incluir mesmo o .h todo)."
(E aqui nem faria sentido: o peixe guardar um *objeto* aquário seria cada peixe ter o seu
próprio aquário inteiro lá dentro — não é o que queremos. Queremos que o peixe **aponte**
para o aquário onde está.)

### 4.4 O que se pode e não pode fazer com uma classe só "anunciada"

Depois de `class Aquario;`, o tipo `Aquario` é um **tipo incompleto**: sabe-se o nome, mas
não o conteúdo.

| Pode | Não pode |
|---|---|
| Declarar ponteiros `Aquario *` (e referências `Aquario &`) | Declarar um atributo/variável `Aquario` (objeto) |
| Receber/devolver `Aquario *` em métodos: `void Atribuir_Aquario(Aquario *A) { Ptr_Aquario = A; }` (só copia um endereço) | Chamar métodos: `Ptr_Aquario->Preciso_de_Comer(this)` |
| Comparar com `NULL`, atribuir ponteiros | Fazer `new Aquario`, `sizeof(Aquario)`, herdar de `Aquario` |

Repara que `Atribuir_Aquario` está **escrito dentro do `.h`** e compila: só copia um
endereço, não precisa de saber o que há dentro do aquário.

### 4.5 E onde se chama `Ptr_Aquario->Preciso_de_Comer(...)`? No `.cpp`!

Para **chamar um método** do aquário, o compilador tem de conhecer a classe completa
(tem de saber que o método existe, que parâmetros recebe, o que devolve). Por isso
`Peixe.cpp` faz:

```cpp
// Peixe.cpp
#include "Peixe.h"
#include "Aquario.h"    // <-- aqui sim, inclui a classe completa
```

E isto **não é circular**, porque um `.cpp` nunca é incluído por ninguém. Ao compilar
`Peixe.cpp`:

1. Cola `Peixe.h` → aparece `class Aquario;` e `class Peixe {...}` (completa).
2. Cola `Aquario.h` → este tenta incluir `Peixe.h`, mas `PEIXE_H` já está definido →
   ignorado (não faz mal: o `Peixe` já é conhecido) → aparece `class Aquario {...}`
   (completa).
3. Agora ambas as classes estão completas e o código de `EstouComFome()` pode chamar
   `Ptr_Aquario->Preciso_de_Comer(this)` à vontade.

E ao compilar `main.cpp` ou `Aquario.cpp` (que incluem `Aquario.h`):

1. `Aquario.h` cola `Peixe.h` → `class Aquario;` + `class Peixe {...}` completa.
2. Volta a `Aquario.h` → `class Aquario { list<Peixe *> LP; ... }` → `Peixe` já é
   conhecido. ✔

A cadeia de inclusões fica **linear**, sem ciclo:

```
  Aquario.h ──inclui──▶ Peixe.h ──(só anuncia "class Aquario;")
      ▲                    ▲
      │                    │
  Aquario.cpp          Peixe.cpp ──inclui──▶ Aquario.h   (os .cpp podem incluir tudo)
  main.cpp             Tubarao.h ──inclui──▶ Peixe.h
```

### 4.6 Regra prática para levar

> **No `.h`, inclui o mínimo possível. Se só precisas de um ponteiro/referência para uma
> classe, usa declaração antecipada (`class X;`). Inclui o `.h` completo no `.cpp`, onde
> chamas os métodos.**

Esta regra até tem uma vantagem extra: quem inclui `Peixe.h` não arrasta consigo todo o
`Aquario.h`, por isso a compilação é mais rápida e as alterações a `Aquario.h` obrigam a
recompilar menos ficheiros.

**Qual dos dois lados deve usar a declaração antecipada?** Em teoria qualquer um que só
use ponteiros. Aqui o `Aquario.h` usa `list<Peixe *>` (também só ponteiros), por isso
também poderia ser ele. A escolha natural é a do professor: o aquário é o "contentor" e é
mais lógico que inclua quem contém; o peixe só precisa de uma "morada" para onde mandar
pedidos.

---

## 5. Herança (`Tubarao : public Peixe`)

### 5.1 O que é

**Herança** permite criar uma classe nova a partir de uma existente, **reaproveitando**
tudo o que ela tem e **acrescentando** ou **especializando** comportamento. Exprime a
relação **"é um"**: um tubarão **é um** peixe.

- `Peixe` é a **classe base** (ou "mãe", "superclasse").
- `Tubarao` é a **classe derivada** (ou "filha", "subclasse").

```cpp
// Tubarao.h
#include "Peixe.h"          // para herdar, é preciso a classe COMPLETA (não basta "class Peixe;")

class Tubarao : public Peixe     // "Tubarao é um Peixe"
{
    int GRAU_PERIGO;             // atributo que só os tubarões têm
public:
    Tubarao();
    Tubarao(int _id, string _especie, int PESO, int gp = 100);
    virtual ~Tubarao();
};
```

Um `Tubarao` tem **automaticamente** tudo o que um `Peixe` tem: `ID`, `ESPECIE`, `PESO`,
`Ptr_Aquario`, `Ja_Avisei_Tenho_Fome`, e os métodos `Show()`, `Run()`, `Alimentar()`,
`Atribuir_Aquario()`... **sem reescrever nada**. E ainda tem `GRAU_PERIGO`.

Em memória, um objeto `Tubarao` é um "peixe com uma extensão":

```
  objeto Tubarao
  ┌───────────────────────────┐
  │  parte Peixe              │  ← ID, ESPECIE, PESO, Ptr_Aquario, Ja_Avisei...
  ├───────────────────────────┤
  │  parte Tubarao            │  ← GRAU_PERIGO
  └───────────────────────────┘
```

### 5.2 O `public` em `: public Peixe`

Significa que **o que é público no `Peixe` continua público no `Tubarao`**. Ou seja, quem
tem um tubarão pode chamar `t.Alimentar(30)` ou `t.Show()`, e — mais importante — um
`Tubarao` pode ser usado **em qualquer sítio onde se espera um `Peixe`**:

```cpp
Peixe *P = new Tubarao(20, "Branco", 300);   // válido! um Tubarao* "é um" Peixe*
AQ.Add(P);                                   // o aquário aceita-o como qualquer peixe
```

É por isto que o aquário **não precisa de saber que os tubarões existem**: `Aquario.h` não
inclui `Tubarao.h`, e mesmo assim pode guardar tubarões na sua `list<Peixe *>`.

### 5.3 O que a classe filha NÃO pode fazer: aceder ao `private` da mãe

Os atributos `PESO`, `ID`, etc. são `private` em `Peixe`. Isto quer dizer que **nem o
`Tubarao` lhes pode tocar diretamente**:

```cpp
void Tubarao::Algo()
{
    PESO += 10;   // ERRO: 'int Peixe::PESO' is private
}
```

Isto é encapsulamento a funcionar também entre mãe e filha: o `Tubarao` herda os dados
(eles existem dentro do objeto), mas só os manipula através dos métodos do `Peixe`
(`Alimentar`, etc.). Se o `Peixe` quiser deixar as filhas aceder a algo, declara-o
**`protected`**:

| Acesso | A própria classe | Classes derivadas | Resto do mundo |
|---|---|---|---|
| `private` | ✔ | ✘ | ✘ |
| `protected` | ✔ | ✔ | ✘ |
| `public` | ✔ | ✔ | ✔ |

Boa prática: manter os **atributos `private`** e, se as filhas precisarem de ler algo,
oferecer um **método** de leitura em vez de pôr o atributo `protected`. Assim continua a
ser só o `Peixe` a decidir como o peso muda. No projeto, esse método é `GetPeso()`. Está
`public`, porque o aquário também o usa para mostrar o peso de quem sai; por isso as
filhas também o podem usar.

No projeto há um exemplo de membro `protected`: `virtual bool EstouGordo();`. O resto do
mundo não o pode chamar, mas as classes derivadas podem redefini-lo (secção 6.5).

### 5.4 Construtores na herança: a lista de inicialização

Quando se cria um `Tubarao`, **primeiro** tem de ser construída a parte `Peixe`, e só
**depois** a parte `Tubarao`. Como os atributos do `Peixe` são `private`, o construtor do
`Tubarao` nem os conseguiria inicializar — tem de **pedir ao construtor do `Peixe`** que o
faça. Isso escreve-se na **lista de inicialização** (depois dos `:` e antes do `{`):

```cpp
Tubarao::Tubarao(int _id, string _especie, int PESO, int gp)
: Peixe(_id, _especie, PESO)      // 1º: constrói a parte Peixe com estes valores
{
    GRAU_PERIGO = gp;             // 2º: inicializa a parte própria do Tubarao
}

Tubarao::Tubarao()
: Peixe(-1, "??", 123)            // tubarão "sem dados": valores de reserva
{
    GRAU_PERIGO = 0;
}
```

Se não se escrevesse `: Peixe(...)`, o C++ tentaria chamar o construtor **sem argumentos**
`Peixe()`. Esse está declarado em `Peixe.h` mas **não está implementado** em `Peixe.cpp`,
por isso daria erro de *linker* (`undefined reference to Peixe::Peixe()`). Chamar
explicitamente o construtor da base evita esse problema (e é a forma correta).

### 5.5 Destrutores na herança

A ordem é a inversa da construção: ao destruir um `Tubarao`, corre **primeiro**
`~Tubarao()` e **depois** `~Peixe()` (automaticamente). Faz sentido: desmonta-se primeiro a
"extensão" e depois a "base".

---

## 6. Polimorfismo (`virtual`)

### 6.1 O problema que o polimorfismo resolve

O aquário tem uma lista de `Peixe*`. Nessa lista podem estar sardinhas, tubarões,
peixes-palhaço... E no ciclo:

```cpp
for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
    (*it)->Run();
```

queremos que **cada peixe faça o seu próprio `Run()`**: a sardinha faz o `Run()` de
sardinha, o tubarão o `Run()` de tubarão — **sem o aquário ter de perguntar "és de que
espécie?"**. "Polimorfismo" significa literalmente "muitas formas": a **mesma chamada**
(`(*it)->Run()`) tem **comportamentos diferentes** consoante o tipo real do objeto.

### 6.2 Sem `virtual` (ligação estática) vs com `virtual` (ligação dinâmica)

Imagina que o `Tubarao` redefine `Show()`:

```cpp
class Tubarao : public Peixe {
    ...
    void Show();       // versão própria
};
void Tubarao::Show() { cout << "TUBARAO! perigo=" << GRAU_PERIGO << endl; }
```

e temos:

```cpp
Peixe *P = new Tubarao(20, "Branco", 300);
P->Show();
```

- **Se `Show()` NÃO fosse `virtual` em `Peixe`**: o compilador olha para o **tipo do
  ponteiro** (`Peixe*`) e chama `Peixe::Show()`. Aparece "Peixe: (20,Branco,300)". O
  tubarão é tratado como peixe genérico. Chama-se **ligação estática** — decidida na
  compilação.
- **Como `Show()` É `virtual` em `Peixe`** (é o caso na resolução: `virtual void Show();`):
  o programa olha, **durante a execução**, para o **tipo real do objeto apontado**
  (`Tubarao`) e chama `Tubarao::Show()`. Aparece "TUBARAO! perigo=100". Chama-se **ligação
  dinâmica**.

Basta escrever `virtual` **na classe base**; nas derivadas o método redefinido é
automaticamente virtual (em C++ moderno costuma escrever-se `override` na filha para o
compilador confirmar que a assinatura bate certo: `void Show() override;`).

**Como funciona por dentro (simplificado):** cada classe com métodos `virtual` tem uma
tabela escondida (*vtable*) com os endereços das versões corretas dos seus métodos, e cada
objeto guarda um ponteiro escondido para a tabela da **sua** classe. Quando se faz
`P->Show()`, o programa vai ao objeto, segue esse ponteiro escondido e chama a função que
lá estiver. Por isso a decisão é feita em tempo de execução.

### 6.3 Porque é que só funciona através de ponteiros (ou referências)

```cpp
Tubarao t(20, "Branco", 300);
Peixe p = t;      // COPIA só a parte Peixe para p ("slicing") — p é um Peixe, ponto final
p.Show();         // chama Peixe::Show, sempre

Peixe *pp = &t;   // pp aponta para o tubarão verdadeiro
pp->Show();       // chama Tubarao::Show  ✔
```

Por isso o aquário guarda `list<Peixe *>` e não `list<Peixe>` (secção 2.3, razão 2).
É a mesma razão pela qual `F1_Escrita` recebe `ostream &` (referência) e não `ostream`.

### 6.4 O destrutor `virtual` — porque é obrigatório aqui

Em `Peixe.h`: `virtual ~Peixe();`. Isto é essencial sempre que se apaga um objeto derivado
através de um ponteiro para a base:

```cpp
Peixe *P = new Tubarao(...);
delete P;
```

- Com `~Peixe()` **virtual**: chama `~Tubarao()` e depois `~Peixe()`. ✔
- Sem `virtual`: chamaria **só** `~Peixe()` — a parte `Tubarao` nunca seria destruída
  corretamente (comportamento indefinido; se o tubarão tivesse memória própria, haveria
  fugas).

Como o aquário vai fazer `delete` de peixes que podem ser de qualquer espécie, **o
destrutor da base tem de ser virtual**. Regra: *se uma classe tem algum método `virtual`,
o destrutor também deve ser `virtual`.*

### 6.5 O polimorfismo no projeto

`Peixe` tem três métodos `virtual` (além do destrutor):

| Método | Onde é chamado | Redefinido no `Tubarao`? |
|---|---|---|
| `virtual void Show()` | `Peixe::Run()` | **Sim** |
| `virtual void Run()` | `Aquario::Run()`, para cada peixe | Não (usa a versão de `Peixe`) |
| `virtual bool EstouGordo()` (`protected`) | `Peixe::Alimentar()` | Não, mas está preparado para isso |

**`Show()` já é redefinido pelo `Tubarao`:**

```cpp
void Tubarao::Show()
{
    cout << "Grau de perigo do Tubarao: " << GRAU_PERIGO;
    Peixe::Show();                    // chama explicitamente a versão da classe base
}
```

`Peixe::Show()` com o prefixo `Peixe::` chama **a versão da classe base**, e não a do
próprio tubarão (que seria uma chamada recursiva infinita). O tubarão acrescenta a sua
parte e reaproveita a do peixe. Há um pormenor de apresentação: falta um separador entre
o número e o "Peixe:", por isso sai `Grau de perigo do Tubarao: 100Peixe: (...)`.

`Show()` é chamado dentro de `Peixe::Run()`. Mesmo assim, para um tubarão é chamada a
versão do tubarão: dentro de um método, `Show()` é na verdade `this->Show()`, e o `this`
aponta para um `Tubarao`.

**`EstouGordo()` é o ponto onde "a decisão varia em função da espécie".** O enunciado diz
que a decisão de sair pode variar com a espécie. Como `EstouGordo()` é `virtual` e é
chamado em `Peixe::Alimentar`, uma espécie só precisa de o redefinir:

```cpp
// Tubarao.h
class Tubarao : public Peixe
{
    int GRAU_PERIGO;
    protected:
        virtual bool EstouGordo();    // critério próprio do tubarão
    public:
        ...
};

// Tubarao.cpp
bool Tubarao::EstouGordo()
{
    return GetPeso() > 500;           // um tubarão só quer sair muito mais tarde
}                                     // (ou "return false;": nunca sai)
```

O tubarão não acede a `PESO` diretamente, porque é `private` em `Peixe` (secção 5.3). Usa
`GetPeso()`.

E no `main`:

```cpp
AQ.Add(new Peixe(10, "Sardinha", 30));
AQ.Add(new Tubarao(99, "Branco", 300, 80));
AQ.Run();      // o aquário chama (*it)->Run() sem saber quem é quem
```

O `Aquario.cpp` **não muda uma linha**. Ao ser alimentado, o tubarão usa o seu próprio
critério de saída, e ao correr mostra-se à sua maneira.
Isto é o que o enunciado pede quando diz "considere que pode haver várias espécies de peixe
e de alguma forma a decisão variava em função da espécie" — a forma certa de "variar em
função da espécie" é **herança + métodos virtuais**, não um `if (especie == ...)` no
aquário.

### 6.6 Resumo herança vs polimorfismo

- **Herança**: *reaproveitar* — o `Tubarao` recebe tudo do `Peixe` sem copiar código.
- **Polimorfismo**: *tratar de forma uniforme* — o aquário trata todos como `Peixe*`, e
  cada um responde à sua maneira.
- Juntos com o **encapsulamento** (cada um decide sobre si próprio), permitem acrescentar
  espécies novas **sem tocar** no aquário.

---

## 7. Execução passo-a-passo do programa

### 7.1 O `main`

```cpp
srand(time(NULL));                       // baralhar os aleatórios
Aquario AQ;                              // aquário vazio (as 3 listas começam vazias)

int NP = Uteis::Aleatorio(2, 6);         // número de peixes: entre 2 e 6
for (int i = 0; i < NP; i++)
{
    Peixe *P = new Peixe(10+i, "Sardinha", Uteis::Aleatorio(20, 50));
    AQ.Add(P);                           // o aquário guarda-o e diz-lhe "és meu"
}
AQ.Run();                                // arranca a simulação (ciclo infinito)
```

Depois de `Add`, para cada peixe existem as duas ligações:

```
   AQ.LP:  [ ●───────────▶ Peixe 12 ]  ── Ptr_Aquario ──▶ AQ
           [ ●───────────▶ Peixe 11 ]  ── Ptr_Aquario ──▶ AQ
           [ ●───────────▶ Peixe 10 ]  ── Ptr_Aquario ──▶ AQ
```

(Os peixes ficam por ordem inversa — 12, 11, 10 — porque `Add` usa `push_front`.)

### 7.2 Um ciclo de `Aquario::Run()`

```cpp
while (true)
{
    for (cada peixe) (*it)->Run();        // 1. cada peixe "vive" um instante
    cout << "Esfomeados: " << LP_Fome.size() << endl;
    cout << "------------------------------" << endl;
    if (LP_Fome.size() >= 2)              // 2. se pelo menos 2 pediram comida...
    {
        Alimentar();                      //    ...alimenta toda a gente
        RemoverParaAssar();               //    ...e trata dos pedidos de saída
    }
    Uteis::wait_segundos(2);              // 3. espera 2 s e repete
}
```

E o `Run()` de cada peixe:

```cpp
void Peixe::Run()
{
    PESO -= Uteis::Aleatorio(1, 5);       // gasta energia: perde 1 a 5 g
    if (PESO < 20)                        // decisão do PEIXE: "estou com fome?"
    {
        cout << "Preciso de Comer: " << ID << endl;
        EstouComFome();                   // avisa o aquário (uma só vez)
    }
    Show();
}
```

### 7.3 Output real (executei o programa) e o que cada linha significa

**Versão original do professor**, antes de existir a saída do peixe (por isso ainda sem
a cor):

```
Aquario
Preciso de Comer: 12           ← peixe 12 desceu abaixo de 20 g → chama Ptr_Aquario->Preciso_de_Comer(this)
Peixe: (12,Sardinha,19)
Preciso de Comer: 11           ← peixe 11 idem → LP_Fome passa a ter 2 peixes
Peixe: (11,Sardinha,15)
Peixe: (10,Sardinha,21)        ← peixe 10 ainda não tem fome
Esfomeados: 2                  ← LP_Fome.size() == 2  → o aquário chama Alimentar()
------------------------------
Alimentacao a DEcorrer.....    ← aquário → cada peixe: Alimentar(20..50 g)
                                 cada peixe soma ao seu PESO e põe Ja_Avisei_Tenho_Fome = false
                                 o aquário limpa LP_Fome
Peixe: (12,Sardinha,53)        ← ciclo seguinte: todos voltaram a ter peso (já com o -1..5 do Run)
Peixe: (11,Sardinha,48)
Peixe: (10,Sardinha,56)
Esfomeados: 0
------------------------------
Peixe: (12,Sardinha,49)        ← e vão emagrecendo outra vez...
...
```

O fluxo completo de uma "fome":

```
 Peixe::Run ─(PESO<20)─▶ Peixe::EstouComFome ─(se ainda não avisou)─▶ Aquario::Preciso_de_Comer(this)
                                                                          │ LP_Fome.push_front(P)
                                                                          │ return true
                                       Ja_Avisei_Tenho_Fome = true ◀──────┘

 Aquario::Run ─(LP_Fome ≥ 2)─▶ Aquario::Alimentar ─(para cada peixe)─▶ Peixe::Alimentar(cal)
                                                                          │ PESO += cal
                                                                          │ Ja_Avisei_Tenho_Fome = false
                                       LP_Fome.clear()
```

Aqui veem-se os dois sentidos da ficha: **aquário → peixe** (`Alimentar`) e
**peixe → aquário** (`Preciso_de_Comer`).

### 7.4 Output real da versão atual, com peixes a sair

Com 5 peixes. No último ciclo antes de comer, os peixes 13 e 11 tinham fome e os pesos
eram 14: 23 g, 13: 17 g, 12: 32 g, 11: 19 g e 10: 29 g:

```
Esfomeados: 2                                   ← 13 e 11 pediram comida
------------------------------
Alimentacao a DEcorrer.....                     ← Aquario::Alimentar: cada peixe recebe 20..50 g
                                                  14: 23+35=58 → EstouGordo() → QueroSair(this)
                                                  12: 32+48=80 → QueroSair(this)
                                                  10: 29+39=68 → QueroSair(this)
                                                  13 e 11 ficam abaixo de 50 g → não pedem nada
O peixe 14, com o peso 58(g), saiu do aquario!  ← Aquario::RemoverParaAssar, já fora do ciclo
O peixe 12, com o peso 80(g), saiu do aquario!
O peixe 10, com o peso 68(g), saiu do aquario!
Peixe: (13,Sardinha,36,Cinzento)                ← ciclo seguinte: só restam 2 peixes
Peixe: (11,Sardinha,43,Cinzento)
Esfomeados: 0
```

O fluxo completo de uma "saída":

```
 Aquario::Alimentar ─(para cada peixe)─▶ Peixe::Alimentar(cal)
                                            │ PESO += cal
                                            │ se EstouGordo() e !Ja_Avisei_Quero_Sair:
                                            │     Ptr_Aquario->QueroSair(this) ──▶ LP_Assar.push_back(P)
                                            │     Ja_Avisei_Quero_Sair = true  ◀── return true
 (o ciclo de Alimentar termina; a LP não foi tocada)

 Aquario::Run ──▶ Aquario::RemoverParaAssar()
                    para cada P em LP_Assar:  mostra ID e peso, LP.remove(P), delete P
                    LP_Assar.clear()
```

**Um efeito na simulação.** O aquário só alimenta quando há **2 ou mais** peixes com fome.
Numa das execuções saíram 3 de 4 peixes e ficou só um. Esse peixe pede comida, mas
`LP_Fome.size()` nunca chega a 2, por isso nunca é alimentado. O peso vai descendo até
valores sem sentido (vi `-2483627` ao fim de muitos ciclos). Não é um erro da saída: as
regras da simulação do professor (alimentar com 20 a 50 g quando há 2 com fome) e a regra
dos 50 g do enunciado não foram pensadas em conjunto. Pode ajustar-se, por exemplo
alimentando quando `LP_Fome.size() >= 1` ou quando metade dos peixes tem fome.

---

## 8. A saída do peixe e a remoção: a solução implementada

Esta secção explica **a extensão feita ao projeto do professor**: o peixe que passa dos
50 g pede para sair e o aquário retira-o, e é possível remover um peixe pelo número de
série. Todo o código mostrado é o código atual do projeto, com os nomes que lá estão.

### 8.1 O que foi acrescentado, ficheiro a ficheiro

| Ficheiro | Acrescento | Para quê |
|---|---|---|
| `Peixe.h` | `string COR;` + parâmetro `string _cor = "Cinzento"` no construtor | A cor do enunciado (opcional, "cinzento" por omissão) |
| `Peixe.h` | `bool Ja_Avisei_Quero_Sair;` (`private`) | Lembrar se o peixe já pediu para sair, para não pedir duas vezes |
| `Peixe.h` | `virtual bool EstouGordo();` (`protected`) | O **critério** de saída: decisão do peixe, que pode variar com a espécie |
| `Peixe.h` | `int GetID()` (`public`) | O aquário precisa do número de série para procurar e para mostrar |
| `Peixe.h` | `int GetPeso()` (`public`) | O aquário mostra o peso de quem sai; as espécies derivadas podem lê-lo |
| `Peixe.cpp` | Em `Alimentar`: `if (EstouGordo() && !Ja_Avisei_Quero_Sair) ...QueroSair(this)` | O **momento** em que o peixe avalia se quer sair: logo depois de comer |
| `Aquario.h/.cpp` | `bool QueroSair(Peixe *P)` | Receber o pedido do peixe e guardá-lo na `LP_Assar` |
| `Aquario.h/.cpp` | `bool RemoverParaAssar()` | Tratar os pedidos guardados: tirar da `LP` e destruir |
| `Aquario.h/.cpp` | `Peixe *Pertence(int id)` | Procurar um peixe pelo número de série |
| `Aquario.h/.cpp` | `bool RemoverID(int id)` | Remover um peixe escolhido por número de série (pedido de fora) |
| `Aquario.cpp` | Em `Run`: `RemoverParaAssar();` logo a seguir a `Alimentar();` | Momento em que os pedidos de saída são tratados |
| `Tubarao.h/.cpp` | `virtual void Show();` redefinido | Exemplo de polimorfismo (secção 6.5) |

### 8.2 Visão geral: os "atores" da remoção

Antes do passo-a-passo, o papel de cada peça:

```
 PEIXE (decide)                                   AQUÁRIO (executa)
 ───────────────────────────────                  ──────────────────────────────────────
 PESO                 o estado que muda           LP        todos os peixes
 EstouGordo()         "quero sair?"               LP_Fome   pedidos de comida
 Ja_Avisei_Quero_Sair "já pedi?"                  LP_Assar  pedidos de saída (fila de espera)
 Ptr_Aquario          "a quem peço"               QueroSair(P)        recebe o pedido
 Alimentar(cal)       onde tudo começa            RemoverParaAssar()  trata os pedidos
 GetID(), GetPeso()   leitura para o aquário      Pertence(id)        procura por número
                                                  RemoverID(id)       remoção pedida de fora
```

A regra que organiza tudo: **o peixe decide, o aquário executa**. O peixe nunca se tira
da lista sozinho, e o aquário nunca decide que um peixe "está gordo demais".

### 8.3 Passo 1: o peixe come e avalia-se (`Peixe::Alimentar`)

Tudo começa quando o aquário alimenta os peixes:

```cpp
void Aquario::Alimentar()
{
    cout << "Alimentacao a DEcorrer....." << endl;
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
    {
        (*it)->Alimentar(Uteis::Aleatorio(20, 50));   // "toma, come, faz o que entenderes"
    }
    LP_Fome.clear();
    //RemoverParaAssar();
}
```

Para cada peixe, é chamado:

```cpp
void Peixe::Alimentar(int cal)
{
    PESO += cal;                                  // (a) a comida vira peso
    Ja_Avisei_Tenho_Fome = false;                 // (b) já não tem fome
    if( EstouGordo() && !Ja_Avisei_Quero_Sair)    // (c) quero sair? e ainda não pedi?
    {
        Ja_Avisei_Quero_Sair = Ptr_Aquario->QueroSair(this);   // (d) peço ao aquário
    }
}

bool Peixe::EstouGordo()
{
    return PESO > 50 ;
}
```

Linha a linha:

- **(a)** O peixe soma a comida ao seu peso. É ele que decide o que a comida lhe faz
  (secção 3).
- **(b)** Acabou de comer, por isso deixa de estar "à espera de comida". Se voltar a ter
  fome mais tarde, poderá pedir outra vez.
- **(c)** Duas condições, e as duas têm de ser verdadeiras:
  - `EstouGordo()`: **o critério**. Neste momento é "mais de 50 g". Como é `virtual`,
    se o peixe for de uma espécie que redefine `EstouGordo()` (por exemplo um
    `Tubarao`), é chamada a versão dessa espécie (secção 6.5). O aquário nunca vê este
    critério.
  - `!Ja_Avisei_Quero_Sair`: **ainda não pedi**. Evita pedidos repetidos (passo 8.6).
- **(d)** O peixe **pede** ao aquário, enviando o seu próprio endereço (`this`, secção
  0.6). O que o aquário devolve (`true` = "pedido aceite") fica guardado na flag. A partir
  daqui `Ja_Avisei_Quero_Sair` é `true` e o peixe não volta a pedir.

**Porque é que a avaliação está em `Alimentar` e não em `Run`?** A fome é verificada em
`Peixe::Run()` porque é aí que o peso **desce**. O excesso de peso só pode acontecer onde o
peso **sobe**, e isso só acontece em `Alimentar`. Cada regra é verificada **onde muda o
estado que a provoca**. É também o momento que o enunciado descreve: "Ser alimentado com
(...) alimento que é somada ao seu peso. Se exceder 50g e estiver num aquário, (...) foge".
O exemplo errado do enunciado, "toma isto, come. Estás com mais de 50 gramas? Então sai",
também coloca a verificação logo a seguir a comer. O erro que aponta é **quem** verifica,
não **quando**.

As duas regras ("comida soma ao peso" e "acima de 50 g quer sair") continuam separadas:
a primeira está na linha `PESO += cal`, a segunda em `EstouGordo()`. O `Alimentar` só
liga uma à outra pela ordem certa.

### 8.4 Passo 2: o aquário regista o pedido (`Aquario::QueroSair`)

```cpp
bool Aquario:: QueroSair ( Peixe * P)
{
    LP_Assar.push_back(P);
    return true;
}
```

O aquário **não remove o peixe aqui**. Só o põe numa fila de espera, a `LP_Assar`, e
responde `true`. É o mesmo padrão do professor para a fome (`Preciso_de_Comer` guarda em
`LP_Fome`).

**Porque é que não pode remover logo?** Repara de onde vem esta chamada:

```
Aquario::Alimentar            ← está a meio do for, com o iterador "it" a apontar para este peixe
  └─ Peixe::Alimentar         ← está a meio da sua execução, dentro do objeto peixe
       └─ Aquario::QueroSair  ← estamos aqui
```

Se `QueroSair` fizesse `LP.remove(P); delete P;`, aconteciam dois problemas graves:

1. **Iterador inválido.** O `for` de `Aquario::Alimentar` tem um iterador `it` a apontar
   para o elemento da lista que acabou de ser apagado. Quando o ciclo fizer `++it`, está a
   avançar a partir de um elemento que já não existe: comportamento indefinido (pode
   saltar peixes, repetir peixes ou rebentar).
2. **Objeto destruído a meio de um método seu.** Quando `QueroSair` terminasse, a execução
   voltava para dentro de `Peixe::Alimentar`, que tenta escrever
   `Ja_Avisei_Quero_Sair = ...` num peixe que já foi apagado com `delete`. Também é
   comportamento indefinido.

Guardar o pedido e tratá-lo **depois** de o ciclo terminar resolve os dois problemas: no
momento em que se remove, já ninguém está a percorrer a `LP` nem a executar um método
desse peixe.

Há também uma razão de encapsulamento: o peixe não sabe **como** nem **quando** o aquário
o vai tirar. Pode ser já, daqui a pouco, ou o aquário pode até recusar (devolvendo
`false`). Essa decisão é do aquário.

### 8.5 Passo 3: o aquário trata os pedidos (`Aquario::RemoverParaAssar`)

Depois de `Alimentar` terminar, `Aquario::Run` chama:

```cpp
if (LP_Fome.size() >= 2)
{
    Alimentar();
    RemoverParaAssar();
}
```

```cpp
bool Aquario::RemoverParaAssar() //precorrer lista de Peixes
{
    for(list<Peixe*>::iterator it = LP_Assar.begin(); it != LP_Assar.end(); ++it )
    {
        cout << "O peixe "<< (*it)->GetID() << ", com o peso " << (*it)->GetPeso() <<  "(g), saiu do aquario!" << endl;
        LP.remove(*it);
        delete *it;
    }
    LP_Assar.clear(); // no fim de remover da lista dos Peixes, limpar lista LP_ASSAR

    return true;
}
```

Para cada peixe que pediu para sair:

1. **Mostra o ID e o peso** com `GetID()` e `GetPeso()`. Tem de ser **antes** do `delete`:
   depois dele o peixe já não existe.
2. **`LP.remove(*it)`** tira o ponteiro da lista principal. A partir daqui o peixe deixa
   de ser alimentado, deixa de correr e o `Pertence` deixa de o encontrar.
   - Aqui não há problema de iterador: o ciclo percorre a `LP_Assar` e remove da `LP`,
     que são listas diferentes.
3. **`delete *it`** destrói o peixe. É o aquário que o faz porque é o **dono** ("poder de
   vida e morte", secção 2.2). Como `~Peixe()` é `virtual`, se for um `Tubarao` corre
   também `~Tubarao()` (secção 6.4).

No fim, **`LP_Assar.clear()`** esvazia a fila. Os ponteiros que lá estavam apontam para
peixes destruídos e não podem ficar lá.

Não é preciso limpar a `LP_Fome` aqui. `Alimentar` já fez `LP_Fome.clear()` antes de
`RemoverParaAssar` correr, por isso nenhum peixe destruído fica nessa lista.

### 8.6 O papel da flag `Ja_Avisei_Quero_Sair`

Na abordagem escolhida, `RemoverParaAssar` corre logo a seguir a `Alimentar`. Por isso,
no funcionamento normal, um peixe que pediu para sair é destruído antes de poder pedir
outra vez. A flag parece desnecessária, mas protege contra um erro grave:

> Se `Alimentar()` fosse chamado duas vezes seguidas **sem** `RemoverParaAssar()` pelo
> meio (por exemplo num menu do `main`), um peixe com mais de 50 g pediria nas duas
> vezes. O mesmo ponteiro ficaria **duas vezes** na `LP_Assar`, e `RemoverParaAssar`
> faria `delete` **duas vezes** ao mesmo peixe: o programa rebenta (*double free*).

Com a flag, o peixe pede **uma única vez**, por muitas vezes que seja alimentado.

É o mesmo papel da `Ja_Avisei_Tenho_Fome` do professor, e pela mesma razão de
encapsulamento: é o **peixe** que evita pedidos repetidos. O aquário não tem de verificar
se o peixe já está na `LP_Assar`.

A flag nunca volta a `false`. Não é preciso: um peixe que pediu para sair vai ser
destruído.

### 8.7 Onde se chama `RemoverParaAssar`: a abordagem escolhida

A chamada está em `Aquario::Run`, logo a seguir a `Alimentar()`. Dentro de
`Alimentar`, a linha `//RemoverParaAssar();` ficou comentada. As duas opções são
corretas, porque ambas tratam os pedidos **depois** do ciclo de alimentação:

| | Em `Run`, a seguir a `Alimentar()` (escolhida) | No fim de `Alimentar()` |
|---|---|---|
| Os pedidos são tratados depois do ciclo? | Sim | Sim |
| `Alimentar` faz uma só coisa? | Sim: só alimenta | Alimenta e trata as saídas |
| Quem chama `Alimentar` tem de se lembrar de `RemoverParaAssar`? | **Sim** | Não |
| Entre `Alimentar` e `RemoverParaAssar`, `Pertence(id)` encontra um peixe que já pediu para sair? | Sim, se alguém chamar entretanto | Nunca |

Na simulação atual, só `Run` chama `Alimentar`, por isso a abordagem escolhida funciona
sem problemas. Há uma consequência a ter em conta se mais tarde existir um menu no `main`
com a opção "alimentar": o menu terá de chamar `AQ.Alimentar();` **e**
`AQ.RemoverParaAssar();`. Se se esquecer da segunda, os peixes ficam "à espera de sair"
indefinidamente (a flag impede que isso cause *double free*, mas eles continuam no
aquário). Nessa altura, ou se descomenta a linha em `Alimentar`, ou se garante que todos
os sítios que alimentam também tratam os pedidos.

### 8.8 A outra forma de remover: `Pertence` + `RemoverID`

O enunciado pede no `main`: "Verificar se um determinado peixe se encontra no aquário dado
o seu número de série" e "Remover um peixe dado o seu número de série".

```cpp
Peixe *Aquario::Pertence(int id) //pesquisa por ID
{
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        if ((*it)->GetID() == id) //chama metodo GetID no Peixe para o qual aponta it
            return *it;
    return NULL;
}
```

`Pertence` percorre a `LP` e compara o número de série de cada peixe com o pedido. Se
encontra, devolve **o ponteiro** para esse peixe (quem chamou pode usá-lo, por exemplo
para o mostrar). Se chega ao fim sem encontrar, devolve `NULL`, que quer dizer "não está
aqui". Usa `GetID()` porque `ID` é `private` no `Peixe`.

```cpp
bool Aquario::RemoverID(int id)
{
    Peixe *P = Pertence(id); //Pesquisa porID, e verifica se existe e tenta eliminar de todas as listas
    if (P == NULL)
        return false;            // não existe nenhum peixe com esse número
    LP.remove(P);
    LP_Fome.remove(P);           // não deixar ponteiros para um peixe apagado
    LP_Assar.remove(P);
    delete P;
    return true;
}
```

1. **Reaproveita `Pertence`** em vez de repetir a pesquisa.
2. **Se não existe, devolve `false`** e não faz mais nada. Quem chamou (o menu) pode
   avisar "não existe nenhum peixe com esse número".
3. **Tira o peixe das três listas.** Isto é essencial: o peixe pode estar na `LP_Fome`
   (pediu comida e ainda não foi alimentado) ou na `LP_Assar` (pediu para sair e o pedido
   ainda não foi tratado). Se ficasse numa delas:
   - na `LP_Fome`, ficava lá um ponteiro para um peixe destruído;
   - na `LP_Assar`, o próximo `RemoverParaAssar` faria `GetID()` e `delete` a um peixe
     já destruído, o que rebenta o programa.
4. **`delete P`** destrói o peixe e **devolve `true`**.

`RemoverID` pode remover logo, porque é chamado **de fora** (do `main`): nesse momento
ninguém está a percorrer a `LP` nem a executar um método do peixe.

### 8.9 A diferença entre `RemoverParaAssar` e `RemoverID`

Os dois acabam a fazer o mesmo (tirar o peixe da `LP` e fazer `delete`), mas respondem a
situações diferentes:

| | `RemoverParaAssar()` | `RemoverID(int id)` |
|---|---|---|
| **Quem toma a iniciativa** | O **peixe**: decidiu sair e pediu com `QueroSair(this)` | **Alguém de fora**: o utilizador, através do `main` |
| **Como se sabe qual é o peixe** | O peixe identificou-se ao enviar o seu endereço (`this`), que ficou guardado na `LP_Assar` | Quem está fora não tem o ponteiro, só o **número de série**. É preciso procurar com `Pertence(id)` |
| **Quantos peixes** | Todos os que pediram (um lote, pode ser 0, 1 ou vários) | Um, o do `id` |
| **Parâmetros** | Nenhum: a informação já está na `LP_Assar` | O `id` a procurar |
| **Quando corre** | Num momento escolhido pelo aquário: depois de `Alimentar`, em `Run` | Quando o utilizador pede |
| **Pode falhar?** | Não: os peixes da `LP_Assar` existem de certeza | Sim: o `id` pode não existir, por isso devolve `false` |
| **Que listas limpa** | `LP` e `LP_Assar` (a `LP_Fome` já foi limpa por `Alimentar`) | As três: `LP`, `LP_Fome` e `LP_Assar` |
| **Porque não remove de imediato no pedido** | O pedido chega a meio do ciclo de `Alimentar` (secção 8.4) | Não há esse problema: é chamado de fora de qualquer ciclo |

**Porque é que não chega ter só um?**

- **Só `RemoverParaAssar`:** o utilizador não conseguiria remover um peixe à sua escolha,
  que é uma funcionalidade pedida no enunciado.
- **Só `RemoverID`, chamado pelo próprio peixe** (`Ptr_Aquario->RemoverID(ID)` dentro de
  `Peixe::Alimentar`): o peixe era destruído a meio do ciclo de `Alimentar` e a meio do
  seu próprio método. São os dois problemas da secção 8.4. E não se poderia mudar
  `RemoverID` para "guardar e tratar depois", porque o utilizador espera que, ao remover,
  o peixe desapareça logo.

**Código repetido.** As linhas `LP.remove(...)` + `delete ...` aparecem nos dois
métodos. Se um dia a remoção tiver de fazer mais alguma coisa (por exemplo, escrever num
ficheiro de registo), é preciso lembrar-se de alterar os dois. Uma melhoria opcional é pôr
esse trabalho comum num método `private` que ambos chamam.

### 8.10 Tudo junto: o percurso completo de um peixe que sai

```
Aquario::Run
 │
 ├─ for cada peixe: Peixe::Run()  (peso desce; com fome → Preciso_de_Comer(this) → LP_Fome)
 │
 ├─ LP_Fome.size() >= 2 ?
 │    │
 │    ├─ Aquario::Alimentar()
 │    │    for cada peixe em LP:
 │    │      Peixe::Alimentar(cal)
 │    │        PESO += cal
 │    │        Ja_Avisei_Tenho_Fome = false
 │    │        EstouGordo()? (virtual: critério da espécie)  e  !Ja_Avisei_Quero_Sair ?
 │    │          └─ sim → Ptr_Aquario->QueroSair(this)
 │    │                     └─ LP_Assar.push_back(P); return true
 │    │                   Ja_Avisei_Quero_Sair = true
 │    │    LP_Fome.clear()
 │    │    (fim do ciclo: a LP não foi alterada durante o percurso)
 │    │
 │    └─ Aquario::RemoverParaAssar()
 │         for cada P em LP_Assar:
 │           mostra P->GetID(), P->GetPeso()
 │           LP.remove(P)
 │           delete P           (virtual ~Peixe → também ~Tubarao, se for tubarão)
 │         LP_Assar.clear()
 │
 └─ espera 2 s e repete

main (quando houver menu):
   AQ.Pertence(id)  → ponteiro ou NULL
   AQ.RemoverID(id) → Pertence; tira de LP, LP_Fome, LP_Assar; delete; true/false
```

### 8.11 Teste

Testei os métodos numa cópia do projeto com um `main` de teste. Criei 4 peixes, 500 a 503,
com 10, 20, 30 e 40 g:

```
Alimentacao a DEcorrer.....
-- Alimentar feito, pedidos ainda pendentes; Pertence(503)? 1      ← ainda está na LP
O peixe 503, com o peso 70(g), saiu do aquario!                     ← RemoverParaAssar
O peixe 502, com o peso 55(g), saiu do aquario!
-- depois de RemoverParaAssar; Pertence(503)? 0                     ← já não está
RemoverID(500): 1                                                   ← existia, removido
RemoverID(999): 0                                                   ← não existe
```

A primeira linha de teste confirma o que a secção 8.7 descreve. Entre `Alimentar` e
`RemoverParaAssar`, o peixe 503 já pediu para sair mas ainda está no aquário. Também se
vê o resultado do que cada peixe comeu (+20..50 g aleatórios): só os que passaram dos 50 g
pediram para sair.

### 8.12 O que ainda falta ou pode ser melhorado

O projeto compila sem erros nem avisos e a remoção funciona. Ficam estes pontos:

**Importantes**

- **Falta verificar `Ptr_Aquario != NULL`** em `Peixe::Alimentar` e em
  `Peixe::EstouComFome`. O enunciado diz que o peixe foge "se exceder 50g **e estiver num
  aquário**", e permite que um peixe seja criado fora de qualquer aquário. Se um desses
  peixes for alimentado acima de 50 g (ou ficar com fome), faz `NULL->QueroSair(this)` e
  o programa rebenta:

  ```cpp
  if (Ptr_Aquario != NULL && EstouGordo() && !Ja_Avisei_Quero_Sair)
  ```

- **`~Aquario()` está vazio.** O aquário é dono dos peixes, por isso, ao ser destruído,
  devia fazer `delete` dos que ainda estão na `LP`. Hoje isso não se nota porque o `Run`
  nunca termina, mas é uma fuga de memória.

**Menores**

- **`RemoverParaAssar` devolve sempre `true`.** Funciona, mas o valor não dá informação.
  Podia ser `void`, ou devolver o número de peixes que saíram.
- **`RemoverParaAssar` e `QueroSair` são `public`.** O `main` pode chamá-los por engano.
  `RemoverParaAssar` tem de ser público enquanto um futuro menu precisar de o chamar
  (secção 8.7); se passar a ser chamado dentro de `Alimentar`, pode ser `private`.
  `QueroSair` tem de ser público para o `Peixe` o poder chamar.
- **`Peixe();`** está declarado em `Peixe.h` e não está implementado. Se alguém o usar, dá
  erro de *linker*. Pode ser apagado (o enunciado exige sempre a espécie).
- **`GetID()`, `GetPeso()` e `EstouGordo()` não alteram o peixe.** Podiam ser `const`
  (`int GetID() const`), o que documenta e garante isso. Se `EstouGordo` passar a `const`,
  as versões das classes derivadas também o têm de ser.
- **`Tubarao::Show()`** escreve `Grau de perigo do Tubarao: 100Peixe: (...)`, sem
  separador.
- Em `Peixe.h` ficou o comentário `// filhos de Peixe podem LER o peso` sozinho na zona
  `protected`, mas `GetPeso()` passou para `public`.
- **Os ficheiros estão gravados em Latin-1** (os acentos dos comentários). Compila, mas
  noutros editores aparecem caracteres estranhos. Convém gravar em UTF-8.

**Do enunciado, ainda por fazer** (não tem a ver com a remoção)

- Número de série automático, a começar em 500 (hoje é passado no construtor: `10+i`).
  Faz-se com um atributo `static int` no `Peixe`, que conta os peixes criados.
- Peso inicial sempre 10 g (hoje é um parâmetro).
- "Obter a descrição textual em string" (hoje `Show()` escreve diretamente no ecrã).
- O `main` com menu: adicionar, listar, alimentar, verificar (`Pertence`) e remover
  (`RemoverID`).

---

## 9. Resumo: porque é que esta é a forma correta

1. **Uma classe por conceito** (`Peixe`, `Aquario`, `Tubarao`), cada uma com uma
   responsabilidade só. O código de cada conceito está num único sítio.
2. **Encapsulamento**: atributos `private`. O aquário **diz** ao peixe "come", e o peixe
   **decide** o que isso lhe faz, incluindo se quer sair (`EstouGordo`). O peixe **pede**
   ao aquário "preciso de comer" (`Preciso_de_Comer`) ou "quero sair" (`QueroSair`), e o
   aquário **decide** como gere as suas listas. Ninguém é marioneta de ninguém. Resultado:
   cada alteração futura (metabolismo, unidades, nova espécie, nova estrutura de dados)
   fica **contida numa classe**.
3. **Relação bidirecional** feita com ponteiros: o aquário guarda `Peixe*` e o peixe
   guarda `Aquario*`. O `Add` estabelece as duas ligações de uma vez (`this`), para nunca
   ficarem incoerentes.
4. **Inclusão circular resolvida** com a declaração antecipada `class Aquario;` em
   `Peixe.h` (o peixe só precisa de um ponteiro, e todos os ponteiros têm o mesmo
   tamanho) e o `#include "Aquario.h"` completo em `Peixe.cpp` (onde se chamam os
   métodos). A cadeia de includes fica linear.
5. **Herança** (`Tubarao : public Peixe`) para reaproveitar tudo o que é comum aos peixes,
   com o construtor da base chamado na lista de inicialização.
6. **Polimorfismo** (`virtual Run`, `virtual Show`, `virtual EstouGordo`,
   `virtual ~Peixe`) + lista de **ponteiros**: o aquário trata todos os peixes por igual e
   cada espécie responde à sua maneira, incluindo o critério de saída. Espécies novas não
   obrigam a mexer no aquário.
7. **Remoção segura**: os pedidos vindos do peixe ficam numa fila (`LP_Assar`) e só são
   tratados **depois** do ciclo que os gerou (`RemoverParaAssar`). A flag
   `Ja_Avisei_Quero_Sair` impede pedidos repetidos. A remoção pedida de fora
   (`RemoverID`) procura pelo número de série e limpa o peixe de todas as listas.
