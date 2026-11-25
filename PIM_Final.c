/*

O programa guarda até 50 agentes de IA (nome, prompt, categoria,
tags e nota) em vetores paralelos e grava tudo em um arquivo de
texto, para que os dados continuem existindo quando o programa
for fechado e aberto de novo. */

#include <stdio.h>    // printf, fgets, fopen, fprintf, sscanf...
#include <stdlib.h>   // exit, system
#include <string.h>   // strcpy, strcat, strcmp, strstr, strchr, strcspn
#include <ctype.h>    // tolower

/* Cada TAM_ já conta 1 posição extra para o '\0' que termina a string. */
#define MAX_AGENTES    50
#define TAM_NOME       81    // 80 caracteres + '\0'
#define TAM_PROMPT     1001  // 1000 caracteres + '\0'
#define TAM_CATEGORIA  31
#define TAM_TAGS       105   // 5 tags de 20 caracteres + 4 vírgulas + '\0'
#define QTD_CATEGORIAS 7
#define ARQUIVO        "modus_dados.txt"

// Lista fixa de categorias: o usuário escolhe pelo número (1 a 7)
const char *CATEGORIAS[QTD_CATEGORIAS] = {
    "Estudos", "Culinária", "Filmes e Séries", "Notícias",
    "Trabalho", "Organização", "Outros"
};

/* Vetores paralelos: a posição p de todos eles descreve o MESMO agente.
Ex.: id[3], nome[3], promptIA[3] são os dados do agente da posição 3. */
int  id[MAX_AGENTES];
int  ativo[MAX_AGENTES];      // 1 = ativo, 0 = excluído
char nome[MAX_AGENTES][TAM_NOME];            // matriz de char: cada linha é um nome
char promptIA[MAX_AGENTES][TAM_PROMPT];      // cada linha é um prompt
char categoria[MAX_AGENTES][TAM_CATEGORIA];  // cada linha é uma categoria
char tags[MAX_AGENTES][TAM_TAGS];            // tags separadas por vírgula
int  qtdTags[MAX_AGENTES];
int  nota[MAX_AGENTES];

int total = 0;      // posições já usadas nos vetores
int proximoId = 1;  // próximo ID a ser usado (só aumenta)

// Protótipos.
void limparBuffer(void);
void lerTexto(char *dest, int tam);
int  lerInteiro(void);
int  contemParcial(char *texto, char *termo);
int  nomeJaUsado(char *n, int ignorar);
int  contarAtivos(void);
int  buscarPorId(int idProcurado);
void mostrarAgente(int p);
void lerCampo(int p, int campo);
void cadastrarAgente(void);
void listarAgentes(void);
void buscarAgente(void);
void acoesAgente(int pos);
void editarCampos(int pos);
void editarAgente(void);
void confirmarExclusao(int pos);
void excluirAgente(void);
void relatorioPorCategoria(void);
void salvarDados(void);
void carregarDados(void);

/* Mostra o menu em laço até o usuário escolher 7 (Sair).
Antes do menu, carrega os dados salvos no arquivo (se existir). */
int main(void) {
    int opcao;

#ifdef _WIN32
    system("chcp 65001 > nul"); // acentos no console do Windows
#endif

    printf("==================== MODUS ====================\n");
    printf("      Seu Catálogo Pessoal de Agentes de IA\n");
    carregarDados();

    do {
        printf("\n---------------- MENU ----------------\n");
        printf("Agentes ativos: %d de %d\n", contarAtivos(), MAX_AGENTES);
        printf("1. Cadastrar agente\n");
        printf("2. Listar agentes\n");
        printf("3. Buscar agente\n");
        printf("4. Editar agente\n");
        printf("5. Excluir agente\n");
        printf("6. Relatório por categoria\n");
        printf("7. Sair\n");
        printf("Opção: ");
        opcao = lerInteiro();

        switch (opcao) {
        case 1: cadastrarAgente();       break;
        case 2: listarAgentes();         break;
        case 3: buscarAgente();          break;
        case 4: editarAgente();          break;
        case 5: excluirAgente();         break;
        case 6: relatorioPorCategoria(); break;
        case 7: printf("\nEncerrando o MODUS. Até a próxima!\n"); break;
        default: printf("Opção inválida.\n");
        }
    } while (opcao != 7);

    return 0;
}

