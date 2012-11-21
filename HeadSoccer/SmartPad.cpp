#include <process.h>

#include <queue>
#include <vector>
using namespace std;

#include "SmartPad.h"

#pragma comment (lib,"ws2_32")


queue<SP_InputEvent> qInput;

unsigned int hServerThread = -1;
int serverPIN = -1;

CRITICAL_SECTION cs;

SP_Version SP_GetVersion(){
	SP_Version ver;
	ver.major = SP_VERSION_MAJOR;
	ver.minor = SP_VERSION_MINOR;
	ver.fix = SP_VERSION_FIX;
	return ver;
}
void SP_PushEvent(SP_InputEvent event){
	EnterCriticalSection(&cs);
	qInput.push(event);
	LeaveCriticalSection(&cs);
}
bool SP_PollEvent(SP_InputEvent *event){
	EnterCriticalSection(&cs);
	if(qInput.empty()){
		LeaveCriticalSection(&cs);
		return false;
	}
	*event = qInput.back();
	qInput.pop();
	LeaveCriticalSection(&cs);
	return true;
	
}

void SP_Serialize(char *msg,void *data,int len){
	//char *msg = (char *)malloc(sizeof(char*)*len);
	memcpy(msg,data,len);
}
void SP_Unserialize(char *msg,void *data,int len){
	memcpy(data,msg,len);
}


int Send(SOCKET hSocket,void *buf,int len,int flag){
	int _len;
	char *msg = (char *)malloc(sizeof(char*) * len);
	SP_Serialize(msg,buf,len);
	_len = send(hSocket,msg,len,flag);
	return _len;
}
int Recv(SOCKET hSock,void *buf,int len,int flag){
	int _len;
	char *msg = (char *)malloc(sizeof(char*) * len);
	_len = recv(hSock,msg,len,flag);
	SP_Unserialize(msg,buf,len);
	free(msg);
	return _len;
}

int RecvPin(SOCKET hSock){
	SP_Pin packet;
	Recv(hSock,&packet,sizeof(SP_Pin),0);
	if(packet.pin == serverPIN){
		printf("PIN certified\n");
		return 1;
	}
	return 0;
}
int RecvVersion(SOCKET hSock){
	SP_Version packet;
	SP_Version current;

	Recv(hSock,&packet,sizeof(SP_Version),0);
	current = SP_GetVersion();
	if(packet.major == current.major
		&&
		packet.minor == current.minor
		&&
		packet.fix == current.fix){
		printf("version certified\n");
		return 1;
	}
	return 0;
}

int RecvTouchInput(SOCKET hSock){
	SP_Touch packet;
	Recv(hSock,&packet,sizeof(SP_Touch),0);

	SP_InputEvent event;
	event.type = SP_INPUT_TOUCH;
	event.touch = packet;

	SP_PushEvent(event);
	return 0;
}
int RecvGyroInput(SOCKET hSock){
	SP_Gyro packet;
	Recv(hSock,&packet,sizeof(SP_Gyro),0);

	SP_InputEvent event;
	event.type = SP_INPUT_GYRO;
	event.gyro = packet;

	SP_PushEvent(event);
	return 0;
}
int RecvMicInput(SOCKET hSock){
	SP_Mic packet;
	Recv(hSock,&packet,sizeof(SP_Mic),0);

	SP_InputEvent event;
	event.type = SP_INPUT_MIC;
	event.mic = packet;

	SP_PushEvent(event);
	return 0;
}
int RecvDataInput(SOCKET hSock){
	SP_Data packet;
	char *data;

	Recv(hSock,&packet,sizeof(SP_Data),0);

	data = (char*)malloc(packet.size);
	Recv(hSock,data,packet.size,0);

	packet.data = data;

	SP_InputEvent event;
	event.type = SP_INPUT_DATA;
	event.data = packet;

	SP_PushEvent(event);
	return 0;
}

int RecvInput(SOCKET hSock){
	SP_Input packet;
	Recv(hSock,&packet,sizeof(SP_Input),0);

	switch(packet.type){
	case SP_INPUT_TOUCH:
		RecvTouchInput(hSock);
		break;
	case SP_INPUT_GYRO:
		RecvGyroInput(hSock);
		break;
	case SP_INPUT_MIC:
		RecvMicInput(hSock);
		break;
	case SP_INPUT_DATA:
		RecvDataInput(hSock);
		break;
	}
	return 0;
}

