#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODIGO 8
#define MAX_NOME 30
#define MAX_CPF_CNPJ 18
#define MAX_TEL 15
#define MAX_END 100

#define MAX_AGENCIA_E_NRO 10

#define MAX_CLIENTES 100
#define MAX_CONTAS 200
#define MAX_TRANSACOES 1000

typedef struct cliente{
    char codigo[MAX_CODIGO];
    char nome[MAX_NOME];
    char cpf_cnpj[MAX_CPF_CNPJ];
    char telefone[MAX_TEL];
    char endereco[MAX_END];
} CLIENTE;

typedef struct conta{
    char agencia_e_nro[MAX_AGENCIA_E_NRO];
    char cliente[MAX_CODIGO]; //codigo do cliente;
    float saldo;
} CONTA;

typedef struct transacao{
    char tipo; // 'd'=debito 'c'=credito
    float valor;
    char data[12]; // formato data[0]=dia data[1]=mes data[2]=ano;
    char conta[MAX_AGENCIA_E_NRO]; // agencia + codigo
} TRANSACAO;

/*------------- Variaveis globais que armazenam os dados ----------*/
    //CLIENTE v_clientes[MAX_CLIENTES];
    //CONTA v_contas[MAX_CONTAS];
    //TRANSACAO v_transacoes[MAX_TRANSACOES];
    int num_clientes;
    int num_contas;
    int num_transacoes;
    //FILE * cli_db; //Base de dados de clientes
    //FILE * ct_db; //Base de dados de contas
    //FILE * tr_db; //Base de dados de transacoes
/*----------------------------------------------------------------*/

//void boot(CLIENTE*, CONTA*, TRANSACAO*, FILE*, FILE*, FILE*);
//void quit(CLIENTE*, CONTA*, TRANSACAO*, FILE*, FILE*, FILE*);

void menu_principal(CLIENTE*, CONTA*, TRANSACAO*);
void menu_cliente(CLIENTE*);
void menu_conta(CLIENTE*, CONTA*, TRANSACAO*);

int compara_valor(CLIENTE*, CONTA*, TRANSACAO*, int, char*, int);

void cadastra_cliente(CLIENTE*);
void lista_cliente(CLIENTE*);
void busca_cliente(CLIENTE*);
void atualiza_cliente(CLIENTE*);
void exclui_cliente(CLIENTE*);

void lista_conta();
void cadastra_conta_p_cliente();
void lista_conta_p_cliente();
void saca_conta();
void deposita_conta();
void transfere_conta();
void extrato_conta();


int main(){

    //boot(v_clientes, v_contas, v_transacoes, cli_db, ct_db, tr_db);

    CLIENTE * v_clientes;
    CONTA * v_contas;
    TRANSACAO * v_transacoes;

    v_clientes = (CLIENTE *) calloc(MAX_CLIENTES, sizeof(CLIENTE));
    v_contas = (CONTA *) calloc(MAX_CONTAS, sizeof(CONTA));
    v_transacoes = (TRANSACAO *) calloc(MAX_TRANSACOES, sizeof(TRANSACAO));

    menu_principal(v_clientes, v_contas, v_transacoes);

    return 0;
}
/*
void boot(CLIENTE* v_clientes, CONTA* v_contas, TRANSACAO* v_transacoes, FILE* cli_db, FILE* ct_db, FILE* tr_db){

    v_clientes = (CLIENTE *) calloc(MAX_CLIENTES, sizeof(CLIENTE));
    v_contas = (CONTA *) calloc(MAX_CONTAS, sizeof(CONTA));
    v_transacoes = (TRANSACAO *) calloc(MAX_TRANSACOES, sizeof(TRANSACAO));

    CLIENTE cli_atual;
    CONTA ct_atual;
    TRANSACAO tr_atual;

    int i;

    cli_db = fopen("dados_clientes.txt", "r");
    if(cli_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CLIENTES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(cli_db, "%d %s %s %s %s", &cli_atual.codigo, &cli_atual.nome, &cli_atual.cpf_cnpj, &cli_atual.telefone, &cli_atual.endereco)!=EOF){
            v_clientes[i]=cli_atual;
            i++;
        }
    }
    fclose(cli_db);

    ct_db = fopen("dados_contas.txt", "r");
    if(ct_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CONTAS *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(ct_db, "%d %d %d %f", &ct_atual.agencia, &ct_atual.numero, &ct_atual.cliente, &ct_atual.saldo)!=EOF){
            v_contas[i]=ct_atual;
            i++;
        }
    }
    fclose(ct_db);

    tr_db = fopen("dados_transacoes.txt", "r");
    if(tr_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE TRANSACOES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(tr_db, "%c %f %s %d", &tr_atual.tipo, &tr_atual.valor, &tr_atual.data, &tr_atual.conta)!=EOF){
            v_transacoes[i]=tr_atual;
            i++;
        }
    }
    fclose(tr_db);


}

void exit(CLIENTE* v_clientes, CONTA* v_contas, TRANSACAO* v_transacoes, FILE* cli_db, FILE* ct_db, FILE* tr_db){

}
*/

