#include <ctype.h>   //— biblioteca para conversao e classificacao de caracteres (ex.: tolower)
#include <stdio.h>   //— printf, scanf, fgets, getchar, fopen, fprintf, fscanf (entrada e saida de dados)
#include <stdlib.h>  //— biblioteca de funcoes utilitarias gerais
#include <string.h>  //— strcmp, strcpy, strlen, strcspn, strtok, strstr (manipulacao de textos e busca de agentes)

#define TAM_CARAC 81 // SOMENTO 80 CARACTERES POR AGENTE!!!!!!!!!!!!!!!!
#define MAX_AGENTES 50 // MAXIMO DE 50 AGENTES EIN, NÃO PASSA DISSO!!!!!!!!
#define CARACTERES 501 // MAXIMO DE 500 CARACTERES!!!!

void limparBuffer(); // Funcao criada para limpeza do buffer!

void cadastro(char nome[MAX_AGENTES][TAM_CARAC], char promptIA[MAX_AGENTES][CARACTERES], char categoriaIA[MAX_AGENTES][CARACTERES], char tag_agente[MAX_AGENTES][CARACTERES], int nota[], int *total); // Vetor com Maximo de agentes e tamanho do nome, sendo (char nome[MAX][TAM], int *total) a prototipagem!
// como a funcao criada e um void, ela nao retorna nada! por isso usamos o ponteiro "*" na variavel de tipo Inteira, para que o ponteiro seja usado para alterar o conteudo da variavel!

void listar(char nome[MAX_AGENTES][TAM_CARAC], char promptIA[MAX_AGENTES][CARACTERES], char categoriaIA[MAX_AGENTES][CARACTERES], char tag_agente[MAX_AGENTES][CARACTERES], int nota[], int total);

void buscar(char nome[MAX_AGENTES][TAM_CARAC], char promptIA[MAX_AGENTES][CARACTERES], char categoriaIA[MAX_AGENTES][CARACTERES], char tag_agente[MAX_AGENTES][CARACTERES],  int nota[], int total);

void voltarmenu();


