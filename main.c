#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TAM_CORPO_LOGIN 30
#define TAM_CORPO_MENU 51
#include <winsock.h>

#define BUFFER_SIZE 128

int server = 0;

char message[BUFFER_SIZE];

struct sockaddr_in remote_address;

WSADATA wsa_data;

// definindo o tamanho da login senha, maximo 20 caracteres
#define qtdMax 21

char polibio[6][6] = {{'a', 'b', 'c', 'd', 'e', 'f'},
                      {'g', 'h', 'i', 'j', 'k', 'l'},
                      {'m', 'n', 'o', 'p', 'q', 'r'},
                      {'s', 't', 'u', 'v', 'w', 'x'},
                      {'y', 'z', '0', '1', '2', '3'},
                      {'4', '5', '6', '7', '8', '9'}};
int linhaColuna[qtdMax * 2];

void criptografa(char strMensagem[])
{
    int linha = 0;
    int coluna = 0;
    int i = 0;
    int indice = 0;
    // vai criptografando ate chegar o fim da mensagem 0
    while (strMensagem[i] != 0)
    {
        for (linha = 0; linha <= 5; linha++)
        {
            for (coluna = 0; coluna <= 5; coluna++)
            {
                if (strMensagem[i] == polibio[linha][coluna])
                {
                    linhaColuna[indice] = linha;
                    indice++;
                    linhaColuna[indice] = coluna;
                    indice++;
                    i++;
                    // seto as variaveis para 5 para resetar os laços de coluna e de linha pois ja foi localizada o char da mensagem.
                    linha = 5;
                    coluna = 5;
                };
            }
        }
    }

    for (i = 0; i < indice; i++)
    {
        strMensagem[i] = linhaColuna[i] + '0';
    }
    // limpando o resto da variavel
    for (i = indice; i < strlen(strMensagem); i++)
    {
        strMensagem[i] = '\0';
    }
}

void descriptografa(char strMensagem[])
{
    int indice = 0;
    int linha = 0;
    int coluna = 1;
    int i;

    // convertendo toda a mensagem que está char para int até o final da mensagem e atribui a um vetor de int.
    for (i = 0; strMensagem[i] != '\0'; i++)
    {
        // se o primeiro char for par ''
        if (indice % 2 == 0)
        {
            switch (strMensagem[i])
            {
            case '0':
                linhaColuna[i] = 0;
                break;
            case '1':
                linhaColuna[i] = 1;
                break;
            case '2':
                linhaColuna[i] = 2;
                break;
            case '3':
                linhaColuna[i] = 3;
                break;
            case '4':
                linhaColuna[i] = 4;
                break;
            case '5':
                linhaColuna[i] = 5;
                break;
            case '6':
                linhaColuna[i] = 6;
                break;
            }
        }
        else
        {
            switch (strMensagem[i])
            {
            case '0':
                linhaColuna[i] = 0;
                break;
            case '1':
                linhaColuna[i] = 1;
                break;
            case '2':
                linhaColuna[i] = 2;
                break;
            case '3':
                linhaColuna[i] = 3;
                break;
            case '4':
                linhaColuna[i] = 4;
                break;
            case '5':
                linhaColuna[i] = 5;
                break;
            case '6':
                linhaColuna[i] = 6;
                break;
            }
        }
        indice++;
    }
    indice = indice / 2;
    for (i = 0; i < indice; i++)
    {
        strMensagem[i] = polibio[linhaColuna[linha]][linhaColuna[coluna]];
        linha = linha + 2;
        coluna = coluna + 2;
    }
    // limpando o restante da variável
    for (i = indice; i < (indice * 2); i++)
    {
        strMensagem[i] = 0;
    }
}
/*
Dicionario das funcoes:
    funcoes vazias com parametros
        menuSuperior(int <tamanho do corpo>, char <tipo de usuario>);
        menuCorpo(int <tamanho do corpo>, char <conteudo do menu>);
        menuInferior(int <tamanho do corpo>);
*/

int i, indice, indiceClientes, ic_logado, qtdLinhas, id_funcionario = 0;
char resposta;

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

