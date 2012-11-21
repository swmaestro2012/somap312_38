#ifndef SMARTPAD_H
#define SMARTPAD_H

//#include <WinSock2.h>
#include <Windows.h>

#define SP_VERSION_MAJOR 1 
#define SP_VERSION_MINOR 0
#define SP_VERSION_FIX   0

#define SP_TOUCH_DOWN	1
#define SP_TOUCH_UP		2

enum SP_InputType{
	SP_INPUT_NONE = 0,

	SP_INPUT_TOUCH,
	SP_INPUT_GYRO,
	SP_INPUT_MIC,
	SP_INPUT_DATA
};
enum SP_Protocol{
	SP_PROTOCOL_NONE = 0,
	SP_PROTOCOL_PIN,
	SP_PROTOCOL_VERSION,
	SP_PROTOCOL_INPUT,
	SP_PROTOCOL_DATA,
	SP_PROTOCOL_CLOSE
};
enum SP_DrawPacket{
	SP_DRAW_NONE = 0,
	SP_DRAW_CLEAR,
	SP_DRAW_FLUSH,
	SP_DRAW_COLOR,
	SP_DRAW_BLEND,
	SP_DRAW_POINT,
	SP_DRAW_LINE,
	SP_DRAW_RECT,
	SP_DRAW_FILLRECT,

	SP_DRAW_TEXTURE
};

struct SP_Header{
	unsigned char type;
};

struct SP_Pin{
	int pin;
};
struct SP_Version{
	unsigned char major;
	unsigned char minor;
	unsigned char fix;
};
struct SP_Input{
	unsigned char type;
};
struct SP_Data{
	unsigned int size;
	char *data;
};

struct SP_Touch{
	int x,y;
	int state;
};
struct SP_Gyro{
	float x,y,z;
};
struct SP_Mic{
	int none;
};

struct SP_InputEvent{
	unsigned char type;

	SP_Touch touch;
	SP_Gyro gyro;
	SP_Mic mic;
	SP_Data data;
};

bool SP_PollEvent(SP_InputEvent *event);
void SP_PushEvent(SP_InputEvent event);

int Send(SOCKET hSocket,void *buf,int len,int flag=0);
int Recv(SOCKET hSocket,void *buf,int len,int flag=0);
#define SP_Send(a,b,c,d) Send(a,b,c,d)
#define SP_Recv(a,b,c,d) Recv(a,b,c,d)

void SP_StartServer(int port,int maxPad = 1);
void SP_StopServer();

bool SP_Connect(char *addr,int port);
void SP_Disconnect();

void SP_SendTouchInput(SP_Touch *touch);
void SP_SendGyroInput(SP_Touch *gyro);
void SP_SendMicInput(SP_Touch *mic);
void SP_SendDataInput(SP_Data *data);

int SP_GetPIN();
void SP_SendPIN(int pin);
SP_Version SP_GetVersion();
void SP_SendVersion(SP_Version ver);

#endif //SMARTPAD_H