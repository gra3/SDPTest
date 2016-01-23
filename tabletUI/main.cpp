#include <SDL.h>
#include <SDL_net.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include "TCPclient.h"
#include "Button.h"
#include "Card.h"

int screenWidth, screenHeight;

SDL_Window* gWindow = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Event e;

SDL_Rect src;
SDL_Rect dest;

SDL_Texture* dealerImage;
SDL_Texture* bigBlindImage;
SDL_Texture* smallBlindImage;

TTF_Font* font;
SDL_Color color;

Button connectButton;
Button buyInButton;
Button startButton;
Button foldButton;
Button betButton;

Card handCard[2];
Card commCard[5];

int playerNum;
int state;
bool isDealer;
bool isBigBlind;
bool isSmallBlind;
bool isBetting;
double potTotal;
string potTotalString;
int playerTotal;
string playerTotalString;
double minToCall;
string minToCallString;
double currentBet;
string currentBetString;
double minToRaise;
string minToRaiseString;

bool waitingForNextRound;
bool isActive;

void setDest(int x, int y, int w, int h)
{
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
}

SDL_Texture* loadTexture( std::string path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str());

    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

    SDL_FreeSurface( loadedSurface );

    return newTexture;
}

SDL_Texture* renderText(std::string &message)
{
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surf);

	SDL_FreeSurface(surf);

	return texture;
}