int main() {

   int opcao; // opcoes do Menu
   int total = 0; // usado para marcar o numero de agentes. Salva o total de agentes.
   int nota[MAX_AGENTES]; // nota da avaliacao de cada agente. Salva a avaliacao do agente.
   char nome[MAX_AGENTES][TAM_CARAC]; // matriz com 2 dimensoes, max = maximo de agentes e tam = tamanho dos caracteres. Salva o nome do agente.
   char promptIA[MAX_AGENTES][CARACTERES]; // Maximo de 50 prompts, e com 500 caracteres cada descricao. Salva o Prompt do agente.
   char categoriaIA[MAX_AGENTES][CARACTERES]; // Variavel para salvar a categoria do agente.
   char usuario[TAM_CARAC]; // nome do usuario que está acessando o MODUS.
   char tag_agente[MAX_AGENTES][CARACTERES]; // TAG para que possamos buscar por palavras chaves.

    printf("\n\n");
    printf("==================================================\n");
    printf("                       MODUS\n");
    printf("          Seu Catálogo Pessoal de Agentes de IA\n");
    printf("==================================================\n");

    printf("\nBem-vindo ao MODUS!\n");
    printf("Organize e gerencie seus agentes de Inteligência Artificial.\n");

    printf("\nPor favor, nos diga como podemos te chamar?:\n> ");
    if (fgets(usuario, TAM_CARAC, stdin) != NULL) { // fgets serve como um scanf, so que para caracteres, guarda na variavel NOME com TAM maximo de 80 Char!
    usuario[strcspn(usuario, "\n")] = '\0';
    }

    printf("\nTenha uma boa experiência com nosso Catálogo, %s!\n ", usuario);

    do { // o "do" forca a rodar esse codigo pelo menos uma vez! o codigo vai se repetir enquanto (while) a opcao nao for 6

    printf("\nSelecione uma opção:\n\n");

    printf("  1. Cadastrar agente\n");
    printf("  2. Listar agentes\n");
    printf("  3. Buscar agente\n");
    printf("  4. Editar agente\n");
    printf("  5. Excluir agente\n");
    printf("  6. Sair\n");

    printf("\n--------------------------------------------------\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    limparBuffer(); // ele limpa o buffer e passa dessa pra melhor o contrabarra N (famoso ENTER).

    switch (opcao) { // funcao usada para fazer um menu. "switch-case"
    case 1:
      cadastro(nome, promptIA, categoriaIA, tag_agente, nota, &total);
      break;
    case 2:
      listar(nome, promptIA, categoriaIA, tag_agente, nota, total);
      break;
    case 3:
      buscar(nome, promptIA, categoriaIA, tag_agente, nota, total);
      break;
    case 4:
      printf("\nEdição de agentes - funcionalidade em desenvolvimento.\n\n");
      break;
    case 5:
      printf("\nExclusão de agentes - funcionalidade em desenvolvimento.\n\n");
      break;
    case 6:
      printf("\nEncerrando o MODUS. Obrigado por utilizar nossa ferramenta! Até a próxima!\n\n");
      break;
    }
  } while (opcao != 6);

  return 0;
}



void limparBuffer() { // Funcao para limpeza de buffer!
  int c;
  while ((c = getchar()) != '\n' && c != EOF); // aqui utilizamos um while para consumir os caracteres restantes ate encontrar o ENTER ou o final do arquivo!
  return;
}



void voltarmenu() { // essa funcao foi criada para que tenha uma confirmacao para voltar pro menu!
    int continuar = 0; // essa variavel vai servir para confirmar a volta ao menu!

  do { // essa parte da funcao cria um looping para que, depois que armazenamos um dado, ele continue rodando!
        printf("\nDigite 1 para retornar ao menu principal:\n> ");
    if (scanf("%d", &continuar) != 1) {
            printf("\nEntrada inválida. Digite o número 1 para continuar.\n");
      limparBuffer();
      continuar = -1; // forca a repetir
      continue;
    }
    limparBuffer();

    if (continuar != 1) {
            printf("\nOpção inválida. Digite 1 para retornar ao menu principal.\n");
    }
  } while (continuar != 1);
}



void cadastro(char nome[MAX_AGENTES][TAM_CARAC], char promptIA[MAX_AGENTES][CARACTERES], char categoriaIA[MAX_AGENTES][CARACTERES], char tag_agente[MAX_AGENTES][CARACTERES], int nota[], int *total) {

    if (*total >= MAX_AGENTES) { // aqui ele simplesmente fala, se o total for IGUAL ou maior que o Maximo, que definido la no comeco do codigo (no caso 50), ele vai avisar que foi atingido!
        printf("\nLimite de 50 agentes atingido!\n");
        printf("Para cadastrar um novo agente, exclua um agente existente pela opção 5 do menu.\n\n");
        return;
  }

  printf("\nCADASTRO DE AGENTE\n");
  printf("Informe um nome para identificar seu agente de IA.\n");
  printf("Nome do agente:\n> ");
  if (fgets(nome[*total], TAM_CARAC, stdin) != NULL) { // fgets serve como um scanf, so que para caracteres, guarda na variavel NOME com TAM maximo de 80 Char!
    nome[*total][strcspn(nome[*total], "\n")] = '\0'; // essa definicao tira o contrabarra N do buffer, e aplica o contrabarra O para entender que acabou a linha.
  }

  printf("\nDescreva as instruções e a finalidade do seu agente.\n");
  printf("Prompt do agente:\n> ");
  if (fgets(promptIA[*total], CARACTERES, stdin) != NULL) {
    promptIA[*total][strcspn(promptIA[*total], "\n")] = '\0';
  }

  printf("\nDescreve em qual categoria seu agente se encaixa.\n"); // essa definicao é para que o usuario crie uma Categoria que se encaixe com o prompt do agente.
  printf("Categoria do agente:\n> ");
  if (fgets(categoriaIA[*total], CARACTERES, stdin) != NULL) {
    categoriaIA[*total][strcspn(categoriaIA[*total], "\n")] = '\0';
  }

  printf("\nDescreva palavras chaves para seu agente (TAG).\n"); // essa definicao é para que o usuario crie uma TAG que se encaixe com a Categoria do agente.
  printf("TAG(s) do agente:\n> ");
  if (fgets(tag_agente[*total], CARACTERES, stdin) != NULL) {
    tag_agente[*total][strcspn(tag_agente[*total], "\n")] = '\0';
  }

  do { // essa parte da funcao cria um looping para que, depois que armazenamos um dado, ele continue rodando!
    printf("\nAtribua uma avaliação ao seu agente (0 a 5):\n> ");
    if (scanf("%d", &nota[*total]) != 1) {
      printf("\nEntrada inválida. Informe um número inteiro entre 0 e 5.\n");
      limparBuffer();
      nota[*total] = -1; // forca a repetir
      continue;
    }
    limparBuffer();

    if (nota[*total] < 0 || nota[*total] > 5) {
      printf("\nAvaliação inválida. A nota deve estar entre 0 e 5.\n");
    }
  } while (nota[*total] < 0 || nota[*total] > 5);

  (*total)++; // aqui incrementamos o total de agentes cadastrados, ou seja, adicionamos mais um agente ao nosso catalogo!

  printf("\nAgente cadastrado com sucesso!\n");
  printf("Total de agentes cadastrados: %d\n", *total);

  // essa parte vai mostrar uma previa do agente que acabou de ser cadastrado, apresentando todas as informacoes que foram armazenadas nos vetores!
  printf("\n==================================================\n");
  printf("                PREVIEW DO AGENTE\n");
  printf("==================================================\n");

  // como o total ja foi incrementado, ele representa o numero de identificacao do agente que acabamos de cadastrar!
  printf("\nNúmero de identificação: #%d\n", *total);

  // utilizamos *total - 1 para acessar a ultima posicao preenchida dos vetores, pois a primeira posicao dos vetores comeca em 0!
  printf("Nome do Agente: %s\n", nome[*total - 1]);
  printf("Prompt e/ou funcionalidade: %s\n", promptIA[*total - 1]);
  printf("Categoria do Agente: %s\n", categoriaIA[*total - 1]);
  printf("TAG'S do Agente: %s\n", tag_agente[*total - 1]);
  printf("Avaliação do Agente: %d/5\n", nota[*total - 1]);

  printf("\n==================================================\n");

  voltarmenu();
}



void listar(char nome[MAX_AGENTES][TAM_CARAC], char promptIA[MAX_AGENTES][CARACTERES], char categoriaIA[MAX_AGENTES][CARACTERES], char tag_agente[MAX_AGENTES][CARACTERES], int nota[], int total) { // essa funcao vai chamar todos os agentes cadastrados (total nao e ponteiro pois nao vai alterar nada!)

  if (total == 0) { // caso nenhum agente for cadastrado (total for igual = 0), ele printa o que esta abaixo.
    printf("\nO catálogo ainda não possui agentes cadastrados.\n");
    printf("Utilize a opção 1 do menu para cadastrar seu primeiro agente.\n\n");
    return;
  }

  printf("\n================ MEUS AGENTES ================\n");
  printf("Total de agentes cadastrados: %d\n", total);

  for (int i = 0; i < total; i++) { // laco que vai printando a caracteristica e o numero de cada agente de IA!
    printf("\n----------------------------------------------\n");
    printf("ID: %d\n", i + 1);
    printf("Nome: %s\n", nome[i]);
    printf("Prompt: %s\n", promptIA[i]);
    printf("Categoria: %s\n", categoriaIA[i]);
    printf("TAG'S: %s\n", tag_agente[i]);
    printf("Avaliação: %d/5\n", nota[i]);
  }

  printf("\n");
  voltarmenu();
}



void buscar(char nome[MAX_AGENTES][TAM_CARAC], char promptIA[MAX_AGENTES][CARACTERES], char categoriaIA[MAX_AGENTES][CARACTERES], char tag_agente[MAX_AGENTES][CARACTERES], int nota[], int total) { // essa funcao vai buscar um agente cadastrado pelo nome ou por parte do nome informado pelo usuario.

    if (total == 0) {
        printf("\nNão existem agentes cadastrados para realizar a busca.\n");
        voltarmenu();

    }

   char buscarIA[TAM_CARAC]; // essa variavel vai ser usada para buscar um agente na funcao!
   int encontrou = 0; // essa variavel vai ser usada para a funcao buscar, para que ela encontre o agente que o usuario procurou!

    printf("\n================ BUSCAR AGENTE ================\n");
    printf("Total de agentes cadastrados: %d\n", total);
    printf("\nDigite o nome ou parte do nome do agente que deseja localizar:\n> ");
    if (fgets(buscarIA, TAM_CARAC, stdin) != NULL) {
        buscarIA[strcspn(buscarIA, "\n")] = '\0';
    }

    for (int i = 0; i < total; i++) {
    if (strstr(nome[i], buscarIA) != NULL) { // strstr procura um trecho do texto informado dentro do nome de cada agente, permitindo realizar buscas parciais!
    printf("\n----------------------------------------------\n");
    printf("ID: %d\n", i + 1);
    printf("Nome: %s\n", nome[i]);
    printf("Prompt: %s\n", promptIA[i]);
    printf("Categoria: %s\n", categoriaIA[i]);
    printf("TAG'S: %s\n", tag_agente[i]);
    printf("Avaliação: %d/5\n", nota[i]);
    encontrou = 1;

    }

}
    voltarmenu();
    if (encontrou == 0) {
        printf("\nNenhum agente foi encontrado com o termo informado.\n");
        printf("Verifique o nome pesquisado e tente novamente.\n");
        voltarmenu();
    }

}
