---Simulador de Instruções MIPS em C---
Alunos:
- Eduardo Cornehl Wozniak
- Carolina Dobjanski

- Este repositório contém um simulador simples de instruções MIPS escrito em C. O programa permite ao usuário inserir instruções em Assembly MIPS, exibe o formato estrutural da instrução, executa as operações definidas e apresenta os valores atualizados dos registradores após a execução.

Funcionalidades:
- Leitura de Instruções: O usuário pode digitar instruções MIPS no formato Assembly, como add $t0, $t1, $t2.
- Análise da Instrução: O programa analisa a instrução inserida, separando o mnemônico e os operandos.
- Exibição do Formato Estrutural: Exibe o formato estrutural da instrução, mostrando os valores decimais de cada campo (opcode, registradores, imediato, etc.).
- Execução da Instrução: Executa a operação definida pela instrução e atualiza os registradores correspondentes.
- Exibição da Operação Realizada: Mostra a operação realizada em um formato legível, por exemplo, [$t0] = [$t1] + [$t2].
- Apresentação dos Registradores: Exibe o valor atualizado de todos os registradores após a execução da instrução.
- Simulação Simplificada de Memória: Para instruções de acesso à memória (lw e sw), é utilizada uma memória estática simulada.

Instruções Suportadas:

Tipo R:
- add: Adição
- sub: Subtração
- and: Operação AND bit a bit
- or: Operação OR bit a bit
- slt: Define se um registrador é menor que outro

Tipo I:
- addi: Adição com imediato
- lw: Carrega palavra da memória
- sw: Armazena palavra na memória
- beq: Branch if equal (desvio se igual)
- bne: Branch if not equal (desvio se diferente)

Tipo J:
- j: Salto incondicional

Estrutura do Código:
- main: Loop principal que lê as instruções e chama as funções de processamento.
- analisarInstrucao: Analisa a instrução inserida, separando mnemônico e operandos.
- exibirInstrucao: Exibe o formato estrutural da instrução.
- executarInstrucao: Executa a instrução e atualiza os registradores.
- exibirRegistradores: Mostra os valores dos registradores após a execução.
- obterNumeroRegistrador: Converte o nome do registrador para seu número correspondente.
- obterOpcode e obterFunct: Obtêm o opcode e o funct a partir do mnemônico.
- obterNomeRegistrador: Retorna o nome do registrador com base em seu número.

Observações
- Simulação Simplificada: O programa não implementa um contador de programa real (PC). Instruções de desvio e salto são indicadas como "simuladas".
- Memória: A memória é representada por um array estático, e os valores são mantidos entre as instruções graças ao uso de static.
- Registradores: Os registradores são armazenados em um array de 32 posições, representando os registradores do MIPS.

Video da explicação do código:
- https://drive.google.com/drive/folders/1r-jkYNZ44LI3cx1CLdjZ6zEQyVk8XHmM?usp=sharing

Roteiro montado para a explicação de detalhação do código:
- https://malachite-earl-ea9.notion.site/Roteiro-MIPS-14d7d98561e7808eb7f1cc67e8f513d0?pvs=4
