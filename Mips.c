// Alunos: 
// Eduardo Cornehl Wozniak
// Carolina Dobjanski

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_REGISTRADORES 32

// Definição das estruturas para os tipos de instrução
typedef struct {
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int shamt;
    unsigned int funct;
} InstrucaoTipoR;

typedef struct {
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    int imediato;
} InstrucaoTipoI;

typedef struct {
    unsigned int opcode;
    unsigned int endereco;
} InstrucaoTipoJ;

// Protótipos das funções
void analisarInstrucao(char *instrucaoStr, char *mnemonic, char operandos[][10]);
void exibirInstrucao(char *mnemonic, char operandos[][10]);
void executarInstrucao(char *mnemonic, char operandos[][10], int *registradores);
void exibirRegistradores(int *registradores);
int obterNumeroRegistrador(char *reg);
unsigned int obterOpcode(char *mnemonic);
unsigned int obterFunct(char *mnemonic);

// Função para obter o nome do registrador a partir do número
void obterNomeRegistrador(int numero, char *nome);

int main() {
    char instrucaoStr[50];
    char mnemonic[10];
    char operandos[3][10];
    int registradores[NUM_REGISTRADORES] = {0};

    while (1) {
        printf("\nDigite uma instrucao MIPS ou 'sair' para encerrar: ");
        fgets(instrucaoStr, sizeof(instrucaoStr), stdin);
        instrucaoStr[strcspn(instrucaoStr, "\n")] = '\0'; // Remover nova linha

        // Opção de saída
        if (strcmp(instrucaoStr, "sair") == 0) {
            printf("\nPrograma encerrado\n");
            break;
        }

        analisarInstrucao(instrucaoStr, mnemonic, operandos);
        exibirInstrucao(mnemonic, operandos);
        executarInstrucao(mnemonic, operandos, registradores);
        exibirRegistradores(registradores);
    }

    return 0;
}

// Função para analisar a instrução e extrair o mnemônico e operandos
void analisarInstrucao(char *instrucaoStr, char *mnemonic, char operandos[][10]) {
    char *token;
    int i = 0;

    token = strtok(instrucaoStr, " ,");
    if (token != NULL) {
        strcpy(mnemonic, token);
    } else {
        mnemonic[0] = '\0';
    }

    while ((token = strtok(NULL, " ,")) != NULL && i < 3) {
        strcpy(operandos[i], token);
        i++;
    }

    // Preencher operandos restantes com strings vazias
    for (; i < 3; i++) {
        operandos[i][0] = '\0';
    }
}

// Função para exibir o formato estrutural da instrução
void exibirInstrucao(char *mnemonic, char operandos[][10]) {
    unsigned int opcode = obterOpcode(mnemonic);
    printf("\nFormato Estrutural da Instrucao:\n");
    printf("mnemonic: %s\n", mnemonic);
    printf("Opcode: %u\n", opcode);

    if (opcode == 0) { // Tipo R
        unsigned int rs = obterNumeroRegistrador(operandos[1]);
        unsigned int rt = obterNumeroRegistrador(operandos[2]);
        unsigned int rd = obterNumeroRegistrador(operandos[0]);
        unsigned int shamt = 0;
        unsigned int funct = obterFunct(mnemonic);

        printf("rs: %u\n", rs);
        printf("rt: %u\n", rt);
        printf("rd: %u\n", rd);
        printf("shamt: %u\n", shamt);
        printf("funct: %u\n", funct);
    } else if (opcode == 2 || opcode == 3) { // Tipo J
        unsigned int endereco = atoi(operandos[0]);
        printf("Endereco: %u\n", endereco);
    } else if (opcode != 63) { // Tipo I
        unsigned int rs = obterNumeroRegistrador(operandos[1]);
        unsigned int rt = obterNumeroRegistrador(operandos[0]);
        int imediato = atoi(operandos[2]);

        printf("rs: %u\n", rs);
        printf("rt: %u\n", rt);
        printf("Imediato: %d\n", imediato);
    } else {
        printf("Instrucao nao suportada.\n");
    }
}

