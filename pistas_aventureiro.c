/*
Detective Quest - Nível Aventureiro
Mapa da mansão (Árvore Binária) + Coleta de Pistas (BST)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estruturas de dados

// Nó da árvore da mansão (mapa)
typedef struct Sala {
    char nome[50]; // Nome do cômodo
    char pista[100]; // Pista opcional ("" se não houver)
    struct Sala* esquerda; // Caminho à esquerda
    struct Sala* direita; // Caminho à direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char valor[100]; // Conteúdo da pista
    struct PistaNode* esquerda; // Subárvore esquerda
    struct PistaNode* direita;  // Subárvore direita
} PistaNode;

// [criarSala] Cria dinamicamente um cômodo com nome e pista (opcional)
// Se 'pista' for NULL, armazena string vazia "" (sem pista).
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL) strcpy(nova->pista, pista);
    else strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita  = NULL;
    return nova;
}


// Utilitário: cria nó de pista (interno da BST)
static PistaNode* criarNoPista(const char* valor) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para a pista!\n");
        exit(1);
    }
    strcpy(novo->valor, valor);
    novo->esquerda = NULL;
    novo->direita  = NULL;
    return novo;
}

// [inserirPista] Insere pista na BST (ordem alfabética por strcmp)
PistaNode* inserirPista(PistaNode* raiz, const char* valor) {
    if (raiz == NULL) {
        return criarNoPista(valor);
    }
    if (strcmp(valor, raiz->valor) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, valor);
    } else {
        raiz->direita  = inserirPista(raiz->direita,  valor);
    }
    return raiz;
}

// [exibirPistas] Imprime as pistas em ordem alfabética (em-ordem)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->valor);
        exibirPistas(raiz->direita);
    }
}

// [explorarSalasComPistas] Navegação e coleta automatica
void explorarSalasComPistas(Sala* atual, PistaNode** pistasRaiz) {
    if (atual == NULL) {
        printf("Mapa vazio. Nada para explorar.\n");
        return;
    }

    printf("\n=== Detective Quest: Exploração da Mansão (Coleta de Pistas) ===\n");

    while (1) {
        // Exibe a sala atual
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta automática da pista
        if (atual->pista[0] != '\0') {
            printf("Pista encontrada aqui: \"%s\"\n", atual->pista);
            *pistasRaiz = inserirPista(*pistasRaiz, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        // Encerramento automático ao chegar no fim (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou ao fim da exploração!\n");
            break;
        }

        // Mostra opções de navegação
        printf("Caminhos disponíveis:\n");
        printf("  [e] Esquerda: %s\n", (atual->esquerda ? atual->esquerda->nome : "-- sem caminho --"));
        printf("  [d] Direita : %s\n", (atual->direita  ? atual->direita->nome  : "-- sem caminho --"));
        printf("  [s] Sair da exploração\n");

        printf("Escolha (e/d/s): ");
        char op;
        if (scanf(" %c", &op) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (op == 's' || op == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else if (op == 'e' || op == 'E') {
            if (atual->esquerda) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda a partir de %s.\n", atual->nome);
            }
        } else if (op == 'd' || op == 'D') {
            if (atual->direita) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita a partir de %s.\n", atual->nome);
            }
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

// Liberação de memória
void liberarArvoreSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvoreSalas(raiz->esquerda);
        liberarArvoreSalas(raiz->direita);
        free(raiz);
    }
}

void liberarArvorePistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

// [main] Monta o mapa fixo e inicia a exploração, no final exibe todas as pistas coletadas em ordem alfabética
int main(void) {

    Sala* hall         = criarSala("Hall de Entrada",          "Pegadas de lama");
    Sala* salaEstar    = criarSala("Sala de Estar",            "Lençol manchado");
    Sala* biblioteca   = criarSala("Biblioteca",               "Livro com página faltando");
    Sala* cozinha      = criarSala("Cozinha",                  "Panela com odor estranho");
    Sala* jardim       = criarSala("Jardim",                   "Pegadas recentes perto do portão");
    Sala* escritorio   = criarSala("Escritório",               "Gaveta arrombada");
    Sala* quarto       = criarSala("Quarto",                   "Chave perdida sob o tapete");

    // Ligações (mapa fixo)
    hall->esquerda       = salaEstar;
    hall->direita        = biblioteca;
    salaEstar->esquerda  = cozinha;
    salaEstar->direita   = jardim;
    biblioteca->esquerda = escritorio;
    biblioteca->direita  = quarto;

    //BST de pistas coletadas
    PistaNode* pistas = NULL;

    //Exploração + coleta
    explorarSalasComPistas(hall, &pistas);

    //Exibição final das pistas em ordem alfabética
    printf("\n=== Pistas coletadas ===\n");
    if (pistas == NULL) {
        printf("(Nenhuma pista coletada.)\n");
    } else {
        exibirPistas(pistas);
    }

    //Liberação de memória
    liberarArvorePistas(pistas);
    liberarArvoreSalas(hall);

    return 0;
}
