
#include "clientes.h"
#include "util.h"
#include "menu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compara_cliente_cpf(void *dado, void *chave) // verifica se o CPF do cliente é igual ao CPF buscado
{
    Cliente *c = (Cliente *) dado; 
    char *cpf_busca = (char *) chave;  

    if (strcmp(c->cpf, cpf_busca) == 0){ // se o CPF do cliente for igual ao CPF buscado,  retorna 1 (verdadeiro)
        return 1;
    }
    return 0; // se não for igual, retorna 0 (falso)
}

int validar_cpf(Lista *lista_clientes, char *cpf, int id_cliente_atual)
{
    // Analisa o tamanho e se contém apenas dígitos
     int i;
     for (i = 0; cpf[i] != '\0'; i++) {
        // Se passar de 11 caracteres ou encontrar algo que não seja número da erro 
        if (i >= 11 || cpf[i] < '0' || cpf[i] > '9') {
            printf("\nERRO: O CPF deve conter exatamente 11 digitos numericos.\n");
            pausar();
            return 0;
        }
    }

    // Se o laço terminou e não chegou a 11 dígitos (ex: a string tinha apenas 5)
    if (i != 11) {
        printf("\nERRO: O CPF deve conter exatamente 11 digitos numericos.\n");
        pausar();
        return 0;
    }
    
    //Verifica se o CPF já está cadastrado no sistema para outro cliente
    Cliente *duplicado = (Cliente *) buscar(lista_clientes, cpf, compara_cliente_cpf);
    
    // Se achou o mesmo CPF E não é o próprio cliente que está sendo atualizado
    if (duplicado != NULL && duplicado->id != id_cliente_atual)
    {
        printf("\nERRO: Este CPF ja esta cadastrado no sistema para outro cliente.\n");
        pausar();
        return 0;
    }
    
    return 1; // Passou em todas as verificações, CPF é válido
}

int validar_nome(Lista *lista_clientes, char *nome, int id_cliente_atual)
{
    // Analisa se a pessoa apenas não digitou nada
    if (nome == NULL || nome[0] == '\0') {
        printf("\nERRO: O nome nao pode estar vazio.\n");
        pausar();
       
        return 0;
    }

    int possui_letra = 0;
    
    for(int i=0;nome[i]!='\0';i++) {
        
        unsigned char c = (unsigned char) nome[i]; // a função isalpha() espera um valor de tipo unsigned char ou EOF, então fazemos o cast para evitar problemas com caracteres acentuados.
        
        
        if(!isalpha(c) && !isspace(c)) {
            printf("\nERRO: O nome deve conter apenas letras e espacos.\n");
            pausar();
            return 0;
        }

        if (isalpha(c)) {
            possui_letra = 1; // Encontrou pelo menos uma letra válida
        } 
        
        if (!possui_letra) {
            printf("\nERRO: O nome nao pode conter apenas espacos sem letras.\n");
            pausar();
            return 0;
        }
    }
    
    return 1; // Passou em todas as verificações, nome é válido
}

int validar_idade(Lista *lista_clientes, char *idade, int id_cliente_atual)
{
    // Analisa o tamanho e se contém apenas dígitos
    if (idade == NULL || idade[0] == '\0') {
        printf("\nERRO: A idade nao pode estar vazia.\n");
        pausar();
        return 0;
    }

    for (int i = 0; idade[i] != '\0'; i++) {
        if (idade[i] < '0' || idade[i] > '9') {
            printf("\nERRO: A idade deve conter apenas digitos numericos.\n");
            pausar();
            return 0;
        }
    }

    int idade_int = atoi(idade);
    if (idade_int < 18 || idade_int > 135) {
        printf("\nERRO: Idades devem estar entre 18 e 135.\n");
        pausar();
        return 0;
    }

    return 1; // Passou em todas as verificações, idade é de fato apenas numeros
}

