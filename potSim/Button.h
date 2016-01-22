#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

class Button
{
public:
	SDL_Texture* buttonImage;


	int selectedButton;

	SDL_Renderer* ren;

	SDL_Rect src;
	SDL_Rect dest;

	Button(void);

	Button(int,int,int,int, SDL_Renderer* inRend, std::string out);

	SDL_Texture * loadTexture(std::string);

	void draw();

	bool handleEvent( SDL_Event* e );

	~Button(void);

	void move(int x, int y);

	bool enabled;

	bool isEnabled();

	void disable()
	{
		enabled = false;
	}

	void enable()
	{
		enabled = true;
	}
};

