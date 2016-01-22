#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

class Card
{
public:
	SDL_Rect dest;
	SDL_Renderer* ren;
	SDL_Texture* cardImage;
	int suit, rank;
	bool enabled;

	Card(void)
	{
		suit = -1;
		rank = -1;
	}

SDL_Texture* loadTexture( std::string path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str());

    newTexture = SDL_CreateTextureFromSurface( ren, loadedSurface );

    SDL_FreeSurface( loadedSurface );

    return newTexture;
}

	void changeImage()
	{
		if(suit==0&&rank==0) cardImage = loadTexture("Resources/cards/b1fv.png");

		if(suit==0&&rank==2) cardImage = loadTexture("Resources/cards/h2.png");
		if(suit==0&&rank==3) cardImage = loadTexture("Resources/cards/h3.png");
		if(suit==0&&rank==4) cardImage = loadTexture("Resources/cards/h4.png");
		if(suit==0&&rank==5) cardImage = loadTexture("Resources/cards/h5.png");
		if(suit==0&&rank==6) cardImage = loadTexture("Resources/cards/h6.png");
		if(suit==0&&rank==7) cardImage = loadTexture("Resources/cards/h7.png");
		if(suit==0&&rank==8) cardImage = loadTexture("Resources/cards/h8.png");
		if(suit==0&&rank==9) cardImage = loadTexture("Resources/cards/h9.png");
		if(suit==0&&rank==10) cardImage = loadTexture("Resources/cards/h10.png");
		if(suit==0&&rank==11) cardImage = loadTexture("Resources/cards/hj.png");
		if(suit==0&&rank==12) cardImage = loadTexture("Resources/cards/hq.png");
		if(suit==0&&rank==13) cardImage = loadTexture("Resources/cards/hk.png");
		if(suit==0&&rank==14) cardImage = loadTexture("Resources/cards/h1.png");

		if(suit==1&&rank==2) cardImage = loadTexture("Resources/cards/s2.png");
		if(suit==1&&rank==3) cardImage = loadTexture("Resources/cards/s3.png");
		if(suit==1&&rank==4) cardImage = loadTexture("Resources/cards/s4.png");
		if(suit==1&&rank==5) cardImage = loadTexture("Resources/cards/s5.png");
		if(suit==1&&rank==6) cardImage = loadTexture("Resources/cards/s6.png");
		if(suit==1&&rank==7) cardImage = loadTexture("Resources/cards/s7.png");
		if(suit==1&&rank==8) cardImage = loadTexture("Resources/cards/s8.png");
		if(suit==1&&rank==9) cardImage = loadTexture("Resources/cards/s9.png");
		if(suit==1&&rank==10) cardImage = loadTexture("Resources/cards/s10.png");
		if(suit==1&&rank==11) cardImage = loadTexture("Resources/cards/sj.png");
		if(suit==1&&rank==12) cardImage = loadTexture("Resources/cards/sq.png");
		if(suit==1&&rank==13) cardImage = loadTexture("Resources/cards/sk.png");
		if(suit==1&&rank==14) cardImage = loadTexture("Resources/cards/s1.png");

		if(suit==2&&rank==2) cardImage = loadTexture("Resources/cards/c2.png");
		if(suit==2&&rank==3) cardImage = loadTexture("Resources/cards/c3.png");
		if(suit==2&&rank==4) cardImage = loadTexture("Resources/cards/c4.png");
		if(suit==2&&rank==5) cardImage = loadTexture("Resources/cards/c5.png");
		if(suit==2&&rank==6) cardImage = loadTexture("Resources/cards/c6.png");
		if(suit==2&&rank==7) cardImage = loadTexture("Resources/cards/c7.png");
		if(suit==2&&rank==8) cardImage = loadTexture("Resources/cards/c8.png");
		if(suit==2&&rank==9) cardImage = loadTexture("Resources/cards/c9.png");
		if(suit==2&&rank==10) cardImage = loadTexture("Resources/cards/c10.png");
		if(suit==2&&rank==11) cardImage = loadTexture("Resources/cards/cj.png");
		if(suit==2&&rank==12) cardImage = loadTexture("Resources/cards/cq.png");
		if(suit==2&&rank==13) cardImage = loadTexture("Resources/cards/ck.png");
		if(suit==2&&rank==14) cardImage = loadTexture("Resources/cards/c1.png");

		if(suit==3&&rank==2) cardImage = loadTexture("Resources/cards/d2.png");
		if(suit==3&&rank==3) cardImage = loadTexture("Resources/cards/d3.png");
		if(suit==3&&rank==4) cardImage = loadTexture("Resources/cards/d4.png");
		if(suit==3&&rank==5) cardImage = loadTexture("Resources/cards/d5.png");
		if(suit==3&&rank==6) cardImage = loadTexture("Resources/cards/d6.png");
		if(suit==3&&rank==7) cardImage = loadTexture("Resources/cards/d7.png");
		if(suit==3&&rank==8) cardImage = loadTexture("Resources/cards/d8.png");
		if(suit==3&&rank==9) cardImage = loadTexture("Resources/cards/d9.png");
		if(suit==3&&rank==10) cardImage = loadTexture("Resources/cards/d10.png");
		if(suit==3&&rank==11) cardImage = loadTexture("Resources/cards/dj.png");
		if(suit==3&&rank==12) cardImage = loadTexture("Resources/cards/dq.png");
		if(suit==3&&rank==13) cardImage = loadTexture("Resources/cards/dk.png");
		if(suit==3&&rank==14) cardImage = loadTexture("Resources/cards/d1.png");
	}

	Card(int rankIn, int suitIn, int x, int y, int w, int h,SDL_Renderer* inRend)
	{
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
		rank = rankIn;
		suit = suitIn;
		ren = inRend;
		changeImage();
		enabled = false;
	}

	void draw()
	{
		if(enabled) SDL_RenderCopy(ren,cardImage,NULL,&dest);
	}

	~Card(void)
	{
	}

	void set(int rankIn, int suitIn)
	{
		rank = rankIn;
		suit = suitIn;
		SDL_DestroyTexture(cardImage);
		changeImage();
	}

	void printCard()
	{
		if(rank!=-1&&suit!=-1)
		{
			if(rank==14) cout << "Ace of ";
			if(rank==13) cout << "Kind of ";
			if(rank==12) cout << "Queen of ";
			if(rank==11) cout << "Jack of ";
			if(rank==10) cout << "10 of ";
			if(rank==9) cout << "9 of ";
			if(rank==8) cout << "8 of ";
			if(rank==7) cout << "7 of ";
			if(rank==6) cout << "6 of ";
			if(rank==5) cout << "5 of ";
			if(rank==4) cout << "4 of ";
			if(rank==3) cout << "3 of ";
			if(rank==2) cout << "2 of ";

			if(suit==0) cout << "Hearts\n";
			if(suit==1) cout << "Spades\n";
			if(suit==2) cout << "Clubs\n";
			if(suit==3) cout << "Diamonds\n";
		}
	}

	bool operator==(const Card &other)
	{
		if(suit==other.suit&&rank==other.rank) return true;
		else return false;
	}

	bool operator!=(const Card &other)
	{
		if(suit!=other.suit||rank!=other.rank) return true;
		else return false;
	}

	bool operator<(const Card &other)
	{
		if(rank<other.rank) return true;
		else return false;
	}

	//void operator=(const Card &other)
	//{
		//rank = other.rank;
		//suit = other.suit;
	//}

	bool isEnabled()
	{
		return enabled;
	}

	void disable()
	{
		enabled = false;
	}

	void enable()
	{
		enabled = true;
	}

};

