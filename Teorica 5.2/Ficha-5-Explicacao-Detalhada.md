# Ficha de Trabalho 5 — Explicação Detalhada da Resolução (Aquário e Peixes)

> Este documento explica a resolução proposta pelo professor na aula teórica para a
> Ficha 5. **Não assume conhecimento prévio**: cada conceito (classe, ponteiro, `this`,
> herança, polimorfismo, encapsulamento, inclusão circular, ...) é explicado antes de ser
> usado. Se já conheces algum, salta essa secção.

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
8. [O que ainda falta face ao enunciado e como completar](#8-o-que-ainda-falta-face-ao-enunciado-e-como-completar)
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
aquário → peixe a dar comida). Na secção 8 mostro como completar para ficar igual ao
enunciado, reaproveitando esse mecanismo.

---

## 2. As classes criadas, porquê, e as relações entre elas

### 2.1 Porque é que existem estas classes

Em Programação Orientada a Objetos, a regra base é: **cada conceito do problema com
identidade, estado e comportamento próprios torna-se uma classe**. Lê-se o enunciado e
procuram-se os "substantivos" que fazem coisas:

| Classe | Porquê existe | Estado (dados) | Comportamento |
|---|---|---|---|
| `Peixe` | O enunciado fala de peixes, cada um com dados próprios e decisões próprias | `ID`, `ESPECIE`, `PESO`, `Ptr_Aquario`, `Ja_Avisei_Tenho_Fome` | `Run`, `Show`, `Alimentar`, `EstouComFome` |
| `Aquario` | É um conceito diferente: um **contentor** que gere vários peixes | listas `LP`, `LP_Fome`, `LP_Assar` | `Add`, `Run`, `Alimentar`, `Remover`, `Pertence`, `Preciso_de_Comer` |
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
                     │ + Remover(int) : bool                │
                     │ + Pertence(int) : Peixe*             │
                     └──────────────────────────────────────┘
                        ◆ 1                          ▲
                        │ "tem/possui"               │ "sabe em que aquário está"
                        │  0..* peixes               │  (ponteiro Ptr_Aquario, 0..1)
                        ▼                            │
                     ┌──────────────────────────────────────┐
                     │                Peixe                 │
                     │--------------------------------------│
                     │ - ID, ESPECIE, PESO                  │
                     │ - Ptr_Aquario : Aquario*             │
                     │ - Ja_Avisei_Tenho_Fome : bool        │
                     │ - EstouComFome()                     │
                     │--------------------------------------│
                     │ + Atribuir_Aquario(Aquario*)         │
                     │ + virtual Show()                     │
                     │ + virtual Run()                      │
                     │ + Alimentar(int)                     │
                     │ + virtual ~Peixe()                   │
                     └──────────────────────────────────────┘
                                       △
                                       │ "é um" (herança)
                                       │
                     ┌──────────────────────────────────────┐
                     │               Tubarao                │
                     │--------------------------------------│
                     │ - GRAU_PERIGO : int                  │
                     └──────────────────────────────────────┘

     Uteis: usada por Peixe e Aquario (dependência simples: "usa")
```

As três relações:

1. **Aquario → Peixe: "tem" (composição/agregação, 1 para muitos).**
   O aquário guarda **muitos** peixes (`list<Peixe*> LP`). O enunciado diz: "quando um
   peixe é posto no aquário, este assume a sua posse e controlo total" — o aquário é o
   **dono** dos peixes que tem. (Por isso, numa versão completa, quando o aquário é
   destruído deve destruir os seus peixes — ver secção 8.)

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
- `LP_Assar` — declarada mas ainda não usada. Pelo nome ("assar") e pelo enunciado, é
  muito provavelmente para os peixes que **pedem para sair/ser removidos** — seguindo o
  mesmo padrão da `LP_Fome`. Na secção 8 é usada exatamente para isso.

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
}
```

O aquário **não sabe** que o peixe tem um atributo `PESO` nem uma flag
`Ja_Avisei_Tenho_Fome`. Nem precisa: `PESO` é `private`, e o aquário nem conseguiria
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

É exatamente o que o enunciado diz para a remoção: "pede ao aquário para o tirar (**não se
tira a ele próprio** – o peixe não faz ideia como é que o aquário guarda os peixes
internamente)".

Cada lado manda **mensagens** ao outro; ninguém mexe nas "tripas" do outro:

```
   Aquario                                      Peixe
      │                                           │
      │ ── Alimentar(cal) ──────────────────────▶ │  (o peixe decide o que fazer
      │                                           │   com a comida)
      │                                           │
      │ ◀──────────────── Preciso_de_Comer(this)──│  (o aquário decide como
      │ ── return true ─────────────────────────▶ │   regista o pedido)
```

