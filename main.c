#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TAM_CORPO_LOGIN 30
#define TAM_CORPO_MENU 51
#include <winsock.h>
#include <conio.h> //getch
#include <ctype.h> //islower

#define BUFFER_SIZE 128
#define COR_INTERNA_MENU 240
#define COR_FUNDADOR 159
#define COR_RECEPCIONISTA 96
#define COR_DENTISTA 176

int server = 0;

char message[BUFFER_SIZE];

struct sockaddr_in remote_address;

WSADATA wsa_data;

// definindo o tamanho da login senha, maximo 20 caracteres
#define qtdMax 21

char polibio[6][6] = {
    {'a', 'b', 'c', 'd', 'e', 'f'},
    {'g', 'h', 'i', 'j', 'k', 'l'},
    {'m', 'n', 'o', 'p', 'q', 'r'},
    {'s', 't', 'u', 'v', 'w', 'x'},
    {'y', 'z', '0', '1', '2', '3'},
    {'4', '5', '6', '7', '8', '9'},
};
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
    // usando (int) para não dar warning de unsigned (usando, mas nao corre o risco de ultrapassar)
    for (i = indice; i < (int)strlen(strMensagem); i++)
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

int i, indice, indiceClientes, ic_logado, qtdLinhas, id_funcionario = 0;
char resposta;

typedef struct cadastro_funcionario
{
    int cd_id;
    char nm_funcionario[50];
    char ds_login[50];
    char ds_senha[50];
    int tp_funcionario;
} Funcionarios;
Funcionarios funcionarios[10];

typedef struct cadastro_cliente
{
    int cd_id;
    char nm_cliente[50];
    char celular[12];
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
    fprintf(ptrArq, "%d|%s|%s|%s|%d\n", indiceClientes, clientes[indiceClientes].nm_cliente, clientes[indiceClientes].celular, clientes[indiceClientes].ds_servico, clientes[indiceClientes].cd_id_funcionario);

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
            strcpy(clientes[i].celular, ptrString);
            ptrString = strtok(NULL, "|");
            strcpy(clientes[i].ds_servico, ptrString);
            ptrString = strtok(NULL, "|");
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

void menuSuperior(int tamCorpo, char strMenu[], int borda)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), borda);
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

void menuCorpo(int tamCorpo, char strMenu[], int borda, int interno)
{
    int contaLetras = strlen(strMenu);
    tamCorpo = (tamCorpo - contaLetras);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), borda);
    printf("%c", 186);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), interno);
    printf("%s", strMenu);
    for (i = 0; i < tamCorpo; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), interno);
        printf("%c", 32);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), borda);
    printf("%c\n", 186);
}

void menuInferior(int tamCorpo, int borda)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), borda);
    printf("%c", 200);

    for (i = 0; i < tamCorpo; i++)
        printf("%c", 205);

    printf("%c\n", 188);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

int login()
{
    fflush(stdin);
    char login[15];
    char senha[15];

    // Criando menu do login
    menuSuperior(TAM_CORPO_LOGIN, "LOGIN", 0);
    system("color 0F");
    menuCorpo(TAM_CORPO_LOGIN, "Login: ", 0, 0);
    system("color 0F");
    // Adiciona ║ antes e depois de pedir login
    printf("%c", 186);
    scanf("%s", login);
    printf("%c", 186);

    // preenche um linha somente de espaco

    for (i = 0; i < TAM_CORPO_LOGIN; i++)
        printf("%c", 32);

    // Adiciona ║ depois para fechar a linha
    printf("%c\n", 186);

    menuCorpo(TAM_CORPO_LOGIN, "Senha: ", 0, 0);
    system("color 0F");

    // Adiciona ║ antes e depois de pedir senha
    printf("%c", 186);
    scanf("%s", senha);
    printf("%c", 186);
    // preenche um linha somente de espaco
    for (i = 0; i < TAM_CORPO_LOGIN; i++)
        printf("%c", 32);
    // Adiciona ║ depois para fechar a linha
    printf("%c\n", 186);

    menuInferior(TAM_CORPO_LOGIN, 0);
    system("color 0F");

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
        menuSuperior(TAM_CORPO_LOGIN, "DADOS ERRADOS", 0);
        system("color 4");
        menuInferior(TAM_CORPO_LOGIN, 0);
        system("color 4");
        system("pause");
        system("cls");
    }
    return 0;
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
        if (funcionarios[i].tp_funcionario != 0)
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
}

