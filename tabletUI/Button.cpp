#include "Button.h"


Button::Button(void)
{
}


Button::~Button(void)
{
}

Button::Button(int x, int y, int w, int h, SDL_Renderer* inRend, std::string image)
{
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	ren = inRend;
	buttonImage = loadTexture(image);
	selectedButton = 0;
	enabled = false;

}



SDL_Texture* Button::loadTexture( std::string path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str());

    newTexture = SDL_CreateTextureFromSurface( ren, loadedSurface );

    SDL_FreeSurface( loadedSurface );

    return newTexture;
}

void Button::draw()
{
	if(selectedButton==0)
	{
		src.x = 0;
		src.y = 0;
		src.w = dest.w;
		src.h = dest.h;
	}
	else if(selectedButton==1)
	{
		src.x = dest.w;
		src.y = 0;
		src.w = dest.w;
		src.h = dest.h;
	}
	else if(selectedButton==2)
	{
		src.x = 0;
		src.y = dest.h;
		src.w = dest.w;
		src.h = dest.h;
	}
	else if(selectedButton==3)
	{
		src.x = dest.w;
		src.y = dest.h;
		src.w = dest.w;
		src.h = dest.h;
	}
	if(enabled) SDL_RenderCopy(ren, buttonImage, &src, &dest);
}

bool Button::handleEvent(SDL_Event* e )
{
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        int x, y;
        SDL_GetMouseState( &x, &y );


        bool inside = true;

        if( x < dest.x )
        {
            inside = false;
        }
		else if( x > dest.x +  dest.w )
        {
            inside = false;
        }
        else if( y < dest.y )
        {
            inside = false;
        }
		else if( y > dest.y + dest.h )
        {
            inside = false;
        }
        if( !inside )
        {
            selectedButton = 0;
        }

        else
        {
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                selectedButton = 1;
                break;

                case SDL_MOUSEBUTTONDOWN:
                selectedButton = 2;
                break;

                case SDL_MOUSEBUTTONUP:
                selectedButton = 3;
                break;
            }
        }
		return inside;
    }
	return 0;
}

void Button::move(int x, int y)
{
	dest.x=x;
	dest.y=y;
}

bool Button::isEnabled()
{
	return enabled;
}
