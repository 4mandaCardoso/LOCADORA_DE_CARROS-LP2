#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "menu.h"
#include "clientes.h"
#include "carros.h"
#include "locacao.h"
#include "relatorio.h"
#include "util.h"


// Imprime o cabeçalho padronizado com a Data/Hora atual
void imprimir_cabecalho(const char *subtitulo) {
    char data_hora_str[30];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(data_hora_str, 30, "%d/%m/%Y %H:%M", tm_info);

    limparTela();
    printf("+--------------------------------------------------+\n");
    printf("|                      LOCAR                       |\n");
    printf("|               %s                   |\n", data_hora_str);
    printf("+--------------------------------------------------+\n");
    if (subtitulo != NULL && strlen(subtitulo) > 0) {
        printf("| %-48s |\n", subtitulo);
        printf("|--------------------------------------------------|\n");
    }
}

// ====================================================================
// SUBMENUS DO ADMINISTRADOR
// ====================================================================

void menu_gerenciar_clientes(Lista *lista_clientes) {
    int opcao;
    do {
        imprimir_cabecalho("Menu de Clientes (Admin)");
        printf("| [1] Listar Clientes                              |\n");
        printf("| [2] Cadastrar Cliente                            |\n");
        printf("| [3] Editar Cliente                               |\n");
        printf("| [4] Excluir Cliente                              |\n");
        printf("| [5] Buscar Cliente                               |\n");
        printf("| [0] Voltar                                       |\n");
        printf("+--------------------------------------------------+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: listar_clientes(lista_clientes); pausar(); break;
            case 2: cadastrar_cliente(lista_clientes); pausar(); break;
            case 3: atualizar_cliente(lista_clientes); pausar(); break;
            case 4: remover_cliente(lista_clientes); pausar(); break;
            case 5: menu_pesquisa_clientes(lista_clientes); pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_gerenciar_carros(Lista *lista_carros, Lista *lista_locacoes) {
    int opcao;
    do {
        imprimir_cabecalho("Menu de Carros (Admin)");
        printf("| [1] Listar Frota                                 |\n");
        printf("| [2] Editar Carro                                 |\n");
        printf("| [3] Buscar Carro                                 |\n");
        printf("| [4] Cadastrar Carro                              |\n");
        printf("| [5] Excluir Carro                                |\n");
        printf("|                                                  |\n");
        printf("| [0] Voltar                                       |\n");
        printf("+--------------------------------------------------+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: listar_frota(lista_carros); pausar(); break;
            case 2: editar_carro(lista_carros); pausar(); break;
            case 3: menu_pesquisa_carros(lista_carros); pausar(); break;
            case 4: cadastrar_carro(lista_carros); pausar(); break;
            case 5: deletar_Carros(lista_carros, lista_locacoes); pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_gerenciar_alugueis(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes) {
    int opcao;
    do {
        imprimir_cabecalho("Menu de Alugueis (Admin)");
        printf("| [1] Listar Alugueis                              |\n");
        printf("| [2] Buscar carro                                 |\n");
        printf("| [3] Cadastrar Aluguel (Retirada)                 |\n");
        printf("| [4] Registrar Devolucao                          |\n");
        printf("|                                                  |\n");
        printf("| [0] Voltar                                       |\n");
        printf("+--------------------------------------------------+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: 
                listar_locacoes(lista_locacoes); pausar(); break;
            case 2: 
                menu_pesquisa_carros(lista_carros); pausar(); break;
            case 3: 
            if (listaVazia(lista_clientes)) {
                printf("Nenhum cliente cadastrado no sistema. Nao e possivel realizar locacao.\n");
                pausar();
                break;
            }
            if (listaVazia(lista_carros)) {
                printf("Nenhum carro cadastrado na frota. Nao e possivel realizar locacao.\n");
                pausar();
                break;
            }
            listar_clientes(lista_clientes);
            listar_frota(lista_carros);
            realizar_locacao(lista_locacoes, lista_clientes, lista_carros, NULL); pausar(); break;
            case 4:realizar_devolucao(lista_locacoes, NULL); pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_admin(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes) {
    int opcao;
    do {
        imprimir_cabecalho("Administrador: Acesso Restrito");
        printf("| [1] Clientes                                     |\n");
        printf("| [2] Carros                                       |\n");
        printf("| [3] Alugueis                                     |\n");
        printf("| [4] Relatorios                                   |\n");
        printf("|                                                  |\n");
        printf("| [0] Sair (Voltar ao Login)                       |\n");
        printf("+--------------------------------------------------+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: menu_gerenciar_clientes(lista_clientes); break;
            case 2: menu_gerenciar_carros(lista_carros, lista_locacoes); break;
            case 3: menu_gerenciar_alugueis(lista_clientes, lista_carros, lista_locacoes); break;
            case 4:
                mostrar_relatorio(lista_locacoes, lista_clientes, lista_carros);
                gerar_relatorio(lista_locacoes, lista_clientes, lista_carros);
                printf("\nRelatorio gerado com sucesso!\n");
                pausar();
                break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

// ====================================================================
// FLUXO DO CLIENTE
// ====================================================================

void listar_meus_alugueis(Lista *lista_locacoes, int id_cliente) {
    printf("\n--- MEUS ALUGUEIS ---\n");
    if (listaVazia(lista_locacoes)) {
        printf("Nenhum aluguel registrado no sistema.\n");
        return;
    }
    
    int encontrou = 0;
    No *aux = lista_locacoes->cabeca;
    char status_str[30];

    while (aux != NULL) {
        Aluguel *loc = (Aluguel *)aux->dado;
        if (loc != NULL && loc->cliente_locador != NULL && loc->cliente_locador->id == id_cliente) {
            encontrou = 1;
            if (loc->status == STATUS_ENCERRADO) strcpy(status_str, "Concluido");
            else strcpy(status_str, "Ativo");
            
            printf("ID Locacao: %d | Carro: %s | Total: R$ %.2f | Status: [%s]\n",
                    loc->id, loc->carro_alugado ? loc->carro_alugado->modelo : "(carro desconhecido)", loc->valorTotal, status_str);
        }
        aux = aux->prox;
    }
    if (!encontrou) printf("Voce ainda nao possui alugueis registrados.\n");
}

void menu_cliente_logado(Cliente *cliente, Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes) {
    int opcao;
    char subtitulo[114];
    sprintf(subtitulo, "Bem-vindo(a), %s", cliente->nome);

    do {
        imprimir_cabecalho(subtitulo);
        printf("| [1] Alugar um carro                              |\n");
        printf("| [2] Meus alugueis                                |\n");
        printf("| [3] Meu perfil (Meus dados)                      |\n");
        printf("| [4] Devolucao de carro (Encerrar locacao)        |\n");
        printf("|                                                  |\n");
        printf("| [0] Sair (Logoff)                                |\n");
        printf("+--------------------------------------------------+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: 
                if (listaVazia(lista_carros)) {
                    printf("Nenhum carro cadastrado na frota. Tente novamente mais tarde.\n");
                    pausar();
                    break;
                }
                listar_frota(lista_carros);
                printf("\n--- Solicitar Locacao ---\n");
                realizar_locacao(lista_locacoes, lista_clientes, lista_carros, cliente);
                pausar();
                break;
            case 2: 
                listar_meus_alugueis(lista_locacoes, cliente->id);
                pausar();
                break;
            case 3:
                printf("\n--- MEUS DADOS ---\n");
                printf("Nome: %s\nCPF: %s\nIdade: %s\n", cliente->nome, cliente->cpf, cliente->idade);
                pausar();
                break;
            case 4:
                listar_meus_alugueis(lista_locacoes, cliente->id);
                realizar_devolucao(lista_locacoes, cliente);
                pausar();
                 break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_login_cliente(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes) {
    int opcao;
    
    do {
        if (listaVazia(lista_clientes)) {
            imprimir_cabecalho("Acesso de Cliente");
            printf("| AVISO: Nenhum cliente cadastrado no sistema.     |\n");
            printf("|--------------------------------------------------|\n");
            printf("| [1] Cadastrar Nova Conta                         |\n");
            printf("| [0] Voltar ao Menu Principal                     |\n");
            printf("+--------------------------------------------------+\n");
            printf("Escolha: ");
            scanf("%d", &opcao);
            limparBuffer();

            if (opcao == 1) {
                cadastrar_cliente(lista_clientes);
                pausar();
            } else if (opcao != 0) {
                printf("Opcao invalida!\n"); pausar();
            }
        } else {
            imprimir_cabecalho("Acesso de Cliente");
            printf("| [1] Fazer Login (via CPF)                        |\n");
            printf("| [2] Cadastrar Nova Conta                         |\n");
            printf("|                                                  |\n");
            printf("| [0] Voltar ao Menu Principal                     |\n");
            printf("+--------------------------------------------------+\n");
            printf("Escolha: ");
            scanf("%d", &opcao);
            limparBuffer();

            if (opcao == 1) {
                char cpf_busca[TAM_CPF];
                printf("Digite seu CPF para login: ");
                fgets(cpf_busca, TAM_CPF, stdin);
                remover_quebra_linha(cpf_busca);
                
                Cliente *logado = (Cliente *) buscar(lista_clientes, cpf_busca, compara_cliente_cpf);
                
                if (logado != NULL) {
                    menu_cliente_logado(logado, lista_clientes, lista_carros, lista_locacoes);
                } else {
                    printf("\nERRO: CPF nao encontrado. Verifique o CPF ou cadastre-se como novo cliente.\n");
                    pausar();
                }
            } else if (opcao == 2) {
                cadastrar_cliente(lista_clientes);
                pausar();
            }
        }
    } while (opcao != 0);
}

// ====================================================================
// MENU PRINCIPAL (LOGIN INICIAL)
// ====================================================================

void iniciar_sistema(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes) {
    int opcao;

    do {
        imprimir_cabecalho("Portal de Acesso");
        printf("| Como deseja acessar o sistema?                   |\n");
        printf("|                                                  |\n");
        printf("| [1] Administrador                                |\n");
        printf("| [2] Cliente                                      |\n");
        printf("| [3] Ajuda                                        |\n");
        printf("|                                                  |\n");
        printf("| [0] Sair e Salvar Dados                          |\n");
        printf("+--------------------------------------------------+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                menu_admin(lista_clientes, lista_carros, lista_locacoes);
                break;
            case 2:
                menu_login_cliente(lista_clientes, lista_carros, lista_locacoes);
                break;
            case 3:
                exibir_arquivo("ajuda.txt"); pausar();  break;
            case 0:
                printf("\nEncerrando o sistema LOCAR...\n");
                printf("Salvando dados em arquivos binarios e texto...\n");
                salvarListaBinarioETxt(lista_clientes, DADOS_CLIENTES_BIN, DADOS_CLIENTES_TXT, sizeof(Cliente), TIPO_CLIENTE);
                salvarListaBinarioETxt(lista_carros, DADOS_CARROS_BIN, DADOS_CARROS_TXT, sizeof(Carro), TIPO_CARRO);
                salvar_locacoes_bin(lista_locacoes, DADOS_LOCACOES_BIN);
                salvarLista_txt(lista_locacoes, DADOS_LOCACOES_TXT, sizeof(Aluguel), TIPO_ALUGUEL);
                gerar_relatorio(lista_locacoes, lista_clientes, lista_carros);
                destruirLista(lista_clientes);
                destruirLista(lista_carros);
                destruirLista(lista_locacoes);
                printf("Memoria liberada com sucesso. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida!\n");
                pausar();
        }
    } while (opcao != 0);
}

void menu_pesquisa_clientes(Lista *lista_clientes) {
    char termo[100];
    int qtd_encontrada = 0;

    printf("\n--- PESQUISA DE CLIENTES ---\n");
    printf("Digite o ID, CPF ou parte do Nome do cliente: ");
    fgets(termo, sizeof(termo), stdin);
    remover_quebra_linha(termo);

    void **clientes = busca_universal(lista_clientes, TIPO_CLIENTE, termo, &qtd_encontrada);

    if (clientes != NULL) {
        printf("\n--- RESULTADOS (%d encontrados) ---\n", qtd_encontrada);
        for (int i = 0; i < qtd_encontrada; i++) {
            Cliente *c = (Cliente *)clientes[i];
            // %03d formata o ID com 3 zeros (ex: 001, 012). %-20s alinha o nome à esquerda ocupando 20 espaços.
            printf("ID: %03d | Nome: %-20s | CPF: %-15s | Idade: %s\n", 
                   c->id, c->nome, c->cpf, c->idade);
        }
        free(clientes); // Libera o array gerado pela busca
    } else {
        printf("\n[AVISO] Nenhum cliente encontrado para o termo: '%s'.\n", termo);
    }
}


void menu_pesquisa_carros(Lista *lista_carros) {
    char termo[100];
    int qtd_encontrada = 0;

    printf("\n--- PESQUISA DE FOTA (CARROS) ---\n");
    printf("Digite o ID, Placa ou parte do Modelo do carro: ");
    fgets(termo, sizeof(termo), stdin);
    remover_quebra_linha(termo);

    void **carros = busca_universal(lista_carros, TIPO_CARRO, termo, &qtd_encontrada);

    if (carros != NULL) {
        printf("\n--- RESULTADOS (%d encontrados) ---\n", qtd_encontrada);
        for (int i = 0; i < qtd_encontrada; i++) {
            Carro *c = (Carro *)carros[i];
            
            // Define o texto do status para ficar mais bonito na tela
            char status_texto[20];
            strcpy(status_texto, c->disponivel ? "Disponivel" : "Alugado");

            printf("ID: %03d | Modelo: %-15s | Placa: %-10s | Diaria: R$ %6.2f | Status: %s\n", 
                   c->id, c->modelo, c->placa, c->diaria, status_texto);
        }
        free(carros); 
    } else {
        printf("\n[AVISO] Nenhum carro encontrado para o termo: '%s'.\n", termo);
    }
}

void menu_pesquisa_locacoes(Lista *lista_locacoes) {
    char termo[100];
    int qtd_encontrada = 0;

    printf("\n--- PESQUISA DE LOCACOES ---\n");
    printf("Digite o ID da locacao ou parte do Nome do cliente: ");
    fgets(termo, sizeof(termo), stdin);
    remover_quebra_linha(termo);

    void **locacoes = busca_universal(lista_locacoes, TIPO_ALUGUEL, termo, &qtd_encontrada);

    if (locacoes != NULL) {
        printf("\n--- RESULTADOS (%d encontrados) ---\n", qtd_encontrada);
        for (int i = 0; i < qtd_encontrada; i++) {
            Aluguel *a = (Aluguel *)locacoes[i];
            
            // Tratamento de segurança: Caso o cliente ou carro tenham sido deletados do sistema
            char nome_cliente[TAM_NOME] = "Desconhecido";
            char modelo_carro[TAM_MODELO] = "Desconhecido";
            
            if (a->cliente_locador != NULL) {
                strcpy(nome_cliente, a->cliente_locador->nome);
            }
            if (a->carro_alugado != NULL) {
                strcpy(modelo_carro, a->carro_alugado->modelo);
            }

            char status_texto[15];
            strcpy(status_texto, a->status == 1 ? "Ativo" : "Encerrado");

            printf("ID: %03d | Cliente: %-15s | Carro: %-12s | Total: R$ %6.2f | Status: %s\n", 
                   a->id, nome_cliente, modelo_carro, a->valorTotal, status_texto);
        }
        free(locacoes); 
    } else {
        printf("\n[AVISO] Nenhuma locacao encontrada para o termo: '%s'.\n", termo);
    }
}