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

    //caso o livro esteja sem estoque
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

void registrarDevolucao(livro **livros, int qnt, emprestimo **emprestimos, int *qntEmp){
    int cod, idx = -1;
    char nomeLeitor[100];

    printf("Registro de Devolucao:\n");
    printf("Digite o codigo do livro: ");
    if(scanf("%d", &cod) != 1){
        printf("Codigo Inválido.\n");
        getchar();
        return;
    }
    getchar();

    printf("Digite o nome do leitor que esta devolvendo o livro: "); //deve-se atualizar essa linha 
    if(!fgets(nomeLeitor, sizeof(nomeLeitor), stdin)){
        printf("Erro ao ler nome.\n");
        return;
    }
    nomeLeitor[strcspn(nomeLeitor, "\r\n")] = '\0';

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

    //remove o emprestimo do vetor em memoria 
    for(int i = 0; i < *qntEmp; i++){
        if((*emprestimos)[i].codLivro == cod && strcmp((*emprestimos)[i].nomeLeitor, nomeLeitor) == 0){
            //encontrou o emprestimo, remove deslocando os restantes + 1
            for(int j = i; j < *qntEmp - 1; j++){
                (*emprestimos)[j] = (*emprestimos)[j + 1];
            }
            (*qntEmp)--;
            break;
        }

    }

    salvarEmprestimo(*emprestimos, *qntEmp);
    printf("Devolucao registrada: '%s' (cod %d). Estoque agora: %d\n", (*livros)[idx].titulo, cod, (*livros)[idx].qnt);
   
}

void salvarEmprestimo(emprestimo *emprestimos, int qnt){
    FILE *arquivo = fopen("emprestimo.txt", "w");
    if(!arquivo){
        printf("Erro ao abrir 'emprestimo.txt' para escrita.\n");
        return;
    }

    for(int i = 0; i < qnt; i++){
        fprintf(arquivo, "%d;%s;%s\n", emprestimos[i].codLivro, emprestimos[i].nomeLeitor, emprestimos[i].data); //escreve o emprestimo no arquivo txt
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
        return;
    }

    //ler cada linha do arquivo
     char linha[256];

    while(fgets(linha, sizeof(linha), arquivo) != NULL){ //enquanto fgets diferente de NULL 
     char nomeTemp[100], dataTemp[20];

     if(sscanf(linha, "%d;%99[^;];%19s", &novo.codLivro, nomeTemp, dataTemp) == 3){ //le a linha no formato "cod;nome;data" e verifica se os 3 campos foram lidos corretamente
   
         strcpy(novo.nomeLeitor, nomeTemp); //copia o nome lido (nomeTemp) para o campo nomeLeitor do novo emprestimo
         strcpy(novo.data, dataTemp); //copia a data lida (dataTemp) para o campo data do novo emprestimo
            
        emprestimo *aux = realloc(*emprestimos, (*qnt + 1) * sizeof(emprestimo)); //aumenta o vetor de emprestimos na memoria para caber mais 1 registro e retorna o novo ponteiro (ou null se faltar memoria)
         if(!aux){
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return;
     }

        *emprestimos = aux; //atualiza ponteiro emprestimos para apontar para a nova area de memoria realocada
        (*emprestimos)[*qnt] = novo; //coloca o novo emprestimo na posicao final do vetor (indice *qnt)
        (*qnt)++; //incrementa o contador para que agora temos mais 1 emprestimo na memoria
        }
    }
    fclose(arquivo);
}

void listarLivro(livro *livros, int qnt){
    if(qnt == 0){
        printf("Nenhum livro cadastrado!\n");
        return;
    }

    printf("\n--- Lista de Livros ---\n");
    for(int i = 0; i < qnt; i++){
        printf("Codigo: %d\n", livros[i].cod);
        printf("Titulo: %s\n", livros[i].titulo);
        printf("Autor: %s\n", livros[i].autor);
        printf("Ano: %d\n", livros[i].ano);
        printf("Quantidade: %d\n", livros[i].qnt);
        printf("---------------------------\n");
    }
}// Listar livros

