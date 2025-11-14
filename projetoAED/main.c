#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main(){
    livro *livros = NULL; //inicializa o ponteiro nao apontando à lugar nenhum
    emprestimo *emprestimos = NULL;

    int op, qnt = 0;
    int qntEmp = 0;
    
    do{
        printf("==== MENU ====\n");
        printf("1. Cadastrar Livros\n");
        printf("2. Buscar Livros\n");
        printf("3. Listar Livros\n");
        printf("4. Registrar Emprestimo\n");
        printf("5. Registrar Devolucao\n");
        printf("6. Relatorio\n");
        printf("0. Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &op);
    }while(op != 0);

    switch (op){
    case 1:
        cadastrarLivro(&livros, &qnt);
        break;
    case 2:
        buscarLivro(livros, qnt);
        break;
    case 3:
        listarLivro(livros, qnt);
        break;
    case 4:
        registrarEmprestimo(livros, qnt, emprestimos, &qntEmp);
        break;
    case 5:
        registrarDevolucao(&livros, qnt);
        break;
    case 6:
        relatorioEmprestimos();
    default:
        break;
    }

    
}