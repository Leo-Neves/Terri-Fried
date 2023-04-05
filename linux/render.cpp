#include "render.h"

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

bool running;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface* egg;

SDL_Surface* playerSprite_surf;
SDL_Surface* lavaSprite_surf;
SDL_Surface* platformSprite_surf;
SDL_Surface* coinSprite_surf;
SDL_Surface* scoreBoxSprite_surf;
SDL_Surface* logo_surf;
SDL_Surface* splashEggSprite_surf;

SDL_Texture* playerSprite;
SDL_Texture* lavaSprite;
SDL_Texture* platformSprite;
SDL_Texture* coinSprite;
SDL_Texture* scoreBoxSprite;
SDL_Texture* logo;
SDL_Texture* splashEggSprite;

SDL_Event e;
bool mouse_released;
bool mouse_pressed;
bool mouse_down;
int mouse_x, mouse_y;

void drawFont(SDL_Renderer *renderer, const char *str, int x, int y, int width, int height, int size, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("resources/font.otf", size);
    
    SDL_Surface* message_surf = TTF_RenderText_Blended(font, str, color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, message_surf);
    SDL_Rect Message_rect = {x, y, width, height};
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(message_surf);
    TTF_CloseFont(font);
}

Render::Render(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

int Render::loadHighScore() {
    FILE *scorefile = fopen("highscore.sav", "rb");
    
    if(!scorefile)
        return 0;
    
    int ret;
    fread(&ret, sizeof(int), 1, scorefile);
    fclose(scorefile);
    
    return ret;
}

void Render::saveHighScore(int highscore) {
    FILE *scorefile = fopen("highscore.sav", "wb");
    
    fwrite(&highscore, sizeof(int), 1, scorefile);
    fclose(scorefile);
}

void Render::setupWindow(){
    running = true;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    window = SDL_CreateWindow(
        "Terri-Fried",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight,
        SDL_WINDOW_OPENGL
    );
    renderer = SDL_CreateRenderer(window, -1, 0);
    egg = IMG_Load("resources/egg.png");
    SDL_SetWindowIcon(window, egg);

    playerSprite_surf = IMG_Load("resources/egg.png");
    lavaSprite_surf = IMG_Load("resources/lava.png");
    platformSprite_surf = IMG_Load("resources/platform.png");
    coinSprite_surf = IMG_Load("resources/coin.png");
    scoreBoxSprite_surf = IMG_Load("resources/scorebox.png");
    logo_surf = IMG_Load("resources/logo.png");
    splashEggSprite_surf = IMG_Load("resources/splash_egg.png");

    playerSprite = SDL_CreateTextureFromSurface(renderer, playerSprite_surf);
    lavaSprite = SDL_CreateTextureFromSurface(renderer, lavaSprite_surf);
    platformSprite = SDL_CreateTextureFromSurface(renderer, platformSprite_surf);
    coinSprite = SDL_CreateTextureFromSurface(renderer, coinSprite_surf);
    scoreBoxSprite = SDL_CreateTextureFromSurface(renderer, scoreBoxSprite_surf);
    logo = SDL_CreateTextureFromSurface(renderer, logo_surf);
    splashEggSprite = SDL_CreateTextureFromSurface(renderer, splashEggSprite_surf);

    mouse_released = false;
    mouse_pressed = false;
    mouse_down = false;
}

bool Render::isMouseMode(){
    return false;
}

bool Render::isKeyboardMode(){
    return true;
}

bool Render::isRunning(){
    return running;
}

int Render::getScreenHeight(){
    return screenHeight;
}

int Render::getScreenWidth(){
    return screenWidth;
}

void Render::checkEvent(){
    mouse_released = false;
    mouse_pressed = false;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT: {
                running = false;
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                mouse_down = true;
                mouse_pressed = true;
                //mouse_released = false;
            } break;
            case SDL_MOUSEBUTTONUP: {
                mouse_down = false;
                mouse_released = true;
                //mouse_pressed = false;
            } break;
        }
    }

    SDL_PumpEvents();
    SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Delay(12);
}
    
