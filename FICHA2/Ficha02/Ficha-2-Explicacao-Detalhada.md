# Ficha de Trabalho 2 — Explicação Detalhada da Resolução (até à ordenação)

> Este documento explica **tudo** o que foi feito na Ficha 2 **até à alínea 3i**
> (inclusive). A alínea 3j (ordenação) e a classe `Ordenacao` ficam de fora.
>
> Não assume conhecimento prévio: cada conceito de C++ é explicado **antes** de
> ser usado. Se já conheces um conceito, passa à frente.
>
> O foco principal está em **como as classes `Pessoa` e `Gestao` interagem**:
> quem cria o quê, quem chama o quê, e quem é responsável por apagar o quê.

Ficheiros envolvidos:

| Ficheiro | O que contém |
|---|---|
| `include/Pessoa.h` | Declaração da classe `Pessoa` (+ getters/setters curtos) |
| `src/Pessoa.cpp` | Implementação dos construtores, `Gravar` e `Show` |
| `include/Gestao.h` | Declaração da classe `Gestao` e da estrutura `DADOS` |
| `src/Gestao.cpp` | Implementação de todos os métodos do gestor |
| `main.cpp` | Programa de teste que chama todos os métodos |
| `pessoas.txt` | Ficheiro de dados de exemplo (o do enunciado) |

---

## Índice

