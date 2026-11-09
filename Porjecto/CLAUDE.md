# Projeto POO 2026/27 — Simulador de Gestão de Fábrica (C++)

Projeto de grupo da cadeira de Programação Orientada a Objetos (ESTGV). Simulador de
consola de uma fábrica com motores, sensores e utilizadores, configurado por XML.

## PRINCÍPIO N.º 1 — Trabalhar como na Ficha 5 (Aquário/Peixe)

O projeto tem de seguir **o tipo de trabalho e o estilo da resolução da Ficha 5**, feita
nas aulas com o docente. É o que vai ser avaliado.

- **Antes de implementar qualquer coisa, ler a referência:**
  - o código em `/mnt/f/POO/Teorica 5.2/` (`F:\POO\Teorica 5.2`): `include/`, `src/`,
    `main.cpp`, `Uteis.h`;
  - a explicação em `Ficha-5-Explicacao-Detalhada.md`, na mesma pasta.

  Alguns ficheiros estão em Latin‑1: ler com `iconv -f latin1 -t utf-8`.
- **Regra de decisão:** perante qualquer dúvida de desenho, perguntar *"como é que o
  Aquário/Peixe da Ficha 5 resolveria isto?"* e fazer igual. Entre uma técnica de C++ mais
  moderna ou "esperta" e a forma ensinada na Ficha 5, escolher a da Ficha 5.

### Correspondência Ficha 5 → Projeto

| Ficha 5 | Projeto |
|---|---|
| `Aquario` (dono dos peixes, corre o ciclo, trata pedidos) | `Fabrica` |
| `Peixe` (base; vive um ciclo; decide sobre si) | `Dispositivo` → `Motor`, `Sensor` |
| `Tubarao : public Peixe` | `MCombustao : public Motor`, `SFumo : public SensorBinario`, … |
| `Aquario *Ptr_Aquario` + `Atribuir_Aquario(this)` no `Add` | `Fabrica *Ptr_Fabrica` + `Atribuir_Fabrica(this)` no `Add` |
| `Peixe::EstouGordo()` → o peixe decide sair | `Motor::ESTOU_QUENTE()` → o motor decide avisar ou parar |
| `Ptr_Aquario->QueroSair(this)` / `Preciso_de_Comer(this)` | reencaminhamentos do `Dispositivo` → `Aviso_Aquecimento`, `Registar_Alarme`, … |
| `LP_Assar` + `RemoverParaAssar()` depois do ciclo | lista de alarmes pendentes processada no fim do `Ciclo` |
| `Ja_Avisei_Tenho_Fome`, `Ja_Avisei_Quero_Sair` | `Ja_Avisei_Amarelo`, `Ja_Avisei` (sensor), flag `Avariado` |
| `Pertence(id)` (devolve ponteiro ou nulo) + `RemoverID(id)` | `Pertence_Motor(id)`, `Remover(id)` |
| `Escreve(ostream &)`, `Show()`, `QuemEs()` | `Escreve`, `EscreveXML`, `EscreveHTML` (com `ostream &`), `QuemEs()` |
| `Run()` com `while`, `TeclaPressionada()`, `Executa_Menu_Acoes()`, `wait` | `Fabrica::Run()` com tecla 'M' |
| `menu()` devolve a opção; `Executa_Menu_Acoes()` faz o `switch`; `XxxMenu()` lê dados e chama a lógica | igual |
| `Uteis` (métodos `static`) | `Uteis` |
| `~Aquario()` faz `delete` de cada peixe | `~Fabrica()` faz `delete` de motores, sensores, configs, utilizador |

### Convenções de código da Ficha 5 a manter

- **Nomes de atributos em MAIÚSCULAS**, como no enunciado e na Ficha 5: `ID`, `MARCA`,
  `PESO`, `CONSUMO_HORA`, `VALOR_AVISO`.
- **Prefixos da Ficha 5:**
  - `Ptr_` para ponteiros de ligação (`Ptr_Fabrica`);
  - `Ja_Avisei_…` para as flags de aviso;
  - `Atribuir_…` para estabelecer ligações;
  - `L…` para listas (`LM` motores, `LS` sensores, `LU` utilizadores, `LS_Alarmes`).