void setColor(int r, int g, int b, int a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void drawText(int x, int y, string text)
{
	int tW,tH;
	SDL_Texture* temp = renderText(text);
	SDL_QueryTexture(temp,NULL,NULL,&tW,&tH);
	setDest(x,y,tW,tH);
	SDL_RenderCopy(gRenderer,temp,NULL,&dest);
	SDL_DestroyTexture(temp);
}

void loadImages()
{
	dealerImage = loadTexture("Resources/dealer.png");
	bigBlindImage = loadTexture("Resources/bigBlind.png");
	smallBlindImage = loadTexture("Resources/smallBlind.png");
}

void createWindow(int w, int h)
{
	gWindow =SDL_CreateWindow("Tablet UI",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_SHOWN);
	if(gWindow==NULL) printf("SDL_CreateWindow: %s\n",SDL_GetError());
	else
	{
		gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
		if(gRenderer==NULL) printf("SDL_CreateRenderer: %s\n",SDL_GetError());
	}
}

void enabledAllCards()
{
	handCard[0].enable();
	handCard[1].enable();
	commCard[0].enable();
	commCard[1].enable();
	commCard[2].enable();
	commCard[3].enable();
	commCard[4].enable();
}

void drawAllCards()
{
	handCard[0].draw();
	handCard[1].draw();
	commCard[0].draw();
	commCard[1].draw();
	commCard[2].draw();
	commCard[3].draw();
	commCard[4].draw();
}

//void processIncData(char msgIn[])
void proccessIncData(string strIn)
{
	//if(strlen(msgIn)>10)
	//{
	//	startButton.disable();
	//	enabledAllCards();
	//}

	if(strIn.length()>10)
	{
		startButton.disable();
	}

	/*string messageString(msgIn);*/
	string messageString = strIn;


	vector<double> data;
	string delim = " ";
	string token;

	size_t pos = 0;
	while((pos = messageString.find(delim))!=string::npos)
	{
		token = messageString.substr(0,pos);
		//data.push_back(stoi(token,nullptr,10));
		data.push_back(stod(token,nullptr));
		messageString.erase(0,pos+delim.length());
	}
	

	if(data.size()==25)
	{
		handCard[0].set(data[1],data[0]);
		handCard[1].set(data[3],data[2]);
		commCard[0].set(data[5],data[4]);
		commCard[1].set(data[7],data[6]);
		commCard[2].set(data[9],data[8]);
		commCard[3].set(data[11],data[10]);
		commCard[4].set(data[13],data[12]);
		state = data[14];
		isDealer = data[15];
		isBigBlind = data[16];
		isSmallBlind = data[17];
		isBetting = data[18];

		//Pot Total
		potTotal = data[19];
		stringstream stream;
		stream << fixed << setprecision(2) << data[19];
		potTotalString = stream.str();

		//Player Ammount
		playerTotal = data[20];
		stream.str(string()); //clear stream
		stream << fixed << setprecision(2) << data[20];
		playerTotalString = stream.str();

		//Min to call
		minToCall = data[21];
		stream.str(string());
		stream << fixed << setprecision(2) << data[21];
		minToCallString = stream.str();

		//Current Bet
		currentBet = data[22];
		stream.str(string());
		stream << fixed << setprecision(2) << data[22];
		currentBetString = stream.str();

		//Min to Raise
		minToRaise = data[23];
		stream.str(string());
		stream << fixed << setprecision(2) << data[23];
		minToRaiseString = stream.str();

		//isActive
		isActive = data[24];
		if(isActive) enabledAllCards();


	}
	else cout << "Error data.size() = " << data.size() << endl;

	if(state==3&&isBetting)
	{
		foldButton.enable();
		betButton.enable();
	}
	else if(state==3&&!isBetting)
	{
		foldButton.disable();
		betButton.disable();
	}
	
}

static int TcpThread(void *ptr)
{
	while(true)
	{
		string data;
		if(getData(data))
		{
			cout << "From Main Loop: " << data << endl;
			proccessIncData(data);
		}
		SDL_Delay(50);
	}
	return 0;
}

int main(int argc, char* args[])
{
	screenWidth = 700;
	screenHeight = 400;
	initSDL();
	TTF_Init();
	//playerNum = playerNumber[0]-48;
	playerNum = playerNumberInt;
	state = 0;
	createWindow(screenWidth,screenHeight);
	loadImages();

	font = TTF_OpenFont("Resources/Instruction.ttf",24);
	if(font==NULL) cout << TTF_GetError() << endl;
	setColor(0,0,0,255);

	connectButton = Button((screenWidth/2)-65,(screenHeight/2)-25,130,50, gRenderer, "Resources/connect.png");
	connectButton.enable();
	buyInButton = Button((screenWidth/2)-65,(screenHeight/2)-25,130,50, gRenderer, "Resources/buyIn.png");
	startButton = Button((screenWidth/2)-65,(screenHeight/2)-25,130,50, gRenderer, "Resources/start.png");
	foldButton = Button(screenWidth-280,screenHeight-60,130,50,gRenderer, "Resources/fold.png");
	betButton = Button(screenWidth- 140,screenHeight-60,130,50,gRenderer, "Resources/bet.png");
	
	handCard[0] = Card(0,0,50,300,71,96,gRenderer);
	handCard[1] = Card(0,0,125,300,71,96,gRenderer);
	commCard[0] = Card(0,0,300,25,71,96,gRenderer);
	commCard[1] = Card(0,0,375,25,71,96,gRenderer);
	commCard[2] = Card(0,0,450,25,71,96,gRenderer);
	commCard[3] = Card(0,0,525,25,71,96,gRenderer);
	commCard[4] = Card(0,0,600,25,71,96,gRenderer);

	waitingForNextRound = false;
	state = 0;
	isActive = false;

	//SDL_Thread* tcpThread;
	//tcpThread = SDL_CreateThread(TcpThread, "TCP THREAD", (void *)NULL);

	bool quit = false;
	while (!quit){

		//char *data;
		//if(getData(data))
		//{
		//	cout << data << endl;
		//	processIncData(msg);
		//}

		//string data;
		//if(getData(data))
		//{
		//	cout << "From Main Loop: " << data << endl;
		//	//proccessIncData(data);
		//}

		while( SDL_PollEvent( &e ) != 0 )
			{

				if( e.type == SDL_QUIT )
				{
					quit = true;
					exit(1);
				}
				//Start Button
				if(startButton.isEnabled()&&startButton.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
				{
					sendServer("start");

				}
				//Buy-in Button
				if(buyInButton.isEnabled()&&buyInButton.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
				{
					if(sendServer("buyIn"))
					{
						buyInButton.disable();
						if(state==0) startButton.enable();
						else if(state>0) waitingForNextRound=true;
					}
				}
				//Connect Button
				if(connectButton.isEnabled()&&connectButton.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
				{
					if(connectToServer())
					{
						connectButton.disable();
						buyInButton.enable();
					}
				}
				//Fold Button
				if(foldButton.isEnabled()&&foldButton.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
				{
					sendServer("fold");
				}
				//Bet Button
				if(betButton.isEnabled()&&betButton.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
				{
					sendServer("bet");
				}

			}

		string data;
		if(getData(data))
		{
			cout << "From Main Loop: " << data << endl;
			proccessIncData(data);
		}

			SDL_RenderClear(gRenderer);
			
			//Draw Player Number
			drawText(10,0,"Player " + to_string(playerNum));

			//Draw Pot Total
			if(state>0&&isActive) drawText(425,235,"Pot Total: $" + potTotalString);

			//Draw Player Total
			if(state>0&&isActive) drawText(425,270,"Chip Total: $:" + playerTotalString);

			//Draw minToCall and currentBet
			if(state==3&&isBetting) 
			{
				drawText(425,130, "Current Bet: $" + currentBetString);
				drawText(425,165,"Min to Call: $" + minToCallString);
				drawText(425,200,"Min to Raise: $" + minToRaiseString);
			}

			//Draw Buttons
			connectButton.draw();
			buyInButton.draw();
			startButton.draw();

			foldButton.draw();
			betButton.draw();

			//Draw Cards
			drawAllCards();

			//Draw Dealer Chip
			setDest(225,300,75,75);
			if(isDealer) SDL_RenderCopy(gRenderer,dealerImage,NULL,&dest);

			//Draw Big/Small Blind chip
			setDest(325,300,75,75);
			if(isBigBlind) SDL_RenderCopy(gRenderer,bigBlindImage,NULL,&dest);
			else if(isSmallBlind) SDL_RenderCopy(gRenderer,smallBlindImage,NULL,&dest);

			//Draw Waiting for next round
			if(waitingForNextRound&&!isActive) drawText(screenWidth/2,screenHeight/2,"Waiting for next hand...");

			SDL_RenderPresent(gRenderer);
			SDL_Delay(30);
	}

	SDL_DestroyWindow( gWindow );
	SDLNet_Quit();
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}