void buscarLivro(livro *livros, int qnt){
    if(qnt == 0){
        printf("Nenhum livro cadastrado!\n");
        return;
    }

    int opc;
    printf("-- Buscar Livro --\n");
    printf("1 - Buscar por titulo\n");
    printf("2 - Buscar por autor\n");
    printf("Opcao: ");
    scanf("%d", &opc);
    getchar();


    // Busca por título
    
    if(opc == 1){
        char busca[150];
        printf("Digite o titulo: ");
        fgets(busca, 150, stdin);
        busca[strcspn(busca, "\n")] = 0;

        int achou = 0;

        for(int i = 0; i < qnt; i++){
            if(strcasecmp(livros[i].titulo, busca) == 0){
                achou = 1;

                printf("\nLivro encontrado:\n");
                printf("Codigo: %d\n", livros[i].cod);
                printf("Titulo: %s\n", livros[i].titulo);
                printf("Autor: %s\n", livros[i].autor);
                printf("Ano: %d\n", livros[i].ano);
                printf("Quantidade: %d\n\n", livros[i].qnt);
            }
        }

        if(!achou)
            printf("Nenhum livro encontrado com esse titulo!\n");
    }

   
    // Busca por autor

    else if(opc == 2){
        char busca[150];
        printf("Digite parte do nome do autor: ");
        fgets(busca, 150, stdin);
        busca[strcspn(busca, "\n")] = 0;

        int achou = 0;

        for(int i = 0; i < qnt; i++){
            if(strcasecmp(livros[i].autor, busca) == 0){
                achou = 1;

                printf("\nLivro encontrado:\n");
                printf("Codigo: %d\n", livros[i].cod);
                printf("Titulo: %s\n", livros[i].titulo);
                printf("Autor: %s\n", livros[i].autor);
                printf("Ano: %d\n", livros[i].ano);
                printf("Quantidade: %d\n\n", livros[i].qnt);
            }
        }

        if(!achou)
            printf("Nenhum autor encontrado!\n");
    }

    else{
        printf("Opção invalida!\n");
    }
}// Buscar livros

void salvarLivros(livro *livros, int qnt){
    FILE *arquivo = fopen("livros.txt", "w");

    if(!arquivo){
        printf("Erro ao abrir 'livros.txt' para escrita!\n");
        return;
    }

    for(int i = 0; i < qnt; i++){
        fprintf(arquivo, "%d;%s;%s;%d;%d;\n",
            livros[i].cod,
            livros[i].titulo,
            livros[i].autor,
            livros[i].ano,
            livros[i].qnt
        );
    }

    fclose(arquivo);
}// Salvar livro

void carregarLivros(livro **livros, int *qnt){
    FILE *arquivo = fopen("livros.txt", "r");
    *qnt = 0;

    if(!arquivo){
        return;
    }

    livro temp;
    while(fscanf(arquivo, "%d;%[^;];%[^;];%d;%d;\n",
                 &temp.cod,
                 temp.titulo,
                 temp.autor,
                 &temp.ano,
                 &temp.qnt) == 5){

        *livros = realloc(*livros, (*qnt + 1) * sizeof(livro));

        if(*livros == NULL){
            printf("Erro de memoria ao carregar livros!\n");
            fclose(arquivo);
            exit(1);
        }

        (*livros)[*qnt] = temp;
        (*qnt)++;
    }

    fclose(arquivo);
}//Carregar livros

void relatorioEmprestimos() {
    FILE *arquivo = fopen("emprestimo.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[500];
    printf("-- Relatorio de Emprestimos --\n");
    printf("%-8s | %-30s | %-10s\n", "CODIGO", "LEITOR", "DATA");

    while (fgets(linha, sizeof(linha), arquivo)) {

        linha[strcspn(linha, "\r\n")] = '\0'; // remove quebra de linha

        int cod;
        char leitor[100];
        char data[20];

        int n = sscanf(linha, "%d;%[^;];%[^;]", &cod, leitor, data);

        if (n != 3) {
            // linha inválida, ignora
            continue;
        }

        printf("%-8d | %-20s | %-10s\n", cod, leitor, data);
    }

    fclose(arquivo);
}
