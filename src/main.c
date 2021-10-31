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
#define MAX_DATA 12

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
    char data[MAX_DATA]; // formato data[0]=dia data[1]=mes data[2]=ano;
    char conta[MAX_AGENCIA_E_NRO]; // agencia + codigo
} TRANSACAO;

/*------------- Variaveis globais que armazenam os dados ----------*/
    CLIENTE * v_clientes;
    CONTA * v_contas;
    TRANSACAO * v_transacoes;
    int num_clientes; //numero de clientes cadastrados atualmente
    int num_contas; //numero de contas cadastrados atualmente
    int num_transacoes; //numero de transacoes cadastrados atualmente
    FILE * cli_db; //Base de dados de clientes
    FILE * ct_db; //Base de dados de contas
    FILE * tr_db; //Base de dados de transacoes
/*----------------------------------------------------------------*/

void boot();
//void quit(CLIENTE*, CONTA*, TRANSACAO*, FILE*, FILE*, FILE*);

void menu_principal();
void menu_cliente();
void menu_conta();

int compara_valor(int, char*, int);
int ordem_alfabetica(char*, char*);
void organiza_vetor(int);

void cadastra_cliente();
void lista_cliente();
void busca_cliente(char*, int);
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

    boot();

    menu_principal();

    return 0;
}

void boot(){

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
        while(fscanf(cli_db, "%s %[^\n] %s %s %[^\n]", &cli_atual.codigo, &cli_atual.nome, &cli_atual.cpf_cnpj, &cli_atual.telefone, &cli_atual.endereco)!=EOF){
            v_clientes[i]=cli_atual;
            i++;
        }
    }
    fclose(cli_db);

    ct_db = fopen("dados_contas.txt", "r");
    if(ct_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CONTAS *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(ct_db, "%s %s %f", &ct_atual.agencia_e_nro, &ct_atual.cliente, &ct_atual.saldo)!=EOF){
            v_contas[i]=ct_atual;
            i++;
        }
    }
    fclose(ct_db);

    tr_db = fopen("dados_transacoes.txt", "r");
    if(tr_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE TRANSACOES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(tr_db, "%c %f %s %s", &tr_atual.tipo, &tr_atual.valor, &tr_atual.data, &tr_atual.conta)!=EOF){
            v_transacoes[i]=tr_atual;
            i++;
        }
    }
    fclose(tr_db);
}

/*
void quit(CLIENTE* v_clientes, CONTA* v_contas, TRANSACAO* v_transacoes, FILE* cli_db, FILE* ct_db, FILE* tr_db){

}
*/

void menu_principal(){

    char opcao;

    while(1){
        printf("\n================== Bem Vindo! ==================\n");
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
                printf("\n*!* Comando invalido digite C, T ou S para prosseguir *!*\n");
            break;
        }
    }
}