### 3.4 Outros pormenores de bom encapsulamento na resolução

- **`EstouComFome()` é `private`.** É um passo interno do peixe. Ninguém de fora deve poder
  "obrigar" um peixe a ter fome. Só o próprio `Run()` do peixe o chama, quando o peso
  baixa de 20.
- **A flag `Ja_Avisei_Tenho_Fome`** é estado interno do peixe que evita que ele peça
  comida em todos os ciclos (e apareça repetido em `LP_Fome`). O aquário não sabe que ela
  existe. Se o peixe não tivesse esta flag, o problema "pedidos repetidos" teria de ser
  resolvido pelo aquário, que ficaria a depender de um pormenor do peixe.
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
- **Não há `getPeso()`/`setPeso()` públicos.** Não são precisos. Getters e setters para
  todos os atributos são quase o mesmo que pôr tudo `public` — a marioneta voltava pela
  porta das traseiras.

### 3.5 Boas práticas resumidas

1. Atributos **sempre `private`**.
2. Métodos públicos exprimem **ações/pedidos** ("alimenta-te"), não acesso a dados
   ("muda o teu peso").
3. **Quem tem os dados é quem toma as decisões sobre eles** ("o peixe é que sabe").
4. Métodos auxiliares internos ficam `private` (`EstouComFome`).
5. Só criar getters quando há uma necessidade real e, se possível, só de leitura (por ex.
   o aquário precisa do número de série para `Pertence(id)` → um `GetID() const` faz
   sentido; um `SetID` não).
6. Um objeto **pede** a outro em vez de lhe mexer nos dados.

### 3.6 Como o encapsulamento protege o código de alterações futuras

Esta é a grande vantagem prática. Vamos imaginar mudanças que o professor (ou um
"cliente") podia pedir, e comparar o que acontece nas duas versões.

**Alteração A — "O metabolismo mudou: o peixe só aproveita 80 % da comida."**

- Versão certa: muda-se **uma linha**, em `Peixe::Alimentar`:
  `PESO += cal * 0.8;`. O `Aquario` não é tocado.
- Versão marioneta: é preciso procurar **todos** os sítios onde alguém faz `PESO += ...`
  (o aquário, o `main`, outras classes...). Se faltar um, há um bug.

**Alteração B — "Várias espécies: a sardinha sai a partir de 50 g, o tubarão nunca sai,
o peixe-palhaço sai a partir de 30 g."** (isto é literalmente o que o enunciado sugere:
"a decisão variava em função da espécie")

- Versão certa: cada espécie redefine o seu critério (com herança + polimorfismo, secção 6).
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
  `list` — ele só chama `Ptr_Aquario->Preciso_de_Comer(this)` (ou, na versão completa,
  `Pedir_Para_Sair(this)`). **O enunciado diz exatamente isto**: "o peixe não faz ideia
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
oferecer um método `protected` (por ex. `int GetPeso() const`), em vez de pôr o
atributo `protected`. Assim continua a ser só o `Peixe` a decidir como o peso muda.

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

### 6.5 O que já está preparado na resolução

`Peixe` declara `virtual void Show();` e `virtual void Run();`. O `Tubarao` ainda **não**
os redefine (a resolução ficou na estrutura), por isso, neste momento, um tubarão usaria
as versões de `Peixe`. Um exemplo de como o tubarão poderia redefinir `Run()`:

```cpp
// Tubarao.h
class Tubarao : public Peixe
{
    int GRAU_PERIGO;
public:
    ...
    virtual void Run();
    virtual void Show();
};

// Tubarao.cpp
void Tubarao::Run()
{
    Peixe::Run();                     // reaproveita o comportamento normal de peixe...
    if (Uteis::Aleatorio(1, 10) == 1)
    {
        cout << "O tubarao assusta toda a gente!" << endl;   // ...e acrescenta o seu
        system("Tubarao.mp3 > null");  // os .mp3 da pasta parecem pensados para isto
    }
}
void Tubarao::Show()
{
    cout << "[TUBARAO perigo=" << GRAU_PERIGO << "] ";
    Peixe::Show();                    // chama explicitamente a versão da classe base
}
```

E no `main`:

```cpp
AQ.Add(new Peixe(10, "Sardinha", 30));
AQ.Add(new Tubarao(99, "Branco", 300, 80));
AQ.Run();      // o aquário chama (*it)->Run() sem saber quem é quem
```

O `Aquario.cpp` **não muda uma linha** e passa a ter tubarões com comportamento próprio.
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
        Alimentar();                      //    ...alimenta toda a gente
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

---

## 8. O que ainda falta face ao enunciado e como completar