void Render::showSplashScreen(){
    SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 1.0 * 255);
    SDL_RenderClear(renderer);
    
    drawFont(renderer, "POLYMARS", screenWidth/2 - 54, screenHeight/2 + 3, 108, 32, 32, {213, 128, 90});
    
    SDL_Rect splashEggSprite_rect = { screenWidth/2 - 16, screenHeight/2 - 16 - 23, 32, 32 };
    SDL_RenderCopy(renderer, splashEggSprite, NULL, &splashEggSprite_rect);
    
    SDL_RenderPresent(renderer);
}

void Render::showIntroMenu(char highscore[32]){
    SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 1.0 * 255);
    SDL_RenderClear(renderer);
    
    SDL_Rect logo_rect = { screenWidth/2 - 200, screenHeight/2 - 45 - 30, 400, 90 };
    SDL_RenderCopy(renderer, logo, NULL, &logo_rect);
    
    drawFont(renderer, highscore, screenWidth/2 - 37, screenHeight/2 + 10, 74, 32, 32, {0, 0, 0});
    drawFont(renderer, "CLICK ANYWHERE TO BEGIN", screenWidth/2 - 134, screenHeight/2 + 50, 268, 32, 32, {178, 150, 125});
    
    SDL_RenderPresent(renderer);
}

bool Render::isMouseDown(){
    return mouse_down;
}

bool Render::isMousePressed(){
    return mouse_pressed;
}

bool Render::isMouseReleased(){
    return mouse_released;
}

int Render::getMouseX(){
    return mouse_x;
}

int Render::getMouseY(){
    return mouse_y;
}

void Render::drawBackground(){
    SDL_SetRenderDrawColor(renderer, 0.933 * 255, 0.894 * 255, 0.882 * 255, 1.0 * 255);
    SDL_RenderClear(renderer);
}

void Render::drawLine(int mouseDownX, int mouseDownY, int playerX, int playerY, int playerWidth, int playerHeight){
    SDL_SetRenderDrawColor(renderer, 178, 150, 125, 255);
    SDL_RenderDrawLine(
        renderer,
        mouseDownX + (playerX - mouseDownX) + (playerWidth/2),
        mouseDownY + (playerY - mouseDownY) + (playerHeight/2),
        mouse_x + (playerX - mouseDownX) + (playerWidth/2),
        mouse_y + (playerY - mouseDownY) + (playerHeight/2)
    );
}

void Render::drawPlatform(int x, int y){
    SDL_Rect platformSprite_rect = { x, y, 100, 32 };
    SDL_RenderCopy(renderer, platformSprite, NULL, &platformSprite_rect);
}

void Render::drawCoin(int x, int y){
    SDL_Rect coinSprite_rect = { x, y, 24, 24 };
    SDL_RenderCopy(renderer, coinSprite, NULL, &coinSprite_rect);
}

void Render::drawPlayer(int x, int y){
    SDL_Rect playerSprite_rect = { x, y, 32, 32 };
    SDL_RenderCopy(renderer, playerSprite, NULL, &playerSprite_rect);
}

void Render::drawScore(char score[32], char highscore[32]){
    SDL_Rect scoreBoxSprite_rect = { 17, 17, 102, 70 };
    SDL_RenderCopy(renderer, scoreBoxSprite, NULL, &scoreBoxSprite_rect);
    
    drawFont(renderer, score, 28, 20, 75, 64, 64, {0, 0, 0});
    drawFont(renderer, highscore, 17, 90, 74, 32, 32, {0, 0, 0});
    
    SDL_RenderPresent(renderer);
}

void Render::drawLava(int y){
    SDL_Rect lavaSprite_rect = { 0, y, 800, 48 };
    SDL_RenderCopy(renderer, lavaSprite, NULL, &lavaSprite_rect);
}

void Render::freeMemory(){
    SDL_DestroyTexture(playerSprite);
    SDL_DestroyTexture(lavaSprite);
    SDL_DestroyTexture(platformSprite);
    SDL_DestroyTexture(coinSprite);
    SDL_DestroyTexture(scoreBoxSprite);
    SDL_DestroyTexture(logo);
    SDL_DestroyTexture(splashEggSprite);
    
    SDL_FreeSurface(playerSprite_surf);
    SDL_FreeSurface(lavaSprite_surf);
    SDL_FreeSurface(platformSprite_surf);
    SDL_FreeSurface(coinSprite_surf);
    SDL_FreeSurface(scoreBoxSprite_surf);
    SDL_FreeSurface(logo_surf);
    SDL_FreeSurface(splashEggSprite_surf);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();   
}