void buscar_clientes()
{
    leArqClientes();
    // verificar essa l�gica // começar em 1 para eliminar de mostrar os usuarios fundadores
    for (i = 0; i < indiceClientes; i++)
    {
        printf("\n::-------------------------------------------------------------------::");
        printf("\n::Codigo: [%d] - Nome: %s - Celular: %s ", clientes[i].cd_id, clientes[i].nm_cliente, clientes[i].celular);

        if (strcmp(clientes[i].ds_servico, " "))
        {
            printf("\n::Procedimento: %s", clientes[i].ds_servico);
            printf("\n::Codigo do Dentista %d", clientes[i].cd_id_funcionario);
        }
        else
            printf("\n::Procedimento: aguardando");

        printf("\n::-------------------------------------------------------------------::\n");
    }
    system("pause");
}

void cadastrar_funcionarios()
{
    int aux = 0;
    int achou = 0;
    system("cls");
    menuSuperior(TAM_CORPO_MENU + 10, "REGRAS DE CADASTRO", COR_FUNDADOR);
    menuCorpo(TAM_CORPO_MENU + 10, "-NOME --> NAO PODE SER MAIOR QUE 50 CARACTERES", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 10, "-LOGIN E SENHA --> NAO PODEM SER MAIORES QUE 20 CARACTERES", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 10, "-SENHA --> SOMENTE LETRAS MINUSCULAS E NUMEROS", COR_FUNDADOR, COR_INTERNA_MENU);
    menuInferior(TAM_CORPO_MENU + 10, COR_FUNDADOR);

    printf("\nNome do Funcionario: ");
    // limpando o buffer
    fflush(stdin);
    gets(funcionarios[indice].nm_funcionario);
    aux = strlen(funcionarios[indice].nm_funcionario);
    do
    {
        for (i = 0; i < aux; i++)
        {
            // verifica se o caractere pertence ao alfabeto e se é um " "(espaco) e depois verifica se é digito.
            if (isdigit(funcionarios[indice].nm_funcionario[i]) || (!isalpha(funcionarios[indice].nm_funcionario[i]) && !isspace(funcionarios[indice].nm_funcionario[i])))
            {
                achou = 1;
                break;
            }
            achou = 0;
        }
        if (achou == 1 || aux >= 50)
        {
            printf("Atencao as regras, digite o nome novamente: ");
            gets(funcionarios[indice].nm_funcionario);
            aux = strlen(funcionarios[indice].nm_funcionario);
        }
    } while (achou);

    printf("Login: ");
    scanf("%s", funcionarios[indice].ds_login);
    // chamando a função e verificando o login
    while (verificaLogin(funcionarios[indice].ds_login) == 1 || strlen(funcionarios[indice].ds_login) >= 20)
    {
        if (strlen(funcionarios[indice].ds_login) >= 20)
        {
            printf("Atencao as regras, digite o login novamente: ");
        }
        else
            printf("Login existente, digite um novo: ");

        scanf("%s", funcionarios[indice].ds_login);
    };

    printf("Senha: ");
    scanf("%s", funcionarios[indice].ds_senha);
    aux = strlen(funcionarios[indice].ds_senha);
    do
    {
        for (i = 0; i < aux; i++)
        {
            // verifica se a letra e maiusculas ou se não faz parte do alfabeto e se nao é hexadecimal (a-z,A-Z,0-9) e solicita para digitar novamente
            // cliente entrou com o char '1', ele não pertence ao alfabeto e nem ao hexadecinal (false) e tbm não é maiúsculo (false).
            if (isupper(funcionarios[indice].ds_senha[i]) || (!isalpha(funcionarios[indice].ds_senha[i]) && !isxdigit(funcionarios[indice].ds_senha[i])))
            {
                achou = 1;
                break;
            }
            achou = 0;
        }
        if (achou == 1 || aux >= 20)
        {
            printf("Atencao as regras, digite a senha novamente: ");
            scanf("%s", funcionarios[indice].ds_senha);
            aux = strlen(funcionarios[indice].ds_senha);
        }
    } while (achou);

    criptografa(funcionarios[indice].ds_senha);

    do
    {
        printf("\n-[ITEM]--------APERTE UMA TECLA-------------");
        printf("\n  [1] - Para cadastrar como RECEPCIONISTA");
        printf("\n  [2] - Para cadastrar como DENTISTA");
        printf("\n--------------------------------------------");
        printf("\nITEM: ");
        scanf(" %c", &resposta);
        switch (resposta)
        {
        case '1':
            funcionarios[indice].tp_funcionario = 1;
            break;
        case '2':
            funcionarios[indice].tp_funcionario = 2;
            break;
        default:
            printf("\n---------------OPCAO INVALIDA---------------\n");
            resposta = 3;
        }
    } while (resposta == 3);

    armazenaFuncionario();
    indice++;
    system("pause");
}