// Função para executar a instrução
void executarInstrucao(char *mnemonic, char operandos[][10], int *registradores) {
    unsigned int opcode = obterOpcode(mnemonic);

    // Variáveis para armazenar os nomes dos registradores
    char nome_rd[10], nome_rs[10], nome_rt[10];

    if (opcode == 0) { // Tipo R
        unsigned int rs = obterNumeroRegistrador(operandos[1]);
        unsigned int rt = obterNumeroRegistrador(operandos[2]);
        unsigned int rd = obterNumeroRegistrador(operandos[0]);
        unsigned int funct = obterFunct(mnemonic);

        obterNomeRegistrador(rd, nome_rd);
        obterNomeRegistrador(rs, nome_rs);
        obterNomeRegistrador(rt, nome_rt);

        if (funct == 32) { // add
            registradores[rd] = registradores[rs] + registradores[rt];
            printf("[%s] = [%s] + [%s]\n", nome_rd, nome_rs, nome_rt);
        } else if (funct == 34) { // sub
            registradores[rd] = registradores[rs] - registradores[rt];
            printf("[%s] = [%s] - [%s]\n", nome_rd, nome_rs, nome_rt);
        } else if (funct == 36) { // and
            registradores[rd] = registradores[rs] & registradores[rt];
            printf("[%s] = [%s] & [%s]\n", nome_rd, nome_rs, nome_rt);
        } else if (funct == 37) { // or
            registradores[rd] = registradores[rs] | registradores[rt];
            printf("[%s] = [%s] | [%s]\n", nome_rd, nome_rs, nome_rt);
        } else if (funct == 42) { // slt
            registradores[rd] = (registradores[rs] < registradores[rt]) ? 1 : 0;
            printf("[%s] = [%s] < [%s]\n", nome_rd, nome_rs, nome_rt);
        } else {
            printf("Instrucao nao suportada.\n");
        }
    } else if (opcode == 35) { // lw
        unsigned int base = obterNumeroRegistrador(operandos[2]);
        unsigned int rt = obterNumeroRegistrador(operandos[0]);
        int offset = atoi(operandos[1]);

        obterNomeRegistrador(rt, nome_rt);
        obterNomeRegistrador(base, nome_rs); // Usando nome_rs para o base

        // Simulação simplificada: vamos supor que a memória é um array de inteiros
        static int memoria[1024] = {0};
        registradores[rt] = memoria[registradores[base] + offset];
        printf("[%s] = MEM[%d]\n", nome_rt, registradores[base] + offset);
    } else if (opcode == 43) { // sw
        unsigned int base = obterNumeroRegistrador(operandos[2]);
        unsigned int rt = obterNumeroRegistrador(operandos[0]);
        int offset = atoi(operandos[1]);

        obterNomeRegistrador(rt, nome_rt);
        obterNomeRegistrador(base, nome_rs); // Usando nome_rs para o base

        // Simulação simplificada: vamos supor que a memória é um array de inteiros
        static int memoria[1024] = {0};
        memoria[registradores[base] + offset] = registradores[rt];
        printf("MEM[%d] = [%s]\n", registradores[base] + offset, nome_rt);
    } else if (opcode == 4) { // beq
        unsigned int rs = obterNumeroRegistrador(operandos[0]);
        unsigned int rt = obterNumeroRegistrador(operandos[1]);
        int offset = atoi(operandos[2]);

        obterNomeRegistrador(rs, nome_rs);
        obterNomeRegistrador(rt, nome_rt);

        if (registradores[rs] == registradores[rt]) {
            printf("Branch if [%s] == [%s] (simulado)\n", nome_rs, nome_rt);
            // Simulação simplificada: não vamos alterar o PC
        }
    } else if (opcode == 5) { // bne
        unsigned int rs = obterNumeroRegistrador(operandos[0]);
        unsigned int rt = obterNumeroRegistrador(operandos[1]);
        int offset = atoi(operandos[2]);

        obterNomeRegistrador(rs, nome_rs);
        obterNomeRegistrador(rt, nome_rt);

        if (registradores[rs] != registradores[rt]) {
            printf("Branch if [%s] != [%s] (simulado)\n", nome_rs, nome_rt);
            // Simulação simplificada: não vamos alterar o PC
        }
    } else if (opcode == 8) { // addi
        unsigned int rs = obterNumeroRegistrador(operandos[1]);
        unsigned int rt = obterNumeroRegistrador(operandos[0]);
        int imediato = atoi(operandos[2]);

        obterNomeRegistrador(rs, nome_rs);
        obterNomeRegistrador(rt, nome_rt);

        registradores[rt] = registradores[rs] + imediato;
        printf("[%s] = [%s] + %d\n", nome_rt, nome_rs, imediato);
    } else if (opcode == 2) { // j
        unsigned int endereco = atoi(operandos[0]);
        printf("Jump to address %u (simulado)\n", endereco);
        // Simulação simplificada: não vamos alterar o PC
    } else {
        printf("Instrucao nao suportada.\n");
    }
}