// Definições das Funções.
// Descarta tudo o que sobrou no teclado até o ENTER (Limpano Buffer).
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Lê uma linha com fgets e tira o ENTER. Se a linha passar do limite, descarta o resto.
void lerTexto(char *dest, int tam) {
    if (fgets(dest, tam, stdin) == NULL) {
        exit(0); // acabou a entrada
    }
    if (strchr(dest, '\n') == NULL) {
        limparBuffer(); // não coube tudo: o ENTER ficou no teclado, então descarta o resto
    }
    dest[strcspn(dest, "\r\n")] = '\0'; // troca o ENTER (\n ou \r\n) por fim de string
}

// Lê uma linha e converte com sscanf. Retorna o número ou -1 se não for número.
int lerInteiro(void) {
    char linha[20];
    char lixo;
    int n;

    lerTexto(linha, 20);
    // %d lê o número; %c tenta ler algo depois dele. Só é válido se leu exatamente 1 item
    if (sscanf(linha, "%d %c", &n, &lixo) != 1) {
        printf("Digite apenas números.\n");
        return -1;
    }
    return n;
}

/* Procura termo dentro de texto sem diferenciar maiúsculas e minúsculas.
Copia os dois para minúsculas em a e b e usa strstr. */
int contemParcial(char *texto, char *termo) {
    char a[TAM_PROMPT], b[TAM_PROMPT];
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        a[i] = tolower((unsigned char)texto[i]);
    }
    a[i] = '\0';
    for (i = 0; termo[i] != '\0'; i++) {
        b[i] = tolower((unsigned char)termo[i]);
    }
    b[i] = '\0';
    return strstr(a, b) != NULL; // strstr devolve NULL se o trecho não existe
}

/* Retorna 1 se já existe outro agente ativo com esse nome.
"ignorar" é a posição do próprio agente (na edição ele pode manter o nome). */
int nomeJaUsado(char *n, int ignorar) {
    int i;
    for (i = 0; i < total; i++) {
        if (ativo[i] == 1 && i != ignorar && strcmp(nome[i], n) == 0) {
            return 1;
        }
    }
    return 0;
}

//Conta quantos agentes estão ativos (os excluídos não entram).
int contarAtivos(void) {
    int i, cont = 0;
    for (i = 0; i < total; i++) {
        if (ativo[i] == 1) {
            cont++;
        }
    }
    return cont;
}

// Retorna a posição do agente ativo com esse ID, ou -1 se não achar.
int buscarPorId(int idProcurado) {
    int i;
    for (i = 0; i < total; i++) {
        if (ativo[i] == 1 && id[i] == idProcurado) {
            return i;
        }
    }
    return -1;
}

// Mostra a ficha completa do agente da posição p.
void mostrarAgente(int p) {
    printf("\nID: %d\n", id[p]);
    printf("Nome: %s\n", nome[p]);
    printf("Prompt: %s\n", promptIA[p]);
    printf("Categoria: %s\n", categoria[p]);
    if (qtdTags[p] > 0) {
        printf("Tags (%d): %s\n", qtdTags[p], tags[p]);
    } else {
        printf("Tags: (sem tags)\n");
    }
    printf("Nota: %d/5\n", nota[p]);
}

/* Lê um campo do agente da posição p: 1 nome, 2 prompt, 3 categoria, 4 tags, 5 nota.
É usada no cadastro e na edição, para não repetir as validações. */
void lerCampo(int p, int campo) {
    int i, op;
    char tag[21]; // 20 caracteres + '\0'

    if (campo == 1) {
        // Nome: obrigatório e único entre os agentes ativos
        do {
            printf("\nNome do agente (até 80 caracteres): ");
            lerTexto(nome[p], TAM_NOME);
            if (nome[p][0] == '\0') {
                printf("O nome é obrigatório.\n");
            } else if (nomeJaUsado(nome[p], p)) {
                printf("Já existe um agente com esse nome.\n");
                nome[p][0] = '\0'; // zera para o laço repetir a pergunta
            }
        } while (nome[p][0] == '\0');
    } else if (campo == 2) {
        // Prompt: obrigatório
        do {
            printf("\nPrompt do agente (até 1000 caracteres): ");
            lerTexto(promptIA[p], TAM_PROMPT);
            if (promptIA[p][0] == '\0') {
                printf("O prompt é obrigatório.\n");
            }
        } while (promptIA[p][0] == '\0');
    } else if (campo == 3) {
        // Categoria: escolhida pelo número na lista CATEGORIAS
        printf("\nCategorias:\n");
        for (i = 0; i < QTD_CATEGORIAS; i++) {
            printf("%d. %s\n", i + 1, CATEGORIAS[i]);
        }
        do {
            printf("Escolha a categoria: ");
            op = lerInteiro();
        } while (op < 1 || op > QTD_CATEGORIAS);
        strcpy(categoria[p], CATEGORIAS[op - 1]); // op - 1 porque o vetor começa em 0
    } else if (campo == 4) {
        // Tags: de 0 a 5, guardadas numa única string separada por vírgulas
        do {
            printf("\nQuantas tags (0 a 5)? ");
            qtdTags[p] = lerInteiro();
        } while (qtdTags[p] < 0 || qtdTags[p] > 5);
        tags[p][0] = '\0'; // começa vazia (descarta as tags antigas, se houver)
        for (i = 0; i < qtdTags[p]; i++) {
            printf("Tag %d (até 20 caracteres): ", i + 1);
            lerTexto(tag, 21);
            if (i > 0) {
                strcat(tags[p], ","); // vírgula só entre as tags
            }
            strcat(tags[p], tag);
        }
    } else {
        // Nota: de 0 a 5
        do {
            printf("\nNota do agente (0 a 5): ");
            nota[p] = lerInteiro();
            if (nota[p] < 0 || nota[p] > 5) {
                printf("A nota deve ser de 0 a 5.\n");
            }
        } while (nota[p] < 0 || nota[p] > 5);
    }
}

