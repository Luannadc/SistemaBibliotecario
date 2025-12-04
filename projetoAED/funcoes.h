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

void cadastrarLivro(livro **livros, int *qnt);
void buscarLivro(livro *livros, int qnt); 
void listarLivro(livro *livros, int qnt); 
void registrarEmprestimo(livro **livros, int qnt, emprestimo **emprestimos, int *qntEmp); 
void registrarDevolucao(livro **livros, int qnt); 
void salvarLivros(livro *livros, int qnt); 
void carregarLivros(livro **livros, int *qnt); 
void salvarEmprestimo(emprestimo *emprestimos, int qnt); 
void carregarEmprestimos(emprestimo **emprestimos, int *qnt);  
void relatorioEmprestimos();  


#endif

