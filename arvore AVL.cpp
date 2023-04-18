#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// function prototype
struct node* deletar(struct node* no, int valor);

//basic struture
struct node {
    int valor;
    struct node* esq;
    struct node* dir;
    int altura;
};

// return max value beetween a and b parameter
int max(int a, int b) {
    return (a > b) ? a : b;
}

// get the height
int altura(struct node* no) {
    if (no == NULL) {
        return 0;
    }
    return no->altura;
}

// factor balance
int fatorBalanceamento(struct node* no) {
    if (no == NULL) {
        return 0;
    }
    return altura(no->esq) - altura(no->dir);
}

// rotation to the right
struct node* rotacaoDireita(struct node* y) {
    struct node* x = y->esq;
    struct node* t2 = x->dir;

    x->dir = y;
    y->esq = t2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
return x;

}

// rotation to the left
struct node* rotacaoEsquerda(struct node* x) {
	struct node* y = x->dir;
	struct node* t2 = y->esq;

	y->esq = x;
	x->dir = t2;

	x->altura = max(altura(x->esq), altura(x->dir)) + 1;
	y->altura = max(altura(y->esq), altura(y->dir)) + 1;

	return y;

}

// rotation right and left
struct node* rotacaoDireitaEsquerda(struct node* z) {
	z->esq = rotacaoEsquerda(z->esq);
	return rotacaoDireita(z);
}

// rotation left and right
struct node* rotacaoEsquerdaDireita(struct node* z) {
	z->dir = rotacaoDireita(z->dir);
	return rotacaoEsquerda(z);
}

// make a new node
struct node* criarNo(int valor) {
	struct node* novoNo = (struct node*) malloc(sizeof(struct node));
	novoNo->valor = valor;
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->altura = 1;
	return novoNo;
}

// insert new node on the tree AVL
struct node* inserir(struct node* no, int valor) {
	if (no == NULL) {
	return criarNo(valor);
	}


	if (valor < no->valor) {
		no->esq = inserir(no->esq, valor);
	} else if (valor > no->valor) {
		no->dir = inserir(no->dir, valor);
	} else {
		// value already on the tree
		return no;
	}

	no->altura = 1 + max(altura(no->esq), altura(no->dir));

	int b = fatorBalanceamento(no);

	if (b > 1 && valor < no->esq->valor) {
		return rotacaoDireita(no);
	}
	if (b < -1 && valor > no->dir->valor) {
		return rotacaoEsquerda(no);
	}
	if (b > 1 && valor > no->esq->valor) {
		return rotacaoEsquerdaDireita(no);
	}
	if (b < -1 && valor < no->dir->valor) {
		return rotacaoDireitaEsquerda(no);
	}

	return no;
 
 
}

// search value on the tree
struct node* buscar(struct node* no, int valor) {
	if (no == NULL || no->valor == valor) {
		return no;
	}
	if (valor < no->valor) {
		return buscar(no->esq, valor);
	}
	return buscar(no->dir, valor);
}

// update value on the tree
struct node* atualizar(struct node* no, int valorAntigo, int valorNovo) {
	no = deletar(no, valorAntigo);
	no = inserir(no, valorNovo);
	return no;
}

// get min node
struct node* minimo(struct node* no) {
    if (no == NULL) {
        return NULL;
    }
    struct node* iterator = no;
    while (iterator->esq != NULL) {
        iterator = iterator->esq;
    }
    return iterator;
}

// remove value 
struct node* deletar(struct node* no, int valor) {
	if (no == NULL) {
	return no;
	}

	if (valor < no->valor) {
		no->esq = deletar(no->esq, valor);
	} else if (valor > no->valor) {
		no->dir = deletar(no->dir, valor);
	} else {
		if (no->esq == NULL || no->dir == NULL) {
			struct node* temp = no->esq ? no->esq : no->dir;

			if (temp == NULL) {
				temp = no;
				no = NULL;
			} else {
				no = temp;
			}

			free(temp);
		} else {
			struct node* temp = minimo(no->dir);
			no->valor = temp->valor;
			no->dir = deletar(no->dir, temp->valor);
        
		}
	}

	if (no == NULL) {
		return no;
	}

	no->altura = 1 + max(altura(no->esq), altura(no->dir));

	int fb = fatorBalanceamento(no);

	if (fb > 1 && fatorBalanceamento(no->esq) >= 0) {
		return rotacaoDireita(no);
	}
	if (fb > 1 && fatorBalanceamento(no->esq) < 0) {
		return rotacaoEsquerdaDireita(no);
	}
	if (fb < -1 && fatorBalanceamento(no->dir) <= 0) {
		return rotacaoEsquerda(no);
	}
	if (fb < -1 && fatorBalanceamento(no->dir) > 0) {
		return rotacaoDireitaEsquerda(no);
	}

	return no;

}
// print all nodes in terminal screen
void listar(struct node* no) {
	if (no != NULL) {
	listar(no->esq);
	printf("%d ", no->valor);
	listar(no->dir);
	}
}
// main function
int main() {
    struct node* raiz = NULL;

	int opcao, valor, valorAntigo;
	// user interaction for test the tree AVL structure
	do {
		printf("\nEscolha uma opcao:\n");
		printf("1 - Inserir valor\n");
		printf("2 - Buscar valor\n");
		printf("3 - Atualizar valor\n");
		printf("4 - Deletar valor\n");
		printf("5 - Listar todos os valores\n");
		printf("0 - Sair\n");

		scanf("%d", &opcao);

		switch (opcao) {
			case 1:
				printf("Digite o valor que deseja inserir: ");
				scanf("%d", &valor);
				raiz = inserir(raiz, valor);
				printf("Valor inserido com sucesso!\n");
				break;
			case 2:
				printf("Digite o valor que deseja buscar: ");
				scanf("%d", &valor);
				if (buscar(raiz, valor) != NULL) {
					printf("Valor encontrado na arvore!\n");
				} else {
					printf("Valor nao encontrado na arvore.\n");
				}
				break;
			case 3:
				printf("Digite o valor que deseja atualizar: ");
				scanf("%d", &valorAntigo);
				printf("Digite o novo valor: ");
				scanf("%d", &valor);
				if (atualizar(raiz, valorAntigo, valor) != NULL) {
					printf("Valor atualizado com sucesso!\n");
				} else {
					printf("Valor nao encontrado na arvore.\n");
				}
				break;
			case 4:
				printf("Digite o valor que deseja deletar: ");
				scanf("%d", &valor);
				if (deletar(raiz, valor) != NULL) {
					printf("Valor deletado com sucesso!\n");
				} else {
					printf("Valor nao encontrado na arvore.\n");
				}
				break;
			case 5:
				printf("Valores na arvore: ");
				listar(raiz);
				printf("\n");
				break;
			case 0:
				printf("Saindo...\n");
				break;
			default:
				printf("Opcao invalida.\n");
				break;
		}
	} while (opcao != 0);

	return 0;


}