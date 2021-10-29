#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 30
#define MAX_CPF 14
#define MAX_CNPJ 18
#define MAX_TEL 15
#define MAX_END 100

#define MAX_CLIENTES 100
#define MAX_CONTAS 200
#define MAX_TRANSACOES 1000

typedef struct cliente{
    int codigo;
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char cnpj[MAX_CNPJ];
    char telefone[MAX_TEL];
    char endereco[MAX_END];
} CLIENTE;

typedef struct conta{
    int agencia;
    int numero;
    int cliente; //codigo do cliente;
    float saldo;
} CONTA;

typedef struct transacao{
    char tipo; // 'd'=debito 'c'=credito
    float valor;
    int data[3]; // formato data[0]=dia data[1]=mes data[2]=ano;
    int conta; // agencia + codigo
} TRANSACAO;

/*------------- Vetores globais que armazenam os dados ----------*/
    //CLIENTE * clientes;
    //CONTA * contas;
    //TRANSACAO * transacoes;
    CLIENTE clientes[MAX_CLIENTES];
    CONTA contas[MAX_CONTAS];
    TRANSACAO transacoes[MAX_TRANSACOES];
/*---------------------------------------------------------------*/

void boot(CLIENTE*, CONTA*, TRANSACAO*);

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

    boot(clientes, contas, transacoes);

    menu_principal();

    return 0;
}

void boot(CLIENTE* clientes, CONTA* contas, TRANSACAO* transacoes){

    clientes = (CLIENTE *) calloc(MAX_CLIENTES, sizeof(CLIENTE));
    contas = (CONTA *) calloc(MAX_CONTAS, sizeof(CONTA));
    transacoes = (TRANSACAO *) calloc(MAX_TRANSACOES, sizeof(TRANSACAO));

    CLIENTE cli_atual;
    CONTA ct_atual;
    TRANSACAO tr_atual;

    FILE * cli_db; //Base de dados de clientes
    FILE * ct_db; //Base de dados de contas
    FILE * tr_db; //Base de dados de transacoes

    char opcao;
    int i;

    cli_db = fopen("dados_clientes.txt", "r");
    if(cli_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CLIENTES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(cli_db, "%d %s %s %s %s %s", &cli_atual.codigo, &cli_atual.nome, &cli_atual.cpf, &cli_atual.cnpj, &cli_atual.telefone, &cli_atual.endereco)!=EOF){
            clientes[i]=cli_atual;
            i++;
        }
    }

    ct_db = fopen("dados_contas.txt", "r");
    if(ct_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CONTAS *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(ct_db, "%d %d %d %f", &ct_atual.agencia, &ct_atual.numero, &ct_atual.cliente, &ct_atual.saldo)!=EOF){
            contas[i]=ct_atual;
            i++;
        }
    }

    tr_db = fopen("dados_transacoes.txt", "r");
    if(tr_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE TRANSACOES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(tr_db, "%c %f %d/%d/%d %d", &tr_atual.tipo, &tr_atual.valor, &tr_atual.data[0], &tr_atual.data[1], &tr_atual.data[2], &tr_atual.conta)!=EOF){
            transacoes[i]=tr_atual;
            i++;
        }
    }
}

void menu_principal(){

    char opcao;

    while(1){
        printf("================== Bem Vindo! ==================\n");
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
            //cadastra_cliente();
        break;
        case 'L':
            //lista_cliente();
        break;
        case 'B':
            //busca_cliente();
        break;
        case 'A':
            //atualiza_cliente();
        break;
        case 'E':
            //exclui_cliente();
        break;
        case 'S':
            printf("Retornando ao menu\n");
            return;
        break;
        default:
            printf("*!* Comando invalido! *!*\n");
            menu_cliente();
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
            //lista_conta();
        break;
        case 'C':
            //cadastra_conta_p_cliente();
        break;
        case 'L':
            //lista_conta_p_cliente();
        break;
        case 'W':
            //saca_conta();
        break;
        case 'D':
            //deposita_conta();
        break;
        case 'T':
            //transfere_conta();
        break;
        case 'E':
            //extrato_conta();
        break;
        case 'S':
            printf("Retornando ao menu\n");
            return;
        break;
        default:
            printf("*!* Comando invalido! *!*\n");
            menu_conta();
        break;
    }
}