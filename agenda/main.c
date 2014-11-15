//
//  main.c
//  Trabalho Final APC2 / LAB2
//
//  Copyright (c) 2014 Rod Elias <rod@wgo.com.br>. All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define QTDE_REGISTROS_AGENDA 2

// representacao do contato
typedef struct {
    char codigo[5];
    char nome[31];
    char telefone[11];
    char logradouro[101];
    char numero[6];
    char bairro[81];
    char cep[10];
    char cidade[21];
    char estado[3]; // sigla do estado, mas deve ter tamanho 3 por causa do terminador nulo '\0'
    char pais[11];
} contato;

// prototipo das funcoes

void menu(void);
void menu_busca(void);
int get_codigo(char *);
int write_codigo(char *, int);
int increment(int);
int inicializa_auxiliar(char *);
int arquivo_existe(char *);
int get_qtde_registros(char *, contato);
int adicionar_contato(char *, contato);
int visualizar_agenda(char *, contato);
contato *carregar_arquivo(char *, int);
int buscar_contato(int, char *, contato *, int);
void visualizar_contato(contato *, int);
contato* remover_contato(contato *, int, int);
int salvar_arquivo(char *, contato *, int);
contato* editar_contato(contato *, int, contato);

int main(int argc, const char * argv[])
{
    int opcao; // opcao do menu principal
    int opcaoBusca; // opcao do menu de busca
    int codigo; // codigo inteiro armazenado no arquivo auxiliar
    int tamanho_agenda;
    int index;
    int opcaoValida = 0; // por padrao, a opcao digitada eh considerada invalida

    char strCodigo[4]; // versao string do codigo [por alguma razao, fwrite() nao consegue gravar no arquivo um valor inteiro ]
    char chave[31];
    contato *agenda;
    contato *novaAgenda;

    contato c; // representa um unico contato
    char *arquivoAuxiliar = ".aux.txt";
    char *arquivoAgenda = "agenda.txt";
    
    printf("\n** Trabalho de APC2 e LAB2 **\n");
    printf("** Aluno: Rodiney Elias Marcal - Matricula 132518 **\n");
    printf("** Docente: Prof. Dra. Luanna Lobato **\n\n");

    do {
        menu();
        printf("\nEscolha uma das opcoes acima: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // adicionar contato
                
                if(get_qtde_registros(arquivoAgenda, c) >= QTDE_REGISTROS_AGENDA) {
                    printf("\n** Atencao: O limite de %d registros na agenda ja foi atingido!**\n\n", QTDE_REGISTROS_AGENDA);
                } else {
                    printf("\n** Adicionar Contato **\n\n");
                    
                    if(!arquivo_existe(arquivoAuxiliar)) {
                        inicializa_auxiliar(arquivoAuxiliar);
                    }

                    codigo = get_codigo(arquivoAuxiliar);
                    
                    sprintf(strCodigo, "%d", codigo); // converte e armazena em strCodigo o codigo inteiro lido do arquivo auxiliar
                    
                    if(codigo == -1) {
                        printf("** Erro: Impossivel obter codigo de registro! Verifique arquivo auxiliar! **\n\n");
                        return(1); // sinaliza erro
                    }
                    
                    // atribui o codigo obtido ao campo codigo do contato
                    strcpy(c.codigo, strCodigo);
                    
                    printf("Digite o nome do contato: ");
                    scanf("%s", &c.nome);
                    printf("Digite o telefone: ");
                    scanf("%s", &c.telefone);
                    printf("Digite o logradouro: ");
                    scanf("%s", &c.logradouro);
                    printf("Digite o numero do logradouro: ");
                    scanf("%s", &c.numero);
                    printf("Digite o bairro: ");
                    scanf("%s", &c.bairro);
                    printf("Digite o CEP: ");
                    scanf("%s", &c.cep);
                    printf("Digite a cidade: ");
                    scanf("%s", &c.cidade);
                    printf("Digite a sigla do estado: ");
                    scanf("%s", &c.estado);
                    printf("Digite o pais: ");
                    scanf("%s", &c.pais);
                    
                    if(adicionar_contato(arquivoAgenda, c)) { // contato adicionado com sucesso, entao...
                        write_codigo(arquivoAuxiliar, increment(codigo)); // ... grava no arquivo auxiliar o codigo incremental para ser usado no proximo contato
                    } else {
                        printf("\n** Falha ao adicionar o contato! Procure o administrador! **\n\n");
                        return(1); // sinaliza erro
                    }

                    printf("\n-> Contato adicionado com sucesso!\n\n");
                }
                
                break;
            case 2:
                printf("\n** Remover Contato **\n\n");
                printf("Digite o codigo do contato a ser removido: ");
                scanf("%s", chave);
                
                tamanho_agenda = get_qtde_registros(arquivoAgenda, c);
                agenda = carregar_arquivo(arquivoAgenda, tamanho_agenda);
                index = buscar_contato(1, chave, agenda, tamanho_agenda); // o 1 indica que a busca eh pelo codigo do contato
                if(index == -1) {
                    printf("\n** Lamento! Contato nao encontrado! **\n\n");
                } else {
                    novaAgenda = remover_contato(agenda, tamanho_agenda, index);
                    
                    if(novaAgenda == NULL) {
                        printf("\n** Infelizmente nao foi possivel remover o contato da agenda!**\n\n");
                    } else { // contato removido da memoria, entao pode salvar o arquivo com o novo conteudo
                        if(salvar_arquivo(arquivoAgenda, novaAgenda, tamanho_agenda - 1)) { // novaAgenda tem o tamanho tamanho_agenda-1, pois foi removido um contato
                            printf("\n-> Contato removido e arquivo salvo com sucesso!\n\n");
                        } else {
                            printf("\n** Nao foi possivel salvar a agenda com o novo conteudo!**\n\n");
                        }
                    }
                }

                break;
            case 3:
                printf("\n** Visualizar Contato **\n\n");
                printf("Escolha o metodo de busca desejado\n\n");
                
                do {
                    menu_busca();
                    printf("\nEscolha uma das opcoes de busca acima: ");
                    scanf("%d", &opcaoBusca);
                    printf("\n");
                    
                    switch (opcaoBusca) {
                        case 1: // busca pelo codigo
                            opcaoValida = 1;
                            printf("Digite o codigo do contato: ");
                            scanf("%s", chave);
                            break;
                        case 2: // busca pelo nome
                            opcaoValida = 1;
                            printf("Digite o nome do contato: ");
                            scanf("%s", chave);
                            break;
                            
                        default: // aberto para outro metodo de busca no futuro
                            break;
                    }
                    
                    if(opcaoValida) { // se foi digitada uma opcao valida...
                        opcaoValida = 0; // ... entao define como sendo opcao invalida e efetua o processamento
                        tamanho_agenda = get_qtde_registros(arquivoAgenda, c);
                        agenda = carregar_arquivo(arquivoAgenda, tamanho_agenda);
                        
                        index = buscar_contato(opcaoBusca, chave, agenda, tamanho_agenda);
                        if(index == -1) { // contato nao localizado
                            printf("\n** Lamento! Contato nao encontrado! **\n\n");
                        } else { // contato encontrado, entao imprime os dados do contato
                            printf("\n-> Contato localizado: \n\n");
                            visualizar_contato(agenda, index);
                            printf("\n");
                        }
                    }
                    
                } while(opcaoBusca != 0);

                break;
            case 4:
                printf("\n** Editar Contato **\n\n");
                
                do {
                    menu_busca();
                    printf("\nEscolha uma das opcoes de busca acima: ");
                    scanf("%d", &opcaoBusca);
                    printf("\n");
                    
                    switch (opcaoBusca) {
                        case 1: // busca pelo codigo
                            opcaoValida = 1;
                            printf("Digite o codigo do contato: ");
                            scanf("%s", chave);
                            break;
                        case 2: // busca pelo nome
                            opcaoValida = 1;
                            printf("Digite o nome do contato: ");
                            scanf("%s", chave);
                            break;
                            
                        default: // aberto para outro metodo de busca no futuro
                            break;
                    }
                    
                    if(opcaoValida) { // se foi digitada uma opcao valida...
                        opcaoValida = 0; // ... entao define como sendo opcao invalida e efetua o processamento
                        tamanho_agenda = get_qtde_registros(arquivoAgenda, c);
                        agenda = carregar_arquivo(arquivoAgenda, tamanho_agenda);
                        
                        index = buscar_contato(opcaoBusca, chave, agenda, tamanho_agenda);
                        if(index == -1) { // contato nao localizado
                            printf("\n** Lamento! Contato nao encontrado! **\n\n");
                        } else { // contato encontrado, entao imprime os dados do contato e pergunta os novos dados para atualizacao
                            printf("\n-> Contato localizado: \n\n");
                            visualizar_contato(agenda, index);
                            printf("\n");
                            
                            strcpy(c.codigo, agenda[index].codigo); // mantem o codigo do registro atual no novo registro
                            printf("Digite o novo nome do contato: ");
                            scanf("%s", &c.nome);
                            printf("Digite o novo telefone: ");
                            scanf("%s", &c.telefone);
                            printf("Digite o novo logradouro: ");
                            scanf("%s", &c.logradouro);
                            printf("Digite o novo numero do logradouro: ");
                            scanf("%s", &c.numero);
                            printf("Digite o novo bairro: ");
                            scanf("%s", &c.bairro);
                            printf("Digite o novo CEP: ");
                            scanf("%s", &c.cep);
                            printf("Digite a nova cidade: ");
                            scanf("%s", &c.cidade);
                            printf("Digite a nova sigla do estado: ");
                            scanf("%s", &c.estado);
                            printf("Digite o novo pais: ");
                            scanf("%s", &c.pais);

                            agenda = editar_contato(agenda, index, c);
                            if(salvar_arquivo(arquivoAgenda, agenda, tamanho_agenda)) {
                                printf("\n-> Contato editado e arquivo salvo com sucesso!\n\n");
                            } else {
                                printf("\n** Nao foi possivel salvar a agenda com o novo conteudo!**\n\n");
                            }
                        }
                    }

                } while(opcaoBusca != 0);

                break;
            case 5:
                printf("\n** Visualizar Agenda **\n\n");
                visualizar_agenda(arquivoAgenda, c);
                break;
            case 0:
                printf("\nExecucao terminada! Obrigado por usar essa agenda!\n\n");
                return(0);
            default:
                printf("\n** Opcao invalida **!\n\n");
        }
        
        
    } while(opcao != 0);

    return 0;
}

