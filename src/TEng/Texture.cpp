#include "../../includes/TEng/Texture.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

Texture::Texture() {
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void Texture::setup(const int w, const int h,SDL_Renderer *renderer) {
    mWidth = w;
    mHeight = h;
    gameRenderer = renderer;
}

void Texture::setup(SDL_Renderer *renderer) {
    gameRenderer = renderer;
}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile(const std::string& filePath) {
    free();

    SDL_Texture* newTexture = nullptr;

    SDL_Surface* loadedSurface = IMG_Load(("resources/textures/" + filePath).c_str() );
    if(loadedSurface == nullptr) {
        SDL_Log( "Unable to load image %s! SDL_image Error: %s\n", filePath.c_str(), IMG_GetError() );
    } else {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
        if(newTexture == nullptr) {
            SDL_Log( "Unable to create texture from %s! SDL Error: %s\n", filePath.c_str(), SDL_GetError() );
        }

        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != nullptr;
}

bool Texture::loadFromRenderedText(const std::string &textureText, const SDL_Color &textColor,TTF_Font * font) {
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended( font, textureText.c_str(), textColor );
    if( textSurface == nullptr ) {
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface );
        if( mTexture == nullptr )
        {
            SDL_Log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture != nullptr;
}

void Texture::render(const int x, const int y, const SDL_RendererFlip flip, const SDL_Rect *srcRect, const double angle, const SDL_Point *center) const {
    SDL_Rect dstRect = {x,y,mWidth,mHeight};

    SDL_RenderCopyEx( gameRenderer, mTexture, srcRect, &dstRect, angle, center, flip );
}

void Texture::free()
{
    //Free texture if it exists
    if( mTexture != nullptr ) {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}
