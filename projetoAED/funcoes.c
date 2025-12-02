#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

void cadastrarLivro(livro **livros, int *qnt){
    livro novo; //declara uma variavel do tipo livro, "novo".
    
    printf("-- Cadastro de Livro --\n");
    printf("Codigo: ");
    scanf("%d", &novo.cod);
    getchar();

    printf("Titulo: ");
    fgets(novo.titulo, 150, stdin);
    novo.titulo[strcspn(novo.titulo, "\n")] = 0;
    
    printf("Autor: ");
    fgets(novo.autor, 150, stdin);
    novo.autor[strcspn(novo.autor, "\n")] = 0;

    printf("Ano: ");
    scanf("%d", &novo.ano);
    getchar();

    printf("Quantidade no estoque: ");
    scanf("%d", &novo.qnt);
    getchar();

    //realoca o vetor para mais um livro
    *livros = realloc(*livros, (*qnt + 1) * sizeof(livro));
    
    if(*livros == NULL){
        printf("Erro de memoria!\n");
        exit(1);
    }

    (*livros) [*qnt] = novo;  //coloca o livro novo no final da lista de livros.
    (*qnt)++; //incrementa a quantidade total de livros cadastrados

    printf("Livro cadastrado com sucesso!\n");

    salvarLivros(*livros, *qnt); //atualiza o arquivo txt
    
}

void registrarEmprestimo(livro **livros, int qnt, emprestimo **emprestimos, int *qntEmp){
    emprestimo novo; //cria nova variavel do tipo emprestimo, novo

    printf("Registro de Emprestimo:\n");
    printf("Digite o codigo do livro: ");
    if(scanf("%d", &novo.codLivro) != 1){
        printf("Código Inválido.\n");
        getchar();
        return;
    }
    getchar();
    printf("Digite o nome do leitor: ");
    fgets(novo.nomeLeitor, 100, stdin);
    novo.nomeLeitor[strcspn(novo.nomeLeitor, "\n")] = 0;

    //verifica se o livro existe no sistema
    int idx = -1; 
    for(int i = 0; i < qnt; i++){
        if((*livros)[i].cod == novo.codLivro){ //verifica se o cod do livro digitado pelo ususario corresponde ao cod do livro atual 
            idx = i;
            break;
        }
    }

    //caso o livro nao seja encontrado
    if(idx == -1){
        printf("Livro nao encontrado!\n");
        return;
    }

    //sem estoque
    if((*livros)[idx].qnt <= 0){
        printf("Livro '%s' sem estoque.\n", (*livros)[idx].titulo);
        return;
    }

    printf("Digite a data (dd/mm/aaaa): ");
    fgets(novo.data, 20, stdin);
    novo.data[strcspn(novo.data, "\r\n")] = '\0';

   (*livros)[idx].qnt--;
    salvarLivros(*livros, qnt);

    emprestimo *tmp = realloc(*emprestimos, (*qntEmp + 1) * sizeof(emprestimo)); //realoca memoria para o vetor de emprestimos com espaco para mais 1 elemento; retorna novo ponteiro tmp
    *emprestimos = tmp;
    (*emprestimos)[*qntEmp] = novo; //salva o novo emprestimo dentro do vetor de emprestimos que ja existe na memoria 
    (*qntEmp)++;

    printf("Emprestimo registrado com sucesso! Estoque atual: %d\n", (*livros)[idx].qnt);  
    salvarEmprestimo(*emprestimos, *qntEmp);
}

void registrarDevolucao(livro **livros, int qnt){
    int cod, idx = -1;

    printf("Registro de Devolução:\n");
    printf("Digite o codigo do livro: ");
    if(scanf("%d", &cod) != 1){
        printf("Codigo Inválido.\n");
        getchar();
        return;
    }
    getchar();

    for(int i = 0; i < qnt; i++){
        if((*livros)[i].cod == cod){
            idx = i;
            break;
        }
    }

    if(idx == -1){
        printf("Codigo Inválido.\n");
        return;
    }

    //atualiza o estoque
    (*livros)[idx].qnt++;
    salvarLivros(*livros, qnt);

    printf("Devolução registrada: '%s' (cod %d). Estoque agora: %d\n", (*livros)[idx].titulo, cod, (*livros)[idx].qnt);
   
}

void salvarEmprestimo(emprestimo *emprestimos, int qnt){
    FILE *arquivo = fopen("emprestimo.txt", "w");
    if(!arquivo){
        printf("Erro ao abrir 'emprestimo.txt' para escrita.\n");
        return;
    }

    for(int i = 0; i < qnt; i++){
        fprintf(arquivo, "%d;%s;%s\n", emprestimos[i].codLivro, emprestimos[i].nomeLeitor, emprestimos[i].data);
    }

    fclose(arquivo);
}

//carrega todos os emprestimos salvos no arquivo e reconstroi o vetor na memória, quando o programa é iniciado.
void carregarEmprestimos(emprestimo **emprestimos, int *qnt){
    emprestimo novo;
    *qnt = 0;
    *emprestimos = NULL;
    
    FILE *arquivo = fopen("emprestimo.txt", "r"); //abre o arquivo para ler e escrever; o arquivo deve existir
    if(!arquivo){
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //ler cada linha do arquivo
     char linha[256];

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
     char nomeTemp[100], dataTemp[20];

     if(sscanf(linha, "%d;%99[^;];%19s", &novo.codLivro, nomeTemp, dataTemp) == 3){
   
         strcpy(novo.nomeLeitor, nomeTemp);
         strcpy(novo.data, dataTemp);
            
         emprestimo *aux = realloc(*emprestimos, (*qnt + 1) * sizeof(emprestimo));
         if(!aux){
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return;
        }

        *emprestimos = aux;
        (*emprestimos)[*qnt] = novo;
        (*qnt)++;
        }
    }
    fclose(arquivo);
}