/* implementacao das funcoes **/


// opcoes do menu principal
void menu(void) {
    printf("1 - Adicionar Contato\n");
    printf("2 - Remover Contato\n");
    printf("3 - Visualizar Contato\n");
    printf("4 - Editar Contato\n");
    printf("5 - Visualizar Agenda\n");
    printf("0 - Sair\n");
}

// opcoes do menu de busca
void menu_busca(void) {
    printf("1 - Buscar pelo codigo\n");
    printf("2 - Buscar pelo nome\n");
    printf("0 - Sair\n");
}

// Obtem do arquivo auxiliar o codigo para ser atribuido ao contato que sera gravado na agenda
int get_codigo(char *arquivoAuxiliar) {
    FILE *fp;
    int codigo;
    
    fp = fopen(arquivoAuxiliar, "r"); // abre arquivo no modo somente-leitura
    if(fp == NULL) {
        return(-1); // sinaliza erro
    }
    
    fscanf(fp, "%d\n", &codigo);
    fclose(fp);
    return(codigo);
}

// Escreve no arquivo auxiliar o codigo que sera atribuido ao proximo contato
int write_codigo(char *arquivoAuxiliar, int codigo) {
    FILE *fp;
    fp = fopen(arquivoAuxiliar, "w"); // abre arquivo para escrita
    if(fp == NULL) {
        return(-1); // sinaliza erro
    }
    
    fprintf(fp, "%d", codigo);
    fclose(fp);
    return(0); // sinaliza sucesso
}

