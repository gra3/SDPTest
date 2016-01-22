#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_net.h>

using namespace std;

	IPaddress servIP;
	TCPsocket servSoc;
	SDLNet_SocketSet set;
	vector<TCPsocket> clientSoc;

	const int MAXLEN = 200;
	char msg[MAXLEN];

	void initSDL()
	{
		//Initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		}

		//Initialize SDL_Net
		if( SDLNet_Init()==1)
		{
			printf( "SDLNet_Init: %s\n", SDLNet_GetError());
		}
	}

	void quitSDL()
	{
		SDL_Quit();
		SDLNet_Quit();
	}

	void createServer(int numPlayers)
	{
		if(SDLNet_ResolveHost(&servIP, NULL, 9001)==-1) printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		else cout << "Server Up\n";
		//Uint8 *quad = (Uint8*)&servIP.host;
		//cout << (unsigned short)quad[0] << "." << (unsigned short)quad[1] << "." << (unsigned short)quad[2] << "." << (unsigned short)quad[3] << endl;
		//cout << SDLNet_Read16(&servIP.port) << endl;
		servSoc = SDLNet_TCP_Open(&servIP);
		if(!servSoc) printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		set = SDLNet_AllocSocketSet(numPlayers);
		for(int i=0;i<numPlayers;i++) clientSoc.push_back(TCPsocket());

	}

	void closeServer()
	{
		SDLNet_TCP_Close(servSoc);
	}

	int acceptConnections()
	{
		TCPsocket newSock;
		newSock = SDLNet_TCP_Accept(servSoc);
		//if(!newSock) printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError());
		if(newSock)
		{
			SDLNet_TCP_Recv(newSock,msg,3);
			string temp = msg;
			int playerNum = stoi(msg,nullptr,10);
			//int playerNum = msg[0] -48;
			clientSoc[playerNum] = newSock;
			SDLNet_TCP_AddSocket(set,clientSoc[playerNum]);
			//cout << "Player " << playerNum << " connected.\n";
			return playerNum;
		}
		else return -1;
	}

int getCommand(char* &outStr,int &identOut)
{
	int numActive = SDLNet_CheckSockets(set,0);
	if(numActive>0)
	{
		for(int i=0;i<clientSoc.size();i++)
		{
			if(SDLNet_SocketReady(clientSoc[i]))
			{
				int result;
				result = SDLNet_TCP_Recv(clientSoc[i],msg,MAXLEN);
				if(result>1)
				{
					identOut = i;
					outStr = msg;
					return i;
				}
			}
			
		}
		return -1;
	}
	else return -1;
}

bool sendClient(int playerNum, char msg[])
{
	int result;
	result = SDLNet_TCP_Send(clientSoc[playerNum],msg,strlen(msg)+1);
	if(result!=strlen(msg)+1) return false;
	else return true;
}