typedef struct cadastro_cliente
{
    int cd_id;
    char nm_cliente[50];
    char ds_servico[500];  // aqui serao armazenados o que foi realizado no cliente.
    int cd_id_funcionario; // id do funcionario que fez o procedimento
} Clientes;
Clientes clientes[10];

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

void armazenaCliente()
{
    FILE *ptrArq;
    // Registro Login e Senha
    ptrArq = fopen("registroClientes.txt", "a");

    // Testando a abertura do arquivo
    if (ptrArq == NULL)
    {
        printf("Erro ao tentar criar ou abrir arquivo!");
        exit(1);
    }

    // Gravando os dados no arquivo
    fprintf(ptrArq, "%d|%s|%s|%d\n", indiceClientes, clientes[indiceClientes].nm_cliente, clientes[indiceClientes].ds_servico, clientes[indiceClientes].cd_id_funcionario);

    // fechando o arquivo
    fclose(ptrArq);
}

void leArqClientes()
{
    FILE *ptrArq;
    // Registro de cadastro
    ptrArq = fopen("registroClientes.txt", "a+");

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
        for (i = 0; i < qtdLinhas; i++)
        {
            ptrString = strtok(fgets(strLinha, 100, ptrArq), "|");
            // como o arquivo eh texto, temos que converter a string em int usando a funcao atoi().
            clientes[i].cd_id = atoi(ptrString);
            ptrString = strtok(NULL, "|");
            strcpy(clientes[i].nm_cliente, ptrString);
            ptrString = strtok(NULL, "|");
            strcpy(clientes[i].ds_servico, ptrString);
            ptrString = strtok(NULL, "|\n");
            clientes[i].cd_id_funcionario = atoi(ptrString);
        }
        indiceClientes = qtdLinhas;
    }

    else
        printf("\nFalha ao ler arquivo!\n");
    // fechando o arquivo
    fclose(ptrArq);
}

