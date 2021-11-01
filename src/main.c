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
    int num_clientes=0; //numero de clientes cadastrados atualmente
    int num_contas=0; //numero de contas cadastrados atualmente
    int num_transacoes=0; //numero de transacoes cadastrados atualmente
    FILE * cli_db; //Base de dados de clientes
    FILE * ct_db; //Base de dados de contas
    FILE * tr_db; //Base de dados de transacoes
/*----------------------------------------------------------------*/

void boot();
//void quit(CLIENTE*, CONTA*, TRANSACAO*, FILE*, FILE*, FILE*);

void menu_principal();
void menu_cliente();
void menu_conta();

int encontra_valor(int, char*, int);
int ordem_alfabetica(char*, char*);
void organiza_vetor(int);

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
        while(fscanf(cli_db, "%s %[^\n] %[^\n] %[^\n] %[^\n]", &cli_atual.codigo, &cli_atual.nome, &cli_atual.cpf_cnpj, &cli_atual.telefone, &cli_atual.endereco)!=EOF){
            v_clientes[i]=cli_atual;
            num_clientes++;
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
            num_contas++;
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
            num_transacoes++;
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
        printf(" C - Gerenciar Clientes\n");
        printf(" T - Gerenciar Contas\n");
        printf(" S - Sair\n");
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

    while(1){
        printf("\n=============== Gerenciar Clientes ===============\n");
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
                //exclui_cliente();
            break;
            case 'S':
                printf("\nRetornando ao menu principal\n");
                return;
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
            break;
        }
    }
}

void menu_conta(){
    char opcao;

    printf("\n================ Gerenciar Contas ================\n");
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
            printf("\nRetornando ao menu principal\n");
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

    switch(tipo){
        case 1:
            switch(categoria){
                case 1:
                    for(i=0; i<MAX_CLIENTES; i++){
                        if(strcmp(v_clientes[i].codigo, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
                case 2:
                    for(i=0; i<MAX_CLIENTES; i++){
                        if(strcmp(v_clientes[i].cpf_cnpj, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
                case 3:
                    for(i=0; i<MAX_CLIENTES; i++){
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
                    for(i=0; i<MAX_CONTAS; i++){
                        if(strcmp(v_contas[i].agencia_e_nro, busca)==0){
                            return i;
                        }
                    }
                    return -1;
                break;
                case 2:
                    for(i=0; i<MAX_CONTAS; i++){
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
                for(i=0; i<MAX_TRANSACOES; i++){
                    if(strcmp(v_transacoes[i].data, busca)==0){
                        return i;
                    }
                }
            }
            return -1;
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
    int i, check;

    while(1){
        check=1;
        printf("\n=============== Cadastro de cliente ===============\n");
        printf("\nInforme o codigo do novo cliente (sem espacos e diferente de 0): ");
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

        if(novo_cli.codigo[0]=='0'){
            printf("\n\n*!* O codigo do cliente nao pode comecar com 0 e nem ser igual a 0! Informe um codigo valido! *!*\n");
            check=0;
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
                    printf("\n------ Cliente cadastrado com sucesso!---------\n");
                    printf("\nRetornando ao menu\n");
                    num_clientes++;
                    return;
                }
            }
            else if(opcao=='n' || opcao=='N'){
                printf("\nDeseja inserir novamente os dados? (s/n): ");
                scanf("%c%*c", &opcao);

                if(opcao=='n' || opcao=='N'){
                    printf("\nRetornando ao menu\n");
                    return;
                }
                else if(opcao=='s' || opcao=='S'){
                    break;
                }
                else{
                    printf("\n*!* Comando invalido! *!*\n");
                }
            }
            else{
                printf("\n*!* Comando invalido! *!*\n");
            }
        }
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
        printf("\n----------------- Fim da lista --------------------\n");
        printf("\nRetornando ao menu\n");
    }
}

void busca_cliente(){

    int i, opcao;
    char busca[MAX_NOME];

    while(1){
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
                i=encontra_valor(1, busca, opcao);
                if(i!=-1){
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
                    printf("\n*!* Nao existe cliente cadastrado com esse codigo! *!*\n");
                }
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca);
                i=encontra_valor(1, busca, opcao);
                if(i!=-1){
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
                    printf("\n*!* Nao existe cliente cadastrado com esse CPF/CNPJ! *!*\n");
                }
            break;
            case 3:
                printf("\nInforme o nome COMPLETO do cliente a ser buscado: ");
                scanf("%[^\n]%*c", &busca);
                i=encontra_valor(1, busca, opcao);
                if(i!=-1){
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
                    printf("\n*!* Nao existe cliente cadastrado com esse nome! *!*\n");
                }
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                scanf("%*c");
            break;
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
    int i, j, opcao, check1, check2, check3; 
    char busca[MAX_CPF_CNPJ];
    CLIENTE cli_atual;

    while(1){
        check1=1;
        check2=1;
        check3=1;
        printf("\n=========== Atualizacao de cliente ===============\n");
        printf("Informe o metodo que deseja usar para buscar o cliente que deseja alterar:\n");
        printf(" 1 - Busca por codigo;\n 2 - Busca por CPF/CNPJ;\n 0 - Retornar ao menu;\n");
        printf("\nEscolha: ");
        scanf("%d%*c", &opcao);

        switch(opcao){
            case 0:
                printf("\nRetornando ao menu\n");
                return;
            break; 
            case 1:
                printf("\nInforme o codigo do cliente a ser atualizado: ");
                scanf("%[^\n]%*c", &busca);

                i=encontra_valor(1, busca, opcao);
            break;
            case 2:
                printf("\nInforme o CPF/CNPJ do cliente a ser atualizado: ");
                scanf("%[^\n]%*c", &busca);

                i=encontra_valor(1, busca, opcao);
            break;
            default:
                printf("\n*!* Comando invalido! *!*\n");
                scanf("%*c");
            break;
        }

        if(i!=-1){
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
                printf("\nInforme o codigo atualizado (sem espacos e diferente de 0): ");
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

                if(cli_atual.codigo[0]=='0'){
                    printf("-----------------------------------------------------");
                    printf("\n*!* O codigo do cliente nao pode comecar com 0 e nem ser igual a 0! Informe um codigo valido! *!*\n");
                    printf("-----------------------------------------------------");
                }
                else{
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
                        scanf("%c%*c", &opcao);
                        if(opcao=='s' || opcao=='S'){
                            v_clientes[i] = cli_atual;
                            if(strcmp(v_clientes[i].codigo, cli_atual.codigo)!=0){
                                printf("\n*!* Erro ao cadastrar cliente! *!*\n");
                                check1=0;
                            }
                            else{
                                printf("\n-------- Cliente atualizado com sucesso! -----------\n");
                                printf("\nRetornando ao menu\n");
                                return;
                            }
                        }
                        else if(opcao=='n' || opcao=='N'){

                            while(check3){
                                printf("\nDeseja inserir novamente os dados? (s/n): ");
                                scanf("%c%*c", &opcao);
                                if(opcao=='n' || opcao=='N'){
                                    printf("\nRetornando ao menu\n");
                                    return;
                                }
                                else if(opcao=='s' || opcao=='S'){
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