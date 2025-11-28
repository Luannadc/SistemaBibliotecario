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
    printf("Digite o nome do livro: ");
    fgets(novo.nomeLeitor, 100, stdin);
    
    FILE *arquivo;
    fopen("emprestimo.txt", "r"); //abre arquivo emprestimo.txt
    if(!arquivo){
        printf("Arquivo 'emprestimo.txt' nao encontrado!\n");
        return; 
    }

    char linha[300];
    while(fgets(linha, sizeof(linha), arquivo)){
        linha[strcspn(linha, "\n")] = 0;
    }
    
}

