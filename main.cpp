#include "raylib.h"
#include <string>

#define WINDOW_W 600
#define WINDOW_H 680

#define MAX_SNAKE 1000

typedef struct {
    int x;
    int y;
} SnakePart;

SnakePart snake[MAX_SNAKE];
int snakeLength = 1;
int grow = 0;

int aX = 0;
int aY = 0;

int gridW = 600/40;
int gridH = 680/40;


void DrawTextCentered(Font font, const char *text, float centerX, float centerY, float fontSize, float spacing, Color color)
{
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing); // center text function
    DrawTextEx(font, text, (Vector2){centerX - size.x / 2, centerY - size.y / 2}, fontSize, spacing, color);
}

void DrawTextureCentered(Texture2D texture, float centerX, float centerY, Color color)
{ // same but for 2d textures
    DrawTexture(texture, centerX - texture.width / 2, centerY - texture.height / 2, color);
}

void appleSpawn() { // apple generator
    aX = GetRandomValue(0, gridW -1);
    aY = GetRandomValue(0, gridH -1);
}

void resetGame(int &pX, int &pY, int &score, int &dir, int &frameCounter) {
    snake[0].x = 7;
    snake[0].y = 7;
    snakeLength = 1;
    grow = 0;

    pX = 7;
    pY = 7;
    score = 0;
    dir = 1;
    frameCounter = 0;
    appleSpawn();
}

int main() {
    InitWindow(600, 680, "SnakeCroccante");
    InitAudioDevice();

    SetTargetFPS(60);
    Sound eat1 = LoadSound("assets/eat1.wav");
    Sound eat2 = LoadSound("assets/eat2.wav");
    Sound wall = LoadSound("assets/wall.wav");
    Sound menu = LoadSound("assets/menu.wav");
    Texture2D logo = LoadTexture("assets/logo.png");
    Texture2D menubg = LoadTexture("assets/menubg.png");
    Texture2D gamebg = LoadTexture("assets/gamebg.png");
    Music bgm = LoadMusicStream("assets/bgm.ogg");
    PlayMusicStream(bgm);

    const int cellSize = 40;

    appleSpawn();

    int pX = 7;
    int pY = 7;
    int score = 0;
    int highscore = 0;
    bool mainmenu = true;

    int frameCounter = 0;

    int dir = 1; //1= up 2=left 3=down 4=right

    bool sconfitta = false;

    snake[0].x = pX;
    snake[0].y = pY;

    
    while (!WindowShouldClose()) {

        UpdateMusicStream(bgm);

        if(!mainmenu)frameCounter++;

        if (mainmenu) // mainmenu
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                mainmenu = false;
                PlaySound(menu);
            }
        }

        if((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && dir!=3) dir=1;
        if((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && dir!=1) dir=3; //player input
        if((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && dir!=4) dir=2;
        if((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && dir!=2) dir=4;

    if(!sconfitta && !mainmenu) { 
        if(frameCounter==8) { // direction movement

            // Passo 1: shifta il corpo dalla coda verso la testa
            for (int i = snakeLength - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
            }

            // [FIX] Passo 1b: se c'è crescita in attesa
            if (grow > 0) {
                snakeLength++;
                grow--;
            }

            // Passo 2: muovi la testa in base alla direzione
            if(dir==1) {
                snake[0].y -= 1; // up
            } else if (dir==2) {
                snake[0].x -= 1; // left
            } else if (dir==3) {
                snake[0].y += 1; //down
            } else if (dir==4) {
                snake[0].x += 1; // right
            }

            pX = snake[0].x;
            pY = snake[0].y;

            if(pX == aX && pY == aY) { // eating apple
                appleSpawn();
                PlaySound(eat1);
                score++;
                grow++;
            }

            for (int i = 1; i < snakeLength; i++)
            {
                if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
                {
                    sconfitta = true;
                    PlaySound(wall);
                }
            }

            frameCounter=0;
        }
    }

        if(!sconfitta && (pX >= gridW || pY >=gridH || pX < 0 || pY < 0)) { // check sconfitta dai muri
            sconfitta = true;
            PlaySound(wall);
        }

        if (sconfitta && IsKeyPressed(KEY_ENTER))
        {
            sconfitta = false;
            resetGame(pX, pY, score, dir, frameCounter);
        }

        BeginDrawing();
        ClearBackground(BLANK);

        if (mainmenu)
        {   
            DrawTexture(menubg, 0, 0, WHITE);
            DrawTextureCentered(logo, WINDOW_W / 2, WINDOW_H / 2, WHITE);
            DrawTextCentered(GetFontDefault(), "Press ENTER", WINDOW_W / 2, 460, 50, 2, WHITE);
            DrawTextCentered(GetFontDefault(), "Music by Pape", WINDOW_W / 2, 640, 30, 2, WHITE);
        } else {
            DrawTexture(gamebg, 0, 0, WHITE);
        if(sconfitta) {
            if(score>highscore) {
                highscore=score;
            }
            DrawTextCentered(GetFontDefault(), "Game Over", WINDOW_W / 2, WINDOW_H / 2 -120, 100, 2, BLACK);
            DrawTextCentered(GetFontDefault(), "Press Enter to restart", WINDOW_W / 2, WINDOW_H / 2, 50, 2, BLACK);
            DrawTextCentered(GetFontDefault(),
                 ("Score: " + std::to_string(score)).c_str(),
                 WINDOW_W / 2,
                 WINDOW_H / 2 + 60,
                 50, 2, BLACK);
        
            DrawTextCentered(GetFontDefault(),
                 ("High Score: " + std::to_string(highscore)).c_str(),
                 WINDOW_W / 2,
                 WINDOW_H / 2 + 120,
                 50, 2, BLACK);
        } else {

            for (int i = 0; i < snakeLength; i++) {
                Color segColor = (i == 0) ? DARKBLUE : BLUE; // snake
                DrawRectangle(snake[i].x * cellSize, snake[i].y * cellSize, cellSize, cellSize, segColor);
            }
            DrawRectangle(aX * cellSize, aY * cellSize, cellSize, cellSize, RED); //apple
        }

        if(!sconfitta){
        DrawText(("Score: " + std::to_string(score)).c_str(),
                  400, 0, 35, BLACK);
        }
        }
        EndDrawing();
    }
    UnloadSound(eat1);
    UnloadSound(eat2);
    UnloadSound(wall);
    UnloadSound(menu);
    UnloadTexture(logo);
    UnloadTexture(menubg);
    UnloadTexture(gamebg);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}