unsigned int _stdcall SP_RecvThread(void *arg){
	SOCKET hSock;
	bool pin_certified = false;
	bool version_certified = false;

	hSock = (SOCKET)arg;

	while(hServerThread != -1){
		int len;
		int ret;
		SP_Header header;

		len = Recv(hSock,&header,sizeof(SP_Header),0);

		if((pin_certified == false ||
			version_certified == false)
			&&
			(header.type != SP_PROTOCOL_PIN &&
			 header.type != SP_PROTOCOL_VERSION)){
				break;
		}
		switch(header.type){
		case SP_PROTOCOL_PIN:
			pin_certified = ret = RecvPin(hSock);
			break;
		case SP_PROTOCOL_VERSION:
			version_certified = ret = RecvVersion(hSock);
			break;
		case SP_PROTOCOL_INPUT:
			ret = RecvInput(hSock);
			break;
		default:
			ret = 0;
			break;
		}

		if(ret == -1)
			break;
	}
CloseConnection:;
	closesocket(hSock);
	return 0;
}
unsigned int _stdcall SP_ServerThread(void *arg){
	int port = (int)arg;
	SOCKET hServSock;
	SOCKET hClntSock;

	HANDLE hThread;
	DWORD dwThreadID;

	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddrSize;

	WSAData wsa;

	InitializeCriticalSection(&cs);

	WSAStartup(MAKEWORD(2,2), &wsa);

	hServSock=socket(PF_INET, SOCK_STREAM, 0);   
	if(hServSock == INVALID_SOCKET)
		printf("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(port);

	if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
		printf("bind() error");

	if(listen(hServSock, 5)==SOCKET_ERROR)
		printf("listen() error");

	serverPIN = rand() % 1000 + 1000;
	printf("%d \n", serverPIN);

	printf("Start SmartPad Server on %d\n", port);
	while(1){
		clntAddrSize=sizeof(clntAddr);
		hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
		if(hClntSock==INVALID_SOCKET)
			printf("accept() error");

		printf("connected... %s\n",inet_ntoa(clntAddr.sin_addr));

		hThread = (HANDLE)_beginthreadex(NULL, 0, SP_RecvThread, (void*)hClntSock, 0, (unsigned *)&dwThreadID);
		if(hThread == 0) {
			printf("cannot create client thread\n");
		}
	}

	WSACleanup();
	return 0;
}
int SP_GetPIN(){
	return serverPIN;
}

void SP_StartServer(int port,int maxPad){
	hServerThread = _beginthreadex(NULL,0,SP_ServerThread,(void*)port,NULL,NULL);
}
void SP_StopServer(){
	if(hServerThread != -1){
		_endthreadex(hServerThread);
		hServerThread = -1;
	}
}


SOCKET hServ;

unsigned int _stdcall SP_ClientThread(void *arg){
	SOCKET hSock = (SOCKET)arg;

	while(1){ 
		int len;
		int ret;
		SP_Header header;

		len = Recv(hSock,&header,sizeof(SP_Header),0);

		switch(header.type){
		case SP_PROTOCOL_INPUT:
			ret = RecvInput(hSock);
			break;
		default:
			ret = 0;
			break;
		}

		if(ret == -1)
			break;
	}
CloseConnection:;
	closesocket(hSock);
	return 0;
}

bool SP_Connect(char *addr,int port){
	WSADATA wsaData;
	SOCKADDR_IN servAddr;
	unsigned int hThread;
	hostent *host;
	SOCKET hSocket;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
		return false;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(
										addr);
	//servAddr.sin_addr.s_addr = *(ULONG*)host->h_addr_list[0];
	servAddr.sin_port = htons(
										port);

	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR){
		return false;
	}

	char t = TRUE;
	setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, &t, sizeof(t));

	hThread = _beginthreadex(NULL,NULL,SP_ClientThread,(void*)hSocket,NULL,NULL);

	hServ = hSocket;

	return true;
}
void SP_Disconnect(){
	closesocket(hServ);
	WSACleanup();
}


void SP_SendHeader(unsigned char type){
	SP_Header header;
	header.type = type;
	SP_Send(hServ,(void*)&header,sizeof(SP_Header),0);
}
void SP_SendInput(unsigned char type){
	SP_Input input;
	input.type = type;
	SP_SendHeader(SP_PROTOCOL_INPUT);
	SP_Send(hServ,(void*)&input,sizeof(SP_Input),0);
}

void SP_SendTouchInput(SP_Touch *touch){
	SP_SendInput(SP_INPUT_TOUCH);
	SP_Send(hServ,(void*)touch,sizeof(SP_Touch),0);
}
void SP_SendGyroInput(SP_Touch *gyro){
	SP_SendInput(SP_INPUT_GYRO);
	SP_Send(hServ,(void*)gyro,sizeof(SP_Gyro),0);
}
void SP_SendMicInput(SP_Touch *mic){
	SP_SendInput(SP_INPUT_MIC);
	SP_Send(hServ,(void*)mic,sizeof(SP_Mic),0);
}
void SP_SendDataInput(SP_Data *data){
	SP_SendInput(SP_INPUT_DATA);
	SP_Send(hServ,(void*)data,sizeof(SP_Data),0);
}

void SP_SendPIN(int pin){
	SP_Pin _pin;
	_pin.pin = pin;
	SP_SendHeader(SP_PROTOCOL_PIN);
	SP_Send(hServ,&_pin,sizeof(SP_Pin),0);
}
void SP_SendVersion(SP_Version ver){
	SP_SendHeader(SP_PROTOCOL_VERSION);
	SP_Send(hServ,&ver,sizeof(SP_Version),0);
}