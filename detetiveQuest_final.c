/*
Detective Quest - Nível Aventureiro/Mestre
Mapa da mansão (Árvore Binária) + Coleta de Pistas (BST) + Tabela Hash de Suspeitos
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
    struct Sala* direita;  // Caminho à direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct PistaNode {
    char valor[100]; // Conteúdo da pista
    struct PistaNode* esquerda; // Subárvore esquerda
    struct PistaNode* direita;  // Subárvore direita
} PistaNode;

// Tabela Hash de suspeitos
#define TAM_HASH 101

typedef struct HashNode {
    char chave[100];   // pista
    char valor[50];    // suspeito
    struct HashNode* prox;
} HashNode;

typedef struct {
    HashNode* buckets[TAM_HASH];
} HashSuspeitos;


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


// Hash: criação e operações

// Função de hash simples para string
static unsigned int hashFunc(const char* s) {
    unsigned int h = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        h = (h * 131) + (unsigned char)s[i];
    }
    return h % TAM_HASH;
}

// Inicializa tabela hash
static void initHash(HashSuspeitos* h) {
    for (int i = 0; i < TAM_HASH; i++) {
        h->buckets[i] = NULL;
    }
}

// [inserirNaHash] insere associação pista/suspeito na tabela hash
void inserirNaHash(HashSuspeitos* h, const char* pista, const char* suspeito) {
    unsigned int idx = hashFunc(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) {
        printf("Erro ao alocar memória na hash!\n");
        exit(1);
    }
    strcpy(novo->chave, pista);
    strcpy(novo->valor, suspeito);
    novo->prox = h->buckets[idx];
    h->buckets[idx] = novo;
}

// [encontrarSuspeito] retorna o suspeito associado à pista (ou NULL se não houver)
const char* encontrarSuspeito(HashSuspeitos* h, const char* pista) {
    unsigned int idx = hashFunc(pista);
    for (HashNode* no = h->buckets[idx]; no != NULL; no = no->prox) {
        if (strcmp(no->chave, pista) == 0) {
            return no->valor;
        }
    }
    return NULL;
}

// Liberação da hash
void liberarHash(HashSuspeitos* h) {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* no = h->buckets[i];
        while (no) {
            HashNode* nxt = no->prox;
            free(no);
            no = nxt;
        }
        h->buckets[i] = NULL;
    }
}


// [explorarSalasComPistas] Navegação e coleta de pistas
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

        // Se for nó-folha, termina exploração automaticamente
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou ao último cômodo deste caminho. Encerrando exploração...\n");
            break;
        }

        // Mostra opções de navegação
        printf("Caminhos disponíveis:\n");
        printf("  [e] Esquerda: %s\n", (atual->esquerda ? atual->esquerda->nome : "-- indisponível --"));
        printf("  [d] Direita : %s\n", (atual->direita  ? atual->direita->nome  : "-- indisponível --"));
        printf("  [s] Sair do jogo (sem julgamento)\n");

        printf("Escolha (e/d/s): ");
        char op;
        if (scanf(" %c", &op) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (op == 's' || op == 'S') {
            printf("Saindo do jogo por solicitação do jogador.\n");
            return;
        } else if (op == 'e' || op == 'E') {
            if (atual->esquerda) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda a partir de %s. Escolha novamente.\n", atual->nome);
            }
        } else if (op == 'd' || op == 'D') {
            if (atual->direita) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita a partir de %s. Escolha novamente.\n", atual->nome);
            }
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

// [explorarSalas] Navega pela arvore e ativa o sistema de pistas
void explorarSalas(Sala* inicio, PistaNode** pistasRaiz) {
    explorarSalasComPistas(inicio, pistasRaiz);
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

// Julgamento final
// Percorre BST e conta quantas pistas apontam para o suspeito acusado
static void contarPistasDoSuspeito(PistaNode* raiz, HashSuspeitos* h, const char* acusado, int* contador) {
    if (raiz == NULL) return;
    contarPistasDoSuspeito(raiz->esquerda, h, acusado, contador);
    const char* s = encontrarSuspeito(h, raiz->valor);
    if (s != NULL && strcmp(s, acusado) == 0) {
        (*contador)++;
    }
    contarPistasDoSuspeito(raiz->direita, h, acusado, contador);
}

// [verificarSuspeitoFinal] conduz à fase de julgamento final
void verificarSuspeitoFinal(PistaNode* pistas, HashSuspeitos* hashSus) {
    // Lista pistas coletadas
    printf("\n=== Pistas coletadas (ordem alfabética) ===\n");
    if (pistas == NULL) {
        printf("(Nenhuma pista coletada.)\n");
    } else {
        exibirPistas(pistas);
    }

    // Exibe lista de suspeitos disponíveis antes da acusação
    printf("\n=== Suspeitos disponíveis ===\n");
    printf("- Jardineiro\n");
    printf("- Camareira\n");
    printf("- Bibliotecário\n");
    printf("- Administrador\n");
    printf("- Cozinheiro\n");

    // Solicita acusação
    char acusado[50];
    printf("\nDigite o nome do suspeito a acusar: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    if (fgets(acusado, sizeof(acusado), stdin) == NULL) {
        printf("Entrada inválida.\n");
        return;
    }
    size_t len = strlen(acusado);
    if (len > 0 && acusado[len-1] == '\n') acusado[len-1] = '\0';

    // Conta evidências
    int cont = 0;
    contarPistasDoSuspeito(pistas, hashSus, acusado, &cont);

    // Resultado (ganha/perde)
    printf("\n=== Julgamento Final ===\n");
    if (cont >= 2) {
        printf("Você VENCEU! Acusação contra \"%s\" confirmada com %d pista(s).\n", acusado, cont);
    } else {
        printf("Você PERDEU. Acusação contra \"%s\" insuficiente: apenas %d pista(s).\n", acusado, cont);
    }
}

// [main] Mapa fixo da mansão, hash de suspeitos, exploração e julgamento
int main(void) {

    //Mansão
    Sala* hall         = criarSala("Hall de Entrada",          "Pegadas de lama");
    Sala* salaEstar    = criarSala("Sala de Estar",            "Lençol manchado");
    Sala* biblioteca   = criarSala("Biblioteca",               "Livro com página faltando");
    Sala* cozinha      = criarSala("Cozinha",                  "Panela com odor estranho");
    Sala* jardim       = criarSala("Jardim",                   "Pegadas recentes perto do portão");
    Sala* escritorio   = criarSala("Escritório",               "Gaveta arrombada");
    Sala* quarto       = criarSala("Quarto",                   "Chave perdida sob o tapete");

    // Ligações
    hall->esquerda       = salaEstar;
    hall->direita        = biblioteca;
    salaEstar->esquerda  = cozinha;
    salaEstar->direita   = jardim;
    biblioteca->esquerda = escritorio;
    biblioteca->direita  = quarto;

    //BST de pistas coletadas
    PistaNode* pistas = NULL;

    // Tabela hash de suspeitos (pista -> suspeito)
    HashSuspeitos hash;
    initHash(&hash);

    // Associações estáticas pista/suspeito
    inserirNaHash(&hash, "Pegadas de lama",                  "Jardineiro");
    inserirNaHash(&hash, "Lençol manchado",                  "Camareira");
    inserirNaHash(&hash, "Panela com odor estranho",         "Camareira");
    inserirNaHash(&hash, "Pegadas recentes perto do portão", "Jardineiro");
    inserirNaHash(&hash, "Livro com página faltando",        "Bibliotecário");
    inserirNaHash(&hash, "Gaveta arrombada",                 "Bibliotecário");
    inserirNaHash(&hash, "Chave perdida sob o tapete",       "Bibliotecário");

    //Explora um único caminho coletando pistas
    explorarSalas(hall, &pistas);

    //Julgamento final
    verificarSuspeitoFinal(pistas, &hash);

    //Liberação de memória
    liberarHash(&hash);
    liberarArvorePistas(pistas);
    liberarArvoreSalas(hall);

    return 0;
}