void cadastrar_clientes()
{
    int aux = 0;
    int achou = 0;
    system("cls");
    menuSuperior(TAM_CORPO_MENU + 10, "REGRAS DE CADASTRO", COR_RECEPCIONISTA);
    menuCorpo(TAM_CORPO_MENU + 10, "-NOME --> NAO PODE SER MAIOR QUE 50 CARACTERES", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 10, "-TELEFONE --> NAO PODEM SER MAIOR QUE 12 DIGITOS", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 10, "-TELEFONE --> SOMENTE NUMEROS", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuInferior(TAM_CORPO_MENU + 10, COR_RECEPCIONISTA);

    printf("\nNome do paciente: \n");

    fflush(stdin);
    gets(clientes[indiceClientes].nm_cliente);
    aux = strlen(clientes[indiceClientes].nm_cliente);
    do
    {
        for (i = 0; i < aux; i++)
        {
            if (isdigit(clientes[indiceClientes].nm_cliente[i]) || (!isalpha(clientes[indiceClientes].nm_cliente[i]) && !isspace(clientes[indiceClientes].nm_cliente[i])))
            {
                achou = 1;
                break;
            }
            achou = 0;
        }
        if (achou == 1 || aux >= 50)
        {
            printf("Atencao as regras, digite o nome novamente: ");
            gets(clientes[indiceClientes].nm_cliente);
            aux = strlen(clientes[indiceClientes].nm_cliente);
        }
    } while (achou);

    printf("Numero de celular: \n");
    scanf("%s", clientes[indiceClientes].celular);
    aux = strlen(clientes[indiceClientes].celular);
    do
    {
        for (i = 0; i < aux; i++)
        {
            // verifica se a letra e maiusculas ou se não faz parte do alfabeto e se nao é hexadecimal (a-z,A-Z,0-9) e solicita para digitar novamente
            // cliente entrou com o char '1', ele não pertence ao alfabeto e nem ao hexadecinal (false) e tbm não é maiúsculo (false).
            if (!isdigit(clientes[indiceClientes].celular[i]))
            {
                achou = 1;
                break;
            }
            achou = 0;
        }
        if (achou == 1 || aux >= 12)
        {
            printf("Atencao as regras, digite o celular novamente: ");
            scanf("%s", clientes[indiceClientes].celular);
            aux = strlen(clientes[indiceClientes].celular);
        }
    } while (achou);
    strcpy(clientes[indiceClientes].ds_servico, " ");
    clientes[indiceClientes].cd_id_funcionario = id_funcionario;
    armazenaCliente();
    indiceClientes++;
    system("pause");
}

// função para abrir um cliente e enviar os arquivos para o servidor salvar.
int backupArquivosSOCKET()
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
        return 0;

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
        printf("\nFalha ao ler ou criar o arquivo de backup!\n");

    send(server, ptrString, strlen(ptrString), 0);
    printf("BACKUP DOS FUNCIONARIOS REALIZADO COM SUCESSO\n");
    fclose(ptrArq);

    //limpando array para não ir residuos de funcionarios
    memset(ptrString, 0, sizeof (ptrString));
    ptrArq = fopen("registroClientes.txt", "r");

    if (ptrArq)
    {
        while (fgets(strLinha, 100, ptrArq) != 0)
        {
            strcat(ptrString, strLinha);
        }
    }
    else
        printf("\nFalha ao ler ou criar o arquivo de backup!\n");

    send(server, ptrString, strlen(ptrString), 0);
    printf("BACKUP DOS CLIENTES REALIZADO COM SUCESSO\n");

    // fechando o arquivo
    fclose(ptrArq);
    system("pause");
    WSACleanup();
    closesocket(server);
    return 1;
}

void menuFundadores()
{
    system("cls");
    menuSuperior(TAM_CORPO_MENU, "FUNDADORES", COR_FUNDADOR);
    menuCorpo(TAM_CORPO_MENU, "[ITEM] - Escolha um item do menu", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[1] - Cadastrar novo funcionario", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[2] - Listar todos os clientes", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[3] - Listar todos os funcionarios", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[4] - Realizar backup", COR_FUNDADOR, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[x] - Qualquer outra tecla desloga do sistema", COR_FUNDADOR, COR_INTERNA_MENU);
    menuInferior(TAM_CORPO_MENU, COR_FUNDADOR);

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
        if (backupArquivosSOCKET() == 0)
        {
            printf("SERVIDOR INDISPONIVEL");
            getch();
        }
        break;
    default:
        ic_logado = 0;
        system("cls");
        main();
    }
}

void menuRecepcionista()
{
    system("cls");
    menuSuperior(TAM_CORPO_MENU, "RECEPCIONISTA", COR_RECEPCIONISTA);
    menuCorpo(TAM_CORPO_MENU + 1, "[ITEM] - Escolha um item do menu", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 1, "[1] - Cadastrar novo cliente", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 1, "[2] - Listar todos os clientes", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU + 1, "[x] - Qualquer outra tecla desloga do sistema", COR_RECEPCIONISTA, COR_INTERNA_MENU);
    menuInferior(TAM_CORPO_MENU + 1, COR_RECEPCIONISTA);

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
    default:
        ic_logado = 0;
        system("cls");
        main();
        break;
    }
}