- **Métodos:** nomes em português, com maiúscula inicial e `_` entre palavras quando
  ajuda a ler (`Preciso_de_Comer`, `Executa_Menu_Acoes`). Os métodos do enunciado usam
  exatamente o nome do enunciado (`Get_Estado`, `ESTOU_QUENTE`, `Ranking_Dos_Fracos`).
- **Parâmetros:** prefixo `_` quando têm o nome de um atributo (`_id`, `_marca`), ou
  `this->X = X`, como no construtor do `Peixe`.
- **Contentores:**
  - `std::list<X *>` (como o `list<Peixe *>` e o `List<Motor *>` do enunciado);
  - **ponteiros simples com `new`/`delete` explícitos**: nada de *smart pointers*, porque
    a gestão de memória explícita é avaliada;
  - iteradores escritos por extenso (`list<Motor *>::iterator it`), com `++it`.
- **Construtor da classe base** chamado na lista de inicialização (`: Peixe(_id, _especie, PESO)`).
- **`virtual`** nos métodos que as subclasses redefinem, e **destrutor virtual** na base.
- **Pesquisa** num único método (`Pertence`) que devolve o ponteiro ou nulo, reutilizado
  por todos os outros. Verificar sempre o nulo antes de usar.
- **Lógica separada da interação com o utilizador:** `Pertence(id)` nunca lê do teclado;
  quem lê é o `PertenceMenu()`.
- **Escrita** sempre com `ostream &` (ecrã ou ficheiro com o mesmo método), e
  `if (!f.is_open()) return false;` ao abrir ficheiros.
- **Ordenações** com uma função de comparação `static` (sem lambdas).
- **Comentários em português que explicam o porquê**, como os da resolução da Ficha 5
  (por exemplo, o comentário do `~Aquario` sobre a fuga de memória, ou
  `// Errado viola encapsulamento…`). Para além disso, comentários Doxygen.
- **Nulo:** usar `nullptr` (o equivalente C++11 do `NULL` da Ficha 5), de forma consistente.

### O que NÃO copiar da Ficha 5

- Os erros listados em `Boas-Praticas-POO.md`, secção 17: `case` sem `break`, nome de
  ficheiro ignorado, código HTML duplicado, `Ptr_` usado sem verificar nulo, XML inválido,
  espera ativa, código morto no `main`.
- **Getters e métodos públicos à vontade:** aqui vale a política de visibilidade (abaixo).
  Na Ficha 5, `GetID`, `QueroSair`, etc. eram públicos. No projeto, a **Fábrica** não pode
  ter públicos fora do enunciado (salvo o inventário do PRD); nas outras classes, só é
  público o que outra classe usa.

## Fontes de verdade (ler antes de implementar)

