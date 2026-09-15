# O que é um histograma

Um **histograma** é uma representação gráfica de dados através de barras, em que a altura de cada barra é proporcional ao valor que representa. É muito usado em estatística para mostrar frequências, mas aqui o conceito é aplicado de forma mais simples: cada dígito do número dado pelo utilizador passa a ser uma "barra" vertical, e a altura da barra é o valor do próprio dígito ($0$ a $9$).

Por exemplo, no número `251`:
* **1º dígito** = $2 \rightarrow$ barra com altura $2$
* **2º dígito** = $5 \rightarrow$ barra com altura $5$
* **3º dígito** = $1 \rightarrow$ barra com altura $1$

Como o dígito máximo possível é $9$, todas as barras "cabem" numa grelha fixa de 9 linhas (de baixo para cima, da linha que representa o valor $1$ até à que representa o valor $9$). Uma célula é `*` se a barra "chega" até àquela altura, e `-` caso contrário. No fim, imprime-se o próprio número (dígito a dígito) por baixo, como "eixo dos x".

---

# Resolução detalhada

## 1. Entender a lógica das barras

A parte difícil não é desenhar — é perceber como decidir, para cada célula da grelha, se deve ser `*` ou `-`.

A grelha tem 9 linhas. Vamos numerar as linhas de cima para baixo por um "limiar" (`linha`) que desce de 9 até 1:

| Linha (limiar) | Significado |
| :---: | :--- |
| **9 (topo)** | só aparece `*` se o dígito for $\ge 9$ |
| **8** | só aparece `*` se o dígito for $\ge 8$ |
| **...** | ... |
| **1 (base)** | aparece `*` se o dígito for $\ge 1$ |

Ou seja, a regra é simplesmente:

$$\text{célula}(\text{digito}, \text{linha}) = \begin{cases} \text{'*'}, & \text{se } \text{digito} \ge \text{linha} \\ \text{'-'}, & \text{caso contrário} \end{cases}$$
### Verificação com o dígito `5` (segunda coluna do exemplo `251`):
* **Linhas 9, 8, 7, 6** $\rightarrow$ $5 \ge \text{linha}$? **Não** $\rightarrow$ `-`
* **Linha 5** $\rightarrow$ $5 \ge 5$? **Sim** $\rightarrow$ `*`
* **Linhas 4, 3, 2, 1** $\rightarrow$ $5 \ge \text{linha}$? **Sim** $\rightarrow$ `*`

**Resultado da coluna do 5 (de cima para baixo):**  
`- - - - * * * * *`  
*(exatamente 4 traços seguidos de 5 asteriscos, que é o que se vê no enunciado)*.

---

## 2. O problema das "listas/vetores" proibidos

Para desenhar uma linha inteira do histograma, precisamos de percorrer todos os dígitos do número, da esquerda para a direita (o mais significativo primeiro), e isso teria de ser feito normalmente guardando os dígitos num *array*. Como isso está proibido, a alternativa é usar **recursão**: a própria pilha de chamadas faz o papel de "memória" que guarda a posição em que estamos, sem precisar de nenhuma estrutura de dados.

### A ideia:

```cpp
void ImprimeLinhaConsole(int n, int linha)
{
    if (n >= 10)
        ImprimeLinhaConsole(n / 10, linha);   // (A) trata primeiro os dígitos mais à esquerda

    int digito = n % 10;                       // (B) só depois trata o dígito atual (o mais à direita)
    cout << (digito >= linha ? '*' : '-') << " ";
}
```

### Como funciona para $n = 251$, $\text{linha} = 5$:

1. `ImprimeLinhaConsole(251, 5)` $\rightarrow$ como $251 \ge 10$, chama primeiro `ImprimeLinhaConsole(25, 5)` antes de imprimir nada.
2. `ImprimeLinhaConsole(25, 5)` $\rightarrow$ como $25 \ge 10$, chama primeiro `ImprimeLinhaConsole(2, 5)`.
3. `ImprimeLinhaConsole(2, 5)` $\rightarrow$ como $2 < 10$, não recursiona mais. Imprime $2 \pmod{10} = 2 \rightarrow 2 \ge 5$? **Não** $\rightarrow$ imprime `-`.
4. Volta à chamada (2): imprime $25 \pmod{10} = 5 \rightarrow 5 \ge 5$? **Sim** $\rightarrow$ imprime `*`.
5. Volta à chamada (1): imprime $251 \pmod{10} = 1 \rightarrow 1 \ge 5$? **Não** $\rightarrow$ imprime `-`.

**Resultado:** `- * -`, exatamente a linha do meio do exemplo `Histograma(251)`.

> **Porque é que isto dá a ordem certa (esquerda $\rightarrow$ direita)?**  
> Porque a chamada recursiva (que trata os dígitos mais significativos, obtidos por `n / 10`) acontece *antes* do `cout` da função atual. Isto é o mesmo truque usado para imprimir um número ao contrário sem *arrays* — só que aqui invertido: recursionamos primeiro, imprimimos depois, o que faz o dígito mais à esquerda sair primeiro no ecrã.

---

## 3. Construir o histograma completo

Com essa função, o resto é um simples ciclo `for` que desce o limiar de 9 até 1, chamando `ImprimeLinhaConsole` para cada linha:

```cpp
for (int linha = 9; linha >= 1; linha--)
{
    ImprimeLinhaConsole(n, linha);
    cout << endl;
}
```

Depois, imprime-se o número decomposto em dígitos por baixo, com a mesma técnica recursiva (mas sem comparação, só imprime o dígito):

```cpp
void ImprimeNumeroConsole(int n)
{
    if (n >= 10)
        ImprimeNumeroConsole(n / 10);
    cout << n % 10 << " ";
}
```

---

## 4. Gravar para ficheiro "à C"

O enunciado pede que também se possa gravar o histograma para ficheiro, usando I/O ao estilo da linguagem C (não `ofstream` do C++, mas `FILE*`, `fopen`, `fprintf`, `fclose`). Por isso existem versões "gémeas" das funções acima que, em vez de `cout <<`, usam `fprintf(f, ...)`:

```cpp
FILE *f = fopen("histograma.txt", "w");          // abre o ficheiro para escrita
...
fprintf(f, "%c ", digito >= linha ? '*' : '-');   // escreve no ficheiro
...
fclose(f);                                        // fecha o ficheiro no fim
```

Chama-se `fopen` uma única vez no início de `Histograma`, e todas as linhas vão sendo escritas no mesmo ficheiro à medida que também são impressas no ecrã, fechando-se o ficheiro (`fclose`) só no fim.

---

## 5. `main`

Por fim, o `main` só pede o número ao utilizador (`cin >> N`), garante que é positivo (caso o utilizador introduza um valor negativo, invertendo o sinal) e chama `Histograma(N)`:

```cpp
int main()
{
    int N;
    cout << "Introduza um numero inteiro: ";
    cin >> N;

    if (N < 0)
        N = -N;

    Histograma(N);
    return 0;
}
```

---

# Resumo do raciocínio

1. **Estrutura:** Cada dígito é uma coluna, e a sua altura ($0$–$9$) determina até onde os `*` sobem.
2. **Grelha:** A grelha tem sempre 9 linhas fixas — o limiar desce de 9 a 1.
3. **Sem arrays:** A recursão substitui o *array*: cada chamada "desce" um dígito (`n / 10`), e ao "subir" de volta na pilha imprime-se o dígito na ordem correta (esquerda para a direita).
4. **I/O duplo:** A mesma lógica é duplicada para escrever no ecrã (`cout`) e no ficheiro (`fprintf`), garantindo o cumprimento do enunciado.