// incrementa um codigo numerico
int increment(int codigo) {
    return(codigo + 1);
}

// verifica se um arquivo existe
int arquivo_existe(char *arquivo) {
    FILE *fp;
    fp = fopen(arquivo, "r"); // tenta abrir para leitura
    if(fp == NULL) { // nao conseguiu abrir o arquivo, entao eh provavel que o arquivo nao existe (ou, em SO Unix like, pode ser problemas de permisao de acesso)
        return(0); // sinaliza que o arquivo nao existe
    }
    
    // se chegou ate aqui eh porque o arquivo existe (pode ser acessado)
    fclose(fp);
    return(1); // sinaliza que o arquivo existe
}

// inicializa o arquivo auxiliar com o valor inicial (1) para o codigo de registro
int inicializa_auxiliar(char *arquivoAuxiliar) {
    FILE *fp;
    fp = fopen(arquivoAuxiliar, "w");
    if(fp == NULL) {
        return(1); // sinaliza erro
    }

    fputc('1', fp); // grava o codigo de registro iniciando-se em 1
    fclose(fp);
    return(0); // sinaliza sucesso
}

// obtem a quantidade de registros gravados na agenda
int get_qtde_registros(char *agenda, contato registro) {
    FILE *fp;
    int qtde = 0;

    fp = fopen(agenda, "r");
    if(fp == NULL) {
        return(-1); // sinaliza erro
    }
    
    while(fread(&registro, sizeof(contato), 1, fp) != 0) {
        qtde++;
    }
    
    fclose(fp);
    return(qtde);
}

