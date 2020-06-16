// udp_client.cpp
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <winsock2.h> 
#include "client.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)


int main() {
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	//创建UDP socket
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket error\n");
		exit(1);
	}

	printf("Client: Please input the ip:\n");
	scanf("%s", ip);
	printf("Please input the port:\n");
	scanf("%d", &port);

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip);
	addr_len = sizeof(server);

	while (1) {
		ShowMenu();
		scanf("%d", &command);
		if (command == SHUTDOWN) {
			ShutDown();
			break;
		}
		else if (command == DOWNLOAD) {
			DownLoad();
		}
		else if (command == UPLOAD) {
			UpLoad();
		}
		else {
			printf("Please make a right choice!\n");
		}
	}
	closesocket(socketfd);
	return 0;
}


void ShowMenu() {
	printf("=====================================\n");
	printf("         Please make a choice:       \n");
	printf("         0:  ||shutdown||            \n");
	printf("         1:  ||download||            \n");
	printf("         2:  || upload ||            \n");
	printf("=====================================\n");
}
void DownLoad() {
	memset(&recv_packet, 0, sizeof(recv_packet));
	memset(&send_packet, 0, sizeof(send_packet));
	memset(file_path, 0, sizeof(file_path));
	printf("Please input the file_path:\n");
	scanf("%s", send_packet.buf);
	send_packet.command = DOWNLOAD;
	sendto(socketfd, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr *)&server, sizeof(server));
	memcpy(send_packet.buf, file_path, strlen(send_packet.buf));
	recvfrom(socketfd, (char*)&recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&server, &addr_len);
	if (recv_packet.command == YES) {
		printf("YES!\n");
		int command;
		printf("Press 5 to start  transmission!\n");
		scanf("%d", &command);
		printf("Please input the download path:\n");
		memset(file_path, 0, sizeof(file_path));
		scanf("%s", file_path);
		if (command == START) {
			send_packet.command = START;
			sendto(socketfd, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr *)&server, sizeof(server));
			int packet_seq = 0;
			FILE *fd = fopen(file_path, "ab");
			if (fd < 0) {
				perror("create file error\n");
				exit(1);
			}
			memset(&recv_packet, 0, sizeof(recv_packet));
			while ((length = recvfrom(socketfd, (char*)&recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&server, &addr_len)) > 0) {
				if (recv_packet.command == CONTENT) {
					if (packet_seq == recv_packet.seq) {
						fwrite(recv_packet.buf, 1, recv_packet.buflen, fd);
						printf("传输数据中... %s\n", recv_packet.buf);
						memset(&recv_packet, 0, sizeof(recv_packet));
						packet_seq++;
					}
					else {
						perror("The file no is not same.Some message is missed! error occured! \n");
						break;
					}
				}
				if (recv_packet.command == END) {
					fclose(fd);
					printf("transmission is successful!\n");
					break;
				}
			}
		}
	}
	else if (recv_packet.command == NO) {
		perror("NO such file on server!\n");
	}
	else {
		perror("recvbuf.command error\n");
		exit(1);
	}
}
void UpLoad() {
	memset(&recv_packet, 0, sizeof(recv_packet));
	memset(&send_packet, 0, sizeof(send_packet));
	memset(file_path, 0, sizeof(file_path));
	printf("Please input the file name:\n");
	scanf("%s", send_packet.buf);
	send_packet.command = UPLOAD;
	sendto(socketfd, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr *)&server, sizeof(server));
	memcpy(file_path, send_packet.buf, strlen(send_packet.buf));

	FILE *fd = fopen(file_path, "rb");
	if (fd < 0) {
		perror("The file you want to trans is not exist!\n");
		exit(1);
	}
	recvfrom(socketfd, (char*)&recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)&server, &addr_len);
	if (recv_packet.command == START) {
		int packet_seq = 0;
		memset(&send_packet, 0, sizeof(send_packet));
		while ((length = fread(send_packet.buf, 1, BUFLEN, fd)) > 0) {
			send_packet.seq = packet_seq;
			send_packet.command = CONTENT;
			send_packet.buflen = length;
			sendto(socketfd, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr *)&server, sizeof(server));
			packet_seq++;
			memset(&send_packet, 0, sizeof(send_packet));
			Sleep(10);
		}
		memset(&send_packet, 0, sizeof(send_packet));
		send_packet.command = END;
		sendto(socketfd, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr *)&server, sizeof(server));
	}
	else if (recv_packet.command == NO) {
		printf("not transmission\n");
	}
	else {
		perror("error! wrong choice!\n");
	}
}

void ShutDown() {
	printf("client is shutdown now!\n");
	memset(&send_packet, 0, sizeof(send_packet));
	send_packet.command == SHUTDOWN;
	sendto(socketfd, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr *)&server, sizeof(server));
}
