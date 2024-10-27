#include <graphics.h>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cmath>    // For sin, cos functions
#include <ctime>    // For time function
#include <cstdlib> 

class GameObject {
protected:
    int x, y;
    int speed;
    int color;
    int size;
public:
    GameObject(int startX, int startY, int objectSpeed, int objectColor, int objectSize)
        : x(startX), y(startY), speed(objectSpeed), color(objectColor), size(objectSize) {}
    
    virtual void draw() = 0;
    virtual void update() = 0;
    
    int getX() const { return x; }
    int getY() const { return y; }
    int getSize() const { return size; }
    
    bool collidesWith(const GameObject& other) {
        int dx = x - other.getX();
        int dy = y - other.getY();
        int minDist = (size + other.getSize()) / 2;
        return (dx * dx + dy * dy) < (minDist * minDist);
    }
};

class Bullet : public GameObject {
public:
    Bullet(int startX, int startY)
        : GameObject(startX, startY, 7, LIGHTBLUE, 4) {}
    
    void draw() override {
        // Enhanced bullet design - energy ball effect
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        fillellipse(x, y, size, size);
        
        // Add glowing effect
        setcolor(WHITE);
        circle(x, y, size + 1);
        circle(x, y, size + 2);
    }
    
    void update() override {
        y -= speed;
    }
    
    bool isOffscreen() {
        return y < 0;
    }
};

class Player : public GameObject {
private:
    std::vector<GameObject*>& bullets;
    int animationFrame;
public:
    Player(std::vector<GameObject*>& bulletList)
        : GameObject(getmaxx()/2, getmaxy()-50, 5, YELLOW, 20), 
          bullets(bulletList), animationFrame(0) {}
    
    void draw() override {
        // Draw the warrior character
        
        // Body
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x, y, size/2, size);  // Body
        
        // Head
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x, y - size, size/2, size/2);
        
        // Eyes
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(x - size/4, y - size - 2, 2, 2);
        fillellipse(x + size/4, y - size - 2, 2, 2);
        
        // Arms (animated)
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        int armOffset = sin(animationFrame * 0.1) * 3;
        fillellipse(x - size/2, y - size/2 + armOffset, size/4, size/2);
        fillellipse(x + size/2, y - size/2 - armOffset, size/4, size/2);
        
        // Legs
        fillellipse(x - size/3, y + size, size/4, size/2);
        fillellipse(x + size/3, y + size, size/4, size/2);
        
        // Weapon effect
        setcolor(WHITE);
        line(x - size, y - size/2, x + size, y - size/2);
        
        animationFrame++;
    }
    
    void update() override {
        if (GetAsyncKeyState(VK_LEFT) && x > size)
            x -= speed;
        if (GetAsyncKeyState(VK_RIGHT) && x < getmaxx() - size)
            x += speed;
    }
    
    void shoot() {
        bullets.push_back(new Bullet(x, y - size*2));
    }
};

class Enemy : public GameObject {
private:
    int animationFrame;
    int type;  // Different types of monsters
public:
    Enemy(int startX)
        : GameObject(startX, 0, 2, RED, 15),
          animationFrame(0),
          type(rand() % 3) {}
    
    void draw() override {
        // Draw different types of monsters
        switch(type) {
            case 0:
                drawBasicMonster();
                break;
            case 1:
                drawEyeMonster();
                break;
            case 2:
                drawTentacleMonster();
                break;
        }
        animationFrame++;
    }
    
    void drawBasicMonster() {
        // Body
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x, y, size, size);
        
