// FTP server
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <string.h>

# define UDPPORT 5000
# define TCPPORT 9000
# define BUFFERSIZE 1024

int main ()
{       int i,j,size=0,r;
	int req_socket,conn_socket,bytes_sent,bytes_received,socku_len,sockt_len,connect_status,bind_status;
	struct sockaddr_in sock_tserver,sock_tclient,sock_userver,sock_uclient;
	char recv_buffer[BUFFERSIZE],send_buffer[BUFFERSIZE],filename[200],filesize[200],stop[30];
	socku_len=sizeof(sock_uclient);
	sockt_len=sizeof(sock_tclient);
	
	// Creating the UDP control socket
	int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP) ;
	
	if (sock<0)
	{
		printf("Socket creation failed.\n") ;
	}
	else
	{
		printf("Socket created with descriptor %d \n ",sock);
	}

	// Initializing and binding the UDP socket
	sock_userver.sin_family=AF_INET ;
	sock_userver.sin_port=htons(UDPPORT) ;
	sock_userver.sin_addr.s_addr=htonl(INADDR_ANY);

	bind_status= bind(sock,(struct sockaddr*) &sock_userver,sizeof(sock_userver));
	
	if (bind_status<0)
	{
		printf("Binding socket failed.\n");
	}
	else
	{
		printf("Binding socket successful.\n");
	}

		bytes_received=recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(struct sockaddr*) &sock_uclient, &socku_len);
		for(i=4;recv_buffer[i]!='\0';i++)
		{
			j=i-4;				
			filename[j]=recv_buffer[i];
		}
		
		filename[j]='\0';
		printf("File name \' %s \'received from %s : %d \n",filename,inet_ntoa(sock_uclient.sin_addr),sock_uclient.sin_port);

		bytes_received=recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(struct sockaddr*) &sock_uclient, &socku_len);
		for(i=4;recv_buffer[i]!='\0';i++)
		{
			j=i-4;				
			filesize[j]=recv_buffer[i];
		}
		
		filesize[j]='\0';
		r=atoi(filesize);
		printf("File size \' %d \' received from %s : %d \n",r,inet_ntoa(sock_uclient.sin_addr),sock_uclient.sin_port);
	
		//File to write
		FILE *file;
		file=fopen(filename,"w");
		
		// Create server socket for requests
		req_socket=socket(AF_INET,SOCK_STREAM,0);
		if(req_socket==-1)
		{
			printf ("Socket creation failed.\n ");
			close(req_socket);
			return 1;
		}
		else
		{
			printf ("Socket creation successful with descriptor %d.\n" ,req_socket);
		}
		
		// Binding the socket
		sock_tserver.sin_family=AF_INET;
		sock_tserver.sin_addr.s_addr=htonl (INADDR_ANY);
		sock_tserver.sin_port=htons(TCPPORT);
		bind_status=bind(req_socket,(struct sockaddr*) &sock_tserver,sizeof(sock_tserver));
		if(bind_status==1)
		{
			printf("Socket binding failed.\n");
			close(req_socket);
			return 1;
		}
		else
		{
			printf("Socket binding successful.\n");
		}

		// Listen for TCP connection requests
		printf("Listening for connection requests.\n");
	
		listen(req_socket,5);
		conn_socket=accept(req_socket,(struct sockaddr*) &sock_tclient,&sockt_len);
		if (conn_socket==-1)
		{
			printf("Socket creation for client failed.\n");
			close(conn_socket);
			close(req_socket);
			return 1;
		}
		else
		{
			printf ("Socket created for client with descriptor %d.\n",conn_socket);
		}
	
		//Receiving and Writing to the file
		while(size<s)
		{
			bytes_received = recv(conn_socket,recv_buffer,BUFFERSIZE,0);
			fwrite(recv_buffer,1,bytes_received,file);
			size= size+bytes_received;	
		}
		if(size==s)
		{
			printf("File received successfully. \n");
		}
		fclose(file);

		bytes_received=recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(struct sockaddr*) &sock_uclient, &socku_len);


		while(1)
		{
		if (strstr(recv_buffer,"close") != NULL)
		{
			close(sock);	
			close(req_socket);
			close(conn_socket);
			printf("disconnection with the client successful \n");
		}
		break;
		}
		
		
	return 0;

}




