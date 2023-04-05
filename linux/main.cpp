#include <math.h>
#include <stdio.h>
#include <time.h>

#include "player.h"
#include "platform.h"
#include "render.h"
#include "audio.h"

Render render = {800, 450};
Audio audio = {};
Platform platforms[4] = {{0}, {1}, {2}, {3}};
Player player(platforms[0].getX() + platforms[0].getWidth()/2 - 26/2, platforms[0].getY() - player.getHeight(), 26, 32);

int scoreInt = 0;
int highscoreInt = render.loadHighScore();
char score[32];
char highscore[32];

bool titleScreen = true;
bool playCoinFX = false;

void addScore(int amount) {
    scoreInt += amount;
    
    if (scoreInt < 10)
        sprintf(score, "00%d", scoreInt);
    else if (scoreInt < 100)
        sprintf(score, "0%d", scoreInt);
    else
        sprintf(score, "%d", scoreInt);
    
    if (scoreInt > highscoreInt) {
        highscoreInt = scoreInt;
        sprintf(highscore, "BEST: %d", highscoreInt);
    }
}

void resetScore() {
    scoreInt = 0;
    sprintf(score, "00%d", scoreInt);
    render.saveHighScore(highscoreInt);
}

void resetGame() {
    resetScore();
    
    for (int i = 0; i < 4; i++)
        platforms[i] = Platform(i);
    
    player.setVelocity(0, 0);
    player.setX(platforms[0].getX() + platforms[0].getWidth()/2 - 26/2);
    player.setY(platforms[0].getY() - player.getHeight());
}


void checkPlayerCollision() {
    bool onPlatform = false;
    
    for (int i = 0; i < 4; i++) {
        if (platforms[i].getHasCoin() && player.getX() + player.getWidth() - 3 > platforms[i].getCoinX() && player.getX() + 3 < platforms[i].getCoinX() + 24 && player.getY() + player.getHeight() - 3 > platforms[i].getCoinY() && player.getY() + 3 < platforms[i].getCoinY() + 24) {
            addScore(1);
            platforms[i].setHasCoin(false);
            playCoinFX = true;
        }
        
        if (player.getX() + 1 < platforms[i].getX() + platforms[i].getWidth() && player.getX() + player.getWidth() > platforms[i].getX() && player.getY() + player.getHeight() >= platforms[i].getY() && player.getY() < platforms[i].getY() + platforms[i].getHeight()) {
            if (player.getY() > platforms[i].getY() + platforms[i].getHeight()/2) {
                player.setVelocity(player.getVelocity().x, 5);
            } 
            else if (player.getY() + player.getHeight() <  platforms[i].getY() + platforms[i].getHeight()) {    
                onPlatform = true;
                player.setY(platforms[i].getY() - player.getHeight());
                player.setY(player.getY() + 1);
            }
        }
    }
    player.setOnPlatform(onPlatform);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    
    resetScore();
    sprintf(highscore, "BEST: %d", highscoreInt);
    
    
    int mouseDownX = 0;
    int mouseDownY = 0;
    
    double lavaY = render.getScreenHeight() - 32;
    double timer = 0; 
    double splashTimer = 0;
    
    bool firstTime = true;
    bool playedSplash = false;
    bool playedSelect = false;
    
    render.setupWindow();
    audio.setupAudio();

    
    while (render.isRunning()) {
        render.checkEvent();
        
        if (titleScreen) {
            if (splashTimer > 120) {
                if (!playedSelect) {
                    audio.playSelect();
                    playedSelect = true;
                }
                render.showIntroMenu(highscore);
                
                if (render.isMousePressed()) {
                    audio.playSelect();
                    titleScreen = false;
                    mouseDownX = render.getMouseX();
                    mouseDownY = render.getMouseY();
                }
            }
            else {
                if (!playedSplash) {
                    audio.playSplash();
                    playedSplash = true;
                }
                render.showSplashScreen();
                
                splashTimer += 1;
            }
        } 
        else {
            if (playCoinFX) {
                audio.playCoin();
                playCoinFX = false;
            }
            
            if (render.isMousePressed() && player.isOnGround()) {
                audio.playClick();
                mouseDownX = render.getMouseX();
                mouseDownY = render.getMouseY();
            }
            
            if (render.isMouseReleased() && player.isOnGround()) {
                if (firstTime) {
                    firstTime = false;
                }
                else {
                    audio.playLaunch();
                    
                    if(player.isOnPlatform()){
                        player.setY(player.getY() - 1);
                    }
                    
                    int velocityX = render.getMouseX() - mouseDownX;
                    int velocityY = render.getMouseY() - mouseDownY;
                    
                    player.setVelocity((double)velocityX*.08, (double)velocityY*.08);
                }
            }
            
            checkPlayerCollision();
            player.updatePosition();
            
            if (player.getY() > render.getScreenHeight()) {
                audio.playDeath();
                resetGame();
            }
            for (int i = 0; i < 4; i++) {
                platforms[i].updatePosition();
            }
            
            lavaY = render.getScreenHeight() - 43 - sin(timer) * 5;
            timer += 0.05;

            render.drawBackground();
            render.drawCursor();

            if (render.isMouseDown() && player.isOnGround()) {
                render.drawLine(mouseDownX, mouseDownY, player.getX(), player.getY(), player.getWidth(), player.getHeight());
            }
                        
            for (int i = 0; i < 4; i++) {
                render.drawPlatform((int)platforms[i].getX(), (int)platforms[i].getY());
                
                if (platforms[i].getHasCoin()) {
                    render.drawCoin(platforms[i].getCoinX(), platforms[i].getCoinY());
                }
            }
            render.drawPlayer((int)player.getX(), (int)player.getY());
            render.drawLava((int)lavaY);    
            render.drawScore(score, highscore);
        }
       
    }

    render.freeMemory();
    audio.freeMemory();   
   
    return 0;
}
