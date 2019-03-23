//FTP client
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#define UDPPORT 5000
#define TCPPORT 9000
#define BUFFERSIZE 1024
#define SERVERADDRESS "127.0.0.1"

int main ()
{	
	int conn_socket,bytes_received,bytes_sent,conn_status,udpsock,sze,size,read_byte;
	struct sockaddr_in sock_tserver,sock_tclient,sock_userver;
	char send_buffer[BUFFERSIZE],filename[100],filesize[100],temp[100],filestatus[20],d[100];
	int socku_len = sizeof(sock_userver),sockt_len=sizeof(sock_tserver); 
	char send_buf[BUFFERSIZE];
	char *name="NAME";
	char *fsze="SIZE";
	char *stop="CLOSE";
	
	// Creating the control client socket
	udpsock = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
	if (udpsock< 0)
	{
		printf(" Socket creation failed .\n");
		return 1;
	}
	else
	{
		printf(" Socket creation successful at %d\n" ,udpsock );
	}

	// Assigning server IP and port for sending control information
	sock_userver.sin_family = AF_INET ;
	sock_userver.sin_port = htons (UDPPORT) ;
	if (!inet_aton(SERVERADDRESS,&sock_userver.sin_addr))
	{
		printf(" IP network format conversion failed .\n"); 
		return 1;
	}
	
		//Requesting the file name
		printf(" Enter the file name to be sent to the server: \n");
		scanf("%s",filename);
		strcpy(d,filename);
		bzero(temp,strlen(temp));
		strcpy(temp,name);
		strcat(temp,filename);	
		strcpy(filename,temp);
	
		// Sending the filename to the server
		bytes_sent=sendto(udpsock,filename,sizeof(filename), 0,(struct sockaddr *)&sock_userver,socku_len);
		printf("%d bytes sent.\n",bytes_sent);
		strcpy(filename,d);

		//Open and determine file size	
		FILE* file;
		file=fopen(filename,"r");
		fseek(file,0L,2);
		sze=ftell(file);
		rewind(file);

		//Sending the file size
		printf("The file size is %d \n.",sze);
		sprintf(filesize,"%s%d",fsze,sze);
		bytes_sent=sendto(udpsock,filesize,sizeof(filesize), 0,(struct sockaddr *)&sock_userver,socku_len);

		// Create a data connection socket for client
		conn_socket=socket(AF_INET,SOCK_STREAM,0);
		if (conn_socket==-1)
		{
			printf("Socket creation failed.\n");
			close(conn_socket);
			return 1;
		}
		else
		{
			printf("Socket creation successful with descriptor %d.\n",conn_socket);
		}
	
		// Requesting for a TCP connection to the server
		memset(&sock_tserver,0,sizeof(sock_tserver));
		sock_tserver.sin_family=AF_INET;
		inet_aton(SERVERADDRESS,&sock_tserver.sin_addr);
		sock_tserver.sin_port=htons(TCPPORT);

		while(1)
		{
			conn_status=connect(conn_socket,(struct sockaddr*) &sock_tserver,sockt_len);
			if(conn_status!=-1)
			{
				printf("Connected to the server.\n");
				break;	
			}
		}

		bzero(send_buffer,1024);
		//Reading and Sending
		size=0;
		int i;
		while(size<sze)
		{
			read_byte=fread(send_buffer,1,5,file);
			i=0;
			while(read_byte>0)
			{
				bytes_sent=send(conn_socket,&send_buffer[i],read_byte,0);
				read_byte=read_byte-bytes_sent;
				i=i+bytes_sent;
			}
			size=size+bytes_sent;	
		}
		printf("\n File Sent\n ");
		
		printf(" Want to close connection with the server -> enter close \n ");
		scanf("%s",temp);
		bytes_sent=sendto(udpsock,temp,sizeof(temp), 0,(struct sockaddr *)&sock_userver,socku_len);
		close(conn_socket);
	return 0;
}
