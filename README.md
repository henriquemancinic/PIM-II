# PIM-II
Criação de um programa em C, com telas de login, menu, relatórios, cadastrar funcionários, clientes e um plus que é realizar uma comunicação cliente-servidor via sockets por TCP/IP e a realização do NAS

## PRINCIPAIS TELAS DO PROJETO
O software possui três níveis de usuários e um total de 13 telas.

- **Login**

	<img src="https://user-images.githubusercontent.com/55888586/208312223-b23ac2f1-82c9-4c04-bbd8-dc0369b214bf.png"  width="500">

- **Fundador**

	<img src="https://user-images.githubusercontent.com/55888586/208311585-aa3fa784-aecc-4d64-a529-e31fa3b33c35.png"  width="500">

- **Recepcionista**

	<img src="https://user-images.githubusercontent.com/55888586/208311620-138e3b25-a46e-46f4-b69e-471d4d8de945.png"  width="500">

- **Dentista**

	<img src="https://user-images.githubusercontent.com/55888586/208311630-a78980fd-0f03-4033-a684-aad044134a2c.png"  width="500">

## COMPILANDO O PROJETO
	
Como o software faz o uso de bibliotecas especificas como winsock, para realizar as comunicações com o servidor via sockets tcp/ip, precisamos linkar a biblioteca ao nosso ambiente de compilação.

- **Code::Blocks**

  Na barra superior, procure por settings > Compiler...
  
  ![image](https://user-images.githubusercontent.com/55888586/204656915-f2d53ecf-632c-4536-a461-2a9f6c813272.png)
  
  Clique em Linker settings > Add e procure a pasta onde está localizado o MinGW>lib e procure pela biblioteca libws2_32.a e dê o ok, para prosseguir, feito isto, o compilador já estará configurado para a compilação e execução do código.
  
  ![image](https://user-images.githubusercontent.com/55888586/204657356-91ca07d9-d6e0-40cb-bbb3-63f01ec457d0.png)

- **Visual Studio Code**

  No Visual Studio Code, o processo é mais rápido, vá para extensões, clique no ícone de configurações e em seguida Extension Settings, procure por C-flags e adicione -lws2_32.
  
  ![image](https://user-images.githubusercontent.com/55888586/204663548-72db7183-ec73-481a-bce3-5d498956a83a.png)

- **Via CMD**

  Abra o cmd e vá para a pasta em que se encontra o código fonte e digite esse comand:
  
      gcc main.c -o main -Wall -Wextra -lws2_32
  
  ![image](https://user-images.githubusercontent.com/55888586/204657741-4329c35e-beaa-458d-834c-6e6c6cd007bf.png)
  
## CONFIGURANDO AMBIENTE
Para de iniciar o software, deverá ser criado um arquivo .txt usando a seguinte estrutura:
  
    0|Moacir|fnd0|05210342|0|
    1|Assis|fnd1|05210343|0|
    2|marcelo|marcelo|20002502041522|1|
    3|alberto queiroz|alberto|00150104253122|2|
  
Após copiar a estrutura, salve no mesmo diretório em que o sistema está localizado e com o nome registroLS.

![image](https://user-images.githubusercontent.com/55888586/204663751-c548ad19-6154-411a-9766-2a41a3fe40f2.png)

> Código Funcionário|Nome|Login|Senha|Tipo Funcionário|


Feito isto, é só entrar no sistema utilizando o login desejado, caso queira entrar com o login de fundador, poderá utilizar os logins fnd0 e fnd1, as senhas estão criptografadas, mas não se preocupe, pois o login e senha estão iguais.

## LINK DA DOCUMENTAÇÃO
https://drive.google.com/file/d/1eqtrhjDpyP-EN95SpRWqHmD0mq1hxxX8/view
