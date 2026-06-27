# Biblioteca `util` — Referência da API

Documentação técnica das funções implementadas em `util.c`. Esta lib fornece uma **lista ligada simples e genérica** (via `void *`), além de utilitários de persistência em arquivo, geração de IDs e manipulação de terminal.

> ⚠️ O arquivo `util.h` não foi fornecido. As assinaturas de `Lista`, `No` e `TipoLista` foram inferidas a partir do uso em `util.c`. Ajuste esta documentação caso o header real divirja.

---

## Sumário

- [Estruturas de dados (inferidas)](#estruturas-de-dados-inferidas)
- [Funções de criação e consulta](#funções-de-criação-e-consulta)
  - [`criarLista`](#lista-criarlista)
  - [`listaVazia`](#int-listavazialista-lista)
  - [`tamanhoLista`](#int-tamanholistalista-lista)
- [Funções de inserção](#funções-de-inserção)
  - [`inserirInicio`](#void-inseririniciolista-lista-void-dado)
  - [`inserirFinal`](#void-inserirfinallista-lista-void-dado)
- [Funções de busca e remoção](#funções-de-busca-e-remoção)
  - [`buscar`](#void-buscarlista-lista-void-chave-int-comparvoid-void)
  - [`remover`](#int-removerlista-lista-void-chave-int-comparvoid-void)
- [Destruição](#destruição)
  - [`destruirLista`](#void-destruirlistalista-lista)
- [Persistência em arquivo](#persistência-em-arquivo)
  - [`salvarLista`](#void-salvarlistalista-lista-char-arquivo-size_t-tamanho)
  - [`carregarLista`](#void-carregarlistalista-lista-char-arquivo-size_t-tamanho)
- [Utilitários de terminal](#utilitários-de-terminal)
  - [`limparTela`](#void-limpartela)
  - [`pausar`](#void-pausar)
- [Geração e sincronização de IDs](#geração-e-sincronização-de-ids)
  - [`gerarID`](#int-geraridtipolista-tipo)
  - [`sincronizarID`](#void-sincronizaridtipolista-tipo-int-maior_id_encontrado)
- [Convenções e cuidados gerais](#convenções-e-cuidados-gerais)

---

## Estruturas de dados (inferidas)

```c
typedef struct No {
    void *dado;
    struct No *prox;
} No;

typedef struct Lista {
    No *cabeca;
} Lista;

typedef enum {
    TIPO_CLIENTE,
    TIPO_CARRO,
    TIPO_ALUGUEL
} TipoLista;
```

Variáveis globais externas (declaradas presumivelmente em `util.h` ou outro `.c`):

```c
extern int id_cliente_atual;
extern int id_carro_atual;
extern int id_aluguel_atual;
```

---

## Funções de criação e consulta

### `Lista *criarLista()`

Aloca e inicializa uma nova lista vazia.

**Retorno:** ponteiro para a `Lista` recém-criada (`cabeca == NULL`).

**Comportamento de erro:** se `malloc` falhar, imprime mensagem e encerra o programa com `exit(EXIT_FAILURE)`.

```c
Lista *clientes = criarLista();
```

---

### `int listaVazia(Lista *lista)`

Verifica se a lista está vazia.

| Parâmetro | Descrição |
|---|---|
| `lista` | lista a verificar |

**Retorno:** `1` se `lista == NULL` **ou** se `cabeca == NULL`; `0` caso contrário.

---

### `int tamanhoLista(Lista *lista)`

Conta o número de nós na lista.

**Retorno:** quantidade de elementos (`0` se `lista == NULL` ou vazia). Complexidade O(n).

---

## Funções de inserção

### `void inserirInicio(Lista *lista, void *dado)`

Insere `dado` no início da lista (complexidade O(1)).

**Parâmetros**

| Parâmetro | Descrição |
|---|---|
| `lista` | lista de destino |
| `dado` | ponteiro genérico para o dado a armazenar |

**Comportamento:**
- Não faz nada se `lista == NULL` ou `dado == NULL`.
- Se a alocação do nó falhar, imprime erro e retorna sem inserir.
- A lista passa a ser responsável por liberar `dado` (ver [Convenções](#convenções-e-cuidados-gerais)).

---

### `void inserirFinal(Lista *lista, void *dado)`

Insere `dado` no final da lista (complexidade O(n), pois percorre até o último nó).

**Parâmetros:** iguais a `inserirInicio`.

**Comportamento:**
- Mesmas validações de `NULL` e falha de `malloc` que `inserirInicio`.
- Se a lista estiver vazia, o novo nó se torna a cabeça.

---

## Funções de busca e remoção

### `void *buscar(Lista *lista, void *chave, int (*comparar)(void *, void *))`

Procura na lista o primeiro dado que satisfaça o critério de `comparar`.

**Parâmetros**

| Parâmetro | Descrição |
|---|---|
| `lista` | lista onde buscar |
| `chave` | valor de referência passado ao comparador |
| `comparar` | função que recebe `(dado_do_no, chave)` e retorna não-zero em caso de igualdade |

**Retorno:** ponteiro para o dado encontrado, ou `NULL` se não encontrado ou `lista == NULL`.

**Exemplo de comparador:**

```c
int comparaPorId(void *dado, void *chave) {
    Cliente *c = (Cliente *) dado;
    int id = *(int *) chave;
    return c->id == id;
}

int idBusca = 5;
Cliente *c = (Cliente *) buscar(clientes, &idBusca, comparaPorId);
```

---

### `int remover(Lista *lista, void *chave, int (*comparar)(void *, void *))`

Remove o primeiro nó cujo dado satisfaça `comparar`.

**Parâmetros:** iguais a `buscar`.

**Retorno:** `1` se um elemento foi removido; `0` se não encontrado ou `lista == NULL`.

**⚠️ Importante:** esta função chama `free(atual->dado)` e `free(atual)`. O dado removido **é destruído** — não use o ponteiro depois de chamar `remover`, e não chame `remover` em dados que não foram alocados dinamicamente (ex.: dados na stack).

---

## Destruição

### `void destruirLista(Lista *lista)`

Libera todos os nós **e** seus respectivos dados (`free(temp->dado)` + `free(temp)`), e por fim libera a própria `Lista`.

**Comportamento:** não faz nada se `lista == NULL`. Após a chamada, o ponteiro `lista` fica inválido (dangling) — não o reutilize.

---

## Persistência em arquivo

### `void salvarLista(Lista *lista, char arquivo[], size_t tamanho)`

Salva os dados da lista em um arquivo binário, gravando cada `dado` com `fwrite(dado, tamanho, 1, file)`.

**Parâmetros**

| Parâmetro | Descrição |
|---|---|
| `lista` | lista a salvar |
| `arquivo` | caminho do arquivo de destino |
| `tamanho` | tamanho em bytes de **cada** struct de dado (`sizeof(MeuStruct)`) |

**Comportamento:**
- Se a lista for `NULL` ou vazia, imprime aviso e não cria/sobrescreve o arquivo.
- Abre o arquivo em modo `"wb"` (sobrescreve se já existir).

**⚠️ Limitação importante:** como a gravação é um `fwrite` binário do bloco de memória bruto, **a struct de dado não pode conter ponteiros** (ex.: `char *nome`) — apenas tipos de tamanho fixo (`int`, `float`, `char nome[50]`, etc.). Ponteiros salvos assim se tornam endereços de memória inválidos ao recarregar.

```c
salvarLista(clientes, "clientes.dat", sizeof(Cliente));
```

---

### `void carregarLista(Lista *lista, char arquivo[], size_t tamanho)`

Lê um arquivo binário gravado por `salvarLista` e reconstrói a lista, inserindo cada registro lido no final via `inserirFinal`.

**Parâmetros:** iguais a `salvarLista`.

**Comportamento:**
- Se o arquivo não existir/não abrir, imprime aviso e retorna.
- Para cada registro: aloca `tamanho` bytes, lê com `fread`; se a leitura falhar (fim de arquivo), libera a memória alocada e interrompe o laço.
- **Pré-condição implícita:** a `lista` passada deve já existir (ser criada com `criarLista`); a função não cria a lista, apenas insere nela.

```c
Lista *clientes = criarLista();
carregarLista(clientes, "clientes.dat", sizeof(Cliente));
```

---

## Utilitários de terminal

### `void limparTela()`

Limpa o terminal. Usa `system("cls")` no Windows e `system("clear")` em outros sistemas (detecção via `_WIN32`/`_WIN64`).

> Observação: uso de `system()` tem custo de performance e implicações de portabilidade/segurança; aceitável em projetos acadêmicos, evitar em produção.

---

### `void pausar()`

Exibe `"Pressione [ENTER] para continuar..."` e bloqueia a execução até o usuário pressionar Enter, descartando caracteres do buffer de entrada até encontrar `'\n'` ou `EOF`.

Útil para pausar menus de console entre uma ação e a exibição da próxima tela.

---

## Geração e sincronização de IDs

### `int gerarID(TipoLista tipo)`

Gera e retorna o próximo ID disponível para o tipo de entidade informado, incrementando o contador global correspondente (pós-incremento).

| `tipo` | Contador usado |
|---|---|
| `TIPO_CLIENTE` | `id_cliente_atual` |
| `TIPO_CARRO` | `id_carro_atual` |
| `TIPO_ALUGUEL` | `id_aluguel_atual` |

**Retorno:** o ID gerado, ou `-1` se `tipo` for inválido (imprime mensagem de erro).

---

### `void sincronizarID(TipoLista tipo, int maior_id_encontrado)`

Ajusta o contador global do tipo informado para `maior_id_encontrado + 1`. Tipicamente usada após `carregarLista`, para garantir que os próximos IDs gerados por `gerarID` não colidam com IDs já existentes no arquivo carregado.

```c
carregarLista(clientes, "clientes.dat", sizeof(Cliente));
// ... percorrer a lista para achar o maior id ...
sincronizarID(TIPO_CLIENTE, maiorIdEncontrado);
```

---

## Convenções e cuidados gerais

- **Posse dos dados (ownership):** uma vez inserido (`inserirInicio`/`inserirFinal`), o dado passa a ser **propriedade da lista**. `remover` e `destruirLista` chamam `free()` sobre ele. Por isso:
  - Insira apenas dados alocados com `malloc`/`calloc`.
  - Nunca insira o mesmo ponteiro em duas listas diferentes (double free).
  - Nunca use o ponteiro do dado após `remover` ou `destruirLista`.
- **Genericidade via `void *`:** a lista não conhece o tipo armazenado. Cabe ao código cliente fazer o cast correto (`(MeuStruct *) dado`) e fornecer comparadores compatíveis com o tipo real.
- **Funções de comparação:** devem seguir a assinatura `int (*)(void *, void *)` e retornar não-zero para "iguais". Não há padrão fixo de qual argumento é o dado do nó e qual é a chave — siga o uso em `buscar`/`remover`: `comparar(dado_do_no, chave)`.
- **Serialização binária:** válida apenas para structs "planas" (sem ponteiros, sem campos de tamanho variável). Mudar o layout do struct invalida arquivos salvos anteriormente.
- **Tratamento de erros:** a lib é tolerante a `NULL` na maioria das funções (retorna/ignora silenciosamente), exceto na falha de alocação em `criarLista`, que aborta o programa.