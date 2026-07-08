#ifndef RELATORIO_H
#define RELATORIO_H

#include "util.h"
#include "clientes.h"
#include "carros.h"
#include "locacao.h"

// Funções para gerar e mostrar relatórios, bem como calcular receitas
void gerar_relatorio(Lista *historico, Lista *lista_clientes, Lista *lista_carros);
void mostrar_relatorio(Lista *historico, Lista *lista_clientes, Lista *lista_carros);
double calcular_receita_mes(Lista *historico, int mes, int ano);
double calcular_receita_total(Lista *historico);

#endif // RELATORIO_H