//CRUD: Função para cadastrar um novo cliente no sistema
void cadastrar_cliente(Lista *lista_clientes)
{
    Cliente *novo = (Cliente *) malloc(sizeof(Cliente));
    if (novo == NULL)
    {
        printf("\nERRO: Erro ao alocar memoria para o novo cliente!\n");
        pausar();
        return;
    }

    novo->id = gerarID(TIPO_CLIENTE); // Gera um ID único para o cliente, garantindo que não haja duplicatas no sistema

    printf("\n--- CADASTRAR CLIENTE ---\n");

    // === NOME ===
    int erro_nome = 0;
    do {
        if (erro_nome > 0) {
            // Sobe exatamente 6 linhas (compensando os \n do erro e do pausar) e apaga tudo para baixo
            printf("\033[6A\033[J"); 
            erro_nome = 0; // Resetando o contador de erros para evitar loop infinito
        }
        
        printf("Digite seu nome: ");
        fflush(stdout); 
        fgets(novo->nome, TAM_NOME, stdin);
        remover_quebra_linha(novo->nome);
        
        // Se a validação retornar 1 (sucesso), ele dá um break e sai do loop
        if (validar_nome(lista_clientes, novo->nome, novo->id) == 1) {
            break;
        }
        erro_nome++; // Se chegou aqui, é porque falhou. Registra o erro para limpar na próxima volta!
    } while (1);


    // === CPF ===
    int erro_cpf = 0;
    do {
        if (erro_cpf > 0) {
            printf("\033[6A\033[J");
            erro_cpf = 0; // Resetando o contador de erros para evitar loop infinito e apagar quantidade de linhas erradas
        }
        
        printf("Digite seu CPF (apenas 11 digitos): ");
        fflush(stdout); 
        fgets(novo->cpf, TAM_CPF, stdin);
        remover_quebra_linha(novo->cpf);
        
        if (validar_cpf(lista_clientes, novo->cpf, novo->id) == 1) {
            break;
        }
        erro_cpf++;
    } while (1);
     

    // === IDADE ===
    int erro_idade = 0;
    do{
        if (erro_idade > 0) {
            printf("\033[6A\033[J"); 
            erro_idade = 0; // Resetando o contador de erros para evitar loop infinito e apagar quantidade de linhas erradas
        }
        
        printf("Digite sua idade: ");
        fflush(stdout);
        fgets(novo->idade, TAM_IDADE, stdin);
        remover_quebra_linha(novo->idade);
        
        if (validar_idade(lista_clientes, novo->idade, novo->id) == 1) {
            break;
        }
        erro_idade++;
    } while (1);

    inserirFinal(lista_clientes, novo);

    printf("\nCliente cadastrado no sistema com sucesso! ID gerado: %03d\n", novo->id);
}


//CRUD: Função para listar todos os clientes cadastrados no sistema
void listar_clientes(Lista *lista_clientes)
{
    if (listaVazia(lista_clientes) == 1)
    {
        printf("\nNenhum cliente cadastrado no momento.\n");
        return;
    }

    No *aux = lista_clientes->cabeca;
    printf("\n========================= LISTA DE CLIENTES =========================\n");
    printf(" === NUMERO DE CLIENTES CADASTRADOS: %d ===\n", tamanhoLista(lista_clientes));
    while (aux != NULL)
    {
        Cliente *c = (Cliente *) aux->dado; //troca do ponteiro genérico para Cliente
        printf("ID: %03d | Nome: %s | CPF: %s | Idade: %s\n", c->id, c->nome, c->cpf, c->idade);
        aux = aux->prox; // Avança usando a estrutura do nó duplamente encadeado
    }
    printf("=====================================================================\n");
}

//CRUD: Função para buscar um cliente pelo CPF
void buscar_cliente(Lista *lista_clientes)
{
    if (listaVazia(lista_clientes) == 1)
    {
        printf("Nenhum cliente cadastrado no sistema.\n");
        return;
    }

    char cpf_busca[TAM_CPF];
    printf("\nDigite o CPF do cliente que deseja buscar: "); 
    fgets(cpf_busca, TAM_CPF, stdin);
    remover_quebra_linha(cpf_busca);

    // Chama a busca genérica passando a função de comparação de CPF
    Cliente *encontrado = (Cliente *) buscar(lista_clientes, cpf_busca, compara_cliente_cpf);

    if (encontrado != NULL)
    {
        printf("\nCliente Encontrado:\n");
        printf("ID: %d\nNome: %s\nCPF: %s\nIdade: %s\n", encontrado->id, encontrado->nome, encontrado->cpf, encontrado->idade);
    }
    else
    {
        printf("\nCliente com o CPF '%s' nao foi encontrado.\n", cpf_busca);
    }
}

