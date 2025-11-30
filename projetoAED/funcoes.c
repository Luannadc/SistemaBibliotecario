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

void registrarEmprestimo(livro **livros, int qnt, emprestimo *emprestimos, int qntEmp){
    emprestimo novo; //cria nova variavel do tipo emprestimo, novo

    printf("Registro de Emprestimo:\n");
    printf("Digite o codigo do livro: ");
    scanf("%d", &novo.codLivro);
    getchar();

    printf("Digite o nome do livro: ");
    fgets(novo.nomeLeitor, 100, stdin);
    novo.nomeLeitor[strcspn(novo.nomeLeitor, "\n")] = 0;

    //verifica se o livro existe no sistema
    int encontrado = 0;
    for(int i = 0; i < qnt; i++){
        if((*livros)[i].cod == novo.codLivro){ //verifica se o cod do livro digitado pelo ususario corresponde ao cod do livro atual 
            encontrado = 1;
            break;
        }
    }

    if(!encontrado){
        printf("Livro nao encontrado!\n");
        return;
    }

    FILE *arquivo = fopen("emprestimo.txt", "a"); //abre arquivo emprestimo.txt
    if(!arquivo){
        printf("Arquivo 'emprestimo.txt' nao encontrado!\n");
        return; 
    }

    //salva no arquivo
    fprintf(arquivo, "%d;%s;\n", novo.codLivro, novo.nomeLeitor);
    fclose(arquivo);

    emprestimos[qntEmp] = novo; //salva o novo emprestimo dentro do vetor de emprestimos que ja existe na memoria 

    printf("Emprestimo registrado com sucesso!\n");  
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
