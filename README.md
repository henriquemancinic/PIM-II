# PIM-II
Criação de um programa em C, com telas de login, menu, relatórios, cadastrar funcionários, clientes e um plus que é realizar uma comunicação cliente-servidor via sockets por TCP/IP e a realização do NAS

## COMPILANDO O PROJETO
	
Como o software faz o uso de bibliotecas especificas como winsock, para realizar as comunicações com o servidor via sockets tcp/ip, precisamos linkar a biblioteca ao nosso ambiente de compilação.

- **Code::Blocks**

  Na barra superior, procure por settings > Compiler...
  
  ![image](https://user-images.githubusercontent.com/55888586/204656915-f2d53ecf-632c-4536-a461-2a9f6c813272.png)
  
  Clique em Linker settings > Add e procure a pasta onde está localizado o MinGW>lib e procure pela biblioteca libws2_32.a e dê o ok, para prosseguir, feito isto, o compilador já estará configurado para a compilação e execução do código.

- **Visual Studio Code**

  No Visual Studio Code, o processo é mais rápido, vá para extensões, clique no ícone de configurações e em seguida Extension Settings, procure por C-flags e adicione -lws2_32.
  
  ![image](https://user-images.githubusercontent.com/55888586/204657356-91ca07d9-d6e0-40cb-bbb3-63f01ec457d0.png)

- **Via CMD**

  Abra o cmd e vá para a pasta em que se encontra o código fonte e digite esse comand:
gcc main.c -o main -Wall -Wextra -lws2_32
  
  ![image](https://user-images.githubusercontent.com/55888586/204657741-4329c35e-beaa-458d-834c-6e6c6cd007bf.png)
  
## CONFIGURANDO AMBIENTE
Para de iniciar o software, deverá ser criado um arquivo .txt usando a seguinte estrutura:
  
  - 0|Moacir|fnd0|05210342|0|
  - 1|Assis|fnd1|05210343|0|
  - 2|marcelo|marcelo|20002502041522|1|
  - 3|alberto queiroz|alberto|00150104253122|2|
  
Após copiar a estrutura, salve no mesmo diretório em que o sistema está localizado e com o nome registroLS.