A resolução da aula é uma **base** que monta a arquitetura (que era o objetivo da aula).
Ficaram por fazer algumas coisas do enunciado, e há alguns pormenores a corrigir.
Esta secção **não é** a resolução do professor — é uma sugestão de como completar
**seguindo o mesmo desenho**.

### 8.1 Diferenças / pontos em aberto

| Ponto | Situação na resolução | Enunciado |
|---|---|---|
| Número de série | Passado à mão no construtor (`10+i`) | Automático, começa em 500, +1 por peixe (a cópia não incrementa) |
| Cor | Não existe | Opcional, por omissão `"cinzento"` |
| Peso inicial | Parâmetro (por omissão 10) | Sempre 10 g |
| Sair do aquário | Não existe (há o mecanismo de "fome") | Se > 50 g e estiver num aquário, o peixe pede para sair |
| `Pertence(id)` / `Remover(id)` | Vazios (`return NULL;` / `return true;`) | Têm de procurar pelo número de série |
| Descrição textual | `Show()` escreve no ecrã | "Obter a descrição textual **em string**" |
| `~Aquario()` | Vazio | O aquário tem "posse" → deve fazer `delete` dos seus peixes |
| `Peixe()` | Declarado mas não implementado | Não deve existir (a espécie é obrigatória) |
| `EstouComFome()` | Usa `Ptr_Aquario` sem verificar `NULL` | Um peixe fora de aquário rebentaria o programa se tivesse fome |

### 8.2 Um cuidado importante: remover durante o percurso da lista

Se, dentro de `Aquario::Alimentar`, o peixe pedir para sair e o aquário o **remover logo
da `LP`**, estamos a apagar um elemento da lista **enquanto o `for` a percorre** — o
iterador `it` ficaria a apontar para um elemento que já não existe (*iterador
invalidado*) → comportamento indefinido / crash.

A solução é o **mesmo padrão que o professor usou para a fome**: o pedido é **registado**
numa lista à parte e **processado depois** do ciclo. É precisamente para isso que serve a
`LP_Assar`.

### 8.3 Sketch de como completar

**Peixe.h**

```cpp
#ifndef PEIXE_H
#define PEIXE_H

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class Aquario;                       // declaração antecipada (continua a ser a solução)

class Peixe
{
    static int PROXIMO_NUMERO;       // partilhado por TODOS os peixes (contador)
    int NUMERO;                      // número de série deste peixe
    string ESPECIE;
    string COR;
    int PESO;
    Aquario *Ptr_Aquario;

protected:
    int GetPeso() const { return PESO; }       // as espécies derivadas podem LER o peso
    virtual bool QueroSair() const;            // a regra que varia com a espécie

public:
    Peixe(string especie, string cor = "cinzento");   // espécie obrigatória, cor opcional
    virtual ~Peixe();

    void Atribuir_Aquario(Aquario *A) { Ptr_Aquario = A; }
    int GetNumero() const { return NUMERO; }    // o aquário precisa para Pertence/Remover
    void Alimentar(int gramas);
    virtual string Descricao() const;
};
#endif
```

Nota sobre a cópia: o enunciado diz que "a construção por cópia não aumenta o número". O
construtor de cópia que o C++ gera automaticamente copia `NUMERO` tal e qual e **não**
passa pelo construtor acima, por isso já cumpre a regra sem escrever nada.

**Peixe.cpp**

```cpp
#include "Peixe.h"
#include "Aquario.h"                  // aqui sim, a classe completa

int Peixe::PROXIMO_NUMERO = 500;      // o primeiro peixe é o 500

Peixe::Peixe(string especie, string cor)
: ESPECIE(especie), COR(cor), PESO(10), Ptr_Aquario(NULL)
{
    NUMERO = PROXIMO_NUMERO++;        // usa o valor atual e incrementa para o próximo
}

Peixe::~Peixe() { }

bool Peixe::QueroSair() const
{
    return PESO > 50;                 // regra "normal" de peixe
}

void Peixe::Alimentar(int gramas)
{
    PESO += gramas;                                   // o PEIXE decide o que faz à comida
    if (Ptr_Aquario != NULL && QueroSair())           // o PEIXE decide se quer sair
        Ptr_Aquario->Pedir_Para_Sair(this);           // e PEDE ao aquário (não se tira sozinho)
}

string Peixe::Descricao() const
{
    ostringstream os;                 // "escreve" para uma string em vez de para o ecrã
    os << "Peixe " << NUMERO << ": " << ESPECIE << ", " << COR << ", " << PESO << " g";
    return os.str();
}
```

(Repara que `ostringstream` também é um `ostream` — a mesma ideia da `F1_Escrita`.)