/* Cadastra um agente novo. Reaproveita a vaga de um agente excluído, se houver.
Mostra um preview e só grava de verdade se o usuário confirmar. */
void cadastrarAgente(void) {
    int i, p = -1, op;

    /* Cópia de segurança dos dados da vaga reaproveitada (agente excluído).
       Os campos são lidos direto nos vetores, então, se o usuário cancelar,
       é preciso devolver o que estava lá. */
    int  bkId, bkQtdTags, bkNota;
    char bkNome[TAM_NOME];
    char bkPrompt[TAM_PROMPT];
    char bkCategoria[TAM_CATEGORIA];
    char bkTags[TAM_TAGS];

    if (contarAtivos() >= MAX_AGENTES) {
        printf("\nLimite de %d agentes atingido! Exclua um agente para liberar vaga.\n", MAX_AGENTES);
        return;
    }

    // procura a vaga de um agente excluído; se não tiver, usa a próxima posição livre
    for (i = 0; i < total && p == -1; i++) {
        if (ativo[i] == 0) {
            p = i;
        }
    }
    if (p == -1) {
        p = total;
    }

    // p < total significa vaga de agente excluído: guarda uma cópia antes de sobrescrever
    if (p < total) {
        bkId        = id[p];
        bkQtdTags   = qtdTags[p];
        bkNota      = nota[p];
        strcpy(bkNome,      nome[p]);
        strcpy(bkPrompt,    promptIA[p]);
        strcpy(bkCategoria, categoria[p]);
        strcpy(bkTags,      tags[p]);
    }

    printf("\n--- CADASTRO DE AGENTE ---\n");
    for (i = 1; i <= 5; i++) {
        lerCampo(p, i);
    }

    // preview: o ID só é gasto se o usuário confirmar
    id[p] = proximoId;
    printf("\n--- PREVIEW ---");
    mostrarAgente(p);
    do {
        printf("\nSalvar este agente? (1 = Sim, 0 = Não): ");
        op = lerInteiro();
    } while (op != 0 && op != 1);

    if (op == 0) {
        // cancelou: se a vaga era de um agente excluído, devolve os dados que estavam nela
        if (p < total) {
            id[p]      = bkId;
            qtdTags[p] = bkQtdTags;
            nota[p]    = bkNota;
            strcpy(nome[p],      bkNome);
            strcpy(promptIA[p],  bkPrompt);
            strcpy(categoria[p], bkCategoria);
            strcpy(tags[p],      bkTags);
        }
        printf("Cadastro cancelado.\n");
        return;
    }

    proximoId++;
    ativo[p] = 1;
    if (p == total) {
        total++; // só aumenta quando usou uma posição nova (não reaproveitada)
    }
    salvarDados();
    printf("Agente cadastrado com sucesso! ID: %d\n", id[p]);
}

// Lista todos os agentes ativos em forma de tabela resumida.
void listarAgentes(void) {
    int i;

    if (contarAtivos() == 0) {
        printf("\nNenhum agente cadastrado ainda.\n");
        return;
    }
    printf("\n%-5s %-30s %-6s %s\n", "ID", "Nome", "Nota", "Categoria");
    for (i = 0; i < total; i++) {
        if (ativo[i] == 1) {
            printf("%-5d %-30s %d/5    %s\n", id[i], nome[i], nota[i], categoria[i]);
        }
    }
}

