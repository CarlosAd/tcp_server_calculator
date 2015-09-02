# Tcp Server Calculator

## Descrição
Este projeto visa desenvolver uma calculadora para operações matriciais simples, executada em um servidor. A comunicação com o cliente é feita atráves de comunicação TCP.

## Instruções de Instalação
A princípio, este projeto só pode ser compilado em sistemas Unix. Para compilá-lo, siga os seguintes passos:

1. Crie um diretório ***build*** dentro do diretório do projeto e acesse-o.<br />
$ mkdir build<br />
$ cd build<br />

2. Execute o CMake no diretório anterior<br />
$ cmake ..<br />

3. Compile o projeto<br />
$ make<br />

## Instruções para utilização
No diretório ***build/bin***, serão gerados os dois executáveis, *server* e *client*.<br />
Para executar o *server*, escreva<br />
$ ./server <número da porta>
Para executar o *client*, escreva<br />
$ ./server <indereço IP do servidor> <número da porta>
