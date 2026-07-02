
#include "clientes.h"
#include "util.h"
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
    if (strlen(cpf) != 11 || strspn(cpf, "0123456789") != 11)
    {
        printf("\nERRO: O CPF deve conter exatamente 11 digitos numericos.\n");
        return 0;
    }

    //Verifica se o CPF já está cadastrado no sistema para outro cliente
    Cliente *duplicado = (Cliente *) buscar(lista_clientes, cpf, compara_cliente_cpf);
    
    // Se achou o mesmo CPF E não é o próprio cliente que está sendo atualizado
    if (duplicado != NULL && duplicado->id != id_cliente_atual)
    {
        printf("\nERRO: Este CPF ja esta cadastrado no sistema para outro cliente.\n");
        return 0;
    }

    return 1; // Passou em todas as verificações, CPF é válido
}

void cadastrar_cliente(Lista *lista_clientes)
{
    Cliente *novo = (Cliente *) malloc(sizeof(Cliente));
    if (novo == NULL)
    {
        printf("nERRO: Erro ao alocar memoria para o novo cliente!\n");
        return;
    }

    novo->id = gerarID(TIPO_CLIENTE);

    printf("\n--- CADASTRAR CLIENTE ---\n");

    printf("Digite seu nome: ");
    fgets(novo->nome, TAM_NOME, stdin);
    remover_quebra_linha(novo->nome);
        
    while (strspn(novo->nome, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz ") != strlen(novo->nome) || strlen(novo->nome) == 0)
    {
        printf("\nERRO: O nome deve conter apenas letras e espacos.\n");

        printf("Digite seu nome novamente: ");
        fgets(novo->nome, TAM_NOME, stdin);
        remover_quebra_linha(novo->nome);
    }


    //avalia se o CPF ja esta cadastrado no sistema, se estiver, não permitir o cadastro
     do {
        printf("\nDigite seu CPF - (apenas 11 digitos): ");
        fgets(novo->cpf, TAM_CPF, stdin);
        remover_quebra_linha(novo->cpf);
    } while (validar_cpf(lista_clientes, novo->cpf, novo->id) != 1);
     

    printf("\nDigite sua idade: ");
    fgets(novo->idade, TAM_IDADE, stdin);
    remover_quebra_linha(novo->idade);

    // Avalia se a idade está no formato correto(apenas digitos), se não estiver, não permitir o cadastro
    while (strspn(novo->idade, "0123456789") != strlen(novo->idade) || strlen(novo->idade) == 0 || atoi(novo->idade) < 0 || atoi(novo->idade) > 135)
    {
        printf("\nERRO: A idade deve conter apenas digitos numericos.\n");

        printf("Digite sua idade novamente: ");
        fgets(novo->idade, TAM_IDADE, stdin);
        remover_quebra_linha(novo->idade);
    }

    // verifica se o cliente é maior de idade, se não for, não permitir o cadastro
    int idade = atoi(novo->idade);
    if (idade < 18)
    {
        printf("\nERRO: O cliente deve ser maior de idade (18+) para ser cadastrado.\n");
        free(novo);
        pausar();
        return;
    }
    // Insere na lista duplamente encadeada genérica
    inserirFinal(lista_clientes, novo);

    printf("\nCliente cadastrado no sistema com sucesso! ID gerado: %03d\n", novo->id);
}

void listar_clientes(Lista *lista_clientes)
{
    if (listaVazia(lista_clientes) == 1)
    {
        printf("\nNenhum cliente cadastrado no momento.\n");
        return;
    }

    No *aux = lista_clientes->cabeca;
    printf("\n========================= LISTA DE CLIENTES =========================\n");
    while (aux != NULL)
    {
        Cliente *c = (Cliente *) aux->dado; //troca do ponteiro genérico para Cliente
        printf("ID: %03d | Nome: %s | CPF: %s | Idade: %s\n", c->id, c->nome, c->cpf, c->idade);
        aux = aux->prox; // Avança usando a estrutura do nó duplamente encadeado
    }
    printf("=====================================================================\n");
}

void buscar_cliente(Lista *lista_clientes)
{
    if (listaVazia(lista_clientes) == 1)
    {
        printf("Nenhum cliente cadastrado no sistema.\n");
        return;
    }

    char cpf_busca[TAM_CPF];
    printf("Digite o CPF do cliente que deseja buscar: "); 
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

void atualizar_cliente(Lista *lista_clientes)
{
   if (listaVazia(lista_clientes) == 1)
   {
       printf("Nenhum cliente cadastrado no sistema para atualizar.\n");
       return;
   }

    char cpf_busca[TAM_CPF];
   printf("Digite o CPF do cliente que deseja atualizar: ");
   fgets(cpf_busca, TAM_CPF, stdin);
   remover_quebra_linha(cpf_busca);

   Cliente *cliente_atualizar = (Cliente *) buscar(lista_clientes, cpf_busca, compara_cliente_cpf);

       if (cliente_atualizar != NULL){

        printf("\nCliente encontrado pelo sistema. Informe os novos dados:\n");


        printf("Digite o novo nome (atual: %s): ", cliente_atualizar->nome);
        fgets(cliente_atualizar->nome, TAM_NOME, stdin);
        remover_quebra_linha(cliente_atualizar->nome);

             while (strspn(cliente_atualizar->nome, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz ") != strlen(cliente_atualizar->nome) || strlen(cliente_atualizar->nome) == 0)
            {
                 printf("\nERRO: O nome deve conter apenas letras e espacos.\n");

                 printf("Digite seu nome novamente: ");
                 fgets(cliente_atualizar->nome, TAM_NOME, stdin);
                 remover_quebra_linha(cliente_atualizar->nome);
             }

            do {
                printf("\nDigite seu CPF - (apenas 11 digitos): ");
                fgets(cliente_atualizar->cpf, TAM_CPF, stdin);
                remover_quebra_linha(cliente_atualizar->cpf);
            } while (validar_cpf(lista_clientes, cliente_atualizar->cpf, cliente_atualizar->id) != 1);

        printf("Digite a nova idade (atual: %s): ", cliente_atualizar->idade);
        fgets(cliente_atualizar->idade, TAM_IDADE, stdin);
        remover_quebra_linha(cliente_atualizar->idade);

            while (strspn(cliente_atualizar->idade, "0123456789") != strlen(cliente_atualizar->idade) || strlen(cliente_atualizar->idade) == 0)
            {
                printf("\nERRO: A idade deve conter apenas digitos numericos.\n");

                printf("Digite sua idade novamente: ");
                fgets(cliente_atualizar->idade, TAM_IDADE, stdin);
                remover_quebra_linha(cliente_atualizar->idade);
            }

            // verifica se o cliente é maior de idade, se não for, não permitir o cadastro
            int idade = atoi(cliente_atualizar->idade);
                if (idade < 18)
                {
                    printf("\nERRO: O cliente deve ser maior de idade (18+) para ser cadastrado.\n");
                    pausar();
                    return;
                }

        printf("\nCliente atualizado com sucesso!\n");
        salvarListaBinarioETxt(lista_clientes, DADOS_CLIENTES_BIN, DADOS_CLIENTES_TXT, sizeof(Cliente), TIPO_CLIENTE);

    } else
    
    {
        printf("\nCliente com o CPF '%s' nao foi encontrado no sistema.\n", cpf_busca);
    }

}    

