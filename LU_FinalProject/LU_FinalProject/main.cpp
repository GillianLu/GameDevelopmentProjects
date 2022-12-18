#include <raylib.h>
#include <raymath.h>
#include <iostream>
  // Used for sinf()
typedef enum { TITLE, GAMEPLAY, ENDING } GameScreen;

struct Index
{
    int index = 0;
    bool moveLeft = false;
};


// COLLISION with paddle
bool intersects(Vector2 playerPos, Vector2 playerSize, Vector2 rectanglePos, Vector2 rectangleSize)
{
    if(playerPos.x + playerSize.x > rectanglePos.x &&
        playerPos.x < rectanglePos.x + rectangleSize.x &&
        playerPos.y+playerSize.y > rectanglePos.y &&
        playerPos.y < rectanglePos.y+rectangleSize.y) {
        return true;
    }
    return false;
}

int main(void) {
    float WIDTH = 480;
    float HEIGHT = 640;
    
    InitWindow(WIDTH, HEIGHT, "Spooky Doodle Jump");
    //InitAudioDevice();
    SetTargetFPS(60);
    
    //BACKGROUNDS
    Texture2D Background = LoadTexture("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/title.png");
    Texture2D GameBackground = LoadTexture("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/gamebg.png");
    
    //DOODLE + PADDLES
    Texture2D Doodle = LoadTexture("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/scary.png");
    Texture2D Paddles = LoadTexture("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/paddles.png");
    
    // MUSIC
    Music music = LoadMusicStream("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/music/main.mp3");
    PlayMusicStream(music);
    
    //SOUND EFFECTS
    Sound winnerSound = LoadSound("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/music/winner.mp3");
    Sound loserSound = LoadSound("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/music/loser.mp3");

    //BACKGROUND, DOODLE, & PADDLE POSITION
    Background.width = WIDTH;
    Background.height = HEIGHT;
    GameBackground.width = WIDTH;
    GameBackground.height = HEIGHT;
    Vector2 doodlePosition = {WIDTH - Doodle.width/2, HEIGHT/3};
    Vector2 doodleSize = {float(Doodle.width), float(Doodle.height)};
    Vector2 paddlesPosition[100];
    
    //FONT
    Font font = LoadFont("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/font/horroweenbold.ttf");
    Font subfont = LoadFont("/Users/Admin/Desktop/LU_FinalProject/LU_FinalProject/resources/font/Horror.ttf");
    
    //DOODLE JUMPING STATS
    int movingSpeed = 10;
    int fallingSpeed = 10;
    float gravity = 1.50f;
    float velocity = 15.0f;
    float jump = velocity;
    float acceleration = 0;
    
    //CUTTING THE PADDLE TILE SET
    Rectangle rect[2];
    rect[0] = {0, 0, 64, 8};
    rect[1] = {64, 0, 64, 8};
    
    //PADDLE & INDEX RANDOMIZER
    for (int i = 0; i < 100; i++)
    {
        paddlesPosition[i].x = GetRandomValue(0, WIDTH - Paddles.width/2);
        if(i > 0)
        {
            paddlesPosition[i].y = paddlesPosition[i-1].y - GetRandomValue(120, 200);
        }
        else
        {
            paddlesPosition[0].y = HEIGHT - HEIGHT/5;
        }
    }

    
    //MOVING INDEX
    Index index[16];
    for(int i = 0; i < 16; i++)
    {
        index[i].index = GetRandomValue(0, 100);
    }
    
    //SCREENS
    bool gameOver = false;
    bool muteScreen = false;
    int framesCounter = 0;
    GameScreen currentScreen = TITLE;
    

    while(!WindowShouldClose())
    {
        UpdateMusicStream(music);
        switch (currentScreen)
        {
            case TITLE:
            {
                if(IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = GAMEPLAY;
                    framesCounter = 0;
                }
            } break;
                
            case GAMEPLAY:
            {
                
                if(muteScreen)
                {
                    StopMusicStream(music);
                    PlaySound(winnerSound);
                    currentScreen = ENDING;

                }
                if(!muteScreen)
                {
                    
                    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
                    {
                        doodlePosition.x -= movingSpeed;
                        if(doodlePosition.x + Doodle.width < 0 )
                        {
                            doodlePosition.x = WIDTH;
                        }
                    }
                    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
                    {
                        doodlePosition.x += movingSpeed;

                        if(doodlePosition.x > WIDTH)
                        {
                            doodlePosition.x = -Doodle.width;
                        }
                    }
                    else if (IsKeyDown(KEY_SPACE))
                    {
                        jump += gravity / 2;
                        acceleration += 0.1f;
                    }
                
                    //COLLISION WITH PADDLE
                    std::cout << jump << std::endl;
                    for(int i = 0; i < 100; i++)
                    {
                        Vector2 RectangleSize = {64,8};
                        if(intersects(doodlePosition, doodleSize, paddlesPosition[i], RectangleSize))
                        {
                            if(jump == 0)
                            {
                                acceleration += 0.05;        // player recieves acceleration when jumping
                                jump = velocity + acceleration;
                                
                            }
                            
                            
                            break;
                        }
                    }
                    
                    //COLLISION WITH ENEMY
                    
                    //if paddle position is greater than this, then doodle wins
                    for (int i = 0; i < 100; i++)
                    {
                        if(paddlesPosition[i].y > HEIGHT/4)
                        {
                            muteScreen = true;

                        }
                        else
                        {
                            muteScreen = false;
                            break;
                        }
                    }
                    
                    // determining the loser
                    for(int i = 0; i < 100; i++)
                    {
                        if(paddlesPosition[i].y < HEIGHT/4)
                        {
                            gameOver = true;
                            StopMusicStream(music);
                            PlaySound(loserSound);
                            currentScreen = ENDING;

                        }
                        else
                        {
                            gameOver = false;
                            break;
                        }
                    }
                    
                    //    move paddles down the screen e. g. player jumps
                    for(int i = 0; i < 100; i++)
                    {
                        paddlesPosition[i].y -= fallingSpeed;

                        if(jump)
                        {
                            paddlesPosition[i].y += jump;

                        }
                    }
                    
                    jump -= gravity;    //    gravity affects player's jumping velocity
                    if (jump < 0)
                    {
                        jump = 0;
                        acceleration = 18;
                    }
                    
                    if(gameOver)
                    {
                        muteScreen = true;
                    }
                    
                    framesCounter ++;

                    
                    //MOVING PADDLES LEFT AND RIGHT
                    for(int i = 0; i < 16; i++)
                    {
                        if(index[i].moveLeft)
                        {
                            paddlesPosition[index[i].index].x -= movingSpeed/3;

                            if(paddlesPosition[index[i].index].x < 0)
                            {
                                index[i].moveLeft = false;
                            }
                        }
                        
                        else
                        {
                            paddlesPosition[index[i].index].x += movingSpeed/2;
                            if(paddlesPosition[index[i].index].x + Paddles.width/2 > WIDTH)
                            {
                                index[i].moveLeft = true;
                            }
                        }
                    }
                    
          
                }
                
            } break;
                
            case ENDING:
            {
                if(IsKeyPressed(KEY_ENTER))
                {
                    PlayMusicStream(music);
                    currentScreen = GAMEPLAY;
                    gameOver = false;
                    muteScreen = false;
                    //reset doodle position
                    for (int i = 0; i < 100; i++)
                    {
                        paddlesPosition[i].x = GetRandomValue(0, WIDTH - Paddles.width/2);
                        if(i > 0)
                        {
                            paddlesPosition[i].y = paddlesPosition[i-1].y - GetRandomValue(128, 200);
                        }
                        else
                        {
                            paddlesPosition[0].y = HEIGHT - HEIGHT/5;
                        }
                    }
                   
                }
                
                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    PlayMusicStream(music);
                    currentScreen = TITLE;
                    
                    //reset the entire game
                    gameOver = false;
                    muteScreen = false;
                    
                    //reset doodle position
                    for (int i = 0; i < 100; i++)
                    {
                        paddlesPosition[i].x = GetRandomValue(0, WIDTH - Paddles.width/2);
                        if(i > 0)
                        {
                            paddlesPosition[i].y = paddlesPosition[i-1].y - GetRandomValue(128, 200);
                        }
                        else
                        {
                            paddlesPosition[0].y = HEIGHT - HEIGHT/5;
                        }
                    }
                    
                }
                
            } break;
            default:break;
        }
        
        BeginDrawing();
        ClearBackground(WHITE);
 
        switch (currentScreen)
        {
            case TITLE:
            {
                DrawTexture(Background, 0, 0, WHITE);
                Vector2 welcome = {WIDTH/9, HEIGHT/3};
                Vector2 to = {float(WIDTH/9), float(HEIGHT/2.38)};
                Vector2 horror = {float(WIDTH/9),float(HEIGHT/2)};
                Vector2 enter = {float(WIDTH/9),float(HEIGHT/1.5)};
                Vector2 Gillian = {float(WIDTH/9), float(HEIGHT/1.2)};
                
                
                DrawTextEx(font, "WELCOME",welcome, 50.0f, 2, WHITE);
                DrawTextEx(font, "TO",to, 50.0f, 2, WHITE);
                DrawTextEx(font, "HORROR DOODLE",horror, 50.0f, 2, RED);
                DrawTextEx(subfont, "Press ENTER to play", enter, 30, 2, WHITE);
                DrawTextEx(subfont, "Created by Gillian Lu", Gillian, 20, 2, RED);


            }break;
            case GAMEPLAY:
            {
                //variables
                Vector2 timerPosition = {float(WIDTH/20), float(HEIGHT/18)};
                DrawTexture(GameBackground, 0, 0, WHITE);
                
                DrawTextEx(font, TextFormat("DURATION: %04i",framesCounter/60), timerPosition, 25, 2, RED);
                Vector2 PaddlebyTwo = {float(Paddles.width/2), float(Paddles.height)};
                Vector2 Zero = {0,0};
                Vector2 ScreenSize = {WIDTH,HEIGHT};
                DrawTexture(Doodle,doodlePosition.x,doodlePosition.y,WHITE);
                //drawing paddles
                for(int i = 0; i < 100; i++)
                {
                    if(intersects(paddlesPosition[i], PaddlebyTwo, Zero, ScreenSize))
                    {
                        if(i%2 == 0) {
                            DrawTextureRec(Paddles, rect[0], paddlesPosition[i], WHITE);
                        } else {
                            DrawTextureRec(Paddles, rect[1], paddlesPosition[i], WHITE);
                        }
                    }
                }
                                
            } break;
            case ENDING:
            {
                if(muteScreen)
                {
                    Vector2 FinalTimePosition = {float(WIDTH/3.8), float(HEIGHT/18)};
                    Vector2 game = {WIDTH/5, HEIGHT/3};
                    Vector2 ENTER = {float(WIDTH/5.6), HEIGHT/2};
                    Vector2 BACKSPACE = {float(WIDTH/4.5), float(HEIGHT/1.5)};
                    Vector2 CONGRATS = {WIDTH/12, HEIGHT/3};
                    Vector2 ALIVE = {WIDTH/4, float(HEIGHT/2.4)};

                    
                    if(gameOver)
                    {
                        DrawTexture(Background, 0, 0, WHITE);
                        DrawTextEx(font, "GAME OVER!", game, 50, 2, RED);
                        DrawTextEx(font, TextFormat("DURATION: %04i",framesCounter/60), FinalTimePosition, 25, 2, RED);
                        DrawTextEx(subfont, "Press ENTER to play again!", ENTER, 25, 2, WHITE);
                        DrawTextEx(subfont, "Press BACKSPACE to menu!", BACKSPACE, 20, 2, WHITE);
                        
                    }
                    else
                    {
                        DrawTexture(Background, 0, 0, WHITE);
                        DrawTextEx(font, "CONGRATULATIONS", CONGRATS, 50, 2, RED);
                        DrawTextEx(font, "You're alive!", ALIVE, 40, 2, WHITE);
                        
                        DrawTextEx(font, TextFormat("DURATION: %04i",framesCounter/60), FinalTimePosition, 25, 2, RED);
                        DrawTextEx(subfont, "Press ENTER to play again!", ENTER, 25, 2, RED);
                        DrawTextEx(subfont, "Press BACKSPACE to menu!", BACKSPACE, 20, 2, RED);

                    }
                }
            } break;
            default:break;
        }
        
        EndDrawing();
    }
    
    //unload textures
    UnloadTexture(Background);
    UnloadTexture(Doodle);
    UnloadTexture(Paddles);
    
    //unload music
    UnloadMusicStream(music);
    
    //unload sounds
    UnloadSound(winnerSound);
    UnloadSound(loserSound);
   
    CloseAudioDevice();
    CloseWindow();
}

