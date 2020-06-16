//udp_client.h
#define BUFLEN 4096
#define SHUTDOWN 0
#define DOWNLOAD 1
#define UPLOAD 2
#define YES 3
#define NO 4
#define START 5
#define END 6
#define CONTENT 7
#define OK 8

struct packet
{
	int command;
	int buflen;
	int seq;
	char buf[BUFLEN];
};

int socketfd;
int addr_len;
struct sockaddr_in server;
struct packet send_packet;
struct packet recv_packet;
int length;
char ip[20];
int port;
int command;
char file_path[256];

void ShowMenu();
void DownLoad();
void UpLoad();
void ShutDown();
#pragma once
