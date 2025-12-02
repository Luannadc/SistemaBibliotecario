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
        printf("4. Registrar Empréstimo\n");
        printf("5. Registrar Devolução\n");
        printf("6. Relatório\n");
        printf("0. Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &op);

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
        registrarEmprestimo(&livros, qnt, &emprestimos, &qntEmp);
        break;
    case 5:
        registrarDevolucao(&livros, qnt);
        break;
    case 6:
        relatorioEmprestimos();
        break;
    case 0:
        printf("Encerrando...\n");
        break;
    default:
        printf("Opcao inválida!\n");//Adicionei mais algumas opções - Crystian
    }  
}while(op != 0);//O switch tava fora do do-while - Crystian
 return 0;
}