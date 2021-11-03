#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CODIGO 10
#define MAX_NOME 30
#define MAX_CPF_CNPJ 20
#define MAX_TEL 15
#define MAX_END 100

#define MAX_AGENCIA 5
#define MAX_NUMERO 10

#define MAX_DESCRICAO 100
#define MAX_TIPO 10

#define MAX_CLIENTES 100
#define MAX_CONTAS 200
#define MAX_TRANSACOES 1000
#define MAX_DATA 18

typedef struct cliente{
    char codigo[MAX_CODIGO];
    char nome[MAX_NOME];
    char cpf_cnpj[MAX_CPF_CNPJ];
    char telefone[MAX_TEL];
    char endereco[MAX_END];
} CLIENTE;

typedef struct conta{
    char agencia[MAX_AGENCIA];
    char numero[MAX_NUMERO];
    char cliente[MAX_CODIGO]; //codigo do cliente;
    char nome_cliente[MAX_NOME];
    float saldo;
} CONTA;

typedef struct transacao{
    char tipo[MAX_TIPO];
    float valor;
    char data[MAX_DATA];
    char conta[MAX_AGENCIA+MAX_NUMERO]; // agencia + codigo (aaaccccccc)
    char descricao[MAX_DESCRICAO];
} TRANSACAO;

/*------------- Variaveis globais que armazenam os dados ----------*/

    CLIENTE * v_clientes; //vetor que guarda os clientes em tempo de execução
    CONTA * v_contas; //vetor que guarda as contas em tempo de execução
    TRANSACAO * v_transacoes; //vetor que guarda as transacoes em tempo de execução

    int num_clientes; //numero de clientes cadastrados atualmente
    int num_contas; //numero de contas cadastrados atualmente
    int num_transacoes; //numero de transacoes cadastrados atualmente

    char data_hoje[MAX_DATA]; //String que armazena a data em que o programa é executado

    FILE * cli_db; //Base de dados de clientes
    FILE * ct_db; //Base de dados de contas
    FILE * tr_db; //Base de dados de transacoes

/*----------------------------------------------------------------*/

void gera_data();
void boot();
void save();

void menu_principal();
void menu_cliente();
void menu_conta();

int encontra_valor(int, char*, int);
void organiza_vetor(int);
void conta_notas(int);
void calcula_data(int, int*, int*, int*);

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

    boot();

    menu_principal();

    save();

    return 0;
}

void gera_data(){

    struct tm *p;
    time_t seconds;
    char c_day[3], c_month[3], c_year[3], zero[]="0";
    int day, month, year; 

    time(&seconds);
    p = localtime(&seconds);

    day = p->tm_mday;
    month = p->tm_mon+1;
    year = p->tm_year+1900;

    itoa(day, c_day, 10);
    if(day<10){
        strcat(zero, c_day);
        strcpy(c_day, zero);
    }
    strcpy(data_hoje, c_day);
    strcat(data_hoje, "/");

    itoa(month, c_month, 10);
    if(month<10){
        strcat(zero, c_month);
        strcpy(c_month, zero);
    }
    strcat(data_hoje, c_month);
    strcat(data_hoje, "/");

    itoa(year, c_year, 10);
    strcat(data_hoje, c_year);
}

void boot(){

    gera_data();

    v_clientes = (CLIENTE *) calloc(MAX_CLIENTES, sizeof(CLIENTE));
    v_contas = (CONTA *) calloc(MAX_CONTAS, sizeof(CONTA));
    v_transacoes = (TRANSACAO *) calloc(MAX_TRANSACOES, sizeof(TRANSACAO));

    CLIENTE cli_atual;
    CONTA ct_atual;
    TRANSACAO tr_atual;

    int i;

    num_clientes=0;
    cli_db = fopen("dados_clientes.txt", "r");
    if(cli_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CLIENTES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(cli_db, "%s %[^\n] %[^\n] %[^\n] %[^\n]", &cli_atual.codigo, &cli_atual.nome, &cli_atual.cpf_cnpj, &cli_atual.telefone, &cli_atual.endereco)!=EOF){
            v_clientes[i]=cli_atual;
            num_clientes++;
            i++;
        }
    }
    fclose(cli_db);

    num_contas=0;
    ct_db = fopen("dados_contas.txt", "r");
    if(ct_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CONTAS *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(ct_db, "%s %s %s %[^\n] %f", &ct_atual.agencia, &ct_atual.numero, &ct_atual.cliente, &ct_atual.nome_cliente, &ct_atual.saldo)!=EOF){
            v_contas[i]=ct_atual;
            num_contas++;
            i++;
        }
    }
    fclose(ct_db);

    num_transacoes=0;
    tr_db = fopen("dados_transacoes.txt", "r");
    if(tr_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE TRANSACOES *!*\n     Arquivo indisponivel ou inexistente!\n --Um novo arquivo sera criado quando dados forem inseridos no programa--\n\n");
    else{
        i=0;
        while(fscanf(tr_db, "%s %f %s %s %[^\n]", &tr_atual.tipo, &tr_atual.valor, &tr_atual.data, &tr_atual.conta, &tr_atual.descricao)!=EOF){
            v_transacoes[i]=tr_atual;
            num_transacoes++;
            i++;
        }
    }
    fclose(tr_db);

    organiza_vetor(1);
    organiza_vetor(2);
}