// adiciona um contato no arquivo da agenda
int adicionar_contato(char *agenda, contato registro) {
    FILE *fp;
    fp = fopen(agenda, "a"); // abre o arquivo no modo append
    if(fp == NULL) {
        return(0); // sinaliza erro
    }
    
    // grava os dados do contato na agenda
    fwrite(&registro, sizeof(contato), 1, fp);
    fclose(fp);
    return(1); // sinaliza sucesso
}

// visualiza todos os registros da agenda
int visualizar_agenda(char *agenda, contato registro) {
    FILE *fp;
    fp = fopen(agenda, "r");
    if(fp == NULL) {
        printf("\n** Erro ao abrir arquivo da agenda para visualizacao! **\n\n");
        return(0); // sinaliza erro
    }
    
    while(fread(&registro, sizeof(contato), 1, fp) != 0) {
        printf("-----------\n");
        printf("Codigo: %s\n", registro.codigo);
        printf("Nome: %s\n", registro.nome);
        printf("Telefone: %s\n", registro.telefone);
        printf("Logradouro: %s\n", registro.logradouro);
        printf("Numero: %s\n", registro.numero);
        printf("Bairro: %s\n", registro.bairro);
        printf("CEP: %s\n", registro.cep);
        printf("Cidade: %s\n", registro.cidade);
        printf("Estado: %s\n", registro.estado);
        printf("Pais: %s\n", registro.pais);
    }
    
    printf("-----------\n\n");
    fclose(fp);
    return(1); // sinaliza sucesso
}

// le qtde_registros do tipo contato do arquivo da agenda e retorna um ponteiro para o array com esses registros
contato* carregar_arquivo(char *agenda, int qtde_registros) {
    FILE *fp;
    int i;
    contato *array_agenda;
    
    fp = fopen(agenda, "r");
    if(fp == NULL) {
        return(NULL); // sinaliza erro
    }
    
    if((array_agenda = (contato *)calloc(qtde_registros, sizeof(contato))) == NULL) { // alocacao dinamica para evitar desperdicio de memoria
        printf("\n** Erro ao alocar memoria! **\n");
        return(NULL); // sinaliza erro;
    }
    
    // se chegou aqui, entao tudo ok

    for(i = 0; i<qtde_registros; i++) {
        fread(&array_agenda[i], sizeof(contato), 1, fp);
    }
    
    fclose(fp);
    return(array_agenda);
}