        // Eyes
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x - size/3, y - size/3, size/4, size/4);
        fillellipse(x + size/3, y - size/3, size/4, size/4);
        
        // Pupils
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(x - size/3, y - size/3, size/8, size/8);
        fillellipse(x + size/3, y - size/3, size/8, size/8);
        
        // Mouth
        arc(x, y, 200, 340, size/2);
    }
    
    void drawEyeMonster() {
        // Body
        setcolor(LIGHTRED);
        setfillstyle(SOLID_FILL, LIGHTRED);
        fillellipse(x, y, size, size);
        
        // Large central eye
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x, y, size*2/3, size*2/3);
        
        // Pupil (moving)
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, BLACK);
        float pupilOffset = sin(animationFrame * 0.1) * (size/4);
        fillellipse(x + pupilOffset, y, size/3, size/3);
        
        // Tentacles
        setcolor(LIGHTRED);
        for(int i = 0; i < 6; i++) {
            float angle = i * 60 + animationFrame * 2;
            int xEnd = x + cos(angle * 3.14159/180) * size;
            int yEnd = y + sin(angle * 3.14159/180) * size;
            line(x, y, xEnd, yEnd);
        }
    }
    
    void drawTentacleMonster() {
        // Body
        setcolor(LIGHTMAGENTA);
        setfillstyle(SOLID_FILL, LIGHTMAGENTA);
        fillellipse(x, y, size*2/3, size);
        
        // Tentacles
        float tentacleOffset = sin(animationFrame * 0.1) * (size/4);
        for(int i = 0; i < 4; i++) {
            int xOffset = cos(i * 90 * 3.14159/180) * size;
            int yOffset = sin(i * 90 * 3.14159/180) * size;
            int ctrlX = x + xOffset/2 + tentacleOffset;
            int ctrlY = y + yOffset/2 + tentacleOffset;
            
            setcolor(LIGHTMAGENTA);
            line(x, y, ctrlX, ctrlY);
            line(ctrlX, ctrlY, x + xOffset, y + yOffset);
        }
        
        // Eyes
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x - size/3, y, size/4, size/4);
        fillellipse(x + size/3, y, size/4, size/4);
    }
    
    void update() override {
        y += speed;
        // Add swaying motion
        x += sin(animationFrame * 0.1) * 0.5;
    }
    
    bool isOffscreen() {
        return y > getmaxy();
    }
};

class Game {
private:
    Player* player;
    std::vector<GameObject*> bullets;
    std::vector<GameObject*> enemies;
    int score;
    int spawnTimer;
    
public:
    Game() : score(0), spawnTimer(0) {
        initwindow(800, 600, "Monster Shooter");
        player = new Player(bullets);
    }
    
    void run() {
        // Set background color
        setbkcolor(DARKGRAY);
        
        while (!GetAsyncKeyState(VK_ESCAPE)) {
            cleardevice();
            drawBackground();
            handleInput();
            updateGameObjects();
            handleCollisions();
            spawnEnemies();
            drawGameObjects();
            displayScore();
            delay(16);
        }
    }
    
private:
    void drawBackground() {
        // Add simple stars in background
        for(int i = 0; i < 50; i++) {
            int x = rand() % getmaxx();
            int y = rand() % getmaxy();
            putpixel(x, y, WHITE);
        }
    }
    
    void handleInput() {
        player->update();
        if (GetAsyncKeyState(VK_SPACE) & 1)
            player->shoot();
    }
    
    void updateGameObjects() {
        for (auto it = bullets.begin(); it != bullets.end();) {
            (*it)->update();
            if (((Bullet*)*it)->isOffscreen()) {
                delete *it;
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
        
        for (auto it = enemies.begin(); it != enemies.end();) {
            (*it)->update();
            if (((Enemy*)*it)->isOffscreen()) {
                delete *it;
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void handleCollisions() {
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            bool enemyHit = false;
            
            for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
                if ((*enemyIt)->collidesWith(**bulletIt)) {
                    // Add explosion effect
                    int ex = (*enemyIt)->getX();
                    int ey = (*enemyIt)->getY();
                    for(int i = 0; i < 360; i += 30) {
                        int x = ex + cos(i * 3.14159/180) * 20;
                        int y = ey + sin(i * 3.14159/180) * 20;
                        setcolor(YELLOW);
                        line(ex, ey, x, y);
                    }
                    
                    delete *bulletIt;
                    bulletIt = bullets.erase(bulletIt);
                    enemyHit = true;
                    score += 10;
                } else {
                    ++bulletIt;
                }
            }
            
            if (enemyHit) {
                delete *enemyIt;
                enemyIt = enemies.erase(enemyIt);
            } else {
                ++enemyIt;
            }
        }
    }
    
    void spawnEnemies() {
        spawnTimer++;
        if (spawnTimer >= 60) {
            int x = rand() % (getmaxx() - 30) + 15;
            enemies.push_back(new Enemy(x));
            spawnTimer = 0;
        }
    }
    
    void drawGameObjects() {
        player->draw();
        for (auto bullet : bullets)
            bullet->draw();
        for (auto enemy : enemies)
            enemy->draw();
    }
    
    void displayScore() {
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        char scoreText[50];
        sprintf(scoreText, "Score: %d", score);
        outtextxy(10, 10, scoreText);
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run();
    return 0;
}