1. **`plans/PRD-Gestao-Fabrica-v2.md`** (= issue #2 no GitHub) — decisões, módulos,
   interfaces, testes. **Prevalece sobre todos os outros documentos.**
2. `Projecto_POO_26_27_Moodle.pdf` — enunciado oficial (nomes e assinaturas dos métodos públicos).
3. `instruções_utilização.pdf` + `ExemploParaSimuladorXML.xml` — formato do XML que o
   `Simulador.exe` lê (o programa escreve-o em cada ciclo).
4. **Código da Ficha 5** (`/mnt/f/POO/Teorica 5.2/`) e `Boas-Praticas-POO.md` — o estilo
   a seguir (ver Princípio n.º 1). Na parte dos getters públicos, vale a política de
   visibilidade do PRD v2.
5. Apoio à defesa: `plans/Relatorio-Melhorias-Extensibilidade.md`; `plans/Regra-Interface-Publica.md`
   (análise com `friend`, ultrapassada pela clarificação; útil só se o docente perguntar).
   `Proposta-Solucao-Projeto.md` e `plans/PRD-Gestao-Fabrica.md` (v1) estão desatualizados.

## Regras obrigatórias

- **C++11**, Code::Blocks + MinGW (Windows). Código o mais standard possível.
- **Proibido `auto`** (regra do enunciado). Iteradores escritos por extenso.
- **Sem `friend`.**
- **Visibilidade — classe `Fabrica` (regra dos docentes):** só pode ter como públicos:
  1. os métodos que o enunciado lhe atribui, com o mesmo nome e assinatura;
  2. o `Run` (exceção aceite);
  3. os métodos de colaboração do inventário do PRD (`Ciclo`, `Notificar_Gestor`,
     `Registar_Alarme`, `Aviso_Aquecimento`, `Aviso_Sobreaquecimento`, `Aviso_Avaria`), num
     bloco separado com o comentário
     `// --- Interface de colaboração (não está no enunciado; aprovada pelo docente) ---`
     e com `@note` Doxygen a dizer quem os chama.

  Tudo o resto da Fábrica é privado. **Nunca acrescentar outro método público à Fábrica**:
  se parecer necessário, parar e avisar o utilizador (tem de ser perguntado ao docente).
  O docente aprovou este inventário (2026-11-09); não há plano B.
- **Visibilidade — restantes classes** (critério do grupo):
  - é público o que outras classes usam;
  - é `protected` o que só as subclasses usam (ganchos do Template Method e
    reencaminhamentos do `Dispositivo` para a Fábrica);
  - é privado o resto.

  Em todas as classes: **sem setters**; getters só se outra classe precisa, e `const`.
- **Polimorfismo, não `if` sobre o tipo**: nada de `if`/`switch`/`dynamic_cast` sobre o
  tipo concreto. A Fábrica não inclui nem conhece classes concretas.
- **Extensibilidade**: um tipo novo de sensor/motor = **uma classe nova + uma linha no
  `Catalogo`**. Se uma alteração obrigar a mexer na `Fabrica` para suportar um tipo, o
  desenho está errado — parar e avisar.
- **Encapsulamento ao estilo Ficha 5**: o objeto decide sobre o seu estado (o motor
  decide se está quente e pára; o sensor decide se está em alarme e como reage); o objeto
  **pede/informa** a Fábrica, nunca mexe nas listas dela. Subclasses falam com a Fábrica
  só através dos reencaminhamentos protegidos do `Dispositivo`.
- **Memória** (penalização de 5 valores): cada `new` tem um dono claro que faz `delete`
  (tabela "Memória" do PRD); destrutores virtuais nas bases; classes donas de ponteiros
  não copiáveis (`= delete`); remover um objeto de todas as listas antes do `delete`.
- **Eventos durante iterações** → lista pendente processada no fim do ciclo (padrão
  `LP_Assar` da Ficha 5).
- **XML**: ler com TinyXML-2 só através de `ElementoXML`, sempre **por nome** e com valor
  por omissão (nunca por posição). Escrever com `ostream &`.
- **Código específico de Windows** (`conio.h`, `windows.h`, `system("start ...")`) só em `Uteis`.
- **Documentação Doxygen** (`/** @brief ... */`) em todas as classes, métodos e
  atributos, incluindo privados (`EXTRACT_PRIVATE = YES`).
- **Testes doctest** para o que for implementado, num alvo de compilação separado. Testar
  comportamento observável pela interface pública; nunca `Run()` (usar `Ciclo()`); vídeos
  desligados; probabilidades 0/100 nos XML de teste.
- Ficheiros em **UTF-8**. Nomes, comentários e mensagens em **português**.

## Organização

```
Porjecto/
├── main.cpp
├── include/   um .h por classe (guardas #ifndef; sem `using namespace` nos .h)
├── src/       um .cpp por classe
├── lib/       tinyxml2.h/.cpp, doctest.h  (bibliotecas externas — não alterar)
├── tests/     testes doctest + XML de teste
├── media/     vídeos (fumo, luz, míssil)
├── plans/     PRD e documentos de decisão
└── docs/      Doxyfile
```

Inclusão circular: declaração antecipada (`class Fabrica;`) nos `.h`, `#include`
completo nos `.cpp`.

## Forma de trabalhar

- **Começar cada tarefa pela Ficha 5:** identificar qual é o equivalente no Aquário/Peixe e
  explicar ao utilizador, em uma ou duas frases, como a solução segue esse padrão.
- **Uma fase/issue de cada vez.** Implementar só o que foi pedido; dizer no fim o que
  ficou feito, o que falta e como testar.
- Deixar o projeto a **compilar** e os **testes a passar** no fim de cada tarefa.
- **Não fazer commit** sem pedido explícito. Em commits, **nunca** referir o Claude como
  coautor.
- Em caso de dúvida sobre o enunciado ou uma decisão não coberta pelo PRD, **perguntar**
  em vez de inventar.
