#ifndef MENU_H
#define MENU_H

#include "util.h"
#include "clientes.h"
#include "carros.h"
#include "locacao.h"

void imprimir_cabecalho(const char *subtitulo);



void menu_gerenciar_clientes(Lista *lista_clientes);
void menu_gerenciar_carros(Lista *lista_carros, Lista *lista_locacoes);
void menu_gerenciar_alugueis(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes);
void menu_admin(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes);

void listar_meus_alugueis(Lista *lista_locacoes, int id_cliente);
void menu_cliente_logado(Cliente *cliente, Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes);
void menu_login_cliente(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes);


void iniciar_sistema(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes);
void menu_pesquisa_carros(Lista *lista_carros);
void menu_pesquisa_clientes(Lista *lista_clientes);
void menu_pesquisa_locacoes(Lista *lista_locacoes);

#endif // MENU_H