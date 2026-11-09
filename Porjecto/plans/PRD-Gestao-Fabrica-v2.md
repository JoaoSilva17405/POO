# PRD v2 — Simulador de Gestão de Fábrica (Projeto POO 2026/27)

> **Versão 2 (2026-11-09).** Substitui o PRD v1 (issue #1). Inclui a revisão de
> extensibilidade (reação polimórfica dos sensores, Catálogo de protótipos separado por
> categoria, configuração atribuída no `Add`, Template Method no `Dispositivo`, sensores
> analógicos e binários, conversões centralizadas) e a **política de visibilidade**
> clarificada pelos docentes: só a Fábrica está limitada aos métodos públicos do
> enunciado (mais o `Run`, exceção aceite). Os poucos métodos de colaboração da Fábrica
> foram aprovados pelo docente (2026-11-09), por isso não é preciso plano B. Sem `friend`.

## Problem Statement

Como grupo de alunos de POO, temos de entregar até 12 de dezembro de 2026 (com uma
avaliação intermédia na semana de 23 de novembro) um programa em C++ que simule a gestão
de uma fábrica: motores que aquecem, avariam e têm de ser parados; sensores (luz,
humidade, fumo, míssil, …) cujos avisos têm consequências na fábrica; utilizadores com
níveis de acesso diferentes; horário de abertura e de fecho; e vários ficheiros de saída
(listagens, XML para um simulador gráfico fornecido, relatório XML, HTML, estado).

A configuração da fábrica vem de um ficheiro XML. Os ficheiros fornecidos pelos docentes
não são uniformes (campos por ordens diferentes, campos presentes só num deles, nomes de
tipos escritos de duas formas, valores com espaços ou unidades, IDs repetidos), e o
professor pediu um leitor **o mais genérico possível**, que não falhe com estas
variações.

A avaliação penaliza fortemente erros de gestão de memória (5 valores), proíbe `auto`,
exige código standard e documentação técnica, e valoriza a aplicação correta dos
princípios de POO trabalhados na Ficha 5 (Aquário/Peixe): encapsulamento, comunicação
bidirecional entre objetos, herança e polimorfismo.

## Solution

Um simulador de consola em C++11 (Code::Blocks + MinGW) organizado à imagem da Ficha 5:
a **Fábrica** faz o papel do Aquário (é dona de tudo, corre o ciclo e trata os pedidos),
os **dispositivos** (motores e sensores) fazem o papel dos peixes (vivem um ciclo de cada
vez, tomam as suas próprias decisões e **pedem/informam** a fábrica).

Do ponto de vista do utilizador:

- Arranca o programa, indica o seu nome e o seu perfil (Admin, Normal ou Visitante) e o
  ficheiro XML de configuração.
- A simulação corre sozinha: um relógio avança 10 minutos por ciclo, a fábrica abre e fecha
  à hora configurada, os motores aquecem enquanto trabalham, os sensores leem valores, e
  os avisos aparecem no ecrã, ficam num histórico e num ficheiro de log.
- O `Simulador.exe` fornecido mostra graficamente a fábrica, porque o programa reescreve
  em cada ciclo um XML no formato que ele espera.
- Ao carregar em **'M'** a simulação pára e aparece um menu com as operações pedidas no
  enunciado. O que cada pessoa pode fazer depende do seu perfil.
- Um aviso de míssil desliga a fábrica, grava o estado e deixa o utilizador no menu, de
  onde pode recarregar esse estado e continuar.
- No fim, toda a memória é libertada.

## User Stories

### Arranque e configuração

1. Como utilizador, quero indicar o meu nome e o meu perfil ao iniciar o programa, para que a fábrica saiba que operações posso fazer.
2. Como utilizador, quero indicar o ficheiro XML de configuração, para poder simular fábricas diferentes.
3. Como utilizador, quero que a fábrica seja criada vazia e só depois carregada do XML, para que a criação e a configuração sejam passos independentes.
4. Como utilizador, quero que o XML seja lido mesmo que os campos de um motor ou sensor venham por outra ordem, para não ter de reescrever ficheiros produzidos por outras pessoas.
5. Como utilizador, quero que o XML seja lido mesmo que as secções (definições, motores, sensores) venham por outra ordem, para que a ordem do ficheiro seja irrelevante.
6. Como utilizador, quero que campos opcionais em falta recebam um valor por omissão, para que um ficheiro incompleto continue utilizável.
7. Como utilizador, quero que campos desconhecidos sejam ignorados, para que extensões futuras do XML não partam o programa.
8. Como utilizador, quero que tipos de dispositivo desconhecidos sejam ignorados com um aviso, para saber o que não foi carregado sem o programa falhar.
9. Como utilizador, quero que valores como `0, 80` (com espaço) e `85%` (com unidade) sejam lidos corretamente, para que pequenas variações de escrita não causem erros.
10. Como utilizador, quero que `SMISSEL` e `SMISSIL` sejam reconhecidos como o mesmo tipo de sensor, para que os dois ficheiros fornecidos funcionem.
11. Como utilizador, quero que um dispositivo com ID repetido seja rejeitado com um aviso, para que cada ID identifique um único dispositivo.
12. Como utilizador, quero que um ficheiro inexistente ou mal formado seja reportado com uma mensagem clara e deixe a fábrica como estava, para não perder o estado atual por engano.
13. Como utilizador, quero que os intervalos VERDE/AMARELO/VERMELHO e a probabilidade de avaria de cada tipo de motor venham do XML, para configurar a fábrica sem recompilar.
14. Como utilizador, quero que a probabilidade de avaria seja lida exatamente como está no ficheiro e interpretada como percentagem (escala 1–100) por ciclo, para que o comportamento seja previsível e documentado.
15. Como utilizador, quero que um tipo de motor sem configuração no XML receba uma configuração por omissão e um aviso, para que o motor nunca fique num estado inválido.
16. Como utilizador, quero que o nome da empresa, as horas de abertura e fecho, a dimensão da fábrica e a distância de vizinhança venham do XML, para que cada fábrica tenha as suas regras.

### Relógio e horário

17. Como utilizador, quero que cada ciclo de simulação corresponda a 10 minutos simulados, para que um dia de trabalho se veja em poucos minutos reais.
18. Como utilizador, quero ver a hora simulada atual e o dia, para perceber em que momento da simulação estou.
19. Como gestor, quero que à hora de fecho todos os motores parem automaticamente, para que a fábrica não trabalhe fora de horas.
20. Como gestor, quero que à hora de abertura todos os motores que não estejam avariados arranquem automaticamente, para que o dia de trabalho comece sem intervenção.
21. Como gestor, quero que os motores avariados continuem parados na abertura até terem manutenção, para não pôr a trabalhar equipamento com problemas.
22. Como gestor, quero que o consumo diário de cada motor volte a zero à abertura, para que o relatório mostre o consumo desse dia de trabalho.
23. Como gestor, quero que os sensores continuem ativos fora de horas, para que fumo ou mísseis sejam detetados mesmo com a fábrica fechada.

### Motores

24. Como gestor, quero que cada motor tenha ID, marca, consumo por hora, posição e o seu tipo (combustão, elétrico ou indução), para identificar e localizar cada equipamento.
25. Como gestor, quero que cada motor esteja num dos estados STOP, START, RUN ou AVARIADO, para saber em cada momento o que está a fazer.
26. Como gestor, quero que um motor em START passe a RUN no ciclo seguinte, para modelar o arranque.
27. Como gestor, quero que um motor em RUN aqueça em cada ciclo, com um ritmo próprio do seu tipo, para simular o funcionamento real.
28. Como gestor, quero que um motor parado arrefeça em cada ciclo, para que volte à zona segura com o tempo.
29. Como gestor, quero que seja o próprio motor a classificar a sua temperatura em VERDE, AMARELO ou VERMELHO segundo os intervalos do seu tipo, para que a decisão fique com quem tem a informação.
30. Como gestor, quero receber um aviso quando um motor entra em AMARELO, para poder agir antes de haver problemas.
31. Como gestor, quero receber esse aviso só uma vez por subida a AMARELO, para não ser inundado de mensagens repetidas.
32. Como gestor, quero que um motor que atinge VERMELHO se pare sozinho e me avise, para que se proteja mesmo que eu não esteja atento.
33. Como gestor, quero que um motor possa avariar em cada ciclo de trabalho com a probabilidade do seu tipo, para simular falhas reais.
34. Como gestor, quero ser avisado quando um motor avaria, para poder mandar fazer manutenção.
35. Como gestor, quero que um motor avariado fique marcado como tal até ter manutenção, para que nenhuma outra ação (arranque, abertura) o ponha a trabalhar.
36. Como gestor, quero que um motor parado por sobreaquecimento, humidade, fumo ou por ordem minha só volte a trabalhar por ação no menu ou na abertura seguinte, para controlar quando a produção recomeça.
37. Como gestor, quero que cada motor conte o tempo que trabalhou, para saber quais trabalharam mais.
38. Como gestor, quero que cada motor acumule o seu consumo diário, para incluir no relatório.
39. Como gestor, quero que cada motor conte as suas avarias, para saber que marcas são menos fiáveis.
40. Como gestor, quero que cada motor tenha as operações RUN, START, RESTART, STOP e ESTOU_QUENTE, redefiníveis por tipo, para cumprir o enunciado e permitir comportamentos específicos.

### Sensores

41. Como gestor, quero que cada sensor tenha ID, marca, valor atual, valor de aviso, posição e probabilidade de avaria, para monitorizar as instalações.
42. Como gestor, quero que cada sensor gere uma nova leitura aleatória em cada ciclo, para simular as condições da fábrica.
43. Como gestor, quero que fumo, míssil e fogo leiam 0 ou 1, com a probabilidade `PROB_1` quando o XML a indica e uma probabilidade baixa por omissão, para que estes alarmes sejam raros.
44. Como gestor, quero que seja o próprio sensor a decidir se está em alarme (a luz alarma abaixo do valor de aviso, os restantes a partir dele), para que cada tipo tenha a sua regra.
45. Como gestor, quero que um sensor avise uma única vez por cada entrada em alarme, para não receber avisos repetidos.
46. Como gestor, quero que um sensor possa avariar e deixe de ler até ter manutenção, para simular falhas de sensores.
47. Como gestor, quero que um alarme de humidade desligue todos os motores a trabalhar dentro da distância de vizinhança desse sensor, e saber quais e quantos foram, para proteger os equipamentos próximos.
48. Como gestor, quero que um alarme de fumo desligue todos os motores e abra um vídeo com as saídas da fábrica, para evacuar em segurança.
49. Como gestor, quero que um alarme de luz abra um vídeo de "bom regresso a casa", para assinalar o fim do dia.
50. Como gestor, quero que um alarme de míssil abra um vídeo com os abrigos, desligue a fábrica e grave o estado atual, para proteger as pessoas sem perder os dados.
51. Como gestor, quero que depois de um alarme de míssil a simulação pare e eu fique no menu, para decidir se recarrego o estado e continuo.
52. Como gestor, quero que os sensores de temperatura e de fogo que aparecem nos XML fornecidos sejam suportados e avisem o gestor, para não perder dispositivos da configuração.
53. Como gestor, quero que os alarmes dos sensores sejam tratados no fim de cada ciclo por ordem de prioridade de cada tipo (míssil, fumo, humidade, luz, restantes), para que os mais graves sejam tratados primeiro e nenhuma lista seja alterada enquanto está a ser percorrida.

### Avisos ao gestor

54. Como gestor, quero que cada aviso apareça imediatamente no ecrã, para atuar logo.
55. Como gestor, quero que os avisos fiquem num histórico em memória, para os consultar no relatório e no HTML.
56. Como gestor, quero que os avisos sejam também gravados num ficheiro de log com a hora simulada, para ter um registo persistente.

### Simulação e menu

57. Como utilizador, quero que a simulação corra continuamente sem intervenção, para observar a fábrica a evoluir.
58. Como utilizador, quero um pequeno resumo no ecrã em cada ciclo (hora, motores por estado, avisos novos), para acompanhar sem ser inundado.
59. Como utilizador, quero carregar em 'M' (maiúscula ou minúscula) para a simulação parar e aparecer o menu, para fazer alterações com a fábrica parada.
60. Como utilizador, quero que a simulação retome quando saio do menu, para continuar de onde estava.
61. Como utilizador, quero uma opção para terminar o programa, para sair de forma limpa e com a memória libertada.
62. Como utilizador, quero que o menu mostre só uma mensagem de "sem permissão" quando escolho uma operação que o meu perfil não permite, para perceber porque não aconteceu nada.

### Utilizadores e permissões

63. Como Admin, quero poder adicionar, remover, listar, correr a simulação e fazer manutenção, para gerir a fábrica por completo.
64. Como utilizador Normal, quero poder adicionar, listar, correr a simulação e fazer manutenção, mas não remover dispositivos, para operar a fábrica sem poder apagar equipamento.
65. Como Visitante, quero poder apenas listar e consultar, para ver o estado da fábrica sem alterar nada.
66. Como utilizador, quero ver o meu nome e o meu perfil nas listagens e no relatório, para que fique registado quem estava a operar.

### Operações do menu

67. Como utilizador com permissão, quero adicionar um motor indicando tipo, ID, marca, consumo e posição, para expandir a fábrica.
68. Como utilizador com permissão, quero que um motor com ID repetido ou posição fora da fábrica seja recusado com uma mensagem, para manter os dados coerentes.
69. Como Admin, quero remover um motor ou sensor pelo ID, para retirar equipamento da fábrica.
70. Como utilizador, quero listar o estado completo da fábrica no ecrã ou num ficheiro, para consultar e guardar a informação.
71. Como utilizador com permissão, quero desligar um motor pelo ID, para o parar manualmente.
72. Como utilizador com permissão, quero voltar a ligar (RESTART) um motor parado pelo ID, para retomar a produção desse motor.
73. Como utilizador, quero saber o estado de um motor pelo ID e ser avisado se o ID não existir, para consultar um equipamento específico.
74. Como utilizador, quero listar e obter todos os motores de um dado tipo, para analisar uma família de motores.
75. Como utilizador com permissão, quero fazer manutenção de todos os dispositivos, deixando cada motor parado com uma temperatura aleatória dentro da sua zona VERDE e limpando avarias e avisos, para repor a fábrica em bom estado.
76. Como utilizador, quero ver as marcas de motores e sensores ordenadas da que mais avaria para a mais fiável, para decidir futuras compras.
77. Como utilizador, quero ver os motores ordenados do que mais trabalhou para o que menos trabalhou, para planear manutenção e desgaste.
78. Como utilizador, quero gerar um relatório XML com o estado da fábrica e o consumo total do dia, para enviar à administração.
79. Como utilizador, quero gerar uma página HTML com o estado atual da fábrica, com os motores coloridos conforme a temperatura, para ver no browser.
80. Como utilizador, quero recarregar a fábrica a partir de um ficheiro XML (incluindo o ficheiro de estado gravado pelo alarme de míssil), para retomar uma simulação anterior.

### Simulador gráfico

81. Como utilizador, quero que em cada ciclo seja reescrito um XML com nome fixo no formato do `Simulador.exe`, para ver a fábrica a evoluir graficamente.
82. Como utilizador, quero que esse XML inclua a temperatura atual, o estado e a cor de cada motor e o valor atual de cada sensor, como pedem as instruções do simulador.
83. Como utilizador, quero que a cor do motor seja calculada pelo nosso programa a partir dos intervalos do seu tipo, porque o simulador não a calcula.
84. Como utilizador, quero que o simulador nunca leia um ficheiro a meio da escrita, para não ver dados corrompidos.
85. Como utilizador, quero que o ficheiro de estado use o mesmo formato, para poder ser carregado de novo e visto no simulador.

### Qualidade, memória e documentação

86. Como avaliador, quero que todo o objeto criado dinamicamente tenha um único dono que o liberta, para não haver fugas de memória.
87. Como avaliador, quero que um dispositivo removido seja retirado de todas as listas antes de ser libertado, para não haver ponteiros pendurados.
88. Como avaliador, quero que as listas devolvidas por consultas (por tipo, rankings) sejam libertadas por quem as pede, sem libertar os dispositivos que contêm, e que isto esteja documentado.
89. Como avaliador, quero que a fábrica não possa ser copiada, para evitar libertações duplicadas.
90. Como avaliador, quero que o código não use `auto` e seja C++ standard, com o código específico do Windows e a biblioteca de XML isolados em classes próprias.
91. Como avaliador, quero que todas as classes, métodos e atributos tenham comentários Doxygen e que a documentação seja gerada, para avaliar a documentação técnica.
92. Como membro do grupo, quero testes automáticos que possa correr num alvo separado do Code::Blocks, para ter confiança ao alterar o código.

### Extensibilidade

93. Como membro do grupo, quero acrescentar um tipo novo de sensor escrevendo apenas a sua classe e uma linha no catálogo de tipos, para responder rapidamente a um pedido novo do docente.
94. Como membro do grupo, quero acrescentar um tipo novo de motor escrevendo apenas a sua classe, uma linha no catálogo e os seus intervalos no XML, para não ter de mexer na Fábrica.
95. Como membro do grupo, quero que um tipo novo passe a ser reconhecido no `Load`, oferecido no menu "adicionar motor", listado por tipo e escrito em todas as saídas sem alterar esse código, para que acrescentar um tipo não tenha efeitos espalhados.
96. Como membro do grupo, quero que o nome de cada tipo (a tag XML) esteja escrito num único sítio, para que não haja nomes desencontrados entre a criação, as listagens e o XML.
97. Como membro do grupo, quero que a consequência do alarme de cada sensor esteja na classe desse sensor, para que um sensor novo não obrigue a acrescentar métodos, flags ou ramos à Fábrica.
98. Como membro do grupo, quero que a lógica comum a todos os dispositivos (avaria, manutenção, leitura e escrita dos campos comuns) esteja escrita uma única vez, para que uma classe nova só contenha o que a distingue.
99. Como membro do grupo, quero que a conversão de estados e cores para texto esteja num único sítio, para que um estado novo não obrigue a procurar `switch` por todo o código.
100. Como avaliador, quero que nenhuma decisão dependa do tipo concreto através de `if`, `switch` ou `dynamic_cast`, para ver o polimorfismo aplicado de forma consistente.
101. Como membro do grupo, quero que um sensor novo herde a geração de valores de uma família (analógica ou binária), para que só tenha de escrever o que o distingue.

### Interface pública

102. Como avaliador, quero que os métodos públicos da Fábrica sejam os do enunciado, com os mesmos nomes e assinaturas (mais o `Run`), para verificar facilmente o cumprimento do que foi pedido.
103. Como avaliador, quero que qualquer método público da Fábrica fora do enunciado esteja num bloco separado, justificado (quem o chama e porquê) e tenha sido perguntado ao docente, para perceber que é uma necessidade de colaboração e não descuido.
104. Como avaliador, quero que tudo o que só é usado dentro de uma classe seja privado e o que só as subclasses usam seja protegido, para ver o encapsulamento aplicado.
105. Como avaliador, quero que não existam setters e que os getters só existam quando outra classe precisa do valor, para que o estado só mude através das ações do enunciado.
106. Como membro do grupo, quero que as subclasses falem com a Fábrica apenas através de métodos protegidos da classe base, para haver um único ponto de contacto e a verificação de "fora da fábrica" estar num só sítio.
107. Como membro do grupo, quero que os dispositivos falem com a Fábrica só através dos métodos de colaboração aprovados pelo docente, para que a interface pública da Fábrica seja exatamente a validada.

## Implementation Decisions

### Ambiente e restrições
- C++11, Code::Blocks + MinGW, Windows. Sem `auto`. Usar `nullptr` e `= delete` (C++11) para proibir cópias.
- Leitura de XML com **TinyXML-2** (biblioteca externa de dois ficheiros compilada com o projeto, licença zlib), isolada num adaptador. A escrita de XML/HTML faz-se com `ostream &`.
- Documentação com **Doxygen**, comentários escritos desde a primeira fase, com `EXTRACT_PRIVATE = YES` para que os métodos privados e protegidos também fiquem documentados.
- Testes com **doctest** (um único cabeçalho), num segundo alvo de compilação que compila os módulos sem o `main` da aplicação.
- **Não se usa `friend`.** Só a Fábrica está limitada aos públicos do enunciado, e os seus métodos de colaboração foram aprovados pelo docente (2026-11-09).

### Política de visibilidade
Clarificação dos docentes (2026-11-09): **só a classe `Fabrica` está limitada aos métodos públicos do enunciado.** Nas restantes classes, a visibilidade fica ao critério do grupo. O docente pediu que qualquer método público da Fábrica que não esteja no enunciado lhe seja **perguntado**.

**Fábrica:**
1. **Público — enunciado**: o construtor com o utilizador, `Load`, `Add(User)`, `Add(Motor)`, `Listar`, `Desligar`, `Get_Estado`, `Listar_Tipo`, `Manutencao`, `Ranking_Dos_Fracos`, `Ranking_Dos_Mais_Trabalhadores`, `Relatorio`, `Aviso_Humidade`, `Aviso_Fumo`, `Aviso_Luz`, `Aviso_Missel`, `Listagem_HTML`. Mais o destrutor.
2. **Público — exceção aceite**: `Run` (o `main` arranca a simulação; equivalente ao `Aquario::Run` da Ficha 5).
3. **Público — colaboração (aprovado pelo docente em 2026-11-09)**: `Ciclo`, `Notificar_Gestor`, `Registar_Alarme`, `Aviso_Aquecimento`, `Aviso_Sobreaquecimento`, `Aviso_Avaria`.
   - Estão num bloco próprio do cabeçalho, com um comentário que o identifica.
   - Cada um tem documentação Doxygen que diz quem o chama e porquê.
   - Ver o inventário abaixo.
4. **Privado**: tudo o resto (menu, `Remover`, `Ligar`, `Adicionar_Sensor`, autorização, pesquisa por ID, leitura do XML, processamento de alarmes, gravadores, limpeza).

**Restantes classes** (critério do grupo, seguindo as boas práticas da Ficha 5):
- é **público** o que outras classes usam;
- é **protegido** o que só as subclasses usam (ganchos do Template Method e os reencaminhamentos do `Dispositivo` para a Fábrica);
- é **privado** o resto.

Em todas as classes: **não há setters**, os getters são `const` e só existem quando outra classe precisa do valor, e um método usado só dentro da própria classe é privado.

### Princípio orientador: extensibilidade com o mínimo de alterações
- A cadeira valoriza a simplicidade e a facilidade de acrescentar um tipo novo de sensor ou de motor. O desenho é avaliado pela pergunta "quantos sítios mudam para acrescentar um tipo?".
- Meta: **um tipo novo = uma classe nova + uma linha no Catálogo**, sem alterar a Fábrica, o `Load`, o menu, as listagens nem as saídas.
- Um sensor com uma consequência **totalmente nova** exige, além disso, um método novo na Fábrica (a ação nova). Nada mais muda nela.
- **Nenhum código decide pelo tipo com `if`/`switch`/`dynamic_cast`.** Toda a variação por tipo é feita por métodos virtuais.
- O relatório `Relatorio-Melhorias-Extensibilidade.md` explica para a defesa os problemas que estas decisões resolvem.

### Módulos

1. **Uteis** (utilitário, só métodos estáticos públicos, como na Ficha 5):
   - números aleatórios inteiros e reais, e `Acontece(percentagem)`;
   - deteção e leitura de tecla, espera sem ocupar o CPU;
   - leitura validada de inteiros e texto, `Trim`;
   - escape de texto para XML/HTML;
   - abertura de vídeo com o leitor do Windows.

   Todo o código específico de Windows fica aqui. Tem um interruptor para desligar a abertura de vídeos (usado nos testes).

2. **Enumerados e conversões**: os enumerados `ESTADO_MOTOR` (STOP, START, RUN, AVARIADO, INEXISTENTE), `COR_TEMP` (VERDE, AMARELO, VERMELHO) e `ACAO` (ADD, REMOVER, LISTAR, RUN, MANUTENCAO), cada um com **uma única** função de conversão para texto e, quando é preciso ler do XML, de texto para valor. Usadas pelo XML, HTML, listagens e `Load`, para que não existam `switch` de conversão espalhados.

3. **ElementoXML** (adaptador profundo sobre TinyXML-2; interface pública por ser uma classe de suporte usada pelas subclasses): vista só de leitura sobre um elemento.
   - Interface: nome da tag, existência de um filho, `Texto`, `Inteiro`, `Real` e `Par` ("a,b"), todos **procurando por nome** e com valor por omissão, mais o filho por nome e a lista de todos os filhos pela ordem do ficheiro.
   - A conversão é tolerante a espaços e sufixos (`85%`).
   - Não é dono de nada. É o único módulo, além do `Load`, que conhece a biblioteca.

4. **Relogio** (módulo profundo; interface pública usada pela Fábrica): guarda o dia e o minuto simulados e as horas de abertura e fecho.
   - `Avancar()` soma 10 minutos e devolve o **evento** ocorrido (nenhum, abertura, fecho).
   - Também diz se está aberto e devolve a hora formatada. Trata a mudança de dia.
   - Não conhece motores: é a Fábrica que reage aos eventos.

5. **Posicao**: valor (X, Y) com a distância euclidiana e as coordenadas (públicas, usadas pela Fábrica e pelos dispositivos). Guardado por valor, sem memória dinâmica.

6. **ConfigMotor** (interface pública usada pelo Motor e pela Fábrica): os intervalos VERDE, AMARELO e VERMELHO e a probabilidade de avaria de um tipo de motor.
   - Classifica uma temperatura numa cor.
   - Fornece a temperatura de aviso (início do AMARELO) e de paragem (início do VERMELHO).
   - Sorteia uma temperatura dentro da zona VERDE e lê-se do XML.
   - Há uma por tipo, dona é a Fábrica, e os motores apontam para ela.

7. **Catalogo** (registo de protótipos, **separado por categoria**):
   - É o **único sítio que enumera os tipos concretos**. O seu construtor regista um protótipo de cada classe concreta (uma linha por tipo) e os sinónimos de tags (`SMISSEL` → `SMISSIL`).
   - Tem dois registos, um de motores e outro de sensores, cada um com o tipo certo. Assim não é preciso *double dispatch* nem um `Add(Sensor)` público.
   - Interface pública (é uma classe de suporte usada pela Fábrica):
     - `CriarMotor(tag)` devolve uma cópia nova de um motor, ou `nullptr`;
     - `CriarSensor(tag)` faz o mesmo para sensores;
     - `Tipos_Motor()` e `Tipos_Sensor()` devolvem os tipos registados. O menu "adicionar motor" usa-os, por isso passa a oferecer tipos novos sem alterações.
   - A chave de cada protótipo é o seu próprio `QuemEs()`, por isso **o nome de cada tipo existe num só sítio**: a própria classe.
   - Uma tag já registada substitui a anterior, que é libertada.
   - É dono dos protótipos (liberta-os no destrutor) e não é copiável. A Fábrica tem um Catálogo por valor. Os protótipos nunca entram na simulação.
   - **Decisão:** não se usa auto-registo por objetos estáticos em cada `.cpp`, porque depende da ordem de inicialização de objetos estáticos e é difícil de explicar.
   - **Porque não pôr a criação na classe `Dispositivo`:** obrigaria a classe base a conhecer todas as filhas (dependência circular), e a classe mais estável da hierarquia passaria a mudar a cada tipo novo.

8. **Dispositivo** (base abstrata de motores e sensores; padrão **Template Method**):
   - **Privado:** ID, marca, posição, probabilidade de avaria, contador de avarias, **flag `Avariado`** e ponteiro (não dono) para a Fábrica.
   - **Público** (usado pela Fábrica e pelo Catálogo):
     - `Ciclo()`: não virtual. Se avariado, não faz nada. Se o gancho `Pode_Avariar()` o permitir, sorteia avaria; se avariar, liga a flag, conta a avaria e avisa a Fábrica. Senão chama o gancho `Funcionar()`. O nome `Ciclo` evita a confusão com o `RUN()` do enunciado.
     - `Manutencao()`: não virtual; limpa a flag e chama `Manutencao_Especifica()`.
     - `LerXML(elemento)` e `EscreveXML(ostream)`: não virtuais; tratam os campos comuns (e, na escrita, a abertura e o fecho da tag `QuemEs()`) e chamam os ganchos.
     - `Escreve(ostream)` e `EscreveHTML(ostream)`: escrevem a parte comum e acrescentam `Descricao_Especifica()`.
     - `QuemEs()` (virtual puro): a tag XML.
     - Os getters `const` de ID, marca, posição, número de avarias e "está avariado".
     - `Atribuir_Fabrica`.
   - **Protegido:**
     - os ganchos virtuais `Funcionar` (puro), `Pode_Avariar` (por omissão verdadeiro), `Manutencao_Especifica`, `LerCamposXML`, `EscreveCamposXML` e `Descricao_Especifica`;
     - os métodos de **reencaminhamento** para a Fábrica (`Avisar_Gestor`, `Avisar_Aquecimento`, `Avisar_Sobreaquecimento`, `Registar_Alarme`). Verificam se o ponteiro para a Fábrica existe e chamam o método de colaboração correspondente.
   - As subclasses **nunca** usam o ponteiro para a Fábrica diretamente: há um único ponto de contacto, e a verificação de `nullptr` fica num só sítio. Isso permite testar um dispositivo sozinho.
   - Destrutor virtual. Um contador estático de dispositivos vivos serve os testes de memória.

9. **Motor** (abstrato) e **MCombustao**, **MEletrico**, **MInducao**:
   - **Público — enunciado:** RUN, START, RESTART, STOP, ESTOU_QUENTE (virtuais, redefiníveis).
   - **Público** (usado pela Fábrica):
     - os getters `const` de estado, temperatura, cor, minutos trabalhados e consumo do dia;
     - `Atribuir_Config`, `Novo_Dia`;
     - `Clone()`, que devolve um `Motor` (usado pelo Catálogo).
   - **Protegido:** os ganchos `Aquecimento()` (puro, ritmo por tipo) e `Arrefecimento()`.
   - **Privado:** estado (STOP, START, RUN), temperatura, consumo por hora, minutos, consumo do dia, flag "já avisei AMARELO" e a configuração do tipo (ponteiro não dono).
   - O estado devolvido é AVARIADO **enquanto a flag `Avariado` da base estiver ligada**: há uma só fonte de verdade. START, RESTART e a abertura não têm efeito num motor avariado; só a manutenção o liberta (o mesmo padrão da flag de fome do peixe na Ficha 5).
   - `Pode_Avariar()`: só em RUN.
   - `Funcionar()` é a máquina de estados: STOP arrefece; START passa a RUN; RUN aquece, conta tempo e consumo e chama `ESTOU_QUENTE`.
   - `ESTOU_QUENTE` classifica a temperatura: AMARELO avisa uma vez e a flag repõe-se em VERDE; VERMELHO faz STOP e avisa.
   - `Manutencao_Especifica`: STOP, temperatura aleatória na zona VERDE e flags repostas.
   - **A configuração é atribuída quando o motor entra na Fábrica** (`Add` → `Atribuir_Config`), como o `Add` do aquário fazia `Atribuir_Aquario`. Os construtores ficam sem argumentos.
   - Uma subclasse concreta só escreve `QuemEs`, `Clone` e `Aquecimento`, e opcionalmente redefine RUN, START, etc.

10. **Sensor** (abstrato), **SensorAnalogico** e **SensorBinario** (intermédios), e **SLuz**, **SHumidade**, **STemperatura** (analógicos), **SFumo**, **SMissil**, **SFogo** (binários):
    - **Público** (usado pela Fábrica e pelo Catálogo):
      - `Reagir(Fabrica)` (virtual puro): a consequência do alarme;
      - `Prioridade()` (por omissão 0);
      - `Desliga_Vizinhanca()` (por omissão falso);
      - `Clone()`, que devolve um `Sensor`;
      - o getter `const` do valor atual.
    - **Protegido:** os ganchos `Gerar_Valor()` (puro) e `EmAlarme()` (por omissão "valor ≥ aviso"), e `Video()` (o ficheiro de vídeo do tipo, usado pelo próprio `Reagir`).
    - **Privado:** valor, valor de aviso e flag "já avisei".
    - `Funcionar()` gera a leitura. Se estiver em alarme e ainda não avisou, regista-se na Fábrica através do reencaminhamento protegido. Ao sair de alarme, repõe a flag.
    - `SensorAnalogico`:
      - leitura **aleatória pura** entre MIN e MAX (valores por omissão do tipo, opcionalmente lidos de `<MIN>` e `<MAX>`);
      - construtor protegido que recebe o intervalo do tipo.
    - `SensorBinario`:
      - 1 com a probabilidade `PROB_1` (lida do XML quando existe; por omissão baixa), senão 0;
      - construtor protegido.
    - **Porquê as intermédias:**
      - há duas famílias de comportamento (valor contínuo e 0/1 raro), e cada uma fica escrita uma vez em vez de seis;
      - cada atributo só existe onde faz sentido;
      - um sensor novo herda a geração sem escrever código para isso;
      - é herança usada para agrupar uma família, como seria uma classe `Tubarao` com várias espécies por baixo.
    - Concretos (cada um só com `QuemEs`, `Clone`, `Reagir` e, se necessário, `Prioridade`, `Video`, `EmAlarme` ou `Desliga_Vizinhanca`):

      | Tipo | Família | Alarme | Prioridade | Reação |
      |---|---|---|---|---|
      | SMissil | binário | por omissão | 40 | `Aviso_Missel(vídeo)` |
      | SFumo | binário | por omissão | 30 | `Aviso_Fumo(lista, vídeo)` |
      | SHumidade | analógico | por omissão; `Desliga_Vizinhanca` verdadeiro | 20 | `Aviso_Humidade(lista)` |
      | SLuz | analógico | **abaixo** do valor de aviso | 10 | `Aviso_Luz(vídeo)` |
      | STemperatura | analógico | por omissão | 0 | avisa o gestor |
      | SFogo | binário | por omissão | 0 | avisa o gestor |

    - As reações principais usam os `Aviso_*` **do enunciado**, que já são públicos: a reação polimórfica encaixa na interface pedida.

11. **User** (concreta: ID, nome) com **Admin**, **Normal** e **Visitante**:
    - **Público** (usado pela Fábrica): `Pode(acao)` (virtual, segundo a tabela do enunciado), `QuemEs()` e os getters `const` de ID e nome.
    - O utilizador atual é criado no arranque e passado ao construtor da Fábrica, que fica dona dele.
    - **`Add(User*)` é esperado** (resposta do docente, 2026-11-09). O desenho (lista `LU`, dono, interação com o utilizador atual e com as permissões) **está por rever** — ver a issue de seguimento.

12. **Fabrica** (fachada; o "Aquário"):
    - É dona dos motores, sensores, configurações e do utilizador atual, e tem um Catálogo por valor. Não é copiável.
    - Contém o relógio, o histórico de avisos e **uma única lista de sensores em alarme pendentes**.
    - **Não conhece nenhum tipo concreto de dispositivo**: não inclui as subclasses, não compara tags e não tem métodos por tipo de sensor.
    - **Público — enunciado:** os 17 métodos (ver a política de visibilidade).
    - **Público — exceção aceite:** `Run`.
    - **Público — colaboração (aprovado pelo docente):**
      - `Ciclo()`: uma iteração da simulação, sem teclado nem espera. É usado pelo `Run`, e é público por causa dos testes; é o único público que existe só para testes.
      - os métodos chamados pelos dispositivos (através dos reencaminhamentos protegidos do `Dispositivo`): `Notificar_Gestor(mensagem)`, `Registar_Alarme(sensor)`, `Aviso_Aquecimento(motor)`, `Aviso_Sobreaquecimento(motor)`, `Aviso_Avaria(dispositivo)`.
    - **Privado:**
      - o menu e o despacho das opções;
      - `Remover(id)` e `Ligar(id)` (chamados só pelo menu);
      - a autorização por perfil e a pesquisa por ID;
      - `Adicionar_Sensor` (usado só pelo `Load`);
      - a leitura das definições e das secções do XML, a obtenção da configuração de um tipo;
      - o processamento dos alarmes e a reação ao relógio;
      - `Desligar_Todos`;
      - o gravador do XML do simulador e de estado, o cabeçalho e rodapé HTML, a limpeza.
    - **Ciclo**:
      1. Avança o relógio e reage ao evento: no fecho, STOP a todos os motores; na abertura, `Novo_Dia` a todos e START aos não avariados.
      2. Chama `Ciclo()` dos motores (só com a fábrica aberta) e dos sensores (sempre).
      3. Processa os alarmes pendentes: ordena a lista por `Prioridade` (decrescente) e chama `Reagir(this)` em cada sensor. Pára se a fábrica tiver sido desligada (míssil). Limpa a lista no fim.
      4. Grava o XML do simulador.
      5. Mostra o resumo.
    - Os alarmes são registados durante o ciclo e tratados só no fim, como o `LP_Assar` da Ficha 5, para não alterar listas que estão a ser percorridas.
    - **`Aviso_Humidade(lista)`**: percorre os sensores em alarme pendentes e, para os que respondem `Desliga_Vizinhanca()`, faz STOP aos motores a trabalhar dentro da distância de vizinhança, sem repetidos. Devolve o número desligado. Mantém a assinatura do enunciado sem a Fábrica conhecer o tipo "humidade".
    - **Run**: enquanto ligada, faz `Ciclo`, verifica a tecla 'M' e espera.
    - Depois de um alarme de míssil, a fábrica desliga-se. O `Run` deixa de correr ciclos e o utilizador fica no menu, de onde pode recarregar o estado e voltar a correr.
    - **Permissões**: um único método privado de autorização, consultado por cada operação.
      - ADD: adicionar motor e `Load`.
      - REMOVER: remover.
      - LISTAR: listar, consultar estado, listar por tipo, rankings, relatório e HTML.
      - RUN: correr a simulação, desligar e ligar motores.
      - MANUTENCAO: manutenção.
    - **Gestor**: `Notificar_Gestor(mensagem)` mostra no ecrã, junta ao histórico e acrescenta ao ficheiro de log, com a hora simulada.
    - **Add(Motor)** e **Adicionar_Sensor**: rejeitam ID repetido ou posição fora da fábrica. Se aceitarem, ligam o dispositivo à Fábrica; o motor recebe também a configuração do seu tipo (por omissão e com aviso, se o XML não a definir).
    - **Load**: é feito em fases. Se o ficheiro for válido, limpa o estado anterior. Lê as definições primeiro, porque criam as configurações de cada tipo de motor.
      - Os filhos de `<MOTORES>` são criados com `CriarMotor`, leem-se a si próprios (`LerXML`) e entram pelo `Add(Motor)` do enunciado.
      - Os de `<SENSORES>` são criados com `CriarSensor` e entram por `Adicionar_Sensor`.
      - Tipos desconhecidos na secção são ignorados com aviso; um dispositivo rejeitado é libertado de imediato.
      - Os campos de estado (`VALOR_TEMP_ATUAL`, `ESTADO_ATUAL`, `VALOR_ATUAL`) são restaurados quando existem.
    - **Saídas**: são todas escritas com `ostream &`. Cada dispositivo escreve-se a si próprio e a Fábrica escreve o "envelope".
      - O XML do simulador e o ficheiro de estado usam o mesmo gravador, num formato idêntico ao do exemplo fornecido.
      - O XML do simulador é escrito num ficheiro temporário que depois substitui o definitivo.
      - O relatório XML inclui a data e hora simuladas, o utilizador, contagens por estado e por cor, o consumo de cada motor e o consumo total do dia, e os avisos do dia.
      - O HTML tem as tabelas de motores (cor conforme a temperatura) e de sensores, e os últimos avisos.
    - **Contratos**:
      - `Get_Estado` de um ID inexistente devolve `INEXISTENTE`.
      - As listas devolvidas por `Listar_Tipo` e pelos rankings são criadas com `new` e libertadas por quem chama; os motores que contêm continuam a pertencer à Fábrica.
      - `Listar_Tipo` compara com `QuemEs()`, por isso funciona com tipos novos.

13. **Programa principal**: inicializa a semente aleatória, pede o nome e o perfil, cria a Fábrica, pede o ficheiro XML, faz `Load` e `Run`. A libertação acontece no destrutor da Fábrica.

### Inventário dos métodos públicos da Fábrica fora do enunciado (aprovado pelo docente)

| Método | Quem o chama | Porquê |
|---|---|---|
| `Run` | `main` | arrancar a simulação — **exceção aceite** |
| `Ciclo` | `Run` e testes | correr uma iteração sem teclado nem espera (testes) |
| `Notificar_Gestor` | `Dispositivo` (reencaminhamento protegido) | avisar o gestor (ecrã, histórico, log) |
| `Registar_Alarme` | `Dispositivo` (reencaminhamento de `Sensor`) | pôr o sensor na lista de alarmes pendentes |
| `Aviso_Aquecimento`, `Aviso_Sobreaquecimento`, `Aviso_Avaria` | `Dispositivo` (reencaminhamentos de `Motor`/`Sensor`) | o equivalente ao `Preciso_de_Comer`/`QueroSair` da Ficha 5 |

O docente aprovou estes métodos (2026-11-09, issue #3). O plano B (classe `Gestor`) que estava previsto deixou de ser necessário.

### Receita para acrescentar um tipo (contrato de extensibilidade)
- **Sensor novo**:
  1. Classe que herda de `SensorAnalogico` ou `SensorBinario`, com `QuemEs`, `Clone` e `Reagir` (e, se necessário, `Prioridade`, `Video`, `EmAlarme`, `Desliga_Vizinhanca`).
  2. Uma linha no construtor do Catálogo.
  3. Acrescentar os ficheiros ao projeto Code::Blocks.
- **Motor novo**:
  1. Classe que herda de `Motor`, com `QuemEs`, `Clone` e `Aquecimento`.
  2. Uma linha no Catálogo.
  3. Os intervalos do tipo em `<DEFINICOES>` do XML.
- Em nenhum dos casos mudam a Fábrica, o `Load`, o menu, as listagens, o relatório, o HTML ou o XML do simulador.

### Decisões consideradas e rejeitadas
- **`friend`**: desnecessário depois da clarificação (só a Fábrica está limitada) e da aprovação dos métodos de colaboração.
- **Plano B (classe `Gestor` a receber os avisos)**: não é necessário, porque o docente aprovou os métodos de colaboração da Fábrica (2026-11-09).
- **`Add(Sensor)` público com *double dispatch* (`Entrar_Na`)**: substituído pelo Catálogo separado por categoria e por um `Adicionar_Sensor` privado. É mais simples e não acrescenta métodos públicos.
- **Auto-registo de tipos** por objetos estáticos: ver o Catálogo.
- **Interface abstrata entre dispositivos e Fábrica**: acrescenta uma abstração sem ganho de extensibilidade e os seus métodos seriam públicos.
- **Motor genérico configurado só por XML**: adiado. O enunciado valoriza que cada tipo possa redefinir os seus métodos.
- **Vídeo e ritmo de aquecimento lidos do XML**: adiado. Ficam como valores virtuais por omissão em cada classe.
- **Métodos de ajuda na classe `Sensor` em vez das classes intermédias**: mais simples, mas põem atributos onde não fazem sentido e mostram menos POO.

### Probabilidade de avaria
- Lida **exatamente** como está no XML, sem arredondar nem limitar, e interpretada como percentagem por ciclo na escala de 1 a 100 (`Acontece(p)` compara um real aleatório em [0,100) com `p`). Um valor de 0.01 é, portanto, 0,01%.

### Memória
- Donos: a Fábrica (motores, sensores, configurações e utilizador), o Catálogo (protótipos) e quem chama as consultas que devolvem listas novas.
- A lista de alarmes pendentes, as listas de saída dos avisos e os ponteiros para a Fábrica e para as configurações nunca são donos.
- As configurações são libertadas depois dos motores. A remoção por ID retira o dispositivo da lista de alarmes pendentes antes de o libertar.

## Testing Decisions

- **O que é um bom teste**: exercita apenas a **interface pública** e verifica comportamento observável (valores devolvidos, estado consultável por getters, ficheiros produzidos, número de objetos vivos). Não depende de atributos privados nem da ordem interna das chamadas. Cada teste é independente e cria os seus próprios objetos.
- **Determinismo**: os testes usam probabilidades de 0 ou 100 nos ficheiros de teste e fixam a semente quando é preciso aleatoriedade. Verificam intervalos (por exemplo, "temperatura dentro da zona VERDE") em vez de valores exatos. Os vídeos ficam desligados através do interruptor em `Uteis`. A simulação é testada com `Ciclo()`, nunca com `Run()`.
- **Acesso**: os testes usam só a interface pública (a do enunciado e a de colaboração da Fábrica, mais a das outras classes). O `Ciclo()` público da Fábrica é o que permite correr a simulação passo a passo. Não se usa nenhum `friend` de testes.
- **Ficheiros de teste**: pequenos XML de teste, escritos para o efeito, cobrem campos fora de ordem, secções trocadas, campos em falta e a mais, tipos desconhecidos, sinónimos, IDs repetidos, valores com espaços ou `%`, e ficheiros mal formados ou inexistentes. Os dois XML fornecidos pelos docentes também são carregados num teste.
- **Módulos com testes (todos)**:
  - **ElementoXML**: leitura por nome independente da ordem, valores por omissão, conversões tolerantes, pares, filhos.
  - **Relogio**: avanço de 10 minutos, eventos de abertura e fecho na hora certa e uma só vez, aberto/fechado, mudança de dia.
  - **ConfigMotor**: classificação nos limites de cada intervalo, temperaturas de aviso e paragem, sorteio dentro da zona VERDE.
  - **Motor** (sem Fábrica e com Fábrica): transições de estado, aquecimento em RUN e arrefecimento em STOP, aviso AMARELO único, STOP automático em VERMELHO, flag de avaria a bloquear START/RESTART até à manutenção, efeitos da manutenção, contadores de tempo, consumo e avarias, `Novo_Dia`.
  - **Dispositivo** (comportamento comum, através de um tipo concreto qualquer): um dispositivo avariado não funciona até à manutenção, a avaria conta para o ranking, a manutenção limpa a avaria, `LerXML` seguido de `EscreveXML` preserva os campos (ida e volta), `Clone` produz um objeto independente e do mesmo tipo.
  - **Sensor** (cada tipo): regra de alarme (incluindo a regra invertida da luz), registo único por entrada em alarme, prioridade, reação (com uma Fábrica real e vídeos desligados), sensores analógicos dentro de MIN–MAX e binários só com 0 ou 1.
  - **Catalogo**: cria cada tipo registado pela sua tag, devolve `nullptr` para tipos desconhecidos, aceita sinónimos, lista os tipos por categoria, cada objeto criado tem `QuemEs()` igual à tag pedida, e os protótipos são libertados.
  - **Conversões**: ida e volta de cada valor dos enumerados para texto.
  - **Extensibilidade** (teste-guarda): uma classe de sensor e outra de motor definidas **apenas no ficheiro de testes**, registadas no Catálogo, têm de carregar de um XML, correr ciclos, reagir e aparecer nas saídas sem nenhuma alteração ao código da aplicação. Se este teste exigir mudar a Fábrica, o contrato de extensibilidade foi quebrado.
  - **User**: tabela de permissões dos três perfis.
  - **Fabrica**:
    - `Load` em todos os cenários dos ficheiros de teste;
    - `Add` a rejeitar IDs repetidos e posições fora da fábrica;
    - `Remover`, `Desligar`, `Ligar`, `Get_Estado` (incluindo `INEXISTENTE`), `Listar_Tipo` e os dois rankings, com ordenação correta;
    - `Aviso_Humidade` (vizinhança, só de sensores que desligam a vizinhança, ausência de repetidos), `Aviso_Fumo` (todos parados, contagem), `Aviso_Missel` (fábrica desligada, ficheiro de estado que volta a carregar com os mesmos dados);
    - processamento de alarmes por ordem de prioridade, e paragem do processamento depois do míssil;
    - reação à abertura e ao fecho, incluindo motores avariados que ficam parados;
    - recusa de operações por perfil;
    - motor que recebe a configuração do seu tipo ao entrar, e configuração por omissão quando o XML não a define;
    - saídas (relatório, HTML, XML do simulador) bem formadas, verificadas com o próprio TinyXML-2;
    - consumo total do dia correto.
  - **Visibilidade** (verificação na revisão de código, não automática): os métodos públicos da Fábrica são só os do enunciado, o `Run` e os do inventário; cada um destes tem documentação que diz quem o chama.
  - **Memória**: um contador estático de dispositivos vivos tem de voltar a zero depois de destruir a Fábrica, após `Load`, `Remover`, recarregamentos e `Add` rejeitados. Complementado por uma execução com Dr. Memory antes da entrega.
- **Prior art**: o repositório não tem testes automáticos. As fichas anteriores (incluindo a Ficha 5) foram testadas manualmente pelo menu. Este é o primeiro conjunto de testes, por isso o alvo de testes e a estrutura de ficheiros de teste devem ficar preparados logo na primeira fase.

## Out of Scope

- Autenticação e palavras-passe. (`Add(User*)` deixou de estar fora do âmbito: é esperado, com desenho a rever.)
- Interface gráfica própria: a visualização é feita pelo `Simulador.exe` fornecido.
- Portabilidade para Linux/macOS (a deteção de tecla e a abertura de vídeos são específicas de Windows, isoladas em `Uteis`).
- Validação do XML contra um esquema (XSD) e escrita de XML através da biblioteca.
- Produção ou escolha dos ficheiros de vídeo: o programa apenas os abre.
- Funcionalidades extra da época de recurso (serão definidas pelos docentes).
- Persistência entre execuções além do ficheiro de estado, do log de avisos e das saídas pedidas.

## Further Notes

- **Calendário**: a 1.ª avaliação é na semana de 23/11/2026 (20%) e a entrega final a 12/12/2026 (30%, com defesa). Até à 1.ª avaliação devem estar prontos: `Uteis`, `ElementoXML`, `Relogio`, `ConfigMotor`, motores, sensores, `Load`, o ciclo com o menu 'M', os avisos e o XML do simulador.
- **Leituras aleatórias puras**: com sensores analógicos aleatórios em cada ciclo (humidade e luz), os alarmes vão entrar e sair com frequência. A flag "já avisei" evita repetições enquanto o alarme dura, mas haverá muitos avisos ao longo do dia. Se isto prejudicar a demonstração, pode afinar-se a gama de valores de cada tipo sem mudar a arquitetura.
- **Ambiguidades dos XML fornecidos**: `PROB_AVARIA` aparece em escalas diferentes (decidido: ler exatamente e interpretar em %); o ID 200 está repetido em `Empresa1XML.xml` (o segundo é rejeitado); `PROB_1` só existe num dos ficheiros (opcional).
- **Documentos relacionados**: `Boas-Praticas-POO.md` (regras de estilo derivadas da Ficha 5; a parte dos getters públicos é substituída pela política de visibilidade deste PRD), `Proposta-Solucao-Projeto.md` (arquitetura inicial, desatualizada), `PRD-Gestao-Fabrica.md` (v1, issue #1), `Relatorio-Melhorias-Extensibilidade.md` (problemas resolvidos pela revisão de extensibilidade, para a defesa) e `Regra-Interface-Publica.md` (análise da regra dos métodos públicos com `friend`, ultrapassada pela clarificação de que só a Fábrica está limitada; útil para explicar o `friend` se for perguntado). Onde divergem, prevalece este PRD.
- **Perguntas para o docente**:
  - Respondido (2026-11-09, issue #3):
    1. Só a Fábrica está limitada aos públicos do enunciado; o `Run` é uma exceção aceite.
    2. Os métodos de colaboração da Fábrica (`Ciclo`, `Notificar_Gestor`, `Registar_Alarme`, `Aviso_Aquecimento`, `Aviso_Sobreaquecimento`, `Aviso_Avaria`) **podem ser públicos**.
    3. `Add(User*)` **é esperado**; o desenho fica para rever mais tarde.
    4. O plano B (classe `Gestor`) **não é necessário**.
- **Defesa**: a correspondência Aquário → Fábrica e Peixe → Dispositivo, as flags "já avisei" e "avariado", a fila de alarmes pendentes, o Template Method do `Dispositivo`, o Catálogo de protótipos, a reação polimórfica dos sensores, as famílias de sensores e a política de visibilidade (a Fábrica só com a interface do enunciado, mais o `Run` e os métodos de colaboração aprovados pelo docente) são os pontos de POO a destacar. A "receita para acrescentar um tipo" deve ser demonstrável ao vivo.
- **Diferenças para o v1**:
  - Catálogo separado por categoria (sem `Entrar_Na` nem `Add(Sensor)` público);
  - `Remover`, `Ligar` e o processamento interno passam a privados;
  - a política de visibilidade (só a Fábrica limitada ao enunciado, com o `Run` e um inventário de colaboração);
  - ganchos protegidos;
  - testes sem `friend`.
