#ifndef BACKGROUND_H
#define BACKGROUND_H

struct SDL_Renderer;

class Background{
private:
    int screen_width_;
    int screen_height_;
    SDL_Renderer *renderer_;
public:
    Background(SDL_Renderer*, int, int);
    void fillBackground();
    void drawGrid();
};

#endif