// Função para exibir os valores dos registradores
void exibirRegistradores(int *registradores) {
    printf("\nValores dos Registradores:\n");
    for (int i = 0; i < NUM_REGISTRADORES; i++) {
        printf("$%d = %d\n", i, registradores[i]);
    }
}

// Função para obter o número do registrador a partir de sua representação em string
int obterNumeroRegistrador(char *reg) {
    if (reg[0] == '$') {
        if (reg[1] == 'z') { // $zero
            return 0;
        } else if (reg[1] == 't') { // $t0 - $t7
            return 8 + (reg[2] - '0');
        } else if (reg[1] == 's') { // $s0 - $s7
            return 16 + (reg[2] - '0');
        } else if (reg[1] == 'a') { // $a0 - $a3
            return 4 + (reg[2] - '0');
        } else if (isdigit(reg[1])) { // $0 - $31
            return atoi(&reg[1]);
        }
    }
    return -1; // Registrador inválido
}

// Função para obter o opcode a partir do mnemônico
unsigned int obterOpcode(char *mnemonic) {
    if (strcmp(mnemonic, "add") == 0 || strcmp(mnemonic, "sub") == 0 ||
        strcmp(mnemonic, "and") == 0 || strcmp(mnemonic, "or") == 0 ||
        strcmp(mnemonic, "slt") == 0) {
        return 0;
    } else if (strcmp(mnemonic, "lw") == 0) {
        return 35;
    } else if (strcmp(mnemonic, "sw") == 0) {
        return 43;
    } else if (strcmp(mnemonic, "beq") == 0) {
        return 4;
    } else if (strcmp(mnemonic, "bne") == 0) {
        return 5;
    } else if (strcmp(mnemonic, "addi") == 0) {
        return 8;
    } else if (strcmp(mnemonic, "j") == 0) {
        return 2;
    } else {
        return 63; // Opcode inválido (não suportado)
    }
}

// Função para obter o funct a partir do mnemônico (para instruções tipo R)
unsigned int obterFunct(char *mnemonic) {
    if (strcmp(mnemonic, "add") == 0) {
        return 32;
    } else if (strcmp(mnemonic, "sub") == 0) {
        return 34;
    } else if (strcmp(mnemonic, "and") == 0) {
        return 36;
    } else if (strcmp(mnemonic, "or") == 0) {
        return 37;
    } else if (strcmp(mnemonic, "slt") == 0) {
        return 42;
    } else {
        return 0;
    }
}

// Função para obter o nome do registrador a partir do número
void obterNomeRegistrador(int numero, char *nome) {
    if (numero == 0) {
        strcpy(nome, "$zero");
    } else if (numero >= 8 && numero <= 15) {
        sprintf(nome, "$t%d", numero - 8);
    } else if (numero >= 16 && numero <= 23) {
        sprintf(nome, "$s%d", numero - 16);
    } else if (numero >= 4 && numero <= 7) {
        sprintf(nome, "$a%d", numero - 4);
    } else if (numero >= 1 && numero <= 3) {
        sprintf(nome, "$v%d", numero - 2);
    } else {
        sprintf(nome, "$%d", numero);
    }
}