/* Busca por ID, nome, categoria ou tag.
Depois de achar, o usuário abre a ficha e pode editar ou excluir o agente. */
void buscarAgente(void) {
    int i, op, pos, idEscolhido, achou = 0;
    char termo[TAM_NOME];

    if (contarAtivos() == 0) {
        printf("\nNenhum agente cadastrado para buscar.\n");
        return;
    }

    printf("\nBuscar por: 1. ID  2. Nome  3. Categoria  4. Tag  0. Voltar\n> ");
    op = lerInteiro();

    if (op == 0) {
        return; // voltar ao menu principal
    }

    if (op == 1) {
        // busca exata por ID: achou, abre a ficha com o menu de ações
        printf("Digite o ID: ");
        pos = buscarPorId(lerInteiro());
        if (pos == -1) {
            printf("Agente não encontrado.\n");
        } else {
            acoesAgente(pos);
        }
    } else if (op >= 2 && op <= 4) {
        // busca parcial (sem diferenciar maiúsculas): lista todos que combinam
        printf("Digite o texto da busca: ");
        lerTexto(termo, TAM_NOME);
        for (i = 0; i < total; i++) {
            if (ativo[i] == 0) {
                continue; // pula os excluídos
            }
            if ((op == 2 && contemParcial(nome[i], termo)) ||
                (op == 3 && contemParcial(categoria[i], termo)) ||
                (op == 4 && contemParcial(tags[i], termo))) {
                if (achou == 0) { // cabeçalho só antes do primeiro resultado
                    printf("\n%-5s %-30s %-6s %s\n", "ID", "Nome", "Nota", "Categoria");
                }
                printf("%-5d %-30s %d/5    %s\n", id[i], nome[i], nota[i], categoria[i]);
                achou = 1;
            }
        }
        if (achou == 0) {
            printf("Nenhum agente encontrado.\n");
        } else {
            // pede o ID de um dos resultados para abrir a ficha (0 volta ao menu)
            printf("\nDigite o ID para abrir o agente (0 para voltar): ");
            idEscolhido = lerInteiro();
            if (idEscolhido != 0) {
                pos = buscarPorId(idEscolhido);
                if (pos == -1) {
                    printf("Agente não encontrado.\n");
                } else {
                    acoesAgente(pos);
                }
            }
        }
    } else {
        printf("Opção inválida.\n");
    }
}

/* Ficha do agente da posição pos + menu de ações (Editar / Excluir / Voltar).
Mostra os dados uma única vez e repete só o menu. Se o agente for excluído,
o menu se encerra sozinho, pois não há mais ficha para mexer. */
void acoesAgente(int pos) {
    int op;

    mostrarAgente(pos); // a ficha aparece uma vez só
    do {
        printf("\n1. Editar  2. Excluir  0. Voltar ao menu\n> ");
        op = lerInteiro();

        if (op == 1) {
            editarCampos(pos);
        } else if (op == 2) {
            confirmarExclusao(pos);
            if (ativo[pos] == 0) {
                op = 0; // foi excluído: sai do menu
            }
        } else if (op != 0) {
            printf("Opção inválida.\n");
        }
    } while (op != 0);
}

/* Laço de edição campo a campo do agente da posição pos.
Cada campo alterado já é salvo no arquivo e a ficha atualizada é mostrada. */
void editarCampos(int pos) {
    int op;

    do {
        printf("\nEditar: 1. Nome  2. Prompt  3. Categoria  4. Tags  5. Nota  0. Voltar\n> ");
        op = lerInteiro();
        if (op >= 1 && op <= 5) {
            lerCampo(pos, op);
            salvarDados();
            printf("Campo atualizado!\n");
            mostrarAgente(pos); // mostra a ficha já com a alteração
        }
    } while (op != 0);
}

// Opção 4 do menu principal: escolhe o agente pelo ID e entra na edição.
void editarAgente(void) {
    int pos;

    if (contarAtivos() == 0) {
        printf("\nNenhum agente cadastrado para editar.\n");
        return;
    }

    listarAgentes();
    printf("\nDigite o ID do agente que deseja editar: ");
    pos = buscarPorId(lerInteiro());
    if (pos == -1) {
        printf("Agente não encontrado.\n");
        return;
    }

    mostrarAgente(pos); // mostra a ficha antes de escolher o campo
    editarCampos(pos);
}

// Pede confirmação e, se o usuário disser sim, exclui o agente da posição pos.
void confirmarExclusao(int pos) {
    int op;

    printf("Excluir \"%s\"? (1 = Sim, 0 = Não): ", nome[pos]);
    op = lerInteiro();
    if (op == 1) {
        ativo[pos] = 0; // exclusão lógica: nada é deslocado nos vetores
        salvarDados();
        printf("Agente excluído. O ID %d não será usado de novo.\n", id[pos]);
    } else {
        printf("Exclusão cancelada.\n");
    }
}