void save(){

    CLIENTE cli_atual;
    CONTA ct_atual;
    TRANSACAO tr_atual;

    int i;

    cli_db = fopen("dados_clientes.txt", "w");
    if(cli_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CLIENTES *!*\n     Arquivo indisponivel!\n\n");
    else{
        for(i=0; i<num_clientes; i++){
            if(v_clientes[i].codigo[0]!=NULL){
                cli_atual = v_clientes[i];
                fprintf(cli_db, "%s\n%s\n%s\n%s\n%s\n", cli_atual.codigo, cli_atual.nome, cli_atual.cpf_cnpj, cli_atual.telefone, cli_atual.endereco);
            }
        }
    }
    fclose(cli_db);

    ct_db = fopen("dados_contas.txt", "w");
    if(ct_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE CONTAS *!*\n     Arquivo indisponivel!\n\n");
    else{
        for(i=0; i<num_contas; i++){
            if(v_contas[i].cliente[0]!=NULL){
                ct_atual = v_contas[i];
                fprintf(cli_db, "%s\n%s\n%s\n%s\n%.2f\n", ct_atual.agencia, ct_atual.numero, ct_atual.cliente, ct_atual.nome_cliente, ct_atual.saldo);
            }
        }
    }
    fclose(ct_db);

    tr_db = fopen("dados_transacoes.txt", "w");
    if(tr_db == NULL) printf("*!* ERRO AO ABRIR BASE DE DADOS DE TRANSACOES *!*\n     Arquivo indisponivel!\n\n");
    else{
        for(i=0; i<num_transacoes; i++){
            if(v_transacoes[i].data[0]!=NULL){
                tr_atual = v_transacoes[i];
                fprintf(tr_db, "%s\n%.2f\n%s\n%s\n%s\n", tr_atual.tipo, tr_atual.valor, tr_atual.data, tr_atual.conta, tr_atual.descricao);
            }
        }
    }
    fclose(tr_db);
}


void menu_principal(){

    char opcao;

    while(1){
        printf("\n================== Bem Vindo! ==================\n");
        printf("Digite um comando para prosseguir:\n");
        printf(" C - Gerenciar Clientes\n");
        printf(" T - Gerenciar Contas\n");
        printf(" S - Sair\n");
        printf("\nEscolha: ");

        scanf("%c%*c", &opcao);
        switch(opcao){
            case'C':
                menu_cliente();
            break;
            case'c':
                menu_cliente();
            break;
            case'T':
                menu_conta();
            break;
            case't':
                menu_conta();
            break;
            case'S':
                return;
            break;
            case's':
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

    while(1){
        printf("\n================ Gerenciar Clientes ================\n");
        printf("Digite um comando para prosseguir:\n");
        printf(" C - Cadastrar um cliente\n");
        printf(" L - Listar todos os clientes cadastrados\n");
        printf(" B - Buscar cliente ja cadastrado\n");
        printf(" A - Atualizar um cliente cadastrado\n");
        printf(" E - Excluir um cliente cadastrado\n");
        printf(" S - Sair\n");
        printf("\nEscolha: ");

        scanf("%c%*c", &opcao);
        switch (opcao){
            case 'C':
                cadastra_cliente();
            break;
            case 'c':
                cadastra_cliente();
            break;
            case 'L':
                lista_cliente();
            break;
            case 'l':
                lista_cliente();
            break;
            case 'B':
                busca_cliente();
            break;
            case 'b':
                busca_cliente();
            break;
            case 'A':
                atualiza_cliente();
            break;
            case 'a':
                atualiza_cliente();
            break;
            case 'E':
                exclui_cliente();
            break;
            case 'e':
                exclui_cliente();
            break;
            case 'S':
                printf("\nRetornando ao menu principal\n");
                return;
            break;
            case 's':
                printf("\nRetornando ao menu principal\n");
                return;
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
            break;
        }
        save();
    }
}

void menu_conta(){
    char opcao;

    while(1){
        printf("\n=============== Gerenciar Contas ==================\n");
        printf("Digite um comando para prosseguir:\n");
        printf(" R - Listagem de todas as contas cadastradas.\n");
        printf(" C - Cadastrar uma conta para um cliente.\n");
        printf(" L - Listar todas as contas de um cliente.\n");
        printf(" W - Realizar um saque em uma conta.\n");
        printf(" D - Realizar um deposito em uma conta.\n");
        printf(" T - Realizar transferencia entre contas.\n");
        printf(" E - Exibir extrato de uma conta.\n");
        printf(" S - Sair\n");
        printf("\nEscolha: ");

        scanf("%c%*c", &opcao);
        switch (opcao){
            case 'R':
                lista_conta();
            break;
            case 'r':
                lista_conta();
            break;
            case 'C':
                cadastra_conta_p_cliente();
            break;
            case 'c':
                cadastra_conta_p_cliente();
            break;
            case 'L':
                lista_conta_p_cliente();
            break;
            case 'l':
                lista_conta_p_cliente();
            break;
            case 'W':
                saca_conta();
            break;
            case 'w':
                saca_conta();
            break;
            case 'D':
                deposita_conta();
            break;
            case 'd':
                deposita_conta();
            break;
            case 'T':
                transfere_conta();
            break;
            case 't':
                transfere_conta();
            break;
            case 'E':
                extrato_conta();
            break;
            case 'e':
                extrato_conta();
            break;
            case 'S':
                printf("\nRetornando ao menu principal\n");
                return;
            break;
            case 's':
                printf("\nRetornando ao menu principal\n");
                return;
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
            break;
        }
        save();
    }
}


/**
 * @brief Funcao que realiza uma busca por um determinado valor no vetor de informacoes especificado
 *  e retorna a posicao do valor encontrado no vetor, ou -1 caso o valor nao seja encontrado;
 *  
 *  Tipos: 1=clientes; 2=contas; 3=transacoes;
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
 * @param tipo inteiro que define se a busca sera de clientes, contas ou transacoes;
 * @param busca o que sera buscado pela funcao;
 * @param categoria o campo a ser comparado durante a busca;
 * @return posicao no vetor referente ao valor buscado;
 */
int encontra_valor(int tipo, char* busca, int categoria){
    
    int i;
    char agencia_e_nro[MAX_AGENCIA+MAX_NUMERO];

    switch(tipo){
        case 1:
            switch(categoria){
                case 1:
                    for(i=0; i<num_clientes; i++){
                        if(strcmp(v_clientes[i].codigo, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
                case 2:
                    for(i=0; i<num_clientes; i++){
                        if(strcmp(v_clientes[i].cpf_cnpj, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
                case 3:
                    for(i=0; i<num_clientes; i++){
                        if(strcmp(v_clientes[i].nome, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;  
            }
        break;
        case 2:
            switch(categoria){
                case 1:
                    for(i=0; i<num_contas; i++){
                        strcpy(agencia_e_nro, v_contas[i].agencia);
                        strcat(agencia_e_nro, v_contas[i].numero);
                        if(strcmp(agencia_e_nro, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
                case 2:
                    for(i=0; i<num_contas; i++){
                        if(strcmp(v_contas[i].cliente, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
            }
        break;
        case 3:
            if(categoria==1){
                for(i=0; i<num_transacoes; i++){
                    if(strcmp(v_transacoes[i].data, busca)==0){
                        return i;
                    }
                }
            }
            return -1;
        break;
    }
}

void organiza_vetor(int tipo){
    /*
        Tipos de vetores a organizar:
        clientes=1  : organiza o vetor de clientes por ordem alfabetica;
        contas=2    : organiza o vetor de contas de acordo com a ordem alfabetica de clientes;
    */
    int i, j, comp, pos_menor, pos_cli_atual;
    char menor[MAX_NOME];
    float maior_saldo;
    CLIENTE aux_cli;
    CONTA aux_conta;

    switch(tipo){
        case 1:
            for(i=0; i<num_clientes; i++){
                if(v_clientes[i].codigo[0]!=NULL){
                    strcpy(menor, v_clientes[i].nome);
                    pos_menor=i;
                    for(j=i; j<num_clientes; j++){
                        comp = strcmp(menor, v_clientes[j].nome);
                        if(comp>0){
                            strcpy(menor, v_clientes[j].nome);
                            pos_menor=j;
                        }
                    }
                    aux_cli = v_clientes[pos_menor];
                    v_clientes[pos_menor]=v_clientes[i];
                    v_clientes[i]=aux_cli;
                }
            }
        break;
        case 2:
            for(i=0; i<num_contas; i++){
                if(v_contas[i].cliente[0]!=NULL){
                    strcpy(menor, v_contas[i].nome_cliente);
                    pos_menor=i;
                    maior_saldo=v_contas[i].saldo;
                    for(j=i; j<num_contas; j++){
                        comp = strcmp(menor, v_contas[j].nome_cliente);
                        if(comp>0){
                            strcpy(menor, v_contas[j].nome_cliente);
                            pos_menor=j;
                        }
                        else if(comp==0){
                            if(maior_saldo<v_contas[j].saldo){
                                maior_saldo=v_contas[j].saldo;
                                pos_menor=j;
                            }
                        }
                    }
                    aux_conta = v_contas[pos_menor];
                    v_contas[pos_menor]=v_contas[i];
                    v_contas[i]=aux_conta;
                }
            }
        break;
    }
}

void cadastra_cliente(){

    CLIENTE novo_cli;
    char opcao;
    int i, check;

    while(1){
        check=1;
        printf("\n=============== Cadastro de cliente ===============\n");
        printf("\nInforme o codigo do novo cliente (sem espacos): ");
        scanf("%s%*c", &novo_cli.codigo);
        printf("\nInforme o nome do novo cliente: ");
        scanf("%[^\n]", &novo_cli.nome);
        printf("\nInforme o cpf/cnpj do novo cliente (sem espacos): ");
        scanf("%s%*c", &novo_cli.cpf_cnpj);
        printf("\nInforme o telefone do novo cliente(sem espacos): ");
        scanf("%s%*c", &novo_cli.telefone);
        printf("\nInforme o endereco do novo cliente: ");
        scanf("%[^\n]%*c", &novo_cli.endereco);

        for(i=0; i<num_clientes; i++){
            if(encontra_valor(1, novo_cli.codigo, 1)!=-1){
                printf("\n\n*!* Ja existe um cliente cadastrado com este codigo! *!*\n");
                check=0;
                break;
            }
            else if(encontra_valor(1, novo_cli.cpf_cnpj, 2)!=-1){
                printf("\n\n*!* Ja existe um cliente cadastrado com este CPF/CNPJ! *!*\n");
                check=0;
                break;
            }
        }

        while(check){
            printf("-----------------------------------------------------");
            printf("\nPor favor, confirme se as informacoes estao corretas:\n\n");
            printf("Codigo: %s\n", novo_cli.codigo);
            printf("Nome: %s\n", novo_cli.nome);
            printf("CPF/CNPJ: %s\n", novo_cli.cpf_cnpj);
            printf("Telefone: %s\n", novo_cli.telefone);
            printf("Endereco: %s\n", novo_cli.endereco);
            printf("-----------------------------------------------------");
            printf("\nDeseja salvar o cliente assim? (s/n): ");
            scanf("%c%*c", &opcao);
            
            if(opcao=='s' || opcao=='S'){
                v_clientes[num_clientes] = novo_cli;
                if(strcmp(v_clientes[num_clientes].codigo, novo_cli.codigo)!=0){
                    printf("\n*!* Erro ao cadastrar cliente! *!*\n");
                }
                else{
                    num_clientes++;
                    organiza_vetor(1);
                    printf("\n--------- Cliente cadastrado com sucesso! -----------\n");
                    printf("\nRetornando ao menu\n");
                    return;
                }
            }
            else if(opcao=='n' || opcao=='N'){

                while(check){
                    printf("\nDeseja inserir novamente os dados? (s/n): ");
                    scanf("%c%*c", &opcao);

                    if(opcao=='n' || opcao=='N'){
                        printf("\nRetornando ao menu\n");
                        return;
                    }
                    else if(opcao=='s' || opcao=='S'){
                        check=0;
                        break;
                    }
                    else{
                        printf("\n*!* Comando invalido! *!*\n");
                    }
                }
            }
            else{
                printf("\n*!* Comando invalido! *!*\n");
                fflush(stdin);
            }
        }
    }
}

void lista_cliente(){

    int i, cont=0;

    for(i=0; i<num_clientes; i++){
        if(v_clientes[i].codigo[0]!=NULL) cont++;
    }

    if(cont==0){
        printf("\n*!* Nenhum cliente cadastrado! *!*\n");
    }
    else{
        printf("\n============= Lista de clientes ==================\n");    
        for(i=0; i<num_clientes; i++){
            if(v_clientes[i].codigo[0]!=NULL){
                printf("\nCodigo: %s", v_clientes[i].codigo);
                printf("\nNome: %s", v_clientes[i].nome);
                printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
                printf("\nTelefone: %s", v_clientes[i].telefone);
                printf("\nEndereco: %s\n", v_clientes[i].endereco);
            }
        }
        printf("\n----------------- Fim da lista --------------------\n");
        printf("\nRetornando ao menu\n");
    }
}

void busca_cliente(){

    int i, opcao, check;
    char busca[MAX_NOME];

    while(1){
        check=1;
        printf("\n============= Busca de cliente ==================\n");
        printf("Informe o metodo que deseja usar para buscar o cliente:\n");
        printf(" 1 - Busca por codigo;\n 2 - Busca por CPF/CNPJ;\n 3 - Busca por nome;\n 0 - Retornar ao menu;\n");
        printf("\nEscolha: ");
        scanf("%d%*c", &opcao);

        switch(opcao){
            case 0:
                printf("\nRetornando ao menu\n");
                return;
            break;
            case 1:
                printf("\nInforme o codigo do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao);
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao);
            break;
            case 3:
                printf("\nInforme o nome COMPLETO do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao);
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                fflush(stdin);
                check=0;
            break;
        }
        if(i!=-1 && check){
            printf("\n------------- Cliente encontrado! ---------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s\n", v_clientes[i].endereco);
            printf("---------------------------------------------------");
            printf("\nRetornando ao menu\n");
            return;
        }
        else{
            switch(opcao){
                case 1:
                    printf("\n*!* Nao existe cliente cadastrado com esse codigo! *!*\n");
                break;
                case 2:
                    printf("\n*!* Nao existe cliente cadastrado com esse CPF/CNPJ! *!*\n");
                break;
                case 3:
                    printf("\n*!* Nao existe cliente cadastrado com esse nome! *!*\n");
                break;
            }
        }
    }
}

void atualiza_cliente(){
    /*
    * As variaveis de check sao correspodentes às perguntas que sao feitas ao longo dessa funcao e exigem uma resposta
    * exata do usuario, portanto os checks garantem que a pergunta rodará num loop até que ela receba uma resposta válida
    *   *check==1->pergunta roda
    *   *check==0->pergunta nao roda
    */
    int i, j, opcao1, check1, check2, check3; 
    char busca[MAX_CPF_CNPJ], opcao2;
    CLIENTE cli_atual;

    while(1){
        check1=1;
        check2=1;
        check3=1;
        printf("\n=========== Atualizacao de cliente ===============\n");
        printf("Informe o metodo que deseja usar para buscar o cliente que deseja alterar:\n");
        printf(" 1 - Busca por codigo;\n 2 - Busca por CPF/CNPJ;\n 0 - Retornar ao menu;\n");
        printf("\nEscolha: ");
        scanf("%d%*c", &opcao1);

        switch(opcao1){
            case 0:
                printf("\nRetornando ao menu\n");
                return;
            break; 
            case 1:
                printf("\nInforme o codigo do cliente a ser atualizado: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao1);
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser atualizado: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao1);
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                fflush(stdin);
                check1=0;
            break;
        }

        if(i!=-1 && check1){
            printf("\n------------- Cliente encontrado! ---------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s\n", v_clientes[i].endereco);
            printf("---------------------------------------------------\n");

            while(check1){
                check2=1;
                check3=1;
                printf("\nInforme o codigo atualizado (sem espacos): ");
                scanf("%s%*c", &cli_atual.codigo);
                printf("\nInforme o nome atualizado: ");
                scanf("%[^\n]", &cli_atual.nome);
                printf("\nInforme o cpf/cnpj atualizado (sem espacos): ");
                scanf("%s%*c", &cli_atual.cpf_cnpj);
                printf("\nInforme o telefone atualizado (sem espacos): ");
                scanf("%s%*c", &cli_atual.telefone);
                printf("\nInforme o endereco atualizado: ");
                scanf("%[^\n]%*c", &cli_atual.endereco);
                
                for(j=0; j<num_clientes; j++){
                    if(encontra_valor(1, cli_atual.codigo, 1)!=-1){
                        printf("\n\n*!* Ja existe um cliente cadastrado com este codigo! *!*\n");
                        break;
                    }
                    else if(encontra_valor(1, cli_atual.cpf_cnpj, 2)!=-1){
                        printf("\n\n*!* Ja existe um cliente cadastrado com este CPF/CNPJ! *!*\n");
                        break;
                    }
                }

                printf("-----------------------------------------------------");
                printf("\nPor favor, confirme se as informacoes estao corretas:\n\n");
                printf("Codigo: %s\n", cli_atual.codigo);
                printf("Nome: %s\n", cli_atual.nome);
                printf("CPF/CNPJ: %s\n", cli_atual.cpf_cnpj);
                printf("Telefone: %s\n", cli_atual.telefone);
                printf("Endereco: %s\n", cli_atual.endereco);
                printf("-----------------------------------------------------");

                while(check2){
                    printf("\nDeseja salvar o cliente assim? (s/n): ");
                    scanf("%c%*c", &opcao2);
                    if(opcao2=='s' || opcao2=='S'){
                        v_clientes[i] = cli_atual;
                        if(strcmp(v_clientes[i].codigo, cli_atual.codigo)!=0){
                            printf("\n*!* Erro ao cadastrar cliente! *!*\n");
                            check1=0;
                        }
                        else{
                            organiza_vetor(1);
                            printf("\n-------- Cliente atualizado com sucesso! -----------\n");
                            printf("\nRetornando ao menu\n");
                            return;
                        }
                    }
                    else if(opcao2=='n' || opcao2=='N'){

                        while(check3){
                            printf("\nDeseja inserir novamente os dados? (s/n): ");
                            scanf("%c%*c", &opcao2);
                            if(opcao2=='n' || opcao2=='N'){
                                printf("\nRetornando ao menu\n");
                                return;
                            }
                            else if(opcao2=='s' || opcao2=='S'){
                                printf("-----------------------------------------------------");
                                check2=0;
                                check3=0;
                                break;
                            }
                            else{
                                printf("\n*!* Comando invalido! *!*\n");
                            }
                        }
                    }
                    else{
                        printf("\n*!* Comando invalido! *!*\n");
                    }
                }
            }
        }
        else{
            switch(opcao1){
                case 1:
                    printf("\n*!* Nao existe cliente cadastrado com esse codigo! *!*\n");
                break;
                case 2:
                    printf("\n*!* Nao existe cliente cadastrado com esse CPF/CNPJ! *!*\n");
                break;
            }
        }
    }
}

void exclui_cliente(){
    int i, opcao1, check; 
    char busca[MAX_CPF_CNPJ], opcao2;

    while(1){
        check=1;
        printf("\n============= Exclusao de cliente ===============\n");
        printf("Informe o metodo que deseja usar para buscar o cliente que deseja excluir:\n");
        printf(" 1 - Busca por codigo;\n 2 - Busca por CPF/CNPJ;\n 0 - Retornar ao menu;\n");
        printf("\nEscolha: ");
        scanf("%d%*c", &opcao1);

        switch(opcao1){
            case 0:
                printf("\nRetornando ao menu\n");
                return;
            break; 
            case 1:
                printf("\nInforme o codigo do cliente a ser excluido: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao1);
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser excluido: ");
                scanf("%[^\n]%*c", &busca);
                fflush(stdin);
                i=encontra_valor(1, busca, opcao1);
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                fflush(stdin);
                check=0;
            break;
        }

        if(i!=-1 && check){
            printf("\n------------- Cliente encontrado! ---------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s\n", v_clientes[i].endereco);
            printf("---------------------------------------------------\n");

            while(check){
                printf("\n*!* Deseja mesmo excluir este cliente permanentemente? *!* (s/n): ");
                scanf("%c%*c", &opcao2);
                if(opcao2=='s' || opcao2=='S'){
                    printf("\n Excluindo o cliente %s\n", v_clientes[i].nome);
                    v_clientes[i].codigo[0]=NULL;
                    v_clientes[i].cpf_cnpj[0]=NULL;

                    if(v_clientes[i].codigo[0]==NULL && v_clientes[i].cpf_cnpj[0]==NULL){
                        organiza_vetor(1);
                        printf("\n------------- Cliente excluido! ---------------\n");
                        printf("\nRetornando ao menu\n");
                        return;
                    }
                    else{
                        printf("\n*!* Erro ao excluir cliente! *!*\n");
                        check=0;
                    }
                }
                else if(opcao2=='n' || opcao2=='N'){
                    printf("\n Retornando ao inicio\n");
                    check=0;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
        }
        else{
            switch(opcao1){
                case 1:
                    printf("\n*!* Nao existe cliente cadastrado com esse codigo! *!*\n");
                break;
                case 2:
                    printf("\n*!* Nao existe cliente cadastrado com esse CPF/CNPJ! *!*\n");
                break;
            }
        }
    }
}


void lista_conta(){

    int i, cont=0;
    
    for(i=0; i<num_contas; i++){
        if(v_contas[i].cliente[0]!=NULL) cont++;
    }

    if(cont==0){
        printf("\n*!* Nenhuma conta cadastrada! *!*\n");
    }
    else{
        organiza_vetor(2);
        printf("\n============= Lista de contas ==================\n");    
        for(i=0; i<num_contas; i++){
            if(v_contas[i].cliente[0]!=NULL){
                if(strcmp(v_contas[i].cliente, v_contas[i-1].cliente)){
                    printf("------------------------------------------------");      
                    printf("\nCodigo do cliente: %s", v_contas[i].cliente);
                    printf("\nCliente: %s\n", v_contas[i].nome_cliente);
                    printf("\nAgencia: %s", v_contas[i].agencia);
                    printf("\nNumero: %s", v_contas[i].numero);
                    printf("\nSaldo: %.2f\n", v_contas[i].saldo);
                }
                else{
                    printf("\nAgencia: %s", v_contas[i].agencia);
                    printf("\nNumero: %s", v_contas[i].numero);
                    printf("\nSaldo: %.2f\n", v_contas[i].saldo);
                }
            }
        }
        printf("\n----------------- Fim da lista --------------------\n");
        printf("\nRetornando ao menu\n");
    }
}

void cadastra_conta_p_cliente(){
    int i, j, opcao1, check1, check2, check3;
    char opcao2, opcao3, busca_cliente[MAX_CPF_CNPJ], busca_conta[MAX_AGENCIA+MAX_NUMERO];

    CONTA nova_conta;

    while(1){
        check1=1;
        check2=1;
        check3=1;
        printf("\n=============== Cadastro de conta ===================\n");
        printf("Informe o metodo que deseja usar para buscar o cliente que sera dono da conta:\n");
        printf(" 1 - Busca por codigo;\n 2 - Busca por CPF/CNPJ;\n 0 - Retornar ao menu;\n");
        printf("\nEscolha: ");
        scanf("%d%*c", &opcao1);

        switch(opcao1){
            case 0:
                printf("\nRetornando ao menu\n");
                return;
            break;
            case 1:
                printf("\nInforme o codigo do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca_cliente);
                i=encontra_valor(1, busca_cliente, opcao1);
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca_cliente);
                i=encontra_valor(1, busca_cliente, opcao1);
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                fflush(stdin);
                check1=0;
            break;
        }
        if(i!=-1 && check1){
            
            while(check2){
                printf("\n------------- Cliente encontrado! ---------------");
                printf("\nCodigo: %s", v_clientes[i].codigo);
                printf("\nNome: %s", v_clientes[i].nome);
                printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
                printf("\nTelefone: %s", v_clientes[i].telefone);
                printf("\nEndereco: %s\n", v_clientes[i].endereco);
                printf("---------------------------------------------------");           
                printf("\nDeseja cadastrar uma nova conta para esse cliente? (s/n): ");
                scanf("%c%*c", &opcao2);

                if(opcao2=='s' || opcao2=='S'){
                    printf("\nInforme a agencia da nova conta (3 digitos s/ espaco): ");
                    scanf("%s", &nova_conta.agencia);
                    printf("\nInforme o numero da nova conta (5 digitos s/ espaco): ");
                    scanf("%s", &nova_conta.numero);

                    if(strlen(nova_conta.agencia)!=3){
                        printf("\n*!* A agencia deve possuir 3 digitos exatos! *!*\n");
                        fflush(stdin);
                    }
                    else if(strlen(nova_conta.numero)!=5){
                        printf("\n*!* O numero da conta deve possuir 5 digitos exatos! *!*\n");
                        fflush(stdin);
                    }
                    else{
                        strcpy(busca_conta, nova_conta.agencia);
                        strcat(busca_conta, nova_conta.numero);

                        j=encontra_valor(2, busca_cliente, 1);

                        if(j!=-1){
                            printf("\n*!* Ja existe uma conta cadastrada com essa agencia e numero! *!*\n");
                        }
                        else{
                            strcpy(nova_conta.cliente, v_clientes[i].codigo);
                            strcpy(nova_conta.nome_cliente, v_clientes[i].nome);
                            nova_conta.saldo = 0.0;

                            printf("\nPor favor confirme se os dados da conta estao corretos:\n");
                            printf("\nCliente: %s", nova_conta.nome_cliente);
                            printf("\nAgencia: %s", nova_conta.agencia);
                            printf("\nNumero: %s", nova_conta.numero);
                            printf("\nSaldo: %.2f\n", nova_conta.saldo);

                            while(check3){
                                printf("Deseja salvar a conta assim? (s/n): ");
                                scanf("%c%*c", &opcao3);

                                if(opcao2=='s' || opcao2=='S'){
                                    v_contas[num_contas]=nova_conta;

                                    if(!strcmp(v_contas[num_contas].cliente, nova_conta.cliente)){
                                        printf("\n----------- Conta cadastrada com sucesso! ------------\n");
                                        printf("\nRetornando ao menu\n");
                                        num_contas++;
                                        organiza_vetor(2);
                                        scanf("%*c");
                                        return;
                                    }
                                    else{
                                        printf("*!* Erro ao cadastrar conta! *!*");
                                        check2=0;
                                        check3=0;
                                    }
                                }
                                else if(opcao2=='n' || opcao2=='N'){
                                    printf("\nRetornando ao inicio\n");
                                    check2=0;
                                    check3=0;
                                }
                                else{
                                    printf("\n*!* Comando invalido! *!*\n");
                                }
                            }
                        }
                    }
                }
                else if(opcao2=='n' || opcao2=='N'){
                    printf("\nRetornando ao inicio\n");
                    check2=0;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
        }
        else{
            switch(opcao1){
                case 1:
                    printf("\n*!* Nao existe cliente cadastrado com esse codigo! *!*\n");
                break;
                case 2:
                    printf("\n*!* Nao existe cliente cadastrado com esse CPF/CNPJ! *!*\n");
                break;
            }
        }
    }
}

void lista_conta_p_cliente(){

    organiza_vetor(2);
    int i, j, opcao, check;
    char cli[MAX_CODIGO], busca_cliente[MAX_CODIGO];

    while(1){
        check=1;
        printf("\n========= Listagem de contas de um cliente =========\n");
        printf("Informe o metodo que deseja usar para buscar o cliente dono das contas a listar:\n");
        printf(" 1 - Busca por codigo;\n 2 - Busca por CPF/CNPJ;\n 0 - Retornar ao menu;\n");
        printf("\nEscolha: ");
        scanf("%d%*c", &opcao);
        fflush(stdin);

        switch(opcao){
            case 0:
                printf("\nRetornando ao menu\n");
                return;
            break;
            case 1:
                printf("\nInforme o codigo do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca_cliente);
                fflush(stdin);
                i=encontra_valor(1, busca_cliente, opcao);
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca_cliente);
                fflush(stdin);
                i=encontra_valor(1, busca_cliente, opcao);
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                fflush(stdin);
                check=0;
            break;
        }
        if(i!=-1 && check){
            printf("\n------------- Cliente encontrado! ---------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s\n", v_clientes[i].endereco);

            strcpy(cli, v_clientes[i].codigo);

            j=encontra_valor(2, cli, 2);
            if(j!=-1){ 
                printf("\n---------- Lista de contas vinculadas -------------\n");
                for(j=0; j<num_contas; j++){
                    if(!strcmp(v_contas[j].cliente, v_clientes[i].codigo)){
                        printf("\nAgencia: %s", v_contas[j].agencia);
                        printf("\nNumero: %s", v_contas[j].numero);
                        printf("\nSaldo: %.2f\n", v_contas[j].saldo);
                    }
                }
                printf("\n----------------- Fim da lista --------------------\n");
            }
            else{
                printf("*!* Nao existe conta vinculada a esse cliente ainda! *!*\n");
            }
        }
        else{
            switch(opcao){
                case 1:
                    printf("\n*!* Nao existe cliente cadastrado com esse codigo! *!*\n");
                break;
                case 2:
                    printf("\n*!* Nao existe cliente cadastrado com esse CPF/CNPJ! *!*\n");
                break;
            }
        }
    }
}

void conta_notas(int valor){
    
    int resto=0, cont2=0, cont5=0, cont10=0, cont20=0, cont50=0, cont100=0, cont200=0;

    do{
        if (valor>=200){
            valor-=200;
            cont200++;
        }
        else if(valor>=100){
            valor-=100;
            cont100++;
        }
        else if(valor >= 50){
            valor-=50;
            cont50++;
        }
        else if(valor>=20){
            valor-=20;
            cont20++;
        }
        else if(valor>=10){
            valor -= 10;
            cont10++;
        }
        else if(valor>=5){
            if(valor%2!=0){
                valor -= 5;
                cont5++;
            }
            else{
                valor-=2;
                cont2++;
            }
        }
        else if(valor>=2){
            valor-=2;
            cont2++;
        }
        else{
            valor--;
            resto++;
        }
    } while (valor > 0);

    printf("\nO dinheiro sera entregue em:\n");
    if(cont200) printf("%d nota(s) de 200\n", cont200);
    if(cont100) printf("%d nota(s) de 100\n", cont100);
    if(cont50) printf("%d nota(s) de 50\n", cont50);
    if(cont20) printf("%d nota(s) de 20\n", cont20);
    if(cont10) printf("%d nota(s) de 10\n", cont10);
    if(cont5) printf("%d nota(s) de 5\n", cont5);
    if(cont2) printf("%d nota(s) de 2\n", cont2);
    if(resto) printf("e uma moeda de um real\n");
}

void saca_conta(){
    int i, j, check;
    char opcao, agencia[MAX_AGENCIA], numero[MAX_NUMERO], busca_conta[MAX_AGENCIA+MAX_NUMERO], cliente[MAX_CODIGO];
    float valor;

    TRANSACAO saque;

    while(1){
        check=1;
        printf("\n================== Saque de conta ==================\n");
        printf("\nInforme a agencia da conta que deseja sacar: ");
        scanf("%s%*c", &agencia);
        printf("\nInforme o numero da conta que deseja sacar: ");
        scanf("%s%*c", &numero);

        strcpy(busca_conta, agencia);
        strcat(busca_conta, numero);
        j=encontra_valor(2, busca_conta, 1);

        if(j!=-1){
            strcpy(cliente, v_contas[j].cliente);
            i=encontra_valor(1, cliente, 1);

            printf("\n------------- Conta encontrada! ---------------");
            printf("\nAgencia: %s", v_contas[j].agencia);
            printf("\nNumero: %s", v_contas[j].numero);
            printf("\nSaldo: %.2f\n", v_contas[j].saldo);
            printf("\n----------- Dados do cliente dono -------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s", v_clientes[i].endereco);
            printf("\n-----------------------------------------------\n");

            while(check){
                printf("\nInforme o valor que deseja sacar (0 para cancelar): ");
                scanf("%f%*c", &valor);
                fflush(stdin);

                if(valor==0){
                    printf("\nRetornando ao inicio\n");
                    check=0;
                }
                else if(valor>v_contas[j].saldo){
                    printf("\n*!* O saldo da conta e insuficiente para este saque! *!*\n");
                    while(1){
                        printf("\nDeseja tentar novamente? (s/n): ");
                        scanf("%c%*c", &opcao);
                        if(opcao=='s' || opcao=='S'){
                            break;
                        }
                        else if(opcao=='n' || opcao=='N'){
                            printf("\nRetornando ao menu\n");
                            return;
                        }
                        else{
                            printf("\n*!* Comando invalido! *!*\n");
                        }
                    }
                }
                else{
                    printf("\nInforme uma descricao para este saque:\n\n");
                    scanf("%[^\n]%*c", saque.descricao);
                    fflush(stdin);
                    strcpy(saque.tipo, "DEBITO");
                    strcpy(saque.conta, busca_conta);
                    strcpy(saque.data, data_hoje);
                    saque.valor=valor;

                    v_transacoes[num_transacoes]=saque;

                    if(!strcmp(saque.conta, v_transacoes[num_transacoes].conta) && saque.valor==v_transacoes[num_transacoes].valor){
                        v_contas[j].saldo -= valor;
                        num_transacoes++;
                        printf("\n-------------- Saque realizado! ---------------\n");
                        conta_notas((int)valor);
                        return;
                    }
                    else{
                        printf("\n*!* Ocorreu um erro ao realizar a operacao! *!*\n");
                        check=0;
                    }
                }
            }
        }
        else{
            printf("\n*!* Nao existe uma conta cadastrada com essa agencia e numero! *!*\n");
            while(1){
                printf("\nDeseja tentar novamente? (s/n): ");
                scanf("%c%*c", &opcao);
                if(opcao=='s' || opcao=='S'){
                    break;
                }
                else if(opcao=='n' || opcao=='N'){
                    printf("\nRetornando ao menu\n");
                    return;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
        }
    }
}

void deposita_conta(){
    int i, j, check;
    char opcao, agencia[MAX_AGENCIA], numero[MAX_NUMERO], busca_conta[MAX_AGENCIA+MAX_NUMERO], cliente[MAX_CODIGO];
    float valor;

    TRANSACAO deposito;

    while(1){
        check=1;
        printf("\n================ Deposito em conta ================\n");
        printf("\nInforme a agencia da conta em que deseja depositar: ");
        scanf("%s%*c", &agencia);
        printf("\nInforme o numero da conta em que deseja depositar: ");
        scanf("%s%*c", &numero);

        strcpy(busca_conta, agencia);
        strcat(busca_conta, numero);
        j=encontra_valor(2, busca_conta, 1);

        if(j!=-1){
            strcpy(cliente, v_contas[j].cliente);
            i=encontra_valor(1, cliente, 1);

            printf("\n------------- Conta encontrada! ---------------");
            printf("\nAgencia: %s", v_contas[j].agencia);
            printf("\nNumero: %s", v_contas[j].numero);
            printf("\nSaldo: %.2f\n", v_contas[j].saldo);
            printf("\n----------- Dados do cliente dono -------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s", v_clientes[i].endereco);
            printf("\n-----------------------------------------------\n");

            while(check){
                printf("\nInforme o valor que deseja depositar (0 para cancelar): ");
                scanf("%f%*c", &valor);
                fflush(stdin);

                if(valor==0){
                    printf("\nRetornando ao inicio\n");
                    check=0;
                }
                else{
                    printf("\nInforme uma descricao para este deposito:\n\n");
                    scanf("%[^\n]%*c", deposito.descricao);
                    fflush(stdin);
                    strcpy(deposito.tipo, "CREDITO");
                    strcpy(deposito.conta, busca_conta);
                    strcpy(deposito.data, data_hoje);
                    deposito.valor=valor;

                    v_transacoes[num_transacoes]=deposito;

                    if(!strcmp(deposito.conta, v_transacoes[num_transacoes].conta) && deposito.valor==v_transacoes[num_transacoes].valor){
                        v_contas[j].saldo += valor;
                        num_transacoes++;
                        printf("\n------------ Deposito realizado! --------------\n");
                        return;
                    }
                    else{
                        printf("\n*!* Ocorreu um erro ao realizar a operacao! *!*\n");
                        check=0;
                    }
                }
            }
        }
        else{
            printf("\n*!* Nao existe uma conta cadastrada com essa agencia e numero! *!*\n");
            while(1){
                printf("\nDeseja tentar novamente? (s/n): ");
                scanf("%c%*c", &opcao);
                if(opcao=='s' || opcao=='S'){
                    break;
                }
                else if(opcao=='n' || opcao=='N'){
                    printf("\nRetornando ao menu\n");
                    return;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
        }
    }
}

void transfere_conta(){
    int i, j, a, b, check1, check2;
    char opcao, agencia[MAX_AGENCIA], numero[MAX_NUMERO], conta[MAX_AGENCIA+MAX_NUMERO], busca_conta[MAX_AGENCIA+MAX_NUMERO], cliente[MAX_CODIGO], descricao[MAX_DESCRICAO];
    float valor;

    TRANSACAO saque;
    TRANSACAO deposito;

    while(1){
        check1=1;
        check2=1;
        printf("\n=================== Transferencia ====================\n");
        printf("\nInforme a agencia da conta de saida: ");
        scanf("%s%*c", &agencia);
        printf("\nInforme o numero da conta de saida: ");
        scanf("%s%*c", &numero);

        strcpy(busca_conta, agencia);
        strcat(busca_conta, numero);
        j=encontra_valor(2, busca_conta, 1);

        if(j!=-1){
            strcpy(cliente, v_contas[j].cliente);
            i=encontra_valor(1, cliente, 1);

            printf("\n------------- Conta encontrada! ---------------");
            printf("\nAgencia: %s", v_contas[j].agencia);
            printf("\nNumero: %s", v_contas[j].numero);
            printf("\nSaldo: %.2f\n", v_contas[j].saldo);
            printf("\n----------- Dados do cliente dono -------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s", v_clientes[i].endereco);
            printf("\n-----------------------------------------------\n");

            while(check1){
                printf("\nInforme a agencia da conta de entrada: ");
                scanf("%s%*c", &agencia);
                printf("\nInforme o numero da conta de entrada: ");
                scanf("%s%*c", &numero);

                strcpy(busca_conta, agencia);
                strcat(busca_conta, numero);
                b=encontra_valor(2, busca_conta, 1);

                if(b!=-1){
                    strcpy(cliente, v_contas[b].cliente);
                    a=encontra_valor(1, cliente, 1);

                    printf("\n------------- Conta encontrada! ---------------");
                    printf("\nAgencia: %s", v_contas[b].agencia);
                    printf("\nNumero: %s", v_contas[b].numero);
                    printf("\nSaldo: %.2f\n", v_contas[b].saldo);
                    printf("\n----------- Dados do cliente dono -------------");
                    printf("\nCodigo: %s", v_clientes[a].codigo);
                    printf("\nNome: %s", v_clientes[a].nome);
                    printf("\nCPF/CNPJ: %s", v_clientes[a].cpf_cnpj);
                    printf("\nTelefone: %s", v_clientes[a].telefone);
                    printf("\nEndereco: %s", v_clientes[a].endereco);
                    printf("\n-----------------------------------------------\n");
                    check1=0;
                }
                else{
                    printf("\n*!* Nao existe uma conta cadastrada com essa agencia e numero! *!*\n");
                    while(1){
                        printf("\nDeseja tentar novamente? (s/n): ");
                        scanf("%c%*c", &opcao);
                        if(opcao=='s' || opcao=='S'){
                            break;
                        }
                        else if(opcao=='n' || opcao=='N'){
                            printf("\nRetornando ao menu\n");
                            return;
                        }
                        else{
                            printf("\n*!* Comando invalido! *!*\n");
                        }
                    }
                }
            }
            while(check2){
                printf("\nInforme o valor que deseja transferir (0 para cancelar): ");
                scanf("%f%*c", &valor);
                fflush(stdin);

                if(valor==0){
                    printf("\nRetornando ao menu\n");
                    return;
                }
                else if(valor>v_contas[j].saldo){
                    printf("\n*!* O saldo da conta e insuficiente para esta transferencia! *!*\n");
                    while(1){
                        printf("\nDeseja tentar novamente? (s/n): ");
                        scanf("%c%*c", &opcao);
                        if(opcao=='s' || opcao=='S'){
                            break;
                        }
                        else if(opcao=='n' || opcao=='N'){
                            printf("\nRetornando ao menu\n");
                            return;
                        }
                        else{
                            printf("\n*!* Comando invalido! *!*\n");
                        }
                    }
                }
                else{
                    
                    strcpy(descricao, "Transferencia para a conta: ");
                    strcat(descricao, v_contas[b].agencia);
                    strcat(descricao, "-");
                    strcat(descricao, v_contas[b].numero);

                    strcpy(conta, v_contas[j].agencia);
                    strcat(conta, v_contas[j].numero);

                    strcpy(saque.tipo, "DEBITO");
                    strcpy(saque.conta, conta);
                    strcpy(saque.data, data_hoje);
                    strcpy(saque.descricao, descricao);
                    saque.valor=valor;

                    v_transacoes[num_transacoes]=saque;

                    if(!strcmp(saque.conta, v_transacoes[num_transacoes].conta) && saque.valor==v_transacoes[num_transacoes].valor){
                        v_contas[j].saldo -= valor;
                        num_transacoes++;

                        strcpy(descricao, "Transferencia de conta: ");
                        strcat(descricao, v_contas[j].agencia);
                        strcat(descricao, "-");
                        strcat(descricao, v_contas[j].numero);

                        strcpy(conta, v_contas[b].agencia);
                        strcat(conta, v_contas[b].numero);

                        strcpy(deposito.tipo, "CREDITO");
                        strcpy(deposito.conta, conta);
                        strcpy(deposito.data, data_hoje);
                        strcpy(deposito.descricao, descricao);
                        deposito.valor=valor;

                        v_transacoes[num_transacoes]=deposito;

                        if(!strcmp(deposito.conta, v_transacoes[num_transacoes].conta) && deposito.valor==v_transacoes[num_transacoes].valor){
                            v_contas[b].saldo += valor;
                            num_transacoes++;
                            printf("\n----------- Transferencia realizada! -------------\n");
                            return;
                        }
                        else{
                            printf("\n*!* Ocorreu um erro ao realizar a operacao! *!*\n");
                            check2=0;
                        }
                    }
                    else{
                        printf("\n*!* Ocorreu um erro ao realizar a operacao! *!*\n");
                        check2=0;
                    }
                }
            }
        }
        else{
            printf("\n*!* Nao existe uma conta cadastrada com essa agencia e numero! *!*\n");
            while(1){
                printf("\nDeseja tentar novamente? (s/n): ");
                scanf("%c%*c", &opcao);
                if(opcao=='s' || opcao=='S'){
                    break;
                }
                else if(opcao=='n' || opcao=='N'){
                    printf("\nRetornando ao menu\n");
                    return;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
        }
    }
}

void calcula_data(int total_dias, int * dia_fim, int * mes_fim, int * ano_fim){
    
    char c_dia[3], c_mes[3], c_ano[5];
    int i_dia, i_mes, i_ano, cont;

    c_dia[0]=data_hoje[0];
    c_dia[1]=data_hoje[1];
    c_dia[2]=NULL;

    c_mes[0]=data_hoje[3]; 
    c_mes[1]=data_hoje[4];
    c_mes[2]=NULL;

    c_ano[0]=data_hoje[6];
    c_ano[1]=data_hoje[7];
    c_ano[2]=data_hoje[8];
    c_ano[3]=data_hoje[9];
    c_ano[4]=NULL;

    i_dia=atoi(c_dia);
    i_mes=atoi(c_mes);
    i_ano=atoi(c_ano);

    for(cont=0; cont<total_dias; cont++){
        i_dia--;
        if(i_dia==0){
            switch(i_mes){
                case 1:
                    i_dia=31;
                    i_mes=12;
                    i_ano--;
                //31
                break;
                case 2:
                    i_dia=31;
                    i_mes=1;
                //28
                break;
                case 3:
                    i_dia=28;
                    i_mes=2;
                //31
                break;
                case 4:
                    i_dia=31;
                    i_mes=3;
                //30
                break;
                case 5:
                    i_dia=30;
                    i_mes=4;
                //31
                break;
                case 6:
                    i_dia=31;
                    i_mes=5;
                //30
                break;
                case 7:
                    i_dia=30;
                    i_mes=6;
                //31
                break;
                case 8:
                    i_dia=31;
                    i_mes=7;
                //31
                break;
                case 9:
                    i_dia=31;
                    i_mes=8;
                //30
                break;
                case 10:
                    i_dia=30;
                    i_mes=9;
                //31
                break;
                case 11:
                    i_dia=31;
                    i_mes=10;
                //30
                break;
                case 12:
                    i_dia=30;
                    i_mes=11;
                //31
                break;
                default:
                    printf("\nErro\n");
                break;
            }
        }
    }

    *dia_fim = i_dia;
    *mes_fim = i_mes;
    *ano_fim = i_ano;
}

void extrato_conta(){
    int i, j, check, total_dias, dia_fim, mes_fim, ano_fim, cont, imprime;
    char opcao, agencia[MAX_AGENCIA], numero[MAX_NUMERO], busca_conta[MAX_AGENCIA+MAX_NUMERO], cliente[MAX_CODIGO];
    char aux_dia[3], aux_mes[3], aux_ano[5];
    int i_dia, i_mes, i_ano;

    TRANSACAO deposito;

    while(1){
        check=1;
        printf("\n================ Extrato de conta ================\n");
        printf("\nInforme a agencia da conta que deseja ver o extrato: ");
        scanf("%s%*c", &agencia);
        printf("\nInforme o numero da conta que deseja ver o extrato: ");
        scanf("%s%*c", &numero);

        strcpy(busca_conta, agencia);
        strcat(busca_conta, numero);
        j=encontra_valor(2, busca_conta, 1);

        if(j!=-1){
            strcpy(cliente, v_contas[j].cliente);
            i=encontra_valor(1, cliente, 1);

            printf("\n------------- Conta encontrada! ---------------");
            printf("\nAgencia: %s", v_contas[j].agencia);
            printf("\nNumero: %s", v_contas[j].numero);
            printf("\nSaldo: %.2f\n", v_contas[j].saldo);
            printf("\n----------- Dados do cliente dono -------------");
            printf("\nCodigo: %s", v_clientes[i].codigo);
            printf("\nNome: %s", v_clientes[i].nome);
            printf("\nCPF/CNPJ: %s", v_clientes[i].cpf_cnpj);
            printf("\nTelefone: %s", v_clientes[i].telefone);
            printf("\nEndereco: %s", v_clientes[i].endereco);
            printf("\n-----------------------------------------------\n");

            while(check){
                printf("\nInforme quantos dias de transacoes deseja ver no extrato (0 para cancelar): ");
                scanf("%d%*c", &total_dias);
                fflush(stdin);

                if(total_dias==0){
                    printf("\nRetornando ao inicio\n");
                    check=0;
                }
                else{
                    printf("\n------------------ Extrato -------------------\n");

                    calcula_data(total_dias, &dia_fim, &mes_fim, &ano_fim);

                    for(cont=num_transacoes; cont>=0; cont--){
                        imprime=0;
                        if(v_transacoes[cont].conta[0]!=NULL){
                            aux_dia[0]=v_transacoes[cont].data[0];
                            aux_dia[1]=v_transacoes[cont].data[1];
                            aux_dia[2]=NULL;

                            aux_mes[0]=v_transacoes[cont].data[3]; 
                            aux_mes[1]=v_transacoes[cont].data[4];
                            aux_mes[2]=NULL;

                            aux_ano[0]=v_transacoes[cont].data[6];
                            aux_ano[1]=v_transacoes[cont].data[7];
                            aux_ano[2]=v_transacoes[cont].data[8];
                            aux_ano[3]=v_transacoes[cont].data[9];
                            aux_ano[4]=NULL;

                            i_dia=atoi(aux_dia);
                            i_mes=atoi(aux_mes);
                            i_ano=atoi(aux_ano);

                            if(!strcmp(busca_conta, v_transacoes[cont].conta)){
                                if(i_ano>=ano_fim){
                                    if(i_mes>=mes_fim){
                                        if(i_dia>=dia_fim){
                                            imprime=1;
                                        }
                                        else if(i_mes>mes_fim){
                                            imprime=1;
                                        }
                                    }
                                    else if(i_ano>ano_fim){
                                        imprime=1;
                                    }
                                }
                            }
                            if(imprime){
                                printf("\nTipo: %s\n", v_transacoes[cont].tipo);
                                printf("valor: %.2f\n", v_transacoes[cont].valor);
                                printf("Data: %s\n", v_transacoes[cont].data);
                                printf("Descricao: %s\n", v_transacoes[cont].descricao);
                                printf("\n-----------------------------------------------\n");
                            }
                        }
                    }
                    printf("\nFim do extrato! Retornando ao menu.\n");
                    return;
                }
            }
        }
        else{
            printf("\n*!* Nao existe uma conta cadastrada com essa agencia e numero! *!*\n");
            while(1){
                printf("\nDeseja tentar novamente? (s/n): ");
                scanf("%c%*c", &opcao);
                if(opcao=='s' || opcao=='S'){
                    break;
                }
                else if(opcao=='n' || opcao=='N'){
                    printf("\nRetornando ao menu\n");
                    return;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
        }
    }
}