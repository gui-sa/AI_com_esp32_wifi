/*Name : Jay Jagtap
3154037
Problem Statement: To achieve File transfer using TCP/IP Protocol
*/

/*
	Server Side
	Please pass port no as command line argument
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno; //Cria o sockfd e newsockfd como file descriptor e o numero da porta
     socklen_t clilen; //um tipo, que armazena um dado inteiro
     char buffer[512];//Um buffer... Qual o maximo a ser enviado ou recebido?
     struct sockaddr_in serv_addr, cli_addr;//Criar uma estrutura para registrar o servidor e o cliente
     int n; //Um file descriptor para o envio
     if (argc < 2) {  //Para checar se o usuario enviou os argumentos minimos para o funcionamento
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);// Criando um socket no formato TCP para o Internet Protocol v4 addresses
     if (sockfd < 0)  //  se der tudo certo, ele retorna um fildes
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));//colocamos zero no serv_addr
     portno = atoi(argv[1]);//Captamos no segundo endereço, o endereço da porta
     serv_addr.sin_family = AF_INET; // Configuramos o servidor como TCP no IPV4
     serv_addr.sin_addr.s_addr = INADDR_ANY; //Linka o IP da maquina com TODAS as interfaces de rede.
     serv_addr.sin_port = htons(portno);//NESTA porta... htons converte o integer para um formato que a funcao aceita
     if (bind(sockfd, (struct sockaddr *) &serv_addr, // Depois de configurarmos o servidor, finalmente linkamos o IP à todas interfaces
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");//se o bind nao retornar um numero maior que zero.. deu erro
     listen(sockfd,5); // Ouvimos o filedescritor no servidor... Deixamos conectar um maximo de 5 clients
     clilen = sizeof(cli_addr);//Obtemos o tamanho do endereço do cliente
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen); //criamos um novo file descriptor associado ao listen.. e dizemos que este vai colocar seus dados no cli_addr
     if (newsockfd < 0) //se o filedescriptor for menor que zero... erro.
          error("ERROR on accept");
          
          
          
          FILE *fp;  //Criar um ponteiro de arquivo
         int ch = 0;
            fp = fopen("glad_receive.txt","a");            
            int words;
		read(newsockfd, &words, sizeof(int));// O cliente primeiro manda quantas palavras o arquivo possui
          while(ch != words)  // Enquanto as palavras nao chegarem, receba e escreva neste novo arquivo
       	   {
        	 read(newsockfd , buffer , 512); 
	   	 fprintf(fp , " %s" , buffer);   
		 //printf(" %s %d "  , buffer , ch); //Line for Testing , Ignore
		 ch++;
	       }
     	printf("The file was received successfully\n");
	   printf("The new file created is glad5.txt");
     close(newsockfd);// encerra comunicação
     close(sockfd);//encerra comunicação
     return 0; 
}
