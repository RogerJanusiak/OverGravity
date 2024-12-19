#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>


class Texture {

public:
    Texture();
    explicit Texture(SDL_Renderer* renderer) : gameRenderer(renderer), mWidth(0), mHeight(0) {};
    ~Texture();

    void free();

    void setup(int w, int h,SDL_Renderer *renderer);
    void setup(SDL_Renderer *renderer);

    bool loadFromFile(const std::string& filePath);
    bool loadFromRenderedText(const std::string &textureText, const SDL_Color &textColor,TTF_Font * font);

    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    SDL_Texture* getTexture() const { return mTexture; }

    void setTexture(SDL_Texture* texture) { mTexture = texture; }

    void render(int x, int y, SDL_RendererFlip
                flip = SDL_FLIP_NONE, const SDL_Rect *srcRect = nullptr, double angle = 0.0, const SDL_Point *center = nullptr) const;



private:
    SDL_Texture* mTexture = nullptr;
    SDL_Renderer *gameRenderer = nullptr;

    int mWidth;
    int mHeight;
};

#endif //TEXTURE_H
