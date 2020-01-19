#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define gif "image/gif"
#define text "text/plain"
#define jpg "image/jpg"
#define jpeg "image/gpeg"
#define png "image/png"
#define zip "image/zip"
#define gz "image/gz"
#define tar "image/tar"
#define htm "text/html"
#define html "text/html"
#define pdf "application/pdf"
#define mp3 "audio/mpeg"
#define mp4 "video/mp4"
#define css "text/css"

int port = 8000;

char *mime_type;

char *page ="<!DOCTYPE html>"
		"<html>"
			"<head><title>Client Web Server</title></head>"
			"<body>"
				"<center>"
					"<h1>Web Server inplemented by Using C </h1>\r\n"
				"</center>"
				"<hr>"
					"<h2>This contains images, videos,audios,pdf and text files </h2>"
					"<hr>"
					"<h3>forest.jpg -image</h3>"
					"<h3>flower.jpg -image</h3>"
					"<h3>readme.pdf-pdf</h3>"
					"<h3>song.mp3-audio</h3>"
					"<h3>tree.png-image</h3>"
					"<h3>car.png-image</h3>"
					"<h3>video.mp4-video</h3>"
			"</body>"
		"</html>";



void send_responce(int client_socket, char *header, void *body,int contentLength) {
  
  char response[contentLength+300]; 
  //Request header to the response
  int response_length = sprintf(response,
                "%s\n"
                "Connection: close\n"
                "Content-Length: %d\n"
                "Content-Type: %s\n"
                "\n",
                header,
                contentLength, 
                mime_type);
  //Response is append to the body
  memcpy(response + response_length, body, contentLength);
  send(client_socket, response, response_length + contentLength, 0);

}


int main(int argc,char *argv[]){
	
		int server_socket;
		int client_socket;

		char buffer[1024]={0};
		char requestType[4];
		char requestpath[1024];

		//create a socket
		server_socket=socket(AF_INET,SOCK_STREAM,0);
		if(server_socket>0){
			printf("The socket is created\n");
		}else{
			printf("The socket is no created\n");
		}

		//specify the addresss for the socket
		struct sockaddr_in server;
		server.sin_family=AF_INET;
		server.sin_addr.s_addr=INADDR_ANY;
		server.sin_port=htons(port);


		//bind  a socket to an address
		if(bind(server_socket,(struct sockaddr*)&server,sizeof(server))==0){
			printf("Bind the socket\n");
		}else{
			printf("Bind Fail\n");
		}

		//Server listen
		if(listen(server_socket,1)<0){
			//printf("Listen the socket\n");
			perror("Listen the socket\n");
			return -1;
		}


		printf("Server Listen port is : %d\n",port);

		while(1){
			
			//accept the request from server
			if((client_socket=accept(server_socket,(struct sockaddr*)&server,(socklen_t*)&server))==-1){
				perror("Accept Fail\n");
				return -1;
			}else{
				printf("Accepted a new connection ");
			}
			
			//read(client_socket,buffer,1024);
			//sscanf(buffer,"%s %s",requestType,requestpath);
			//printf("Request Type %s\n",buffer);

			//if((strcmp(buffer,"return")==fork())==0){
				//close(server_socket);
				//while(1){
					//recv(client_socket,buffer,1024,0);
					//if(strcmp(buffer,"Exit")==0){
				//printf("Disconnected from %s:%d\n",inet_ntoa(server.sin_addr),ntohs(server.sin_port));
				//break;

				//}else{
			//Send and receive message
					read(client_socket,buffer,1024);
					sscanf(buffer,"%s %s",requestType,requestpath);
					printf("Request Type %s\n",buffer);
				//}
				//}
			//}
			//char* message ="Hello! I'm a server. I'm running now to the OS. I'm a requesting server" ;
			//write(client_socket,message,strlen(message));

			
			//write(client_socket,page,sizeof(page)-1);
			
			char *mime = strrchr(requestpath, '.')+1;
        		char *name = strtok(requestpath, "/");
			
       			 if(mime){
           			 mime_type = mime;
       			 } else{
            			mime_type = NULL;
			 }


        		if (!strcmp(requestType, "GET") && !strcmp(requestpath, "/")) {
            			//char *data = "hello get request";
	    			 char *data=page;
           			 mime_type = html;
				 char* statusCode="HTTP/1.1 200 OK";
				 printf("Status Code : %s\n",statusCode);
				 printf("Request Type %s\n",buffer);
            			 send_responce(client_socket,statusCode, data, strlen(data));
       		        }else if (!strcmp(requestType, "POST") && !strcmp(requestpath, "/")) {
                                 char *data = "hello post request";
                                 mime_type= html;
				 char* statusCode="HTTP/1.1 200 OK";
				 printf("Status Code : %s\n",statusCode);
				 printf("Request Type %s\n",buffer);
                                 send_responce(client_socket,statusCode, data, strlen(data));
                        }else{
                                 char *source;
    				 FILE *file = fopen(name, "r");
                                 size_t bufsize;
                                 if (file != NULL) {
					char* statusCode="HTTP/1.1 200 OK";
				 	printf("Status Code : %s\n",statusCode);
					printf("Request Type %s\n",buffer);
       					 if (fseek(file, 0L, SEEK_END) == 0) {
            					bufsize = ftell(file);
           					source = malloc(sizeof(char) * (bufsize + 1));
            					fseek(file, 0L, SEEK_SET);    
            					fread(source, sizeof(char), bufsize, file);
                                                send_responce(client_socket,statusCode, source, bufsize);
       					 }
                           		 free(source);
                                         fclose(file);
   			         }else{
					char* statusCode="HTTP/1.1 404 NOT FOUND";
				 	printf("Status Code : %s\n",statusCode);
					printf("Request Type %s\n",buffer);
                                        char *error = "<center>file not found</center>";
                                        mime_type = html;
                                        send_responce(client_socket,statusCode, error, strlen(error));
                                 }
  
                        }  
  
			close(client_socket);

		}
	
		close(server_socket);
		return 0;
}



	






		
