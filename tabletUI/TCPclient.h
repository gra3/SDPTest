#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_net.h>

using namespace std;

TCPsocket clientSoc;
IPaddress serverIP;
SDLNet_SocketSet set;
char playerNumber[2];
int playerNumberInt;

const int MAXLEN = 200;
char msg[MAXLEN];

string partial;
int missing;

void initSDL()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}

	//Initialize SDL_Net
	if( SDLNet_Init()==1)
	{
		printf( "SDLNet_Init: %s\n", SDLNet_GetError());
	}

	set = SDLNet_AllocSocketSet(1);

	//Read playerNumber from file
	ifstream data;
	data.open("settings.txt");
	while(!data.eof())
	{
		data >> playerNumber;
	}
	//data.close();
	data.clear();
	data.seekg(0, ios::beg);
	while(!data.eof())
	{
		data >> playerNumberInt;
	}
	data.close();
}

void quitSDL()
{
	SDL_Quit();
	SDLNet_Quit();
}

bool connectToServer()
{
	if(SDLNet_ResolveHost(&serverIP, "192.168.25.100", 9001)==-1) 
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return false;
	}
	clientSoc = SDLNet_TCP_Open(&serverIP);
	if(!clientSoc) 
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return false;
	}
	else
	{
		SDLNet_TCP_AddSocket(set,clientSoc);
		SDLNet_TCP_Send(clientSoc,playerNumber,2);
		cout << "Connected to Server!!!\n";
		partial = "";
		missing = 0;
		return true;
	}
	//SDLNet_TCP_Send(clientSoc,playerNumber,2);
}

bool sendServer(char msg[])
{
	int result;
	result = SDLNet_TCP_Send(clientSoc,msg,strlen(msg)+1);
	if(result!=strlen(msg)+1) return false;
	else return true;
}

//bool getData(char* &outStr)
//{
//	int numActive = SDLNet_CheckSockets(set,0);
//	if(numActive>0)
//	{
//		if(SDLNet_SocketReady(clientSoc))
//		{
//			int result;
//			result = SDLNet_TCP_Recv(clientSoc,msg,MAXLEN);
//			cout << "NUMBER OF BYTES RECIEVED: " << result << endl;
//			cout << msg << endl;
//			if(result>1)
//			{
//				outStr = msg;
//				return true;
//			}
//			else return false;
//		}
//		else return false;
//	}
//	else return false;
//}

bool getData(string &outString)
{
	int numActive = SDLNet_CheckSockets(set,0);
	if(numActive>0)
	{
		if(SDLNet_SocketReady(clientSoc))
		{
			int result;
			result = SDLNet_TCP_Recv(clientSoc,msg,100);
			if(result>1)
			{
				string temp = msg;
				size_t found = temp.find("\n");

				//Case where recv'd has full message
				//if(found == temp.length()-1)
				//{
					//cout << "Case 1:   " << "found: " << found << "   temp.length()-1: " << temp.length()-1 << endl;
					outString = temp;
					return true;
				//}
				//Case where truncated
				//else
				//{
					outString = outString;
					//cout << "full msg not recieved" << endl;
					//string temp = msg;
					//int found;
					//while((found = temp.find("\n"))!=string::npos)
					//{
					//	outString = partial + temp.substr(0,found);
					//	temp.erase(0,found);
					//	partial = temp;
					//}
					//return false;
				//}
			}//else return false;
		}
		else return false;
	}
	else return false;
}