// dada uma opcao de busca, uma chave e um vetor de contatos, retorna (se houver) a posicao/index no vetor onde o contato foi encontrado. -1 indica que o contato nao foi localizado
int buscar_contato(int opcao, char *chave, contato *agenda, int tamanho_agenda) {
    int i, index = -1;
    switch (opcao) {
        case 1: // busca pelo codigo do contato
            for(i=0; i<tamanho_agenda; i++) {
                if(strcmp(chave, agenda[i].codigo) == 0) {
                    index = i; // armazena a posicao encontrada na variavel index
                    break; // registro encontrado, entao ja deixa o loop
                }
            }
            break;
        case 2: // busca pelo nome do contato
            for(i=0; i<tamanho_agenda; i++) {
                if(strcmp(chave, agenda[i].nome) == 0) {
                    index = i;
                    break;
                }
            }
            break;
        default: // metodo de busca ainda nao implementado, entao considera que o contato nao foi localizado
            index = -1;
    }

    return(index);
}

// imprime na tela os dados de um contato armazenado no vetor agenda na posicao index
void visualizar_contato(contato *agenda, int index) {
    printf("-----------\n");
    printf("Codigo: %s\n", agenda[index].codigo);
    printf("Nome: %s\n", agenda[index].nome);
    printf("Telefone: %s\n", agenda[index].telefone);
    printf("Logradouro: %s\n", agenda[index].logradouro);
    printf("Numero: %s\n", agenda[index].numero);
    printf("Bairro: %s\n", agenda[index].bairro);
    printf("CEP: %s\n", agenda[index].cep);
    printf("Cidade: %s\n", agenda[index].cidade);
    printf("Estado: %s\n", agenda[index].estado);
    printf("Pais: %s\n", agenda[index].pais);
    printf("-----------\n");
}

// remove do vetor agenda de tamanho tamanho_agenda um (1) contato que esta na posicao index. A funcao retorna um ponteiro para a novo array sem o contato desejado
contato* remover_contato(contato *agenda, int tamanho_agenda, int index) {
    contato *novaAgenda;
    int i, j = 1;
    
    if((novaAgenda = (contato *)calloc(tamanho_agenda-1, sizeof(contato))) == NULL) { // aloca espaco para o vetor sendo tamanho menor que o espaco do vetor de agenda atual
        printf("\n** Erro ao alocar memoria! **\n");
        return(NULL); // sinaliza erro
    }
    
    // se chegou aqui, entao tudo ok

    for (i=0; i<tamanho_agenda; i++) {
        if(i == index) {
            continue;
        }
        novaAgenda[j-1] = agenda[i];
        j++;
    }

    return(novaAgenda);
}

// salva no arquivo agenda o conteudo dos contatos mantidos em um array
int salvar_arquivo(char *agenda, contato *array_agenda, int tamanho_agenda) {
    FILE *fp;
    int i;
    
    fp = fopen(agenda, "w"); // abre o arquivo de agenda no modo escrita. Sobrescreve entao o arquivo da agenda
    if(fp == NULL) {
        return(0); // sinaliza erro
    }
    
    // arquivo aberto, entao o conteudo pode ser gravado
    
    for(i = 0; i<tamanho_agenda; i++) {
        fwrite(&array_agenda[i], sizeof(contato), 1, fp);
    }
    
    fclose(fp);
    return(1); // sinaliza sucesso
}

// atribui novoContato em uma posicao do array array_agenda
contato * editar_contato(contato *array_agenda, int posicao, contato novoContato) {
    array_agenda[posicao] = novoContato;
    return(array_agenda);
}
