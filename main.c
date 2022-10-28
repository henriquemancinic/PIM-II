#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define QTD_FUNCIONARIOS 10
#define TAM_CORPO_LOGIN 30
#define TAM_CORPO_MENU 51

/*
Dicionario das funcoes:
    funcoes vazias com parametros
        menuSuperior(int <tamanho do corpo>, char <tipo de usuario>);
        menuCorpo(int <tamanho do corpo>, char <conteudo do menu>);
        menuInferior(int <tamanho do corpo>);
*/

int i, indice = 0;
int ic_logado, qtdLinhas, tp_funcionario = 0;

/*typedef struct tp_data_contratacao
{
    int dia, mes, ano;
} dt_contratacao;
*/

typedef struct cadastro_funcionario
{
    int cd_id;
    char nm_funcionario[50];
    char ds_login[50];
    char ds_senha[50];
    // dt_contratacao dt_contratacao;
    int tp_funcionario;
} Funcionarios;
Funcionarios funcionarios[10];

void armazenaFuncionario()
{
    FILE *ptrArq;
    // Registro Login e Senha
    ptrArq = fopen("registroLS.txt", "a");

    // Testando a abertura do arquivo
    if (ptrArq == NULL)
    {
        printf("Erro ao tentar criar ou abrir arquivo!");
        exit(1);
    }

    // Gravando os dados no arquivo
    fprintf(ptrArq, "\n%d|%s|%s|%s|%d|", indice, funcionarios[indice].nm_funcionario, funcionarios[indice].ds_login, funcionarios[indice].ds_senha, funcionarios[indice].tp_funcionario);

    // fechando o arquivo
    fclose(ptrArq);
}

int leArqLogin()
{
    FILE *ptrArq;
    // Registro de cadastro
    ptrArq = fopen("registroLS.txt", "r");

    char *ptrString;
    char strLinha[500];
    char c;

    qtdLinhas = 0;
    // Verifica se o arquivo existe ou foi aberto com sucesso.
    if (ptrArq)
    {
        // OBS- criar um outro arquivo de clientes
        // contando quantas linhas do arquivo.
        for (c = fgetc(ptrArq); c != EOF; c = fgetc(ptrArq))
            if (c == '\n')
                qtdLinhas++;
        // retorna para o inicio do arquivo.
        rewind(ptrArq);
        /*Aqui abaixo vamos separar a nossa linha do arquivo por token -> strtok(), ent�o acada separador nosso |(pipe) significa que � um token e assim por diante, ele entra
        no primeiro token com a linha(string) e depois que ele passa no primeiro looping ele procura o null e parte para outro token e assim vai ate acabar a linha
        primeiro token ID, segundo NOME, terceito LOGIN, quarto SENHA e por �ltimo tipoFUNCIONARIO, sendo 0 FUNDADOR e 1 FUNCIONARIO.
        */
        for (i = 0; i <= qtdLinhas; i++)
        {
            ptrString = strtok(fgets(strLinha, 100, ptrArq), "|");
            // como o arquivo eh texto, temos que converter a string em int usando a funcao atoi().
            funcionarios[i].cd_id = atoi(ptrString);
            ptrString = strtok(NULL, "|");
            strcpy(funcionarios[i].nm_funcionario, ptrString);
            ptrString = strtok(NULL, "|");
            strcpy(funcionarios[i].ds_login, ptrString);
            ptrString = strtok(NULL, "|");
            strcpy(funcionarios[i].ds_senha, ptrString);
            ptrString = strtok(NULL, "|");
            funcionarios[i].tp_funcionario = atoi(ptrString);
            ptrString = strtok(NULL, "|");
        }
        indice = qtdLinhas + 1;
    }

    else
        printf("\nFalha ao criar ou editar o arquivo!\n");
    // fechando o arquivo
    fclose(ptrArq);
}

void menuSuperior(int tamCorpo, char strMenu[])
{
    int contaLetras = strlen(strMenu);
    tamCorpo = (tamCorpo - contaLetras) / 2;

    printf("%c", 201);
    printf("%c", 205);
    for (i = 0; i < tamCorpo; i++)
        printf("%c", 205);

    for (i = 0; i < contaLetras; i++)
        printf("%c", strMenu[i]);

    for (i = 0; i < tamCorpo; i++)
        printf("%c", 205);

    printf("%c\n", 187);
}

void menuCorpo(int tamCorpo, char strMenu[])
{
    int contaLetras = strlen(strMenu);
    tamCorpo = (tamCorpo - contaLetras);
    printf("%c", 186);
    printf("%s", strMenu);
    for (i = 0; i < tamCorpo; i++)
        printf("%c", 32);
    printf("%c\n", 186);
}

void menuInferior(int tamCorpo)
{
    printf("%c", 200);

    for (i = 0; i < tamCorpo; i++)
        printf("%c", 205);

    printf("%c\n", 188);
}

