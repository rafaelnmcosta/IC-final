#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 30
#define MAX_CPF 14
#define MAX_CNPJ 18
#define MAX_TEL 15
#define MAX_END 100

typedef struct cliente{
    int codigo;
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char cnpj[MAX_CNPJ];
    char telefone[MAX_TEL];
    char endereco[MAX_END];
} CLIENTE;

void menu_principal();
void menu_cliente();
void menu_conta();

void cadastra_cliente();
void lista_cliente();
void busca_cliente();
void atualiza_cliente();
void exclui_cliente();

void lista_conta();
void cadastra_conta_p_cliente();
void lista_conta_p_cliente();
void saca_conta();
void deposita_conta();
void transfere_conta();
void extrato_conta();

int main(){

    menu_principal();

    return 0;
}

void menu_principal(){

    char opcao;

    while(1){
        printf("================= Bem Vindo! =================\n");
        printf("Digite um comando para prosseguir:\n");
        printf("C - Gerenciar Clientes\n");
        printf("T - Gerenciar Contas\n");
        printf("S - Sair\n");
        printf("\nEscolha: ");

        scanf("%c%*c", &opcao);
        switch(opcao){
            case'C':
                menu_cliente();
            break;
            case'T':
                menu_conta();
            break;
            case'S':
                return;
            break;
            default:
                printf("\n*!* Comando inválido digite C, T ou S para prosseguir *!*\n");
            break;
        }
    }
}

void menu_cliente(){
    char opcao;

    printf("=============== Gerenciar Clientes ===============\n");
    printf("Digite um comando para prosseguir:\n");
    printf("C - Cadastrar um cliente\n");
    printf("L - Listar todos os clientes cadastrados\n");
    printf("B - Buscar cliente já cadastrado\n");
    printf("A - Atualizar um cliente cadastrado\n");
    printf("E - Excluir um cliente cadastrado\n");
    printf("S - Sair\n");
    printf("\nEscolha: ");

    scanf("%c%*c", &opcao);
    switch (opcao){
        case 'C':
            cadastra_cliente();
        break;
        case 'L':
            lista_cliente();
        break;
        case 'B':
            busca_cliente();
        break;
        case 'A':
            atualiza_cliente();
        break;
        case 'E':
            exclui_cliente();
        break;
        case 'S':
            printf("Retornando ao menu\n");
            return;
        break;
        default:
            printf("*!* Comando invalido! *!*\n");
        break;
    }
}

void menu_conta(){
    char opcao;

    printf("================ Gerenciar contas ================\n");
    printf("Digite um comando para prosseguir:\n");
    printf("R - Listagem de todas as contas cadastradas.\n");
    printf("C - Cadastrar uma conta para um cliente.\n");
    printf("L - Listar todas as contas de um cliente.\n");
    printf("W - Realizar um saque em uma conta.\n");
    printf("D - Realizar um depósito em uma conta.\n");
    printf("T - Realizar transferência entre contas.\n");
    printf("E - Exibir extrato de uma conta.\n");
    printf("S - Sair\n");
    printf("\nEscolha: ");

    scanf("%c%*c", &opcao);
    switch (opcao){
        case 'R':
            printf("Funcao de R\n");
        break;
        case 'C':
            printf("Funcao de C\n");
        break;
        case 'L':
            printf("Funcao de L\n");
        break;
        case 'W':
            printf("Funcao de W\n");
        break;
        case 'D':
            printf("Funcao de D\n");
        break;
        case 'T':
            printf("Funcao de T\n");
        break;
        case 'E':
            printf("Funcao de E\n");
        break;
        case 'S':
            printf("Retornando ao menu\n");
            return;
        break;
        default:
            printf("*!* Comando invalido! *!*\n");
        break;
    }
}