void menu_cliente(){

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
            cadastra_cliente();
        break;
        case 'L':
            lista_cliente();
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
            menu_conta();
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
int compara_valor(int tipo, char* busca, int categoria){
    /*
    *    Tipos: 1=clientes; 2=contas; 3=transacoes;
    *
    *    Categorias:
    *        clientes:
    *            1=codigo;
    *            2=cpf/cnpj;
    *            3=nome;
    *        contas:
    *            1=agencia+conta
    *            2=codigo de cliente
    *        transacoes:
    *            1=data;
    * 
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
                case 3:
                    for(i=0; i<MAX_CLIENTES; i++){
                        if(strcmp(v_clientes[i].nome, busca)==0){
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
                for(i=0; i<MAX_TRANSACOES; i++){
                    if(strcmp(v_transacoes[i].data, busca)==0){
                        return 1;
                    }
                }
            }
            return 0;
        break;
    }
}

/*
int ordem_alfabetica(char * comp_1, char * comp_2){

}
*/

/*
void organiza_vetor(int tipo){
    //
        Tipos de vetores a organizar:
        clientes=1  : organiza o vetor de clientes por ordem alfabetica;
        contas=2    : organiza o vetor de contas de acordo com a ordem alfabetica de clientes;
        transacoes=3: organiza o vetor de transacoes de acordo com a data;
    //
    CLIENTE * clientes_aux;
    CONTA * contas_aux;
    TRANSACAO * transacoes_aux;

    CLIENTE * clientes_ord;
    CONTA * contas_ord;
    TRANSACAO * transacoes_ord;

    int i, j;

    clientes_aux = (CLIENTE *) calloc(MAX_CLIENTES, sizeof(CLIENTE));
    contas_aux = (CONTA *) calloc(MAX_CONTAS, sizeof(CONTA));
    transacoes_aux = (TRANSACAO *) calloc(MAX_TRANSACOES, sizeof(TRANSACAO));

    clientes_ord = (CLIENTE *) calloc(MAX_CLIENTES, sizeof(CLIENTE));
    contas_ord = (CONTA *) calloc(MAX_CONTAS, sizeof(CONTA));
    transacoes_ord = (TRANSACAO *) calloc(MAX_TRANSACOES, sizeof(TRANSACAO));

    switch(tipo){
        case 1:
            clientes_aux = v_clientes;
            for(i=0; i<MAX_CLIENTES; i++){
                for(j=0; j<MAX_CLIENTES; j++){
                    if(ordem_alfabetica(clientes_aux[i], clientes_aux[j])){

                    }
                }
            }
        break;
        case 2:

        break;
        case 3:

        break;
    }
}
*/

void cadastra_cliente(){

    CLIENTE novo_cli;
    char opcao;
    int i;

    while(1){
        printf("\n================ Cadastro de clientes ================\n");
        printf("\nInforme o codigo do novo cliente: ");
        scanf("%s%*c", &novo_cli.codigo);
        printf("\nInforme o nome do novo cliente: ");
        scanf("%s%*c", &novo_cli.nome);
        printf("\nInforme o cpf/cnpj do novo cliente: ");
        scanf("%s%*c", &novo_cli.cpf_cnpj);
        printf("\nInforme o telefone do novo cliente: ");
        scanf("%s%*c", &novo_cli.telefone);
        printf("\nInforme o endereco do novo cliente: ");
        scanf("%s%*c", &novo_cli.endereco);

        printf("\nPor favor, confirme se as informacoes estao corretas:\n\n");
        printf("Codigo: %s\n", novo_cli.codigo);
        printf("Nome: %s\n", novo_cli.nome);
        printf("CPF/CNPJ: %s\n", novo_cli.cpf_cnpj);
        printf("Telefone: %s\n", novo_cli.telefone);
        printf("Endereco: %s\n", novo_cli.endereco);
        printf("\nDeseja salvar o cliente assim? (s/n): ");
        scanf("%c%*c", &opcao);
        
        /*
        if(opcao=='s' || opcao=='S'){
            for(i=0; i<MAX_CLIENTES; i++){
                if(compara_valor(v_clientes, NULL, NULL, 1, novo_cli.codigo, 1)){
                    printf("\n\n*!* Ja existe um cliente cadastrado com este codigo! *!*\n");
                    break;
                }
                else if(compara_valor(v_clientes, NULL, NULL, 1, novo_cli.cpf_cnpj, 2)){
                    printf("\n\n*!* Ja existe um cliente cadastrado com este CPF/CNPJ! *!*\n");
                    break;
                }
                else{
                    v_clientes[num_clientes] = novo_cli;
                    if(strcmp(v_clientes[num_clientes].codigo, novo_cli.codigo)!=0){
                        printf("\n*!* Erro ao cadastrar cliente! *!*\n");
                    }
                    else{
                        printf("\n------ Cliente cadastrado com sucesso!---------\n");
                        num_clientes++;
                        return;
                    }
                }
            }
        }
        */
    }
}

void lista_cliente(){

    int i;
    if(v_clientes[0].codigo[0]==0){
        printf("\n*!* Nenhum cliente cadastrado! *!*\n");
    }
    else{
        printf("\n============= Lista de clientes ==================\n");    
        for(i=0; i<MAX_CLIENTES; i++){
            if(v_clientes[i].codigo[0]!=0){
                printf("\nCodigo: %s", v_clientes[i].codigo);
                printf("\nNome: %s", v_clientes[i].nome);
                printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
                printf("\nTelefone: %s", v_clientes[i].telefone);
                printf("\nEndereco: %s\n", v_clientes[i].endereco);
            }
        }
    }
}