// Opção 5 do menu principal: escolhe o agente pelo ID e pede a confirmação.
void excluirAgente(void) {
    int pos;

    if (contarAtivos() == 0) {
        printf("\nNenhum agente cadastrado para excluir.\n");
        return;
    }

    listarAgentes();
    printf("\nDigite o ID do agente que deseja excluir: ");
    pos = buscarPorId(lerInteiro());
    if (pos == -1) {
        printf("Agente não encontrado.\n");
        return;
    }

    confirmarExclusao(pos);
}

// Para cada categoria, mostra quantos agentes ativos existem e a nota média.
void relatorioPorCategoria(void) {
    int i, j, qtd, soma;

    if (contarAtivos() == 0) {
        printf("\nNenhum agente cadastrado para o relatório.\n");
        return;
    }

    printf("\n%-20s %-12s %s\n", "Categoria", "Quantidade", "Nota média");
    for (i = 0; i < QTD_CATEGORIAS; i++) {   // laço de fora: cada categoria
        qtd = 0;
        soma = 0;
        for (j = 0; j < total; j++) {        // laço de dentro: todos os agentes
            if (ativo[j] == 1 && strcmp(categoria[j], CATEGORIAS[i]) == 0) {
                qtd++;
                soma = soma + nota[j];
            }
        }
        if (qtd > 0) {
            printf("%-20s %-12d %.1f\n", CATEGORIAS[i], qtd, (float)soma / qtd);
        } else {
            printf("%-20s %-12d -\n", CATEGORIAS[i], qtd); // evita dividir por zero
        }
    }
}

/* Formato: proximoId, total e, para cada posição, uma informação por linha:
id, ativo, nome, prompt, categoria, tags, qtdTags, nota */
void salvarDados(void) {
    int i;
    FILE *arq = fopen(ARQUIVO, "w"); // "w" apaga o conteúdo antigo e grava tudo de novo

    if (arq == NULL) {
        printf("ATENÇÃO: não foi possível salvar o arquivo %s!\n", ARQUIVO);
        return;
    }
    fprintf(arq, "%d\n%d\n", proximoId, total);
    for (i = 0; i < total; i++) {
        fprintf(arq, "%d\n%d\n%s\n%s\n%s\n%s\n%d\n%d\n",
                id[i], ativo[i], nome[i], promptIA[i], categoria[i], tags[i], qtdTags[i], nota[i]);
    }
    fclose(arq);
}

// Lê o arquivo na mesma ordem em que salvarDados gravou.
void carregarDados(void) {
    int i;
    char linha[TAM_PROMPT + 2]; // cabe o maior campo (prompt) + '\n' + '\0'
    FILE *arq = fopen(ARQUIVO, "r");

    if (arq == NULL) {
        printf("\nNenhum arquivo de dados encontrado. O catálogo começa vazio.\n");
        return; // o arquivo será criado no primeiro salvamento
    }

    fgets(linha, sizeof(linha), arq);
    sscanf(linha, "%d", &proximoId);
    fgets(linha, sizeof(linha), arq);
    sscanf(linha, "%d", &total);
    if (total < 0 || total > MAX_AGENTES) {
        total = 0; // valor absurdo no arquivo: ignora para não estourar os vetores
    }

    for (i = 0; i < total; i++) {
        fgets(linha, sizeof(linha), arq);
        sscanf(linha, "%d", &id[i]);
        fgets(linha, sizeof(linha), arq);
        sscanf(linha, "%d", &ativo[i]);

        // os textos são lidos na linha e o ENTER do fim é tirado antes de copiar
        fgets(linha, sizeof(linha), arq);
        linha[strcspn(linha, "\r\n")] = '\0';
        strcpy(nome[i], linha);
        fgets(linha, sizeof(linha), arq);
        linha[strcspn(linha, "\r\n")] = '\0';
        strcpy(promptIA[i], linha);
        fgets(linha, sizeof(linha), arq);
        linha[strcspn(linha, "\r\n")] = '\0';
        strcpy(categoria[i], linha);
        fgets(linha, sizeof(linha), arq);
        linha[strcspn(linha, "\r\n")] = '\0';
        strcpy(tags[i], linha);

        fgets(linha, sizeof(linha), arq);
        sscanf(linha, "%d", &qtdTags[i]);
        fgets(linha, sizeof(linha), arq);
        sscanf(linha, "%d", &nota[i]);
    }
    fclose(arq);
    printf("\n%d registro(s) recuperado(s) do arquivo %s.\n", total, ARQUIVO);
}
