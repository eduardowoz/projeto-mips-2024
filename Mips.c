#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_REGISTERS 32

// Definição das estruturas para os tipos de instrução
typedef struct {
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int shamt;
    unsigned int funct;
} RTypeInstruction;

typedef struct {
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    int immediate;
} ITypeInstruction;

typedef struct {
    unsigned int opcode;
    unsigned int address;
} JTypeInstruction;

// Protótipos das funções
void parseInstruction(char *instructionStr, char *mnemonic, char operands[][10]);
void displayInstruction(char *mnemonic, char operands[][10]);
void executeInstruction(char *mnemonic, char operands[][10], int *registers);
void displayRegisters(int *registers);
int getRegisterNumber(char *reg);
unsigned int getOpcode(char *mnemonic);
unsigned int getFunct(char *mnemonic);

int main() {
    char instructionStr[50];
    char mnemonic[10];
    char operands[3][10];
    int registers[NUM_REGISTERS] = {0};

    while (1) {
        printf("\nDigite uma instrucao MIPS ou 'sair' para encerrar: ");
        fgets(instructionStr, sizeof(instructionStr), stdin);
        instructionStr[strcspn(instructionStr, "\n")] = '\0'; // Remover nova linha

        // Opção de saída
        if (strcmp(instructionStr, "sair") == 0) {
            printf("\nprograma encerrado\n");
            break;
        }

        parseInstruction(instructionStr, mnemonic, operands);
        displayInstruction(mnemonic, operands);
        executeInstruction(mnemonic, operands, registers);
        displayRegisters(registers);
    }

    return 0;
}

// Função para analisar a instrução e extrair o mnemônico e operandos
void parseInstruction(char *instructionStr, char *mnemonic, char operands[][10]) {
    char *token;
    int i = 0;

    token = strtok(instructionStr, " ,");
    if (token != NULL) {
        strcpy(mnemonic, token);
    } else {
        mnemonic[0] = '\0';
    }

    while ((token = strtok(NULL, " ,")) != NULL && i < 3) {
        strcpy(operands[i], token);
        i++;
    }

    // Preencher operandos restantes com strings vazias
    for (; i < 3; i++) {
        operands[i][0] = '\0';
    }
}

// Função para exibir o formato estrutural da instrução
void displayInstruction(char *mnemonic, char operands[][10]) {
    unsigned int opcode = getOpcode(mnemonic);
    printf("\nFormato Estrutural da Instrucao:\n");
    printf("Mnemonic: %s\n", mnemonic);
    printf("Opcode: %u\n", opcode);

    if (opcode == 0) { // Tipo R
        unsigned int rs = getRegisterNumber(operands[1]);
        unsigned int rt = getRegisterNumber(operands[2]);
        unsigned int rd = getRegisterNumber(operands[0]);
        unsigned int shamt = 0;
        unsigned int funct = getFunct(mnemonic);

        printf("rs: %u\n", rs);
        printf("rt: %u\n", rt);
        printf("rd: %u\n", rd);
        printf("shamt: %u\n", shamt);
        printf("funct: %u\n", funct);
    } else if (opcode == 2 || opcode == 3) { // Tipo J
        unsigned int address = atoi(operands[0]);
        printf("Endereço: %u\n", address);
    } else if (opcode != 63) { // Tipo I
        unsigned int rs = getRegisterNumber(operands[1]);
        unsigned int rt = getRegisterNumber(operands[0]);
        int immediate = atoi(operands[2]);

        printf("rs: %u\n", rs);
        printf("rt: %u\n", rt);
        printf("Immediate: %d\n", immediate);
    } else {
        printf("Instrucao nao suportada.\n");
    }
}

// Função para executar a instrução
void executeInstruction(char *mnemonic, char operands[][10], int *registers) {
    unsigned int opcode = getOpcode(mnemonic);

    if (opcode == 0) { // Tipo R
        unsigned int rs = getRegisterNumber(operands[1]);
        unsigned int rt = getRegisterNumber(operands[2]);
        unsigned int rd = getRegisterNumber(operands[0]);
        unsigned int funct = getFunct(mnemonic);

        if (funct == 32) { // add
            registers[rd] = registers[rs] + registers[rt];
        } else if (funct == 34) { // sub
            registers[rd] = registers[rs] - registers[rt];
        } else if (funct == 36) { // and
            registers[rd] = registers[rs] & registers[rt];
        } else if (funct == 37) { // or
            registers[rd] = registers[rs] | registers[rt];
        } else if (funct == 42) { // slt
            registers[rd] = (registers[rs] < registers[rt]) ? 1 : 0;
        } else {
            printf("Instrucao nao suportada.\n");
        }
    } else if (opcode == 35) { // lw
        unsigned int base = getRegisterNumber(operands[1]);
        unsigned int rt = getRegisterNumber(operands[0]);
        int offset = atoi(operands[2]);
        // Simulação simplificada: vamos supor que a memória é um array de inteiros
        int memory[1024] = {0};
        registers[rt] = memory[registers[base] + offset];
    } else if (opcode == 43) { // sw
        unsigned int base = getRegisterNumber(operands[1]);
        unsigned int rt = getRegisterNumber(operands[0]);
        int offset = atoi(operands[2]);
        // Simulação simplificada: vamos supor que a memória é um array de inteiros
        int memory[1024] = {0};
        memory[registers[base] + offset] = registers[rt];
    } else if (opcode == 4) { // beq
        unsigned int rs = getRegisterNumber(operands[0]);
        unsigned int rt = getRegisterNumber(operands[1]);
        int offset = atoi(operands[2]);
        if (registers[rs] == registers[rt]) {
            // Simulação simplificada: não vamos alterar o PC
            printf("Branch taken (simulado).\n");
        }
    } else if (opcode == 5) { // bne
        unsigned int rs = getRegisterNumber(operands[0]);
        unsigned int rt = getRegisterNumber(operands[1]);
        int offset = atoi(operands[2]);
        if (registers[rs] != registers[rt]) {
            // Simulação simplificada: não vamos alterar o PC
            printf("Branch taken (simulado).\n");
        }
    } else if (opcode == 8) { // addi
        unsigned int rs = getRegisterNumber(operands[1]);
        unsigned int rt = getRegisterNumber(operands[0]);
        int immediate = atoi(operands[2]);
        registers[rt] = registers[rs] + immediate;
    } else if (opcode == 2) { // j
        unsigned int address = atoi(operands[0]);
        // Simulação simplificada: não vamos alterar o PC
        printf("Jump to address %u (simulado).\n", address);
    } else {
        printf("Instrucao nao suportada.\n");
    }
}

// Função para exibir os valores dos registradores
void displayRegisters(int *registers) {
    printf("\nValores dos Registradores:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("$%d = %d\n", i, registers[i]);
    }
}

// Função para obter o número do registrador a partir de sua representação em string
int getRegisterNumber(char *reg) {
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
unsigned int getOpcode(char *mnemonic) {
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
unsigned int getFunct(char *mnemonic) {
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