void menu_principal(CLIENTE * v_clientes, CONTA * v_contas, TRANSACAO * v_transacoes){

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
                menu_cliente(v_clientes);
            break;
            case'T':
                menu_conta(v_clientes, v_contas, v_transacoes);
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

void menu_cliente(CLIENTE * v_clientes){

    char opcao;

    printf("=============== Gerenciar Clientes ===============\n");
    printf("Digite um comando para prosseguir:\n");
    printf("C - Cadastrar um cliente\n");
    printf("L - Listar todos os v_clientes cadastrados\n");
    printf("B - Buscar cliente já cadastrado\n");
    printf("A - Atualizar um cliente cadastrado\n");
    printf("E - Excluir um cliente cadastrado\n");
    printf("S - Sair\n");
    printf("\nEscolha: ");

    scanf("%c%*c", &opcao);
    switch (opcao){
        case 'C':
            cadastra_cliente(v_clientes);
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
            menu_cliente(v_clientes);
        break;
    }
}

void menu_conta(CLIENTE * v_clientes, CONTA * v_contas, TRANSACAO * v_transacoes){
    char opcao;

    printf("================ Gerenciar Contas ================\n");
    printf("Digite um comando para prosseguir:\n");
    printf("R - Listagem de todas as v_contas cadastradas.\n");
    printf("C - Cadastrar uma conta para um cliente.\n");
    printf("L - Listar todas as v_contas de um cliente.\n");
    printf("W - Realizar um saque em uma conta.\n");
    printf("D - Realizar um depósito em uma conta.\n");
    printf("T - Realizar transferência entre v_contas.\n");
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
            menu_conta(v_clientes, v_contas, v_transacoes);
        break;
    }
}

/**
 * @brief Funcao que realiza uma busca por um determinado valor no vetor de informacoes especificado
 *  e retorna um codigo de acordo com o resultado da busca;
 * 
 * @param v_clientes vetor que possui os clientes armazenados;
 * @param v_contas vetor que possui as contas armazenadas;
 * @param v_transacoes vetor que possui as transacoes armazenadas;
 * @param tipo inteiro que define se a busca sera de clientes, contas ou transacoes;
 * @param busca o que sera buscado pela funcao;
 * @param categoria o campo a ser comparado durante a busca;
 * @return codigo referente ao resultado da busca: 1=encontrado 0=nao encontrado
 */
int compara_valor(CLIENTE* v_clientes, CONTA* v_contas, TRANSACAO* v_transacoes, int tipo, char* busca, int categoria){
    /*
    Tipos: 1=clientes; 2=contas; 3=transacoes;

    Categorias:
        clientes:
            1=codigo;
            2=cpf/cnpj;
        contas:
            1=agencia+conta
            2=codigo de cliente
        transacoes:
            1=data;
    */
    
    int i;

    switch(tipo){
        case 1:
            switch(categoria){
                case 1:
                    for(i=0; i<MAX_CLIENTES; i++){
                        if(strcmp(v_clientes[i].codigo, busca)==0){
                            return 1;
                        }
                    }
                    return 0;
                break;
                case 2:
                    for(i=0; i<MAX_CLIENTES; i++){
                        if(strcmp(v_clientes[i].cpf_cnpj, busca)==0){
                            return 1;
                        }
                    }
                    return 0;
                break;
            }
        break;
        case 2:
            switch(categoria){
                case 1:
                    for(i=0; i<MAX_CONTAS; i++){
                        if(strcmp(v_contas[i].agencia_e_nro, busca)==0){
                            return 1;
                        }
                    }
                    return 0;
                break;
                case 2:
                    for(i=0; i<MAX_CONTAS; i++){
                        if(strcmp(v_contas[i].cliente, busca)==0){
                            return 1;
                        }
                    }
                    return 0;
                break;
            }
        break;
        case 3:
            if(categoria==1){
                /*@TODO
                    -----------------------------------implementar busca por data;--------------------------------
                */
            }
        break;
    }
}

void cadastra_cliente(CLIENTE* v_clientes){

    CLIENTE novo_cli;
    char opcao;
    int i;

    while(1){
        printf("\n================ Cadastro de clientes ================\n");
        printf("\nInforme o codigo do novo cliente: ");
        scanf("%d", &novo_cli.codigo);
        printf("\nInforme o nome do novo cliente: ");
        scanf("%s", &novo_cli.nome);
        printf("\nInforme o cpf/cnpj do novo cliente: ");
        scanf("%s", &novo_cli.cpf_cnpj);
        printf("\nInforme o telefone do novo cliente: ");
        scanf("%s", &novo_cli.telefone);
        printf("\nInforme o endereco do novo cliente: ");
        scanf("%s", &novo_cli.endereco);

        printf("\nPor favor, confirme se as informacoes estao corretas:\n");
        printf("Codigo: %d\n", novo_cli.codigo);
        printf("Nome: %s\n", novo_cli.nome);
        printf("CPF/CNPJ: %s\n", novo_cli.cpf_cnpj);
        printf("Telefone: %s\n", novo_cli.telefone);
        printf("Endereco: %s\n", novo_cli.endereco);
        printf("Deseja salvar o cliente assim? (s/n): ");
        scanf("%c", &opcao);

        if(opcao=='s' || opcao=='S'){
            for(i=0; i<MAX_CLIENTES; i++){
                if(compara_valor(v_clientes, NULL, NULL, 1, novo_cli.codigo, 1)){
                    printf("\n\n*!* Ja existe um cliente cadastrado com este codigo! *!*\n");
                }
                else if(compara_valor(v_clientes, NULL, NULL, 1, novo_cli.cpf_cnpj, 2)){
                    printf("\n\n*!*Ja existe um cliente cadastrado com este CPF/CNPJ! *!*\n");
                }
                else{
                    
                }
            }
        }
    }

}