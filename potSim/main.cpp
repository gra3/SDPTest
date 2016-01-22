#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include "Button.h"


using namespace std;

int screenWidth, screenHeight;

SDL_Window* gWindow = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Event e;

SDL_Rect src;
SDL_Rect dest;

TTF_Font* font;
SDL_Color color;

Button chip_1_plus_Button;
Button chip_1_minus_Button;
Button chip_2_plus_Button;
Button chip_2_minus_Button;
Button chip_3_plus_Button;
Button chip_3_minus_Button;
Button chip_4_plus_Button;
Button chip_4_minus_Button;
Button clearButton;
Button player_plus_Button;
Button player_minus_Button;
Button player_launch_Button;

SDL_Texture* whiteImage;
SDL_Texture* redImage;
SDL_Texture* greenImage;
SDL_Texture* blackImage;

void initSDL()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
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

void createWindow(int w, int h)
{
	gWindow =SDL_CreateWindow("Pot Simulator",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_SHOWN);
	if(gWindow==NULL) printf("SDL_CreateWindow: %s\n",SDL_GetError());
	else
	{
		gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
		if(gRenderer==NULL) printf("SDL_CreateRenderer: %s\n",SDL_GetError());
	}
}

void setDest(int x, int y, int w, int h)
{
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
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



void setColor(int r, int g, int b, int a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

int main(int argc, char* args[])
{
	screenWidth = 500;
	screenHeight = 400;
	initSDL();
	TTF_Init();
	createWindow(screenWidth,screenHeight);

	chip_1_plus_Button = Button(25,225,50,50,gRenderer,"Resources/plus.png");
	chip_2_plus_Button = Button(140,225,50,50,gRenderer,"Resources/plus.png");
	chip_3_plus_Button = Button(255,225,50,50,gRenderer,"Resources/plus.png");
	chip_4_plus_Button = Button(370,225,50,50,gRenderer,"Resources/plus.png");
	chip_1_minus_Button = Button(80,225,50,50,gRenderer,"Resources/minus.png");
	chip_2_minus_Button = Button(195,225,50,50,gRenderer,"Resources/minus.png");
	chip_3_minus_Button = Button(310,225,50,50,gRenderer,"Resources/minus.png");
	chip_4_minus_Button = Button(425,225,50,50,gRenderer,"Resources/minus.png");
	clearButton = Button(350,10,130,50,gRenderer,"Resources/clear.png");
	player_plus_Button = Button(350,325,50,50,gRenderer, "Resources/plus.png");
	player_minus_Button = Button(410,325,50,50,gRenderer, "Resources/minus.png");
	player_launch_Button = Button(25,325,130,50,gRenderer, "Resources/launch.png");

	chip_1_plus_Button.enable();
	chip_2_plus_Button.enable();
	chip_3_plus_Button.enable();
	chip_4_plus_Button.enable();
	chip_1_minus_Button.enable();
	chip_2_minus_Button.enable();
	chip_3_minus_Button.enable();
	chip_4_minus_Button.enable();
	clearButton.enable();
	player_plus_Button.enable();
	player_minus_Button.enable();
	player_launch_Button.enable();


	whiteImage = loadTexture("Resources/white.png");
	redImage = loadTexture("Resources/red.png");
	greenImage = loadTexture("Resources/green.png");
	blackImage = loadTexture("Resources/black.png");

	font = TTF_OpenFont("Resources/Instruction.ttf",24);
	if(font==NULL) cout << TTF_GetError() << endl;
	setColor(0,0,0,255);

	int chip1,chip2,chip3,chip4;
	int numberOfPlayers = 2;
	double potTotal;
	string potTotalString;
	
	fstream pot;
	pot.open("pot.txt");
	while(!pot.eof())
	{
		pot >> chip1;
		pot >> chip2;
		pot >> chip3;
		pot >> chip4;
	}
	pot.close();

	fstream settings;

	bool quit = false;
	while (!quit){

		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			//CHIP 1 PLUS
			if(chip_1_plus_Button.isEnabled()&&chip_1_plus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				chip1 += 1;
				pot.open("pot.txt");
				pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
				pot.close();
			}
			//CHIP 1 MINUS
			if(chip_1_minus_Button.isEnabled()&&chip_1_minus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				if(chip1!=0)
				{
					chip1 -= 1;
					pot.open("pot.txt");
					pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
					pot.close();
				}
			}
			//CHIP 2 PLUS
			if(chip_2_plus_Button.isEnabled()&&chip_2_plus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				chip2 += 1;
				pot.open("pot.txt");
				pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
				pot.close();
			}
			//CHIP 2 MINUS
			if(chip_2_minus_Button.isEnabled()&&chip_2_minus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				if(chip2!=0)
				{
					chip2 -= 1;
					pot.open("pot.txt");
					pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
					pot.close();
				}
			}
			//CHIP 3 PLUS
			if(chip_3_plus_Button.isEnabled()&&chip_3_plus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				chip3 += 1;
				pot.open("pot.txt");
				pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
				pot.close();
			}
			//CHIP 3 MINUS
			if(chip_3_minus_Button.isEnabled()&&chip_3_minus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				if(chip3!=0)
				{
					chip3 -= 1;
					pot.open("pot.txt");
					pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
					pot.close();
				}
			}
			//CHIP 4 PLUS
			if(chip_4_plus_Button.isEnabled()&&chip_4_plus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				chip4 += 1;
				pot.open("pot.txt");
				pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
				pot.close();
			}
			//CHIP 4 MINUS
			if(chip_4_minus_Button.isEnabled()&&chip_4_minus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				if(chip4!=0)
				{
					chip4 -= 1;
					pot.open("pot.txt");
					pot << chip1 << " " << chip2 << " " << chip3 << " " << chip4;
					pot.close();
				}
			}
			//Clear
			if(clearButton.isEnabled()&&clearButton.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				pot.open("pot.txt");
				pot << "0 0 0 0                  ";
				pot.close();
				chip1 = 0;
				chip2 = 0;
				chip3 = 0;
				chip4 = 0;
			}
			//Player plus
			if(player_plus_Button.isEnabled()&&player_plus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				numberOfPlayers +=1;
			}
			//Player minus
			if(player_minus_Button.isEnabled()&&player_minus_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				if(numberOfPlayers!=2)
				{
					numberOfPlayers -= 1;
				}
			}
			//Player Launch
			if(player_launch_Button.isEnabled()&&player_launch_Button.handleEvent(&e)&&e.type == SDL_MOUSEBUTTONUP)
			{
				for(int i=numberOfPlayers-1;i>=0;i--)
				{
					settings.open("settings.txt",std::ofstream::out | std::ofstream::trunc);
					settings << i;
					settings.close();
					SDL_Delay(50);
					STARTUPINFO si;
					PROCESS_INFORMATION pi;

					ZeroMemory( &si, sizeof(si) );
					si.cb = sizeof(si);
					ZeroMemory( &pi, sizeof(pi) );

					CreateProcess(NULL, "tabletUI.exe",NULL,NULL,false,0,NULL,NULL,&si,&pi);
					SDL_Delay(100);
				}
			}

		}

		potTotal = chip1*.25+chip2*.5+chip3*1.0+chip4*5;


		SDL_RenderClear(gRenderer);
				
		//Draw Pot Total
		stringstream stream;
		stream << fixed << setprecision(2) << potTotal;
		potTotalString = stream.str();
		drawText(0,0,"Pot Total: $" + potTotalString);

		//Draw Chip Totals
		drawText(75,190,to_string(chip1));
		drawText(190,190,to_string(chip2));
		drawText(300,190,to_string(chip3));
		drawText(415,190,to_string(chip4));

		//Draw Number of Players
		drawText(160,335, "# Players: " + to_string(numberOfPlayers));
			
		//Draw Buttons
		chip_1_plus_Button.draw();
		chip_2_plus_Button.draw();
		chip_3_plus_Button.draw();
		chip_4_plus_Button.draw();
		chip_1_minus_Button.draw();
		chip_2_minus_Button.draw();
		chip_3_minus_Button.draw();
		chip_4_minus_Button.draw();
		clearButton.draw();
		player_plus_Button.draw();
		player_minus_Button.draw();
		player_launch_Button.draw();

		//Draw Images
		setDest(30,80,100,100);
		SDL_RenderCopy(gRenderer,whiteImage,NULL,&dest);
		setDest(145,80,100,100);
		SDL_RenderCopy(gRenderer,redImage,NULL,&dest);
		setDest(265,80,100,100);
		SDL_RenderCopy(gRenderer,greenImage,NULL,&dest);
		setDest(375,80,100,100);
		SDL_RenderCopy(gRenderer,blackImage,NULL,&dest);

		SDL_RenderPresent(gRenderer);
		SDL_Delay(30);

	
	}
	return 0;
}