**Tubarao — a decisão varia com a espécie**

```cpp
class Tubarao : public Peixe
{
    int GRAU_PERIGO;
protected:
    virtual bool QueroSair() const { return false; }  // um tubarão nunca foge
public:
    Tubarao(int gp = 100) : Peixe("Tubarao", "azul"), GRAU_PERIGO(gp) { }
    virtual string Descricao() const
    {
        ostringstream os;
        os << Peixe::Descricao() << " [perigo " << GRAU_PERIGO << "]";
        return os.str();
    }
};
```

`Peixe::Alimentar` chama `QueroSair()`; como é `virtual`, se o objeto for um tubarão é
chamada a versão do tubarão. Nem o `Peixe::Alimentar` nem o aquário precisam de saber que
há tubarões.

**Aquario.h / Aquario.cpp** (só o que muda)

```cpp
// Aquario.h
public:
    void Alimentar(int gramas);
    void Pedir_Para_Sair(Peixe *P);
    Peixe *Pertence(int numero);
    bool Remover(int numero);
    void Listar(ostream &os) const;

// Aquario.cpp
Aquario::~Aquario()
{
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        delete *it;                    // o aquário é dono → destrói os seus peixes
}

void Aquario::Pedir_Para_Sair(Peixe *P)
{
    LP_Assar.push_back(P);             // regista o pedido; não mexe na LP agora
}

void Aquario::Alimentar(int gramas)
{
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        (*it)->Alimentar(gramas);      // "toma, come, faz o que entenderes"

    // só agora, fora do ciclo, trata dos pedidos de saída
    for (list<Peixe *>::iterator it = LP_Assar.begin(); it != LP_Assar.end(); ++it)
    {
        LP.remove(*it);
        delete *it;                    // "poder de vida e morte": o aquário decide o destino
    }
    LP_Assar.clear();
}

Peixe *Aquario::Pertence(int numero)
{
    for (list<Peixe *>::iterator it = LP.begin(); it != LP.end(); ++it)
        if ((*it)->GetNumero() == numero)
            return *it;
    return NULL;
}

bool Aquario::Remover(int numero)
{
    Peixe *P = Pertence(numero);       // reaproveita a pesquisa
    if (P == NULL)
        return false;
    LP.remove(P);
    delete P;
    return true;
}

void Aquario::Listar(ostream &os) const   // serve para cout e para ficheiros!
{
    for (list<Peixe *>::const_iterator it = LP.begin(); it != LP.end(); ++it)
        os << (*it)->Descricao() << endl;
}
```

(Quanto ao destino de um peixe que sai — destruí-lo, devolvê-lo a quem o pediu, ou
mudá-lo para outro aquário — é uma decisão de desenho; o importante é que é **o aquário**
que a executa, a pedido do **peixe**.)

O `main` passaria a ter um menu com as opções do enunciado (adicionar, listar com
`AQ.Listar(cout)`, alimentar, `Pertence`, `Remover`).

---

## 9. Resumo: porque é que esta é a forma correta

1. **Uma classe por conceito** (`Peixe`, `Aquario`, `Tubarao`), cada uma com uma
   responsabilidade só. O código de cada conceito está num único sítio.
2. **Encapsulamento**: atributos `private`; o aquário **diz** ao peixe "come" e o peixe
   **decide**; o peixe **pede** ao aquário "preciso de comer / quero sair" e o aquário
   **decide** como gere as suas listas. Ninguém é marioneta de ninguém. Resultado: cada
   alteração futura (metabolismo, unidades, nova espécie, nova estrutura de dados) fica
   **contida numa classe**.
3. **Relação bidirecional** feita com ponteiros: o aquário guarda `Peixe*` e o peixe
   guarda `Aquario*`. O `Add` estabelece as duas ligações de uma vez (`this`), para nunca
   ficarem incoerentes.
4. **Inclusão circular resolvida** com a declaração antecipada `class Aquario;` em
   `Peixe.h` (o peixe só precisa de um ponteiro, e todos os ponteiros têm o mesmo
   tamanho) e o `#include "Aquario.h"` completo em `Peixe.cpp` (onde se chamam os
   métodos). A cadeia de includes fica linear.
5. **Herança** (`Tubarao : public Peixe`) para reaproveitar tudo o que é comum aos peixes,
   com o construtor da base chamado na lista de inicialização.
6. **Polimorfismo** (`virtual Run`, `virtual Show`, `virtual ~Peixe`) + lista de
   **ponteiros**: o aquário trata todos os peixes por igual e cada espécie responde à sua
   maneira. Espécies novas não obrigam a mexer no aquário — que é exatamente o que o
   enunciado sugere quando diz que "a decisão varia em função da espécie".