//CRUD: Função para remover um cliente do sistema
void remover_cliente(Lista *lista_clientes)
{
    if (listaVazia(lista_clientes) == 1)
    {
        printf("Nenhum cliente cadastrado no sistema para remover.\n");
        return;
    }
    printf("\n--- REMOVER CLIENTE ---\n");

    char cpf_busca[TAM_CPF];
    printf("Digite o CPF do cliente que deseja remover: ");
    fgets(cpf_busca, TAM_CPF, stdin);
    remover_quebra_linha(cpf_busca); 

    // apenas busca o cliente para exibir os dados antes de remover para evitar remoção acidental
    Cliente *encontrado = (Cliente *) buscar(lista_clientes, cpf_busca, compara_cliente_cpf);

    if (encontrado != NULL)
    {
        printf("\nCliente Encontrado:\n");
        printf("ID: %d\nNome: %s\nCPF: %s\nIdade: %s\n", encontrado->id, encontrado->nome, encontrado->cpf, encontrado->idade);
       
            char opcao;

        printf("\nTem certeza que deseja remover este cliente? (S/N): ");
        scanf(" %c", &opcao);
        limparBuffer();

        if (opcao == 'S' || opcao == 's')
        {
            int removido = remover(lista_clientes, cpf_busca, compara_cliente_cpf);
            if (removido == 1)
            {
                printf("\nCliente removido com sucesso do sistema!\n");
            }
            else
            {
                printf("\nNao foi possivel remover o cliente.\n");
            }
        }
        else
        {
            printf("\nOperacao cancelada. Nenhum cliente foi removido.\n");
        }
    }
    else
    {
        printf("\nNao foi possivel remover o cliente: CPF '%s' nao cadastrado no sistema.\n", cpf_busca);
    }
}

//CRUD: Função para atualizar informações de um cliente no sistema
void atualizar_cliente(Lista *lista_clientes)
{
    if (listaVazia(lista_clientes) == 1)
    {
        printf("\nNenhum cliente cadastrado no sistema para atualizar.\n");
        return;
    }

     printf("\n--- ATUALIZAR CLIENTE ---\n");

    char cpf_busca[TAM_CPF];
    printf("\nDigite o CPF do cliente que deseja atualizar: ");
    fgets(cpf_busca, TAM_CPF, stdin);
    remover_quebra_linha(cpf_busca);

    Cliente *cliente_atualizar = (Cliente *) buscar(lista_clientes, cpf_busca, compara_cliente_cpf);

    if (cliente_atualizar != NULL)
    {
        // Variáveis temporárias para total segurança e integridade dos dados
        char temp_nome[TAM_NOME];
        char temp_cpf[TAM_CPF];
        char temp_idade[TAM_IDADE];

        printf("\nCliente encontrado pelo sistema. Informe os novos dados:\n");

        // === NOVO NOME ===
        int erro_nome = 0;
        do {
            if (erro_nome > 0) {
                printf("\033[6A\033[J"); // Sobe 6 linhas e limpa o rastro do erro
                erro_nome = 0; // Resetando o contador de erros para evitar loop infinito e apagar quantidade de linhas erradas
            }
            
            printf("Digite o novo nome (atual: %s): ", cliente_atualizar->nome);
            fflush(stdout);
            fgets(temp_nome, TAM_NOME, stdin);
            remover_quebra_linha(temp_nome);
            
            if (validar_nome(lista_clientes, temp_nome, cliente_atualizar->id) == 1) {
                break;
            }
            erro_nome++;
        } while (1);

        // === NOVO CPF ===
        int erro_cpf = 0;
        do {
            if (erro_cpf > 0) {
                printf("\033[6A\033[J"); // Sobe 6 linhas e limpa o rastro do erro
                erro_cpf = 0; // Resetando o contador de erros para evitar loop infinito e apagar quantidade de linhas erradas
            }
            
            printf("Digite o novo CPF (atual: %s): ", cliente_atualizar->cpf);
            fflush(stdout);
            fgets(temp_cpf, TAM_CPF, stdin);
            remover_quebra_linha(temp_cpf);
            
            if (validar_cpf(lista_clientes, temp_cpf, cliente_atualizar->id) == 1) {
                break;
            }
            erro_cpf++;
        } while (1);

        // === NOVA IDADE ===
        int erro_idade = 0;
        do {
            if (erro_idade > 0) {
                printf("\033[6A\033[J"); // Sobe 6 linhas e limpa o rastro do erro
                erro_idade = 0; // Resetando o contador de erros para evitar loop infinito e apagar quantidade de linhas erradas
            }
            
            printf("Digite a nova idade (atual: %s): ", cliente_atualizar->idade);
            fflush(stdout);
            fgets(temp_idade, TAM_IDADE, stdin);
            remover_quebra_linha(temp_idade);
            
            if (validar_idade(lista_clientes, temp_idade, cliente_atualizar->id) == 1) {
                break;
            }
            erro_idade++;
        } while (1);

        // Se o programa passou de todos os loops com sucesso, consolida a alteração na memória física
        strcpy(cliente_atualizar->nome, temp_nome);
        strcpy(cliente_atualizar->cpf, temp_cpf);
        strcpy(cliente_atualizar->idade, temp_idade);

        printf("\nCliente atualizado com sucesso!\n");
        
    } 
    else
    {
        printf("\nCliente com o CPF '%s' nao foi encontrado no sistema.\n", cpf_busca);
    }
}   

