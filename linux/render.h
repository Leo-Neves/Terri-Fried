#ifndef RENDER_H
#define RENDER_H

class Render {
private:
    int screenWidth;
    int screenHeight;
    
public:
    Render(int screenWidth, int screenHeight);
    void saveHighScore(int highscore);
    int loadHighScore();
    void setupWindow();
    void showSplashScreen();
    void showIntroMenu(char highscore[32]);
    bool isMouseMode();
    bool isKeyboardMode();
    bool isRunning();
    int getScreenHeight();
    int getScreenWidth();
    void checkEvent();
    bool isMouseDown();
    bool isMousePressed();
    bool isMouseReleased();
    int getMouseX();
    int getMouseY();
    void drawBackground();
    void drawLine(int mouseDownX, int mouseDownY, int playerX, int playerY, int playerWidth, int playerHeight);
    void drawPlatform(int x, int y);
    void drawCoin(int x, int y);
    void drawPlayer(int x, int y);
    void drawLava(int y);
    void drawScore(char score[32], char highscore[32]);
    void freeMemory();
};
#endif