void anexanoarquivo(int linha)
{
    FILE *ptrArq;
    // Registro de cadastro
    ptrArq = fopen("registroClientes.txt", "w");
    // Verifica se o arquivo existe ou foi aberto com sucesso.
    if (ptrArq)
    {
        for (i = 0; i < linha; i++)
        {
            // retira -1 por conta do \n do final da string

            fprintf(ptrArq, "%d|%s|%s|%s|%d\n", i, clientes[i].nm_cliente, clientes[i].celular, clientes[i].ds_servico, clientes[i].cd_id_funcionario);
        }
    }
    else
        printf("\nFalha ao ler arquivo!\n");
    // fechando o arquivo
    fclose(ptrArq);
}

void listaClientesDentista(int clientes_emEspera)
{
    int count = 0;

    for (i = 0; i < indiceClientes; i++)
    {
        if (strcmp(clientes[i].ds_servico, " ") != clientes_emEspera)
        {
            if (clientes_emEspera)
            {
                printf("\n::-------------------------------------------------------------------::");
                printf("\n::Codigo: [%d] - Nome: %s - Celular: %s ", clientes[i].cd_id, clientes[i].nm_cliente, clientes[i].celular);
                count++;
            }
            else
            {
                if (clientes[i].cd_id_funcionario == id_funcionario)
                {
                    printf("\n::-------------------------------------------------------------------::");
                    printf("\n::Codigo: [%d] - Nome: %s", clientes[i].cd_id, clientes[i].nm_cliente);
                    printf("\n::Celular: %s", clientes[i].celular);
                    printf("\n::Procedimento realizado: %s", clientes[i].ds_servico);
                    count++;
                }
            }
        }
    }

    if (count)
    {
        printf("\n::-------------------------------------------------------------------::\n\n");
    }
    else
        printf("\n::---------------------NAO TEM CLIENTES NA ESPERA--------------------::\n\n");

    system("pause");
}

void anexaProcedimento()
{
    int id;
    int count = 0;
    for (i = 0; i < indiceClientes; i++)
    {
        if (!strcmp(clientes[i].ds_servico, " "))
        {
            printf("\n::-------------------------------------------------------------------::");
            printf("\n::Codigo: [%d] - Nome: %s - Celular: %s ", clientes[i].cd_id, clientes[i].nm_cliente, clientes[i].celular);
            count++;
        }
    }
    if (count)
    {
        printf("\n::-------------------------------------------------------------------::\n");
        printf("\nDigite o codigo do cliente para anexar um procedimento realizado: \n");
        scanf(" %d", &id);
        while (isdigit(id) || (id < 0 || id > count))
        {
            fflush(stdin);
            printf("\nDigite um codigo existente: \n");
            scanf(" %d", &id);
        }
        fflush(stdin);
        printf("\nDigite o procedimento realizado:\n");
        gets(clientes[id].ds_servico);
        clientes[id].cd_id_funcionario = id_funcionario;
        anexanoarquivo(indiceClientes);
    }
    else
        printf("\n::---------------------NAO TEM CLIENTES NA ESPERA--------------------::\n\n");
    system("pause");
    main();
}

void menuDentista()
{
    system("cls");
    menuSuperior(TAM_CORPO_MENU, "DENTISTA", COR_DENTISTA);
    menuCorpo(TAM_CORPO_MENU, "[ITEM] - Escolha um item do menu", COR_DENTISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[1] - Anexar procedimento realizado no cliente", COR_DENTISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[2] - Listar clientes em espera", COR_DENTISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[3] - Listar seus clientes", COR_DENTISTA, COR_INTERNA_MENU);
    menuCorpo(TAM_CORPO_MENU, "[x] - Qualquer outra tecla desloga do sistema", COR_DENTISTA, COR_INTERNA_MENU);
    menuInferior(TAM_CORPO_MENU, COR_DENTISTA);

    printf("\nITEM: ");
    scanf(" %c", &resposta);

    switch (resposta)
    {
    case '1':
        anexaProcedimento();
        break;
    case '2':
        listaClientesDentista(1);
        break;
    case '3':
        listaClientesDentista(0);
        break;
    default:
        ic_logado = 0;
        system("cls");
        main();
        break;
    }
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

    do
    {
        switch (funcionarios[id_funcionario].tp_funcionario)
        {
        case 0:
            menuFundadores();
            break;
        case 1:
            menuRecepcionista();
            break;
        case 2:
            menuDentista();
            break;
        default:
            printf("\n HOUVE UM ERRO INESPERADO, CASO OCORRA NOVAMENTE CONTATAR A EQUIPE DE SOFTWARE!");
            system("pause");
            ic_logado = 0;
            main();
            break;
        }
    } while (ic_logado == 1);
    return 0;
}