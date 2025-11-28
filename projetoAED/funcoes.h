#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct{
    int cod;
    char titulo[150];
    char autor[150];
    int ano;
    int qnt;
}livro;

typedef struct {
    int codLivro;
    char nomeLeitor[100];
    char data[20];
}emprestimo;

void cadastrarLivro(livro **livros, int *qnt); // feito
void buscarLivro(livro *livros, int qnt); //c
void listarLivro(livro *livros, int qnt); //c 
void registrarEmprestimo(livro *livros, int qnt, emprestimo **emprestimos, int *qntEmp); //l
void registrarDevolucao(livro *livros, int qnt); //l 
void salvarLivros(livro *livros, int qnt); //c
void carregarLivros(livro **livros, int *qnt); //c
void salvarEmprestimo(emprestimo *emprestimos, int qnt); //l
void carregarEmprestimos(emprestimo **emprestimos, int qnt);  //l
void relatorioEmprestimos(); //ultimo 


#endif