0. [O que pede a ficha](#0-o-que-pede-a-ficha)
1. [Conceitos base de C++](#1-conceitos-base-de-c)
2. [A estrutura de dados escolhida (alínea 2)](#2-a-estrutura-de-dados-escolhida-alínea-2)
3. [A classe `Pessoa` (alínea 1)](#3-a-classe-pessoa-alínea-1)
4. [A classe `Gestao`: declaração, construtor, destrutor e cópia](#4-a-classe-gestao-declaração-construtor-destrutor-e-cópia)
5. [Os métodos do `Gestao` (alíneas 3a a 3i)](#5-os-métodos-do-gestao-alíneas-3a-a-3i)
6. [`Gravar` vs `GravarFicheiro`](#6-gravar-vs-gravarficheiro)
7. [Todas as interações entre `Pessoa` e `Gestao`](#7-todas-as-interações-entre-pessoa-e-gestao)
8. [O `main.cpp`, passo a passo](#8-o-maincpp-passo-a-passo)
9. [Os erros que existiam no código original e porque foram corrigidos](#9-os-erros-que-existiam-no-código-original-e-porque-foram-corrigidos)
10. [Perguntas de revisão (com resposta)](#10-perguntas-de-revisão-com-resposta)

---

## 0. O que pede a ficha

O objetivo é **trabalhar com dicionários** (`list` e `map`). Temos um ficheiro
de texto em que cada linha tem uma pessoa e a cidade onde vive:

```
Cidade;Nome da Pessoa;Idade
```

Exemplo do enunciado (é exatamente o conteúdo de `pessoas.txt`):

```
Viseu;Luis Miguel;25
Viseu; Carlos Lopes Andrade; 30
Lisboa;Jose Francisco;45
Porto;Antonio Lopes;35
Porto; Pedro Miguel;
Porto; Joana Monteiro; 23
Coimbra; Miguel Sales; 24
```

Repara em três "armadilhas" que o ficheiro tem de propósito:

1. **Espaços** depois do `;` (`"Viseu; Carlos Lopes Andrade; 30"`).
2. **Idade em falta** (`"Porto; Pedro Miguel;"`) → a ficha diz: considerar **18**.
3. As pessoas da mesma cidade **não estão todas seguidas** necessariamente
   (aqui estão, mas o código não pode depender disso).

O enunciado pede:

1. Uma classe `Pessoa` com nome, cidade e idade.
2. Uma classe gestora (`Gestor` no enunciado; no nosso código chama-se `Gestao`)
   que guarda **toda** a informação numa estrutura de dados.
3. Os métodos `AddPessoa` (2 versões), `LerFicheiro`, `Mostrar`, `Remover`
   (2 versões), `GravarFicheiro`, `PesquisarPessoa`, `GetCidadeMaisHabitantes`
   e a ordenação (esta última não é coberta aqui).

E duas observações importantes:

- Idade em falta → **18 anos**.
- **Atenção à gestão da memória** — não pode haver memória perdida.

Esta segunda observação é a razão de muitas das escolhas feitas. Vamos ver
porquê.

---

## 1. Conceitos base de C++

### 1.1 Classe e objeto

Uma **classe** é um "molde": descreve que dados uma coisa tem e o que essa coisa
sabe fazer. Um **objeto** é uma coisa concreta feita a partir desse molde.

```cpp
class Pessoa { ... };       // o molde

Pessoa a("Ana", "Viseu", 40);   // um objeto concreto
Pessoa b("Rui", "Braga", 19);   // outro objeto, com os seus próprios dados
```

- Os **dados** de uma classe chamam-se **atributos** (ex.: `NOME`, `IDADE`).
- As **funções** de uma classe chamam-se **métodos** (ex.: `GetNome()`, `Show()`).
- Cada objeto tem a **sua própria cópia** dos atributos: o `NOME` de `a` é
  `"Ana"` e o `NOME` de `b` é `"Rui"`.

### 1.2 `public`, `private` e `protected`

Dentro de uma classe, cada membro tem um nível de acesso:

| Nível | Quem pode usar |
|---|---|
| `public` | Qualquer código (o `main`, o `Gestao`, ...) |
| `private` | **Só** os métodos da própria classe |
| `protected` | A própria classe e classes que herdam dela (herança — ainda não usado) |

Na `Pessoa`, os atributos são `private`. Isto quer dizer que o `Gestao`
**não pode** escrever `P->NOME`. Tem de pedir à pessoa: `P->GetNome()`.

**Porquê esconder os dados?** Chama-se **encapsulamento**: a classe controla o
acesso aos seus próprios dados. Se um dia quisermos, por exemplo, impedir idades
negativas, basta pôr essa verificação no `SetIdade` — e mais nenhum código
precisa de mudar, porque ninguém mexe diretamente em `IDADE`.

### 1.3 Ficheiros `.h` e `.cpp`

Em C++ cada classe costuma estar dividida em dois ficheiros:

- **`.h` (header / cabeçalho)** — a **declaração**: diz *que* métodos e
  atributos existem. É como o índice de um livro.
- **`.cpp` (implementação)** — a **definição**: diz *como* cada método funciona.
  É o texto dos capítulos.

Quem quer usar a classe só precisa de fazer `#include "Pessoa.h"`. O compilador
fica a saber que métodos existem; o código propriamente dito é ligado mais tarde
pelo **linker** (o passo que junta todos os `.cpp` compilados num executável).

Se um método está **declarado** no `.h` mas **não definido** em lado nenhum, o
código compila mas o linker dá erro: `undefined reference to ...`. Isto
acontecia no código original (ver secção 9).

### 1.4 `#include`, *include guards* e `using namespace std`

```cpp
#ifndef PESSOA_H
#define PESSOA_H
...
#endif // PESSOA_H
```

O `#include` copia literalmente o conteúdo de outro ficheiro para aquele sítio.
Se o `Pessoa.h` for incluído duas vezes (por exemplo, o `main.cpp` inclui o
`Gestao.h`, que por sua vez inclui o `Pessoa.h`), a classe seria declarada duas
vezes → erro. As três linhas acima (**include guards**) evitam isso:

- Na 1.ª vez, `PESSOA_H` ainda não existe → entra, e define `PESSOA_H`.
- Na 2.ª vez, `PESSOA_H` já existe → salta tudo até ao `#endif`.

Includes usados:

| Include | Para quê |
|---|---|
| `<string>` | O tipo `string` (texto) |
| `<iostream>` | `cout` (ecrã) e o tipo `ostream` |
| `<fstream>` | `ifstream` (ler ficheiro) e `ofstream` (escrever ficheiro) |
| `<sstream>` | `stringstream` (tratar uma `string` como se fosse um ficheiro) |
| `<list>` | O contentor `list` |
| `<map>` | O contentor `map` |

`using namespace std;` permite escrever `string` em vez de `std::string`,
`cout` em vez de `std::cout`, etc. Toda a biblioteca standard do C++ vive dentro
de um "espaço de nomes" chamado `std`.

### 1.5 Construtor e destrutor

- O **construtor** é um método com o **mesmo nome da classe** e sem tipo de
  retorno. Corre **automaticamente** quando um objeto é criado. Serve para dar
  valores iniciais aos atributos.
- O **destrutor** chama-se `~NomeDaClasse()`. Corre **automaticamente** quando o
  objeto deixa de existir. Serve para "arrumar a casa" (por exemplo, libertar
  memória que o objeto pediu).

Pode haver vários construtores, desde que tenham parâmetros diferentes
(chama-se **sobrecarga** / *overloading*). Exemplo na `Pessoa`:

```cpp
Pessoa();                                               // sem parâmetros
Pessoa(string _nome, string _cidade, int _idade = 18);  // com parâmetros
```

O compilador escolhe qual usar conforme os argumentos que lhe passas. O mesmo
mecanismo é usado no `Gestao` para ter dois `AddPessoa` e dois `Remover`.

### 1.6 Parâmetros com valor por defeito

```cpp
Pessoa(string _nome, string _cidade, int _idade = 18);
```

O `= 18` quer dizer: **se não passares a idade, vale 18**.

```cpp
Pessoa a("Ana", "Viseu", 40);  // idade = 40
Pessoa b("Rui", "Braga");      // idade = 18 (valor por defeito)
```

O valor por defeito escreve-se **só no `.h`** (na declaração), nunca repetido no
`.cpp`.

### 1.7 Métodos `const`

```cpp
string GetNome() const { return NOME; }
```

O `const` no fim é uma promessa: **este método não altera o objeto**. O
compilador verifica: se tentasses fazer `NOME = "x";` dentro de um método
`const`, dava erro. É boa prática pôr `const` em todos os métodos que só leem
dados (getters, `Show`, `Gravar`).

### 1.8 Métodos definidos dentro do `.h`

Os getters e setters estão escritos **diretamente no `.h`**:

```cpp
string GetNome() const { return NOME; }
void SetNome(string _nome) { NOME = _nome; }
```

Para métodos de **uma linha** isto é comum e aceitável: poupa escrever a mesma
coisa duas vezes (declaração + definição). Métodos maiores (`Gravar`, `Show`,
construtores) ficam no `.cpp`.

### 1.9 Memória: *stack* e *heap*

Um programa tem duas zonas principais de memória para variáveis:

| | Stack (pilha) | Heap (memória dinâmica) |
|---|---|---|
| Como se cria | `Pessoa a("Ana","Viseu",40);` | `Pessoa *P = new Pessoa("Ana","Viseu",40);` |
| Quando é apagada | **Automaticamente**, quando a função/bloco termina | **Só quando fazes `delete P;`** |
| Se te esqueces | Não há problema | **Memória perdida** (*memory leak*) |

Porque usar o heap? Porque um objeto no stack morre quando a função termina. O
`LerFicheiro` cria as pessoas, mas elas têm de **continuar a existir depois** de
o `LerFicheiro` terminar — por isso são criadas com `new`.

### 1.10 Ponteiros

Um **ponteiro** é uma variável que guarda um **endereço de memória** — isto é,
"onde" está outro objeto.

```cpp
Pessoa *P = new Pessoa("Ana", "Viseu", 40);
```

- `new Pessoa(...)` cria uma Pessoa no heap e **devolve o endereço** dela.
- `Pessoa *P` é uma variável que guarda esse endereço.
- Para chamar um método através de um ponteiro usa-se `->`:
  `P->GetNome()` (equivalente a `(*P).GetNome()`).
- `*P` quer dizer "o objeto para onde `P` aponta".

Analogia: a Pessoa é uma **casa**; o ponteiro é um **papel com a morada**.
Podes ter vários papéis com a mesma morada — continua a haver **uma só casa**.
Copiar o papel não constrói uma casa nova.

### 1.11 `nullptr`

`nullptr` é o valor de um ponteiro que **não aponta para nada**. É usado para
dizer "não encontrei" ou "não existe":

```cpp
Pessoa *P = G.PesquisarPessoa("Ninguem");
if (P == nullptr) { /* não encontrou */ }
```

Usar `P->...` quando `P == nullptr` faz o programa **rebentar**. Por isso os
métodos que recebem um ponteiro verificam sempre primeiro se é `nullptr`.

### 1.12 `delete`, *memory leak* e *dangling pointer*

- `delete P;` apaga o objeto para onde `P` aponta e liberta essa memória.
- **Memory leak (memória perdida):** fazer `new` e nunca fazer `delete`. A
  memória fica ocupada até o programa terminar, sem ninguém a poder usar.
- **Dangling pointer (ponteiro pendurado):** depois de `delete P;`, a variável
  `P` **continua a ter o mesmo endereço**, mas a casa foi demolida. Usar `P`
  depois disso é um erro grave (o programa pode rebentar ou ler lixo).
- **Double delete:** fazer `delete` duas vezes do mesmo endereço → o programa
  normalmente rebenta.

As regras de ouro:

1. Cada `new` tem de ter **exatamente um** `delete`.
2. Tem de estar sempre claro **quem é o dono** (quem é responsável por fazer o
   `delete`) de cada objeto. Na nossa solução, **o dono de todas as pessoas é o
   `Gestao`** (secção 7.3).

### 1.13 Referências (`&`)

Uma referência é um **outro nome** para uma variável que já existe. Não é uma
cópia.

```cpp
list<Pessoa *> &lp = i_m->second;   // lp É a lista que está dentro do map
list<Pessoa *>  lp = i_m->second;   // lp é uma CÓPIA da lista
```

Usamos referências por duas razões:

- **Eficiência:** não copiar uma lista inteira só para a percorrer.
- **Alterar o original:** no `Remover`, queremos tirar a pessoa da lista que
  está **dentro do map**, não de uma cópia.

Também aparece em parâmetros: `void Gravar(ostream &f)` recebe o **próprio**
stream, não uma cópia (os streams, aliás, nem podem ser copiados).

### 1.14 Streams: `cout`, `ifstream`, `ofstream`, `ostream`, `stringstream`

Um **stream** é um "canal" por onde passam dados. Escreve-se com `<<` e lê-se
com `>>` ou `getline`.

| Tipo | O que é |
|---|---|
| `cout` | Canal de saída para o **ecrã** |
| `ifstream` | Canal de **entrada** a partir de um **ficheiro** (i = input) |
| `ofstream` | Canal de **saída** para um **ficheiro** (o = output). Abrir assim **apaga** o conteúdo antigo |
| `ostream` | O tipo **genérico** de "qualquer canal de saída". Tanto `cout` como um `ofstream` **são** `ostream` |
| `stringstream` | Faz de conta que uma `string` é um ficheiro, para a podermos partir com `getline` |

O ponto mais importante para esta ficha: **uma função que recebe `ostream &`
aceita tanto `cout` como um ficheiro aberto**. É isso que torna o
`Pessoa::Gravar` tão útil (secção 6).

`getline(canal, s)` lê uma linha inteira para `s`.
`getline(canal, s, ';')` lê **até ao próximo `;`** (e consome o `;`).

### 1.15 O contentor `list`

Uma `list<T>` é uma sequência de elementos do tipo `T`, onde se pode
**acrescentar e remover em qualquer ponto de forma barata**.

```cpp
list<Pessoa *> lp;
lp.push_back(P);   // acrescenta no fim
lp.size();         // quantos elementos tem
lp.empty();        // true se não tiver nenhum
lp.erase(it);      // remove o elemento na posição "it"
lp.clear();        // remove todos
```

Uma `list` **não** tem acesso por índice (`lp[2]` não existe). Para a
percorrer usam-se **iteradores** (1.17).

### 1.16 O contentor `map` (dicionário)

Um `map<Chave, Valor>` guarda pares **chave → valor**, como um dicionário:
procuras pela palavra (chave) e obténs a definição (valor).

- As chaves são **únicas** (não há duas entradas `"Porto"`).
- As chaves ficam **ordenadas** automaticamente (por isso o `Mostrar` aparece
  por ordem alfabética: Coimbra, Lisboa, Porto, Viseu).

Formas de aceder:

| Código | O que faz | Se a chave não existir |
|---|---|---|
| `DADOS["Porto"]` | Devolve o valor de `"Porto"` | **Cria** a entrada com um valor vazio |
| `DADOS.find("Porto")` | Devolve um iterador para a entrada | Devolve `DADOS.end()` — **não cria nada** |
| `DADOS.erase(it)` | Remove a entrada | — |

Esta diferença entre `[]` e `find` é **muito importante** e é usada de propósito:
`[]` no `AddPessoa` (queremos criar a cidade se não existir) e `find` no
`Remover` (não queremos criar uma cidade vazia só por ter procurado).

### 1.17 Iteradores

Um **iterador** é um "marcador" que aponta para um elemento de um contentor e
que sabe avançar para o seguinte. Funciona de forma parecida com um ponteiro.

```cpp
for (list<Pessoa *>::iterator i_l = lp.begin(); i_l != lp.end(); i_l++)
```

- `lp.begin()` → marcador no **primeiro** elemento.
- `lp.end()` → marcador **depois do último** (não é um elemento válido; é o
  sinal de "acabou").
- `i_l++` → avança para o elemento seguinte.
- `*i_l` → o elemento para onde o marcador aponta.

Num `map`, cada elemento é um **par**:

- `i_m->first` → a **chave** (o nome da cidade).
- `i_m->second` → o **valor** (a lista de pessoas dessa cidade).

**Atenção à dupla indireção** que aparece muitas vezes no código:

```cpp
(*i_l)->GetNome()
```

- `i_l` é um iterador da lista.
- `*i_l` é o elemento da lista, que é um `Pessoa *` (um ponteiro).
- `(*i_l)->GetNome()` chama `GetNome()` na pessoa para onde esse ponteiro aponta.

Os parênteses são obrigatórios: `*i_l->GetNome()` seria interpretado de outra
forma e não compila.

### 1.18 `size_t`

`size()` de um contentor devolve um `size_t`: um inteiro **sem sinal**
(nunca negativo). Comparar um `int` com um `size_t` gera um aviso do compilador
(`-Wsign-compare`), porque misturar números com e sem sinal pode dar resultados
estranhos. Por isso, no `GetCidadeMaisHabitantes`, `MAX` é `size_t`.

### 1.19 Funções `static` fora de classes

```cpp
static string Trim(const string &s) { ... }
```

Uma função `static` escrita **fora de uma classe**, num `.cpp`, só é visível
**dentro desse ficheiro**. É uma função auxiliar "privada" do `Gestao.cpp`:
ninguém de fora precisa dela, por isso não a pusemos no `.h`.

(Não confundir com um **método** `static` dentro de uma classe, como os da
`Ordenacao` — aí `static` quer dizer "não precisa de um objeto para ser
chamado". É a mesma palavra com significados diferentes conforme o sítio.)

### 1.20 `= delete` em métodos

```cpp
Gestao(const Gestao &) = delete;
```

Diz ao compilador: **este método é proibido**. Se alguém tentar usá-lo, dá erro
de compilação. Explicado em detalhe na secção 4.4.

### 1.21 `virtual` no destrutor

O Code::Blocks gera os destrutores como `virtual ~Pessoa();`. A palavra
`virtual` só tem efeito quando existe **herança** (uma classe derivada de
outra), matéria que ainda não foi dada. Aqui não faz diferença nenhuma, mas
também não faz mal — foi deixada como o Code::Blocks a gerou.

---

## 2. A estrutura de dados escolhida (alínea 2)

### 2.1 A declaração

Em `include/Gestao.h`:

```cpp
private:
    // chave = nome da cidade, valor = lista de pessoas dessa cidade
    map<string, list<Pessoa *>> DADOS;
```

Lê-se: **um dicionário em que a chave é o nome da cidade (`string`) e o valor é
uma lista de ponteiros para pessoas (`list<Pessoa *>`)**.

### 2.2 Como fica a memória depois de ler o ficheiro

```
 Gestao G   (no stack do main)
   │
   └── DADOS : map
         │
         ├── "Coimbra" ──► list: [ ● ]
         │                         │
         │                         ▼
         │                  ┌──────────────────────────┐
         │                  │ Pessoa (heap)            │
         │                  │ NOME   = "Miguel Sales"  │
         │                  │ CIDADE = "Coimbra"       │
         │                  │ IDADE  = 24              │
         │                  └──────────────────────────┘
         │
         ├── "Lisboa"  ──► list: [ ● ] ──► Pessoa{Jose Francisco, Lisboa, 45}
         │
         ├── "Porto"   ──► list: [ ●,  ●,  ● ]
         │                         │   │   └──► Pessoa{Joana Monteiro, Porto, 23}
         │                         │   └──────► Pessoa{Pedro Miguel, Porto, 18}
         │                         └──────────► Pessoa{Antonio Lopes, Porto, 35}
         │
         └── "Viseu"   ──► list: [ ●,  ● ]
                                   │   └──► Pessoa{Carlos Lopes Andrade, Viseu, 30}
                                   └──────► Pessoa{Luis Miguel, Viseu, 25}
```

Os `●` são ponteiros (moradas). As caixas `Pessoa` vivem no **heap** e só
desaparecem quando alguém faz `delete`.

### 2.3 Porquê um `map` com a cidade como chave?

Porque **quase tudo o que a ficha pede é organizado por cidade**:

- `GetCidadeMaisHabitantes` → basta ver o `size()` de cada lista. Não é preciso
  contar nada à mão.
- A ordenação (3j) é "**para cada cidade**, ordenar as pessoas" → cada cidade já
  tem a sua lista separada.
- O `Mostrar` sai agrupado por cidade e por ordem alfabética, sem esforço.
- O enunciado diz literalmente "carregar os dados para um **dicionário**".

### 2.4 Porquê `list<Pessoa *>` (ponteiros) e não `list<Pessoa>` (objetos)?

Esta é talvez **a escolha mais importante da ficha**. Com ponteiros:

1. **`PesquisarPessoa` pode devolver a pessoa verdadeira.** O enunciado pede
   `Pessoa *PesquisarPessoa(string nome)` — devolve um ponteiro. Para isso, as
   pessoas têm de ter um endereço estável algures; com ponteiros na lista,
   devolvemos exatamente a morada que lá está guardada.
2. **`AddPessoa(Pessoa *P)` e `Remover(Pessoa *P)`** recebem ponteiros (é o que o
   enunciado pede). Guardar ponteiros permite que o `Remover(P)` compare
   moradas (`*i_l == P`) e encontre exatamente aquela pessoa.
3. **Não há cópias de pessoas.** Mover ponteiros (por exemplo, na ordenação) é
   copiar um número (o endereço), não copiar nomes e cidades.

O preço a pagar: **temos de gerir a memória nós próprios** — cada pessoa foi
criada com `new`, e alguém tem de fazer `delete`. É exatamente aqui que entra a
observação da ficha "Atenção à gestão da memória!".

### 2.5 Porquê `list` e não outro contentor?

O enunciado diz para trabalhar com `list` e `map`. Além disso, uma `list`
permite remover um elemento do meio (`Remover`) de forma barata e sem mexer nos
outros.

### 2.6 Porque é que `DADOS` é `private`?

Se `DADOS` fosse `public`, o `main` podia fazer, por exemplo,
`G.DADOS["Porto"].clear();` — e as pessoas do Porto ficavam no heap sem
ninguém que as apagasse (memory leak). Com `private`, **só os métodos do
`Gestao` mexem na estrutura**, e todos eles respeitam as regras da memória.

---

## 3. A classe `Pessoa` (alínea 1)

### 3.1 O `.h` completo

```cpp
#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <iostream>
using namespace std;

class Pessoa
{
    public:
        Pessoa();
        Pessoa(string _nome, string _cidade, int _idade = 18);
        virtual ~Pessoa();

        // Getters (usados pelo Gestao para pesquisar / ordenar / gravar)
        string GetNome() const   { return NOME; }
        string GetCidade() const { return CIDADE; }
        int    GetIdade() const  { return IDADE; }

        // Setters
        void SetNome(string _nome)     { NOME = _nome; }
        void SetCidade(string _cidade) { CIDADE = _cidade; }
        void SetIdade(int _idade)      { IDADE = _idade; }

        // Escreve a pessoa no formato do ficheiro: Cidade;Nome;Idade
        void Gravar(ostream &f) const;
        void Show() const;

    protected:

    private:
        string NOME;
        string CIDADE;
        int IDADE;
};

#endif // PESSOA_H
```

### 3.2 Os atributos

```cpp
private:
    string NOME;
    string CIDADE;
    int IDADE;
```

Os três campos que o enunciado pede. São `private` (ver 1.2): ninguém de fora
os altera diretamente.

**Porque é que a `Pessoa` guarda a `CIDADE`, se o `map` já sabe a cidade?**
Porque há sítios onde **só temos a pessoa** e precisamos de saber em que lista
ela está:

- `AddPessoa(Pessoa *P)` recebe só a pessoa → tem de perguntar
  `P->GetCidade()` para saber em que lista a pôr.
- `Remover(Pessoa *P)` recebe só a pessoa → usa `P->GetCidade()` para ir
  **diretamente** à lista certa, em vez de procurar em todas.
- `Gravar` tem de escrever a cidade no ficheiro.

Além disso, o enunciado pede explicitamente os três campos.

### 3.3 Os construtores

```cpp
Pessoa::Pessoa()
{
    NOME = "";
    CIDADE = "";
    IDADE = 18;
}

Pessoa::Pessoa(string _nome, string _cidade, int _idade)
{
    NOME = _nome;
    CIDADE = _cidade;
    IDADE = _idade;
}
```

- **Construtor sem parâmetros:** cria uma pessoa "vazia". As `string` já
  começariam vazias sozinhas, mas um `int` **não é inicializado
  automaticamente** — ficaria com um valor qualquer que estivesse naquela zona da
  memória (lixo). Por isso damos `IDADE = 18`, coerente com a regra da ficha.
- **Construtor com parâmetros:** copia os valores recebidos para os atributos.
  Graças ao `= 18` no `.h`, a idade é opcional.

O `_` antes dos nomes dos parâmetros (`_nome`) é só uma convenção para não os
confundir com os atributos (`NOME`).

### 3.4 Getters e setters

- **Getters** (`GetNome`, `GetCidade`, `GetIdade`): devolvem o valor de um
  atributo. São `const` (não alteram nada).
- **Setters** (`SetNome`, `SetCidade`, `SetIdade`): alteram um atributo.

Os getters são **essenciais** para o `Gestao`, porque os atributos são
`private`. Cada getter é usado em:

| Getter | Usado por |
|---|---|
| `GetNome()` | `AddPessoa(Pessoa*)` (verificar repetidos), `PesquisarPessoa` |
| `GetCidade()` | `AddPessoa(Pessoa*)` (escolher a lista), `Remover(Pessoa*)` (encontrar a lista) |
| `GetIdade()` | A ordenação (3j) |

> ⚠️ **Cuidado com `SetCidade` (e `SetNome`) numa pessoa que já está no
> `Gestao`.** Se fizeres `G.PesquisarPessoa("Ana Silva")->SetCidade("Porto")`,
> a pessoa passa a dizer que é do Porto **mas continua na lista de Viseu** — o
> `map` não sabe que a cidade mudou. A seguir, `Remover(P)` iria procurar na
> lista do Porto e não a encontrava. O mesmo com `SetNome` e nomes repetidos.
> Os setters existem porque a ficha pede "vários métodos" e são úteis **antes**
> de a pessoa entrar no gestor. Para mudar a cidade de uma pessoa que já lá
> está, o correto seria criar um método `MudarCidade(nome, novaCidade)` no
> `Gestao` que tira o ponteiro da lista antiga **sem fazer `delete`**, chama
> `SetCidade` e volta a pôr o ponteiro na lista da cidade nova. (Não serve usar
> `Remover` + `AddPessoa` com o mesmo ponteiro, porque o `Remover` apaga a
> pessoa.)

### 3.5 `Show()` — mostrar no ecrã para um humano ler

```cpp
void Pessoa::Show() const
{
    cout << "  " << NOME << " (" << IDADE << " anos) - " << CIDADE << endl;
}
```

Escreve no ecrã, num formato **bonito para pessoas**:

```
  Pedro Miguel (18 anos) - Porto
```

Os dois espaços no início servem para ficar indentado debaixo do nome da cidade
no `Mostrar`.

### 3.6 `Gravar(ostream &f)` — escrever no formato do ficheiro

```cpp
void Pessoa::Gravar(ostream &f) const
{
    f << CIDADE << ";" << NOME << ";" << IDADE << endl;
}
```

Escreve **uma linha** no formato `Cidade;Nome;Idade` no stream `f` que recebe.
Não abre nem fecha ficheiros. A explicação completa de porque é assim está na
secção 6.

### 3.7 O destrutor

```cpp
Pessoa::~Pessoa()
{
    //dtor
}
```

Está vazio porque a `Pessoa` **não pediu memória com `new`**. As `string`
arrumam-se sozinhas. Corre automaticamente sempre que se faz `delete P;`.

---

## 4. A classe `Gestao`: declaração, construtor, destrutor e cópia

### 4.1 O `.h` (sem a parte da ordenação)

```cpp
#include <string>
#include <list>
#include <map>
#include "Pessoa.h"
using namespace std;

class Gestao
{
    public:
        Gestao();
        virtual ~Gestao();

        // O Gestao e "dono" dos ponteiros: uma copia faria delete duas vezes
        Gestao(const Gestao &) = delete;
        Gestao &operator=(const Gestao &) = delete;

        bool AddPessoa(string _nome, string _cidade, int _idade);   // 3a
        bool AddPessoa(Pessoa *P);                                  // 3b
        bool LerFicheiro(string nome_ficheiro);                     // 3c
        void Mostrar();                                             // 3d
        bool Remover(Pessoa *P);                                    // 3e
        bool Remover(string _nome);                                 // 3f
        bool GravarFicheiro(string ficheiro);                       // 3g
        Pessoa *PesquisarPessoa(string _nome);                      // 3h
        string GetCidadeMaisHabitantes();                           // 3i
        void OrdenarPorIdade();                                     // 3j

    private:
        map<string, list<Pessoa *>> DADOS;
};
```

O `#include "Pessoa.h"` é obrigatório: o `Gestao` usa o tipo `Pessoa` em quase
todos os métodos. A dependência é **num só sentido**: o `Gestao` conhece a
`Pessoa`, mas a `Pessoa` **não sabe** que o `Gestao` existe. Isto é bom — a
`Pessoa` pode ser reutilizada noutro programa sem arrastar o gestor.

Os métodos devolvem `bool` (`true`/`false`) para quem os chama saber se a
operação correu bem.

### 4.2 O construtor

```cpp
Gestao::Gestao()
{
    //ctor
}
```

Vazio: o `map` `DADOS` já começa vazio sozinho. Não há mais nada a preparar.

### 4.3 O destrutor — onde a memória é libertada

```cpp
Gestao::~Gestao()
{
    cout << "passei em <" << __FUNCTION__ << ">" << endl;
    map<string, list<Pessoa *>>::iterator it = DADOS.begin();
    while (it != DADOS.end())
    {
        list<Pessoa *> &LP = it->second;
        for (list<Pessoa *>::iterator i_l = LP.begin(); i_l != LP.end(); i_l++)
        {
            delete (*i_l); // apaga o conteudo do ponteiro
        }
        LP.clear(); // a lista fica so com ponteiros invalidos, limpamos
        it++; // iterador do while para percorrer o map
    }
    DADOS.clear();
}
```

**Porque é preciso este código?** Quando o `Gestao` é destruído, o `map` e as
`list` são destruídos automaticamente — mas uma `list<Pessoa *>`, ao ser
destruída, **apaga apenas os ponteiros (os papéis com as moradas), não as
pessoas (as casas)**. Se o destrutor estivesse vazio, todas as pessoas
ficavam no heap sem ninguém que as apagasse → memory leak de todas as pessoas.

Passo a passo:

1. `cout << ... __FUNCTION__` — `__FUNCTION__` é substituído pelo compilador
   pelo nome da função atual (`~Gestao`). Serve só para vermos no ecrã que o
   destrutor correu (prova de que a memória foi libertada).
2. `it = DADOS.begin()` — começa na primeira cidade.
3. `while (it != DADOS.end())` — repete para cada cidade.
4. `list<Pessoa *> &LP = it->second;` — `LP` é **a própria lista** dessa cidade
   (referência, não cópia).
5. O `for` percorre a lista e faz `delete (*i_l)` — `*i_l` é o `Pessoa *`
   guardado; `delete` apaga a pessoa para onde aponta. É aqui que corre o
   `~Pessoa()` de cada pessoa.
6. `LP.clear()` — depois do `delete`, os ponteiros que estão na lista são
   **dangling** (apontam para casas demolidas). Limpamos a lista para não
   ficarem lá.
7. `it++` — passa à cidade seguinte.
8. `DADOS.clear()` — esvazia o map.

(Os passos 6 e 8 não são estritamente obrigatórios, porque o objeto vai
desaparecer logo a seguir, mas é uma boa prática nunca deixar ponteiros
inválidos guardados.)

**Quando é que este destrutor corre?** No `main`, `Gestao G;` é uma variável no
stack. Quando o `main` chega ao fim (`return 0;`), `G` deixa de existir e o
C++ chama `~Gestao()` automaticamente. É por isso que a última linha do output
é `passei em <~Gestao>`.

### 4.4 Porque é que a cópia do `Gestao` é proibida (`= delete`)?

```cpp
Gestao(const Gestao &) = delete;             // construtor de cópia
Gestao &operator=(const Gestao &) = delete;  // atribuição (G2 = G1)
```

Por defeito, o C++ sabe copiar qualquer objeto: copia **atributo a atributo**.
Para o `Gestao`, isso quer dizer copiar o `map` — que copia as listas — que
copiam **os ponteiros**. Ou seja, copiam-se os papéis com as moradas, **não as
casas**:

```
G1.DADOS["Porto"] ──► [ ●, ● ] ─┐
                                ├──► as MESMAS Pessoas no heap
G2.DADOS["Porto"] ──► [ ●, ● ] ─┘
```

Quando `G2` for destruído, o destrutor faz `delete` de todas as pessoas. Quando
`G1` for destruído a seguir, faz `delete` **das mesmas pessoas outra vez** →
**double delete** → o programa rebenta.

Isto aconteceria em situações tão inocentes como:

```cpp
void Imprimir(Gestao g) { g.Mostrar(); }  // passar por valor = copiar!
...
Imprimir(G);   // a cópia "g" é destruída no fim da função → apaga as pessoas de G
```

Com `= delete`, **o compilador recusa este código**, e o erro é apanhado antes
de o programa correr. (A forma certa seria `void Imprimir(Gestao &g)`, por
referência.)

> Isto é a chamada **"regra dos três"** do C++: se uma classe precisa de um
> destrutor que liberta memória, então também tem de tratar da cópia (ou
> proibi-la, que foi o que fizemos).

---

## 5. Os métodos do `Gestao` (alíneas 3a a 3i)

Vou apresentar os métodos por uma ordem que facilita a compreensão (os mais
simples e os que são reutilizados pelos outros primeiro). As alíneas estão
indicadas em cada um.

```
Dependências entre métodos (quem chama quem):

  LerFicheiro (3c) ──► Trim
        │
        ▼
  AddPessoa(nome,cidade,idade) (3a) ──► new Pessoa(...)
        │
        ▼
  AddPessoa(Pessoa*) (3b) ──► PesquisarPessoa (3h)

  Remover(string) (3f) ──► PesquisarPessoa (3h)
        │
        ▼
  Remover(Pessoa*) (3e) ──► delete

  GravarFicheiro (3g) ──► Pessoa::Gravar
  Mostrar (3d)        ──► Pessoa::Show
```

---

### 5.1 `PesquisarPessoa` (3h) — `src/Gestao.cpp:174`

```cpp
Pessoa *Gestao::PesquisarPessoa(string _nome)
{
    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        list<Pessoa *> &lp = i_m->second; // referencia: evita copiar a lista
        for (list<Pessoa *>::iterator i_l = lp.begin(); i_l != lp.end(); i_l++)
        {
            if ((*i_l)->GetNome() == _nome)
                return *i_l;
        }
    }
    return nullptr;
}
```

**O que faz:** procura uma pessoa pelo nome e devolve o **ponteiro para a
pessoa verdadeira** que está na estrutura. Se não existir, devolve `nullptr`.

**Como:** dois ciclos encaixados.

- O de fora percorre as **cidades** (`i_m`).
- O de dentro percorre as **pessoas** dessa cidade (`i_l`).
- Para cada pessoa, pergunta-lhe o nome (`(*i_l)->GetNome()`) — **interação com
  a `Pessoa`**: o gestor não consegue ler `NOME` diretamente porque é `private`.
- Se for igual, `return *i_l;` — devolve o ponteiro e **sai logo da função**
  (não vale a pena continuar a procurar).
- Se os ciclos acabarem sem encontrar, `return nullptr;`.

**Porque é que tem de percorrer todas as cidades?** Porque a chave do `map` é a
cidade, não o nome. Só sabendo o nome, não sabemos em que lista está.

**Porque `&lp` (referência)?** Sem o `&`, cada cidade obrigava a copiar a lista
inteira, só para ler. E pior: o `return *i_l` continuaria a funcionar (o
ponteiro copiado tem a mesma morada), mas seria um desperdício.

**O que se pode fazer com o ponteiro devolvido?** Ler (`P->Show()`,
`P->GetIdade()`) ou passá-lo ao `Remover(P)`. **Não se deve** fazer
`delete P` fora do gestor — a pessoa continua dentro da lista e o destrutor do
gestor faria `delete` outra vez.

---

### 5.2 `AddPessoa(Pessoa *P)` (3b) — `src/Gestao.cpp:56`

```cpp
bool Gestao::AddPessoa(Pessoa *P)
{
    if (P == nullptr)
        return false;
    // nomes repetidos nao: o Remover(nome) e o PesquisarPessoa assumem nome unico
    if (PesquisarPessoa(P->GetNome()) != nullptr)
        return false;

    // o operator[] do map cria a lista vazia se a cidade ainda nao existir
    DADOS[P->GetCidade()].push_back(P);
    return true;
}
```

**O que faz:** recebe uma pessoa **já criada** (com `new`, por quem chama) e
guarda-a na estrutura.

Passo a passo:

1. **`P == nullptr` → `false`.** Não se pode guardar "nada". Se não
   verificássemos, `P->GetNome()` rebentava o programa.
2. **Nome repetido → `false`.** Pergunta à pessoa o nome (`P->GetNome()`) e usa
   o `PesquisarPessoa` para ver se já existe alguém com esse nome. Porquê
   proibir repetidos? Porque o `PesquisarPessoa(nome)` e o `Remover(nome)`
   trabalham **por nome** — se houvesse dois "Luis Miguel", só conseguiríamos
   chegar ao primeiro; o segundo ficava inacessível por nome.
3. **`DADOS[P->GetCidade()].push_back(P);`** — esta linha faz muita coisa:
   - `P->GetCidade()` → pergunta à pessoa em que cidade vive (ex.: `"Braga"`).
   - `DADOS["Braga"]` → se `"Braga"` **já existe** no map, devolve a sua lista;
     se **não existe**, o `operator[]` **cria** a entrada `"Braga"` com uma lista
     vazia e devolve-a. Não precisamos de um `if` para tratar a cidade nova.
   - `.push_back(P)` → acrescenta o ponteiro no fim dessa lista.
4. `return true;`

**Regra de propriedade (muito importante):** a partir do momento em que este
método devolve `true`, **o `Gestao` passa a ser o dono da pessoa**. É ele que
fará o `delete` (no `Remover` ou no destrutor). Quem chamou **não deve** fazer
`delete P`.

Se devolver `false`, a pessoa **não** ficou guardada, e **continua a ser
responsabilidade de quem chamou** — que deve fazer `delete P` para não perder
memória. É exatamente isso que a 3a faz.

---

### 5.3 `AddPessoa(string, string, int)` (3a) — `src/Gestao.cpp:44`

```cpp
bool Gestao::AddPessoa(string _nome, string _cidade, int _idade)
{
    Pessoa *P = new Pessoa(_nome, _cidade, _idade);
    if (!AddPessoa(P))
    {
        delete P; // nao ficou guardada, senao era memoria perdida
        return false;
    }
    return true;
}
```

**O que faz:** recebe os **dados soltos**, cria a pessoa e guarda-a.

1. `new Pessoa(_nome, _cidade, _idade)` — cria a pessoa no **heap** (tem de ser
   no heap porque tem de continuar a existir depois de este método terminar).
   Corre o construtor com parâmetros da `Pessoa`.
2. `AddPessoa(P)` — **reutiliza a 3b**. O compilador sabe que é a outra versão
   porque o argumento é um `Pessoa *` (sobrecarga, ver 1.5).
3. Se a 3b recusar (nome repetido), a pessoa não ficou em lado nenhum → **temos
   de fazer `delete P`**, senão era memória perdida (ninguém mais sabe aquela
   morada).

**Porque reutilizar a 3b em vez de repetir o código?** Porque assim as regras
(nada de `nullptr`, nada de repetidos, escolher a lista pela cidade) estão
escritas **num só sítio**. Se um dia mudarem, só mudam ali.

---

### 5.4 `Trim` (função auxiliar) — `src/Gestao.cpp:8`

```cpp
static string Trim(const string &s)
{
    const string espacos = " \t\r\n";
    size_t ini = s.find_first_not_of(espacos);
    if (ini == string::npos)
        return "";
    size_t fim = s.find_last_not_of(espacos);
    return s.substr(ini, fim - ini + 1);
}
```

**O que faz:** devolve a string **sem espaços no início e no fim**.
`"  Carlos Lopes Andrade "` → `"Carlos Lopes Andrade"`.

**Porque é precisa?** O ficheiro tem linhas como
`Viseu; Carlos Lopes Andrade; 30`. Partindo pelo `;`, o nome viria
`" Carlos Lopes Andrade"` (com um espaço à frente). Depois,
`PesquisarPessoa("Carlos Lopes Andrade")` **não o encontraria**, porque
`" Carlos..."` e `"Carlos..."` são strings diferentes.

Como funciona:

- `espacos` é a lista de caracteres a remover: espaço, tab (`\t`), e os
  caracteres de fim de linha `\r` e `\n`.
  - **Porquê o `\r`?** Os ficheiros criados no Windows terminam cada linha com
    `\r\n`. O `getline` tira o `\n` mas deixa o `\r` agarrado ao fim. Sem o
    tirar, a idade `"25\r"` ou a última palavra ficavam com um carácter
    invisível.
- `find_first_not_of` → posição do **primeiro** carácter que não é espaço.
- Se não houver nenhum (`string::npos` = "não encontrado"), a string era só
  espaços → devolve `""`.
- `find_last_not_of` → posição do **último** carácter que não é espaço.
- `substr(ini, comprimento)` → recorta o pedaço do meio.

É `static` porque é só uma ajuda interna do `Gestao.cpp` (ver 1.19).

---

### 5.5 `LerFicheiro` (3c) — `src/Gestao.cpp:70`

```cpp
bool Gestao::LerFicheiro(string nome_ficheiro)
{
    ifstream F(nome_ficheiro);
    if (!F.is_open())
    {
        cout << "Erro ao abrir o ficheiro <" << nome_ficheiro << ">" << endl;
        return false;
    }

    string linha;
    while (getline(F, linha))
    {
        if (Trim(linha).empty())
            continue;

        stringstream ss(linha);
        string cidade, nome, s_idade;
        getline(ss, cidade, ';');
        getline(ss, nome, ';');
        getline(ss, s_idade);   // o resto da linha (pode estar vazio)

        cidade = Trim(cidade);
        nome = Trim(nome);
        s_idade = Trim(s_idade);
        if (cidade.empty() || nome.empty())
            continue; // linha mal formada

        int idade = 18;
        if (!s_idade.empty())
        {
            try { idade = stoi(s_idade); }
            catch (...) { idade = 18; } // idade invalida -> 18
        }
        AddPessoa(nome, cidade, idade);
    }
    F.close();
    return true;
}
```

Passo a passo:

1. **Abrir o ficheiro.** `ifstream F(nome_ficheiro);` tenta abrir para leitura.
   Se não conseguir (o ficheiro não existe, nome errado, pasta errada),
   `is_open()` dá `false` → avisamos e devolvemos `false`.
2. **Ler linha a linha.** `while (getline(F, linha))` — o `getline` devolve
   "verdadeiro" enquanto conseguir ler; quando chega ao fim do ficheiro, o ciclo
   termina.
3. **Saltar linhas vazias.** `continue` passa logo à próxima volta do ciclo.
   (Um ficheiro com uma linha em branco no fim é muito comum.)
4. **Partir a linha pelos `;`.** Pomos a linha num `stringstream` para a poder
   ler "como se fosse um ficheiro":
   - `getline(ss, cidade, ';')` → lê até ao 1.º `;` → `"Porto"`.
   - `getline(ss, nome, ';')` → lê até ao 2.º `;` → `" Pedro Miguel"`.
   - `getline(ss, s_idade)` → lê **o resto da linha** → `""` (vazio, neste
     caso).
5. **Limpar espaços** com `Trim` nos três campos.
6. **Validar.** Sem cidade ou sem nome não dá para criar uma pessoa → ignora a
   linha.
7. **A idade:**
   - Começa em **18** (regra da ficha).
   - Se o texto da idade não estiver vazio, converte-o para número com
     `stoi` ("string to int").
   - Se o texto não for um número (`"abc"`), o `stoi` **lança uma exceção**
     (um sinal de erro que, se ninguém o tratar, termina o programa). O
     `try { ... } catch (...) { ... }` apanha esse erro e, em vez de rebentar,
     usa 18. (Exceções ainda não foram dadas em detalhe; a ideia é só esta:
     "tenta isto; se der erro, faz aquilo".)
8. **`AddPessoa(nome, cidade, idade);`** — usa a **3a**, que cria a pessoa com
   `new` e a guarda (ou faz `delete` se o nome for repetido). Assim o
   `LerFicheiro` não precisa de saber nada sobre memória nem sobre o `map` —
   delega tudo.
9. `F.close()` e `return true`.

Exemplo de como cada linha do ficheiro é tratada:

| Linha | cidade | nome | s_idade | idade final |
|---|---|---|---|---|
| `Viseu;Luis Miguel;25` | `Viseu` | `Luis Miguel` | `25` | 25 |
| `Viseu; Carlos Lopes Andrade; 30` | `Viseu` | `Carlos Lopes Andrade` | `30` | 30 |
| `Porto; Pedro Miguel;` | `Porto` | `Pedro Miguel` | *(vazio)* | **18** |

**Nota:** o `LerFicheiro` **acrescenta** aos dados que já existem; não apaga o
que lá estava. Se o mesmo ficheiro for lido duas vezes, a segunda leitura não
acrescenta nada, porque todos os nomes já existem (a 3b recusa-os e a 3a faz o
`delete` de cada um).

---

### 5.6 `Mostrar` (3d) — `src/Gestao.cpp:110`

```cpp
void Gestao::Mostrar()
{
    if (DADOS.empty())
    {
        cout << "(sem dados)" << endl;
        return;
    }
    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        cout << i_m->first << " [" << i_m->second.size() << " pessoa(s)]" << endl;
        for (list<Pessoa *>::iterator i_l = i_m->second.begin(); i_l != i_m->second.end(); i_l++)
            (*i_l)->Show();
    }
}
```

1. Se não há dados, avisa e sai.
2. Para cada cidade, escreve o **nome** (`i_m->first`) e **quantas pessoas
   tem** (`i_m->second.size()`).
3. Para cada pessoa dessa cidade, **pede à própria pessoa que se mostre**:
   `(*i_l)->Show()`.

Resultado:

```
Porto [3 pessoa(s)]
  Antonio Lopes (35 anos) - Porto
  Pedro Miguel (18 anos) - Porto
  Joana Monteiro (23 anos) - Porto
```

**Divisão de trabalho:** o `Gestao` sabe **a organização** (cidades, quantas
pessoas, por que ordem). A `Pessoa` sabe **como se apresenta**. O gestor não
precisa de saber o formato de uma pessoa — se quiseres mudar como uma pessoa
aparece no ecrã, mudas só o `Show`.

---

### 5.7 `Remover(Pessoa *P)` (3e) — `src/Gestao.cpp:127`

```cpp
bool Gestao::Remover(Pessoa *P)
{
    if (P == nullptr)
        return false;

    map<string, list<Pessoa *>>::iterator i_m = DADOS.find(P->GetCidade());
    if (i_m == DADOS.end())
        return false;

    list<Pessoa *> &lp = i_m->second;
    for (list<Pessoa *>::iterator i_l = lp.begin(); i_l != lp.end(); i_l++)
    {
        if (*i_l == P)
        {
            lp.erase(i_l);      // tira o ponteiro da lista
            delete P;           // apaga a pessoa
            if (lp.empty())     // cidade sem ninguem sai do map
                DADOS.erase(i_m);
            return true;
        }
    }
    return false;
}
```

**O que faz:** tira a pessoa da estrutura **e apaga-a da memória**.

Passo a passo:

1. **`nullptr` → `false`.** Muito importante por causa da 3f (ver 5.8).
2. **Ir diretamente à cidade certa.** Pergunta à pessoa a cidade
   (`P->GetCidade()`) e usa `DADOS.find(...)`. Assim não precisa de procurar em
   todas as cidades.
   - **Porquê `find` e não `DADOS[...]`?** Porque o `[]` **criaria** uma cidade
     vazia se ela não existisse (ver 1.16). Estaríamos a "sujar" a estrutura só
     por ter tentado remover.
   - Se a cidade não existe → a pessoa não está cá → `false`.
3. **Procurar na lista comparando moradas.** `*i_l == P` compara **ponteiros**
   (endereços), não nomes. Ou seja, procura **aquela pessoa exata**, não "alguém
   com o mesmo nome".
4. Quando encontra:
   - `lp.erase(i_l)` → tira o ponteiro da lista. **A ordem importa**: depois
     de `erase`, o iterador `i_l` fica inválido — por isso, a seguir, não o
     voltamos a usar (fazemos `return` logo).
   - `delete P` → apaga a pessoa (corre o `~Pessoa()`). Tem de ser feito
     **aqui**: depois de tirar o ponteiro da lista, o `Gestao` já não teria
     forma de saber que esta pessoa existia, e o destrutor do gestor nunca a
     apagaria.
   - Se a cidade ficou sem ninguém, **remove a cidade do map**. Senão, o
     `Mostrar` mostraria `Lisboa [0 pessoa(s)]`, e uma cidade com 0 pessoas não
     faz sentido no dicionário.
   - `return true`.
5. Se percorreu a lista toda e não encontrou → `false`.

> ⚠️ **Depois de `Remover(P)` devolver `true`, `P` é um dangling pointer.** A
> variável de quem chamou continua com a mesma morada, mas a pessoa foi apagada.
> Não se deve usar `P` para mais nada.

---

### 5.8 `Remover(string _nome)` (3f) — `src/Gestao.cpp:152`

```cpp
bool Gestao::Remover(string _nome)
{
    return Remover(PesquisarPessoa(_nome));
}
```

Uma linha só, porque **reaproveita duas alíneas**:

1. `PesquisarPessoa(_nome)` (3h) encontra a pessoa e devolve o ponteiro (ou
   `nullptr` se não existir).
2. `Remover(...)` (3e) remove-a e apaga-a.

E se o nome não existir? O `PesquisarPessoa` devolve `nullptr`, que é passado
ao `Remover(Pessoa *)`, cuja **primeira linha** é
`if (P == nullptr) return false;`. Por isso não há rebentamento e o resultado
é `false` — é por isto que essa verificação é tão importante.

---

### 5.9 `GravarFicheiro` (3g) — `src/Gestao.cpp:158`

```cpp
bool Gestao::GravarFicheiro(string ficheiro)
{
    ofstream F(ficheiro);
    if (!F.is_open())
        return false;

    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
        for (list<Pessoa *>::iterator i_l = i_m->second.begin(); i_l != i_m->second.end(); i_l++)
            (*i_l)->Gravar(F);

    F.close();
    return true;
}
```

1. `ofstream F(ficheiro)` abre o ficheiro **para escrita**. Se já existir, o
   conteúdo antigo é **apagado** (é o comportamento normal do `ofstream`). É o
   que queremos: o ficheiro passa a refletir exatamente o estado atual.
2. Se não conseguir abrir (por exemplo, pasta sem permissões) → `false`.
3. Percorre todas as cidades e todas as pessoas e, **para cada pessoa, pede-lhe
   que se escreva no ficheiro**: `(*i_l)->Gravar(F)`.
4. Fecha e devolve `true`.

O formato gravado é **o mesmo do ficheiro de entrada** (`Cidade;Nome;Idade`),
por isso o ficheiro gravado pode ser lido de novo com `LerFicheiro`.

A relação entre este método e o `Pessoa::Gravar` tem uma secção própria — a 6.

---

### 5.10 `GetCidadeMaisHabitantes` (3i) — `src/Gestao.cpp:196`

```cpp
string Gestao::GetCidadeMaisHabitantes()
{
    map<string, list<Pessoa *>>::iterator i_m;
    string CidadeMais = "";
    size_t MAX = 0;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        if (i_m->second.size() > MAX)
        {
            MAX = i_m->second.size();
            CidadeMais = i_m->first;
        }
    }
    return CidadeMais;
}
```

É o algoritmo clássico de **procurar o máximo**:

1. Começa com "o melhor até agora" = nenhuma cidade (`""`) com 0 habitantes.
2. Para cada cidade, se o tamanho da lista for **maior** do que o melhor até
   agora, essa cidade passa a ser a melhor.
3. No fim, devolve a melhor.

Notas:

- **Não é preciso contar pessoas à mão** — a estrutura escolhida já tem uma
  lista por cidade, basta `size()`. Esta é uma das vantagens da escolha da
  secção 2.3.
- **Empates:** como se usa `>` (e não `>=`), em caso de empate fica a
  **primeira** cidade por ordem alfabética.
- **Sem dados:** devolve `""` (string vazia).
- Este método **não interage com a `Pessoa`** — só olha para o tamanho das
  listas.

---

## 6. `Gravar` vs `GravarFicheiro`

Há dois métodos com "Gravar" no nome, um em cada classe. Faz sentido ter os
dois, porque **cada um tem uma responsabilidade diferente**: a `Pessoa` sabe
**como** se escreve, o `Gestao` sabe **onde** e **quem**.

### 6.1 As diferenças

| | `Pessoa::Gravar(ostream &f)` | `Gestao::GravarFicheiro(string ficheiro)` |
|---|---|---|
| **Responsabilidade** | Escrever **uma** pessoa no formato `Cidade;Nome;Idade` | Gravar **todas** as pessoas num ficheiro |
| **Conhece** | Os seus próprios atributos (`NOME`, `CIDADE`, `IDADE`) | O nome do ficheiro e a estrutura `DADOS` (map + listas) |
| **Abre ficheiros?** | **Não** — escreve no canal que lhe dão | **Sim** — abre **uma vez**, percorre tudo, fecha |
| **Devolve** | `void` | `bool` (conseguiu abrir o ficheiro?) |

Na prática colaboram assim:

```cpp
ofstream F(ficheiro);                 // Gestao: abre UMA vez
for (cada cidade) for (cada pessoa P)
    P->Gravar(F);                     // Pessoa: escreve a SUA linha
```

Sequência quando o `main` chama `G.GravarFicheiro("pessoas_out.txt")`:

```
main                    Gestao                         Pessoa (cada uma)
 │                        │                                  │
 │ GravarFicheiro("..")   │                                  │
 │──────────────────────► │                                  │
 │                        │ abre ofstream F                  │
 │                        │                                  │
 │                        │ para cada pessoa:                │
 │                        │    Gravar(F) ──────────────────► │
 │                        │                                  │ F << CIDADE << ";"
 │                        │                                  │   << NOME << ";"
 │                        │                                  │   << IDADE << endl
 │                        │ ◄──────────────────────────────  │
 │                        │                                  │
 │                        │ fecha F                          │
 │ ◄──── true ─────────── │                                  │
```

### 6.2 Porque não pôr tudo num só sítio?

**Opção A — tudo no `Gestao`:**

```cpp
F << (*i_l)->GetCidade() << ";" << (*i_l)->GetNome() << ";" << (*i_l)->GetIdade() << endl;
```

Funciona, mas o `Gestao` passa a decidir **o formato de uma `Pessoa`**. Se a
`Pessoa` ganhasse um campo novo (ex.: email), terias de mudar o código do
`Gestao`. Com `Gravar`, cada classe trata dos seus próprios dados —
**encapsulamento**, uma ideia central da POO.

**Opção B — tudo na `Pessoa`** (era o `gravarFicheiro(string nf)` do código
original): cada pessoa abria o ficheiro sozinha. Problemas:

- Abrindo com `ofstream F(nf)` (modo normal), **cada pessoa apagava o que a
  anterior escreveu** → no fim só ficava a última pessoa no ficheiro.
- Abrindo em modo "acrescentar" (`ios::app`), funcionava, mas:
  - abria e fechava o ficheiro **N vezes** (lento);
  - gravar duas vezes seguidas **duplicava** os dados em vez de os substituir
    (alguém teria de apagar o ficheiro antes — e quem? A pessoa não sabe se é a
    primeira).

### 6.3 Vantagem extra do `ostream &`

Como o `Gravar` recebe um `ostream` genérico (e não um nome de ficheiro), o
**mesmo método** escreve em qualquer sítio:

```cpp
P->Gravar(F);      // num ficheiro (ofstream é um ostream)
P->Gravar(cout);   // no ecrã, no formato do ficheiro — útil para debug
```

Resumindo: **o `Gravar` é o tijolo, o `GravarFicheiro` é a parede.**

### 6.4 E o `Show`? Não é a mesma coisa que `Gravar(cout)`?

Não. Têm destinatários diferentes:

| | `Show()` | `Gravar(cout)` |
|---|---|---|
| Para quem | Para uma **pessoa** ler | Para o **programa** voltar a ler |
| Formato | `  Pedro Miguel (18 anos) - Porto` | `Porto;Pedro Miguel;18` |
| Pode mudar à vontade? | Sim, é só estética | **Não** — tem de bater certo com o `LerFicheiro` |

Por isso faz sentido manter os dois.

---

## 7. Todas as interações entre `Pessoa` e `Gestao`

### 7.1 Visão geral

```
         ┌──────────────────────────────────────────┐
         │                 Gestao                   │
         │  private: map<string,list<Pessoa*>> DADOS│
         │                                          │
         │  - organiza (por cidade)                 │
         │  - cria (new) e apaga (delete) pessoas   │
         │  - pesquisa, remove, mostra, grava       │
         └───────────────┬──────────────────────────┘
                         │  conhece e usa
                         │  (só pelos métodos public)
                         ▼
         ┌──────────────────────────────────────────┐
         │                 Pessoa                   │
         │  private: NOME, CIDADE, IDADE            │
         │                                          │
         │  - guarda os seus dados                  │
         │  - sabe mostrar-se (Show)                │
         │  - sabe escrever-se (Gravar)             │
         │  - NÃO sabe que o Gestao existe          │
         └──────────────────────────────────────────┘
```

### 7.2 Tabela completa: que método do `Gestao` usa que método da `Pessoa`

| Método do `Gestao` | Método(s) da `Pessoa` usados | Para quê |
|---|---|---|
| `AddPessoa(nome,cidade,idade)` (3a) | **Construtor** `Pessoa(...)` via `new`; **destrutor** via `delete` (se recusada) | Criar a pessoa; apagá-la se não puder ser guardada |
| `AddPessoa(Pessoa *P)` (3b) | `GetNome()` | Verificar se o nome já existe |
| | `GetCidade()` | Escolher a lista onde a pôr |
| `LerFicheiro` (3c) | *(nenhum diretamente)* | Delega na 3a |
| `Mostrar` (3d) | `Show()` | Cada pessoa mostra-se a si própria |
| `Remover(Pessoa *P)` (3e) | `GetCidade()` | Ir diretamente à lista certa |
| | **destrutor** via `delete` | Apagar a pessoa |
| `Remover(string)` (3f) | *(nenhum diretamente)* | Delega na 3h e na 3e |
| `GravarFicheiro` (3g) | `Gravar(ostream&)` | Cada pessoa escreve a sua linha |
| `PesquisarPessoa` (3h) | `GetNome()` | Comparar com o nome procurado |
| `GetCidadeMaisHabitantes` (3i) | *(nenhum)* | Só usa o `size()` das listas |
| `~Gestao()` (destrutor) | **destrutor** via `delete` | Apagar todas as pessoas restantes |

Repara que **o `Gestao` nunca mexe diretamente em `NOME`, `CIDADE` ou
`IDADE`** — só usa os métodos `public` da `Pessoa`. E os setters da `Pessoa`
**não são usados pelo `Gestao`** (ver o aviso em 3.4).

### 7.3 Quem é o dono de cada pessoa (propriedade da memória)

A regra da solução é simples: **toda a pessoa que está dentro do `Gestao`
pertence ao `Gestao`.** Só o `Gestao` faz `delete` dessas pessoas.

| Situação | Quem é o dono | Quem faz `delete` |
|---|---|---|
| `main` fez `new Pessoa(...)` mas ainda não a deu ao gestor | O `main` | O `main` |
| `AddPessoa(P)` devolveu **`true`** | **O `Gestao`** | `Remover` ou `~Gestao` |
| `AddPessoa(P)` devolveu **`false`** | Continua a ser quem chamou | Quem chamou |
| `AddPessoa(nome,cidade,idade)` (3a) | Tratado internamente | A própria 3a faz `delete` se for recusada |
| Ponteiro devolvido por `PesquisarPessoa` | **Continua a ser o `Gestao`** (é só um "empréstimo" para consultar) | **Ninguém fora do gestor** |
| Depois de `Remover(P)` devolver `true` | Já não existe | — (já foi apagada; `P` está dangling) |

### 7.4 O ciclo de vida completo de uma pessoa

Exemplo: a linha `Porto; Pedro Miguel;` do ficheiro.

```
 1. main chama G.LerFicheiro("pessoas.txt")
 2.   LerFicheiro lê a linha, parte-a, faz Trim → "Porto", "Pedro Miguel", ""
 3.   idade vazia → idade = 18
 4.   LerFicheiro chama AddPessoa("Pedro Miguel", "Porto", 18)        [3a]
 5.     3a faz new Pessoa(...)            → nasce no heap (construtor)
 6.     3a chama AddPessoa(P)                                          [3b]
 7.       3b chama P->GetNome()           → "Pedro Miguel"
 8.       3b chama PesquisarPessoa(...)   → nullptr (não há repetido)  [3h]
 9.       3b chama P->GetCidade()         → "Porto"
10.       3b faz DADOS["Porto"].push_back(P)
11.       3b devolve true                 → a partir daqui o dono é o Gestao
12.     3a devolve true
...
13. main chama G.Mostrar()                                              [3d]
14.   Mostrar chama P->Show()             → "  Pedro Miguel (18 anos) - Porto"
...
15. main chama G.GravarFicheiro("pessoas_out.txt")                      [3g]
16.   GravarFicheiro chama P->Gravar(F)   → "Porto;Pedro Miguel;18"
...
17. main termina → G sai de âmbito → ~Gestao()
18.   ~Gestao faz delete P                → morre (destrutor ~Pessoa)
```

Se, pelo caminho, alguém tivesse chamado `G.Remover("Pedro Miguel")`, a pessoa
morreria no passo do `Remover` (`delete P`), e o `~Gestao` já não a
encontraria na lista — por isso **nunca há double delete**.

### 7.5 Porque é que a `Pessoa` não conhece o `Gestao`?

Porque não precisa. Uma pessoa é só dados + como se mostra/escreve. Se a
`Pessoa` incluísse o `Gestao.h`, ficariam as duas classes "coladas" (para usar
uma, era preciso a outra), e não podias reutilizar a `Pessoa` noutra ficha. Uma
dependência num só sentido torna o código mais simples de perceber e de mudar.

---

## 8. O `main.cpp`, passo a passo

(Até antes da ordenação.)

```cpp
Gestao G;
```

Cria o gestor no stack. Corre o construtor (vazio). `DADOS` começa vazio.
Quando o `main` terminar, corre o destrutor automaticamente.

### 8.1 Ler e mostrar

```cpp
if (!G.LerFicheiro("pessoas.txt"))
    return 1;
G.Mostrar();
```

Se o ficheiro não abrir, termina com código de erro 1. O ficheiro tem de estar
na pasta onde o programa corre — no Code::Blocks é a pasta do projeto, que é
onde está o `pessoas.txt`.

Output:

```
Coimbra [1 pessoa(s)]
  Miguel Sales (24 anos) - Coimbra
Lisboa [1 pessoa(s)]
  Jose Francisco (45 anos) - Lisboa
Porto [3 pessoa(s)]
  Antonio Lopes (35 anos) - Porto
  Pedro Miguel (18 anos) - Porto
  Joana Monteiro (23 anos) - Porto
Viseu [2 pessoa(s)]
  Luis Miguel (25 anos) - Viseu
  Carlos Lopes Andrade (30 anos) - Viseu
```

Repara: cidades por **ordem alfabética** (o `map` ordena as chaves), pessoas
pela **ordem do ficheiro** (`push_back` acrescenta no fim), Pedro Miguel com
**18** (idade em falta) e nomes **sem espaços a mais** (graças ao `Trim`).

### 8.2 Adicionar

```cpp
G.AddPessoa("Ana Silva", "Viseu", 40)                 // 3a → 1 (true)
G.AddPessoa(new Pessoa("Rui Costa", "Braga", 19))     // 3b → 1 (true), cria "Braga"
G.AddPessoa("Luis Miguel", "Porto", 50)               // 3a → 0 (false), nome repetido
```

- `cout` mostra `true` como `1` e `false` como `0`.
- **Rui Costa:** a cidade "Braga" não existia; o `DADOS["Braga"]` cria-a.
- **Luis Miguel repetido:** a 3b recusa; a 3a faz `delete` da pessoa que tinha
  acabado de criar → sem memória perdida.
- **Sobre `AddPessoa(new Pessoa(...))`:** aqui é seguro porque sabemos que o
  Rui Costa não é repetido. Em geral, se a 3b pudesse devolver `false`, o
  ponteiro criado com `new` perdia-se (não ficou guardado em variável nenhuma).
  A forma totalmente segura seria:
  ```cpp
  Pessoa *P = new Pessoa("Rui Costa", "Braga", 19);
  if (!G.AddPessoa(P))
      delete P;
  ```

### 8.3 Pesquisar

```cpp
Pessoa *P = G.PesquisarPessoa("Pedro Miguel");
if (P) P->Show();
```

`if (P)` é o mesmo que `if (P != nullptr)`. Mostra
`Pedro Miguel (18 anos) - Porto`. Pesquisar `"Ninguem"` devolve `nullptr` →
`nao encontrou`.

### 8.4 Cidade com mais habitantes

Neste momento: Coimbra 1, Lisboa 1, Porto 3, Viseu 3 (Ana Silva foi
adicionada), Braga 1. Porto e Viseu empatam com 3; como "Porto" vem primeiro
por ordem alfabética e o código usa `>`, o resultado é **`Porto`**.

### 8.5 Remover

```cpp
G.Remover("Jose Francisco")                  // 3f → 1. Lisboa fica vazia → sai do map
G.Remover(G.PesquisarPessoa("Rui Costa"))    // 3e → 1. Braga fica vazia → sai do map
G.Remover("Ninguem")                         // 3f → 0. Pesquisar dá nullptr → Remover(nullptr) → false
```

### 8.6 Gravar

```cpp
G.GravarFicheiro("pessoas_out.txt")          // 3g → 1
```

Cria o ficheiro `pessoas_out.txt` com uma linha `Cidade;Nome;Idade` por pessoa.

### 8.7 Fim

```cpp
return 0;
```

`G` deixa de existir → corre `~Gestao()` → aparece `passei em <~Gestao>` e
todas as pessoas que restam são apagadas.

O programa foi testado com ferramentas que detetam erros de memória
(*AddressSanitizer* / *LeakSanitizer*): **nenhuma memória perdida e nenhum
acesso inválido**.

---

## 9. Os erros que existiam no código original e porque foram corrigidos

### 9.1 `Pessoa.h`

| Original | Problema | Correção |
|---|---|---|
| Sem `#include <string>` nem `using namespace std` | O compilador não sabe o que é `string` → erro | Adicionados |
| `Pessoa(_nome,_cidade,_idade);` | Em C++, todo o parâmetro precisa de **tipo** → erro | `Pessoa(string _nome, string _cidade, int _idade = 18)` |
| Sem getters | Os atributos são `private`; o `Gestao.cpp` usava `GetNome()`, que não existia → erro | Adicionados `GetNome/GetCidade/GetIdade` |
| `setNOME`, `setCIDADE`, `setIDADE` | Não é erro, mas era inconsistente com `GetNome` | Renomeados para `SetNome/SetCidade/SetIdade` |
| `gravarFicheiro(string nf)` | Cada pessoa abria o ficheiro (ver 6.2) | Substituído por `Gravar(ostream &f)` |
| Métodos declarados sem implementação | Erro do linker (`undefined reference`) assim que fossem usados | Implementados |
| Construtor por defeito não inicializava `IDADE` | `int` ficava com lixo | `IDADE = 18` |

### 9.2 `Gestao.h`

| Original | Problema | Correção |
|---|---|---|
| Sem o atributo `DADOS` | O `.cpp` usava `DADOS`, que não existia → erro | `map<string, list<Pessoa *>> DADOS;` |
| Sem includes | `map`, `list`, `string`, `Pessoa` desconhecidos | Adicionados |
| Métodos do `.cpp` não declarados na classe | Erro: não se pode definir um método que a classe não declara | Declarados |
| Cópia permitida | Double delete se alguém copiasse o gestor (ver 4.4) | `= delete` |

### 9.3 `Gestao.cpp`

**Destrutor:**

| Original | Problema |
|---|---|
| `map<...>::iterador` | O nome correto é `iterator` (inglês) |
| `it != DADOS.end` | Faltam os `()` — `end` é um método, tem de ser chamado |
| `list<<Pessoa *>>` | Sintaxe errada: é `list<Pessoa *>` |
| `delete(*i_l)` sem `;` | Falta o `;` → erro de sintaxe |
| `cout` sem include | Resolvido pelo `#include <iostream>` (via `Pessoa.h`) |

**`PesquisaPessoa` → `PesquisarPessoa`:**

| Original | Problema |
|---|---|
| Nome `PesquisaPessoa` | O enunciado pede `PesquisarPessoa` |
| `list <Pesoa*>` | Erro de escrita: `Pessoa` |
| `list<Pessoa*> lp = i_m->second;` | **Copiava** a lista inteira em cada cidade → passou a referência `&lp` |
| `i_l` sem declaração | Variável não declarada → erro |
| `return null;` | `null` não existe em C++ → `nullptr` |

**`GetCidadeMaisHabitantes`** — três erros graves:

| Original | Problema |
|---|---|
| `:::` | Erro de sintaxe, é `::` |
| `i_m =! DADOS.end()` | **Não é uma comparação!** Lê-se `i_m = (!DADOS.end())` — uma **atribuição**. O correto é `!=` |
| `int MAX;` | Não inicializado → começa com lixo → a comparação `size() > MAX` dá resultados imprevisíveis. Passou a `size_t MAX = 0` |
| Sem `return` | Uma função que promete devolver `string` e não devolve tem **comportamento indefinido** (pode rebentar ou devolver lixo) |

**Encoding:** o ficheiro estava em Latin-1 com acentos nos comentários. Os
comentários passaram a ASCII (sem acentos), para não aparecerem caracteres
estranhos no Code::Blocks.

### 9.4 Projeto

Não havia `main.cpp` → sem `main` o linker não consegue criar o executável.
Foi criado e adicionado ao `Ficha02.cbp`.

---

## 10. Perguntas de revisão (com resposta)

**1. Porque é que a lista guarda `Pessoa *` e não `Pessoa`?**
Para o `PesquisarPessoa` devolver a pessoa verdadeira, para `Remover(P)`
poder encontrar exatamente aquela pessoa comparando moradas, e para não copiar
pessoas. O custo é termos de gerir a memória (`new`/`delete`).

**2. Se o destrutor do `Gestao` estivesse vazio, o que acontecia?**
As listas seriam destruídas, mas só apagariam os ponteiros, não as pessoas.
Todas as pessoas ficariam no heap → memory leak.

**3. Porque é que a 3a faz `delete P` quando a 3b devolve `false`?**
Porque a pessoa foi criada com `new` mas não ficou guardada em lado nenhum. Se
não fosse apagada ali, ninguém mais teria a morada dela → memória perdida.

**4. Qual a diferença entre `DADOS["Porto"]` e `DADOS.find("Porto")`?**
O `[]` cria a entrada se não existir; o `find` não cria nada e devolve
`end()` se não encontrar. Usamos `[]` no `AddPessoa` (queremos criar) e `find`
no `Remover` (não queremos criar).

**5. O que acontece em `G.Remover("Ninguem")`?**
`PesquisarPessoa` devolve `nullptr` → `Remover(nullptr)` → a primeira linha
verifica `nullptr` e devolve `false`. Sem essa verificação, `P->GetCidade()`
rebentava o programa.

**6. Porque é que `Remover` apaga a cidade quando a lista fica vazia?**
Para o dicionário não ter cidades com 0 pessoas (apareceriam no `Mostrar` e
não fazem sentido).

**7. Posso fazer `delete G.PesquisarPessoa("Ana Silva");`?**
Não. A pessoa continua dentro da lista; mais tarde o `~Gestao` faria `delete`
da mesma morada → double delete. Para apagar uma pessoa, usa-se sempre
`G.Remover(...)`.

**8. Porque é que o `Gestao` não pode ser copiado?**
Porque a cópia ficaria com os mesmos ponteiros; os dois destrutores fariam
`delete` das mesmas pessoas → double delete.

**9. Porque existe `Gravar` na `Pessoa` e `GravarFicheiro` no `Gestao`?**
A `Pessoa` sabe **como** se escreve (formato); o `Gestao` sabe **onde** (o
ficheiro, aberto uma só vez) e **quem** (todas as pessoas da estrutura). Ver
secção 6.

**10. Porque é preciso o `Trim`?**
Por causa dos espaços depois do `;` no ficheiro e do `\r` dos ficheiros
Windows. Sem ele, `" Carlos Lopes Andrade"` não seria encontrado ao pesquisar
`"Carlos Lopes Andrade"`.

**11. Se eu mudar a cidade de uma pessoa que já está no gestor com
`SetCidade`, o que acontece?**
A pessoa diz que é da cidade nova, mas continua na lista da cidade antiga. O
`Remover(P)` deixa de a encontrar (procura na lista da cidade nova). Para
mudar de cidade é preciso um método do `Gestao` que mude a pessoa de lista
(ver o aviso em 3.4).