void menuFundadores()
{
    system("cls");
    menuSuperior(TAM_CORPO_MENU, "FUNDADORES");
    menuCorpo(TAM_CORPO_MENU, "[ITEM] - Escolha um item do menu");
    menuCorpo(TAM_CORPO_MENU, "[1] - Cadastrar novo funcionario");
    menuCorpo(TAM_CORPO_MENU, "[2] - Listar todos os clientes");
    menuCorpo(TAM_CORPO_MENU, "[3] - Listar todos os funcionarios");
    menuCorpo(TAM_CORPO_MENU, "[x] - Fechar sistema");
    menuInferior(TAM_CORPO_MENU);
}

void menuRecepcionista()
{
    system("cls");
    int largMenu = 60;
    menuSuperior(largMenu, "RECEPCIONISTA");
    menuCorpo(largMenu, "[ITEM] - Escolha um item do menu");
    menuCorpo(largMenu, "[1] - Cadastrar novo cliente");
    menuCorpo(largMenu, "[2] - Listar todos os clientes");
    menuCorpo(largMenu, "[x] - Fechar sistema");
    menuInferior(largMenu);
}

int login()
{
    char login[15];
    char senha[15];

    // Criando menu do login
    menuSuperior(TAM_CORPO_LOGIN, "LOGIN");
    menuCorpo(TAM_CORPO_LOGIN, "Login: ");
    // Adiciona ║ antes e depois de pedir login
    printf("%c", 186);
    scanf("%s", &login);
    printf("%c", 186);

    // preenche um linha somente de espaco
    for (i = 0; i < TAM_CORPO_LOGIN; i++)
        printf("%c", 32);
    // Adiciona ║ depois para fechar a linha
    printf("%c\n", 186);

    menuCorpo(TAM_CORPO_LOGIN, "Senha: ");
    // Adiciona ║ antes e depois de pedir senha
    printf("%c", 186);
    scanf("%s", &senha);
    printf("%c", 186);
    // preenche um linha somente de espaco
    for (i = 0; i < TAM_CORPO_LOGIN; i++)
        printf("%c", 32);
    // Adiciona ║ depois para fechar a linha
    printf("%c\n", 186);

    menuInferior(TAM_CORPO_LOGIN);

    // percorre o arquivo e atribui os conteudos em struct
    leArqLogin();

    // percorremos a struct funcionarios para validar o login
    for (i = 0; i <= qtdLinhas; i++)
    {
        if (strcmp(funcionarios[i].ds_login, login) == 0 && strcmp(funcionarios[i].ds_senha, senha) == 0)
        {
            ic_logado = 1;
            tp_funcionario = funcionarios[i].tp_funcionario;
        }
    }

    if (ic_logado != 1)
    {
        system("color 4");
        menuSuperior(TAM_CORPO_LOGIN, "DADOS ERRADOS");
        menuInferior(TAM_CORPO_LOGIN);
        system("pause");
        system("cls");
        system("color 7");
    }
}

void buscar_funcionarios();

void cadastrar_funcionarios();

int main()
{
    while (ic_logado == 0)
    {
        login();
    }

    if (tp_funcionario == 0)
    {
        menuFundadores();
    }
    else
    {
        menuRecepcionista();
    }

    int resposta;
    printf("\nITEM: ");
    scanf("%d", &resposta);

    switch (resposta)
    {
    case 1:
        cadastrar_funcionarios();
        break;
    case 2:
        // buscar_clientes(clientes);
        break;
    case 3:
        buscar_funcionarios(i);
        break;
    }
    // menuRecepcionista();
    return 0;
}

void buscar_funcionarios(int qtd_funcionarios)
{
    // verificar essa l�gica // começar em 1 para eliminar de mostrar os usuarios fundadores
    for (i = 1; i <= qtd_funcionarios; i++)
    {
        if (funcionarios[i].tp_funcionario == 1)
        {
            printf("\n::----------------------------------------------::");
            printf("\n:: %s", funcionarios[i].nm_funcionario);
            printf("\n:: %s", funcionarios[i].ds_login);
            printf("\n:: %s", funcionarios[i].ds_senha);
            printf("\n::----------------------------------------------::\n");
        }
    }
    system("pause");
    menuFundadores();
    main();
}

void cadastrar_funcionarios()
{
    system("cls");
    printf("Nome do funcionario: \n");
    scanf("%s", funcionarios[indice].nm_funcionario);
    printf("Login: \n");
    scanf("%s", funcionarios[indice].ds_login);
    printf("Senha: \n");
    scanf("%s", funcionarios[indice].ds_senha);
    funcionarios[indice].tp_funcionario = 1;
    armazenaFuncionario();
    indice++;
    system("pause");
    menuFundadores();
    main();
}
