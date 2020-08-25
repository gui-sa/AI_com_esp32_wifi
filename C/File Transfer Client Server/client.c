/*Name : Jay Jagtap
3154037
Problem Statement: To achieve File transfer using TCP/IP Protocol
*/

/*
	Client Side
	Please pass ip address and port no as command line arguments in the same sequence
	glad.txt file should be already created before you run the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n; //Cria sockfd e n como file descriptor e portno para receber o numero da porta
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[512]; // o buffer, que registra quantos chars o codigo espera enviar ou receber.
    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);  //recebe do prompt de comando. é o  terceiro argumento.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//socket TCP para ipv4
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]); //A função gethostbyname capta o servidor e completa o hostent... Ela retorna NULL se o server nao existit
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); //limpa a variavel
    serv_addr.sin_family = AF_INET; // configura o tipo de endereco para IPV4
    bcopy((char *)server->h_addr,  
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length); // Copia as informações da struct hostent para a serv_addr, adress
    serv_addr.sin_port = htons(portno); // Nesta porta
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // Conectar-se a este endereço... o endereço do server
        error("ERROR connecting");
  
        bzero(buffer,512); // Limpar o buffer
        
    FILE *f; // abrir ponteiro de arquivo
    
    int words = 0;// falar que temos words
    char c; 
     f=fopen("glad.txt","r"); // abre o arquivo de nome glad.txt como leitura
    while((c=getc(f))!=EOF)			//contar todas as palavras que contem no arquivo
	{	
		fscanf(f , "%s" , buffer);
		if(isspace(c)||c=='\t')
		words++;	
	}
	//printf("Words = %d \n"  , words);	//Ignore
       
      
	write(sockfd, &words, sizeof(int));// Enviar o numero de palavras
     	rewind(f);// retorna o ponteiro do arquivo para a posição incial
      

       
    char ch ;
       while(ch != EOF)  //Envia TODAS AS STRINGS
      {
		
		fscanf(f , "%s" , buffer);
		//printf("%s\n" , buffer);	//Ignore
		write(sockfd,buffer,512);
		ch = fgetc(f);
      }
	printf("The file was sent successfully");
    
    close(sockfd);//encerra comunicação
    return 0;
}