void leArqLogin()
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
        printf("\nFalha ao ler o arquivo!\n");
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
    menuCorpo(TAM_CORPO_MENU, "[4] - Realizar backup");
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
        descriptografa(funcionarios[i].ds_senha);
        if (strcmp(funcionarios[i].ds_login, login) == 0 && strcmp(funcionarios[i].ds_senha, senha) == 0)
        {
            id_funcionario = funcionarios[i].cd_id;
            return 1;
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

int verificaLogin(char login[])
{
    // i vai ate indice (funcionarios)
    for (i = 0; i < indice; i++)
    {
        if (strcmp(funcionarios[i].ds_login, login) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void buscar_funcionarios()
{
    // verificar essa l�gica // começar em 1 para eliminar de mostrar os usuarios fundadores
    for (i = 1; i < indice; i++)
    {
        if (funcionarios[i].tp_funcionario == 1)
        {
            printf("\n::----------------------------------------------::");
            printf("\n::NOME: %s", funcionarios[i].nm_funcionario);
            printf("\n::LOGIN: %s", funcionarios[i].ds_login);
            descriptografa(funcionarios[i].ds_senha);
            printf("\n::SENHA: %s", funcionarios[i].ds_senha);
            printf("\n::----------------------------------------------::\n");
        }
    }
    system("pause");
    main();
}

void buscar_clientes()
{
    // verificar essa l�gica // começar em 1 para eliminar de mostrar os usuarios fundadores
    for (i = 0; i < indiceClientes; i++)
    {
        printf("\n::----------------------------------------------::");
        printf("\n:: %d", clientes[i].cd_id);
        printf("\n:: %s", clientes[i].nm_cliente);
        printf("\n:: %s", clientes[i].ds_servico);
        printf("\n:: %d", clientes[i].cd_id_funcionario);
        printf("\n::----------------------------------------------::\n");
    }
    system("pause");
    main();
}

void cadastrar_funcionarios()
{
    system("cls");
    menuSuperior(TAM_CORPO_MENU + 10, "REGRAS DE CADASTRO");
    menuCorpo(TAM_CORPO_MENU + 10, "-NOME --> NAO PODE SER MAIOR QUE 50 CARACTERES");
    menuCorpo(TAM_CORPO_MENU + 10, "-LOGIN E SENHA --> NAO PODEM SER MAIORES QUE 20 CARACTERES");
    menuCorpo(TAM_CORPO_MENU + 10, "-SENHA --> SOMENTE LETRAS MINUSCULAS E NUMEROS");
    menuInferior(TAM_CORPO_MENU + 10);

    printf("\n");

    printf("-----------------CADASTRAR");
    printf("\nNome do Funcionario: ");
    // limpando o buffer
    fflush(stdin);
    gets(funcionarios[indice].nm_funcionario);
    printf("Login: ");
    scanf("%s", funcionarios[indice].ds_login);
    // chamando a função e verificando o login
    while (verificaLogin(funcionarios[indice].ds_login) == 1)
    {
        printf("Login existente, digite um novo: ");
        scanf("%s", funcionarios[indice].ds_login);
    };
    printf("Senha: ");
    scanf("%s", funcionarios[indice].ds_senha);
    criptografa(funcionarios[indice].ds_senha);
    funcionarios[indice].tp_funcionario = 1;
    armazenaFuncionario();
    indice++;
    system("pause");
    main();
}

void cadastrar_clientes()
{
    system("cls");
    printf("Nome do paciente: \n");
    // limpando o buffer
    fflush(stdin);
    gets(clientes[indiceClientes].nm_cliente);
    printf("Descreva o servico realizado: \n");
    gets(clientes[indiceClientes].ds_servico);
    clientes[indiceClientes].cd_id_funcionario = id_funcionario;
    armazenaCliente();
    indiceClientes++;
    system("pause");
    main();
}

// função para abrir um cliente e enviar os arquivos para o servidor salvar.
void backupArquivosSOCKET()
{
    WSAStartup(MAKEWORD(2, 0), &wsa_data);
    //(comunicação ipv4, protocolo TCP,0)
    int server = socket(AF_INET, SOCK_STREAM, 0);
    int porta = 80;

    // preenchendo o remote_address (servidor)
    memset(&remote_address, 0, sizeof(remote_address));
    // Estamos lidando com ipv4 AF_INET, caso ipv6 AF_INET6
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote_address.sin_port = htons(porta);

    // conecta e testa conexão, caso erro, returna pra main
    if (connect(server, (struct sockaddr *)&remote_address, sizeof(remote_address)) == SOCKET_ERROR)
    {
        printf("SERVIDOR INDISPONIVEL\n");
        getch();
        main();
    };

    printf("CONECTADO COM SERVIDOR\n");

    FILE *ptrArq;
    // Registro de cadastro
    ptrArq = fopen("registroLS.txt", "r");

    char ptrString[500];
    char strLinha[500];

    // Verifica se o arquivo existe ou foi aberto com sucesso.
    if (ptrArq)
    {
        while (fgets(strLinha, 100, ptrArq) != 0)
        {
            strcat(ptrString, strLinha);
        }
    }
    else
        printf("\nFalha ao ler o arquivo de usuarios!\n");

    // fechando o arquivo
    fclose(ptrArq);
    // envia a mensagem para o servidor
    send(server, ptrString, strlen(ptrString), 0);
    printf("BACKUP REALIZADO COM SUCESSO\n");
    getch();
    WSACleanup();
    closesocket(server);
    main();
}

int main()
{
    system("cls");
    // inicializa/percorre arquivo de clientes
    leArqClientes();

    while (ic_logado == 0)
    {
        ic_logado = login();
    }

    if (funcionarios[id_funcionario].tp_funcionario == 0)
    {
        menuFundadores();

        printf("\nITEM: ");
        scanf(" %c", &resposta);
        switch (resposta)
        {
        case '1':
            cadastrar_funcionarios();
            break;
        case '2':
            buscar_clientes();
            break;
        case '3':
            buscar_funcionarios();
            break;
        case '4':
            backupArquivosSOCKET();
            break;
        case 'z':
            ic_logado = 0;
            system("cls");
            main();
        }
    }
    else if (funcionarios[id_funcionario].tp_funcionario == 1)
    {
        menuRecepcionista();

        printf("\nITEM: ");
        scanf(" %c", &resposta);

        switch (resposta)
        {
        case '1':
            cadastrar_clientes();
            break;
        case '2':
            buscar_clientes();
            break;
        case 'z':
            ic_logado = 0;
            system("cls");
            main();
            break;
        }
    }

    return 0;
}
