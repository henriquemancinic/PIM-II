#include <stdio.h>
#include <winsock2.h>
#include <winsock.h>

// servidor
int main()
{
    //inicia winsock dll e já informa a versão utilizada
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);

    // recebe os parametros de conexao
    struct sockaddr_in caddr;   // cliente
    struct sockaddr_in saddr = {// servidor;
                                // vai lidar com enderecos ipv4 caso ipv6 AF_INET6
                                .sin_family = AF_INET,
                                /* recebe o ip da maquina - INADDR_ANY vai pegar os IPs da maquina 
                                livre para esperar receber algo (da rede).*/
                                .sin_addr.s_addr = htonl(INADDR_ANY),
                                // htons converte a porta
                                .sin_port = htons(80)};

    // coloca 0 no ultimo parametro, pois nao precisamos que retorne outro protocolo 
    //(normamente usa-se em sistemas embarcados)
    int server = socket(AF_INET, SOCK_STREAM, 0);

    bind(server, (struct sockaddr *)&saddr, sizeof saddr);

    // deixa o servidor na espera de uma conexão (servidor, quantidade de conexoes simultaneas)
    listen(server, 5);

    printf("ESPERANDO CONEXAO COM O CLIENTE...\n");
    int csize = sizeof caddr;
    int cliente;
    char buff[200];

    while (1)
    {
        // aceita as configuracoes do cliente
        cliente = accept(server, (struct sockaddr *)&caddr, &csize);
        printf("Cliente foi conectado\n");
        // limpando o buffer
        memset(buff, 0, sizeof buff);
        // depois disso jogamos os dados em uma variavel (0 eh para gravar em bufffer)
        recv(cliente, buff, sizeof buff, 0);

        printf("\nBackup: \n%s\n", buff);

        // salvando arquivo/sobreescreve
        FILE *ptrArq;
        ptrArq = fopen("backupRegistroLS.txt", "w");
        
        if (ptrArq == NULL)
        {
            printf("Erro ao tentar criar ou abrir arquivo!");
            exit(1);
        }
        fprintf(ptrArq, "%s\n", buff);
        fclose(ptrArq);

        // limpando o buffer
        memset(buff, 0, sizeof(buff));

        // depois disso jogamos os dados em uma variavel (0 eh para gravar em bufffer)
        recv(cliente, buff, sizeof buff, 0);
        printf("\nBackup: \n%s\n", buff);

        ptrArq = fopen("backupClientes.txt", "w");
        if (ptrArq == NULL)
        {
            printf("Erro ao tentar criar ou abrir arquivo!");
            exit(1);
        }
        fprintf(ptrArq, "%s\n", buff);
        fclose(ptrArq);
    }
    closesocket(cliente);
    WSACleanup();
    return 0;
}
