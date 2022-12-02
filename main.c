#include <stdint.h>
#include <stdio.h>
#include <raylib.h>

typedef enum
{
    EMPTY,
    SNAKE,
    APPLE
} FieldType;

typedef enum
{
    NONE,
    UP,
    RIGHT,
    DOWN,
    LEFT
} Direction;

/* Snake tick delay, decrase to speed up snake */
const double TICK_DELAY = 0.15;

/* Board 16x16 */
/* 2D positions are represented as 1D index */
/* [0, 0] = 0 */
/* [1, 0] = 1 */
/* [15, 0] = 15 */
/* [0, 1] = 16 */
/* [1, 1] = 17 */
/* [2, 1] = 18 */
/* etc. */

/* Stores snake position, [0] is a head*/
uint16_t snake[256];

/* Stores apple position*/
uint16_t apple;

/* Stores data about every cell on board */
/* Helps to avoid looping through snake with each collision calculation */
FieldType board[256];

uint16_t snakeLength;
Direction direction;
double elapsedTime;

/* Returns mapped 2D coords to 1D index */
uint16_t ToIndex(uint16_t x, uint16_t y)
{
    return x + ((y % 16) * 16);
}

void ResetGame(void)
{
    for(uint16_t i = 0;i < 256;i++) board[i] = EMPTY;
    snakeLength = 1;
    snake[0] = ToIndex(4, 4);
    board[snake[0]] = SNAKE;
    apple = ToIndex(12, 12);
    board[apple] = APPLE;
    direction = NONE;
    elapsedTime = GetTime();
}

void InitializeGame(void)
{
    InitWindow(800, 800, "SnakeGame");
    SetTargetFPS(60);
    ResetGame();
}

/* Returns next head position based on direction */
uint16_t GetNextIndex(void)
{
    if(direction == UP) return snake[0] - 16;
    else if(direction == RIGHT) return snake[0] + 1;
    else if(direction == DOWN) return snake[0] + 16;
    else if(direction == LEFT) return snake[0] - 1;
    else return snake[0];
}

/* Updates direction based on input */
void UpdateDirection(void)
{
    if(IsKeyPressed(KEY_W) && direction != DOWN) direction = UP;
    else if(IsKeyPressed(KEY_D) && direction != LEFT) direction = RIGHT;
    else if(IsKeyPressed(KEY_S) && direction != UP) direction = DOWN;
    else if(IsKeyPressed(KEY_A) && direction != RIGHT) direction = LEFT;
}

/* Returns true if snake hits wall in next move */
bool CheckBoardCollision(void)
{
    if(snake[0] < 16 && direction == UP) return true;
    else if(snake[0] % 16 == 15 && direction == RIGHT) return true;
    else if(snake[0] / 16 == 15 && direction == DOWN) return true;
    else if(snake[0] % 16 == 0 && direction == LEFT) return true;
    else return false;
}

/* Increases snake size and generates new position for apple */
void EatApple(void)
{
    snakeLength++;
    board[apple] = EMPTY;

    // Search for empty cell
    uint16_t randomPosition = GetRandomValue(0, 256);
    while(board[randomPosition] == SNAKE || randomPosition == snake[0] || randomPosition == apple)
    {
        randomPosition = GetRandomValue(0, 256);
    }

    apple = randomPosition;
    board[apple] = APPLE;
}

/* Updates snake position and checks for collisions */
void UpdateSnake(void)
{  
    if(CheckBoardCollision() || board[GetNextIndex()] == SNAKE)
    {
        ResetGame();
        return;
    }
    
    // Clear only last part of tail from board
    board[snake[snakeLength - 1]] = EMPTY;

    if(GetNextIndex() == apple)
    {
        EatApple();
    }
    
    // Update snake position
    // IDEA: Snake as linked list could be better idea
    for(uint16_t i = (snakeLength - 1);i > 0;i--)
    {
        snake[i] = snake[i-1]; 
    }
    
    // Update head positon
    snake[0] = GetNextIndex();
    board[snake[0]] = SNAKE;
}

void DrawSnake(void)
{
    for(uint16_t i = 0;i < snakeLength;i++)
    {
        DrawRectangle((snake[i] % 16) * 50.f, (snake[i] / 16) * 50.f, 50.f, 50.f, GREEN);
    }
}

void DrawBoard(void)
{
    for(int i = 1;i < 16;i++)
    {
        Vector2 startV = {i * 50.f, 0.f};
        Vector2 endV = {i * 50.f, 800.f};
        DrawLineEx(startV, endV, 3.f, BLACK);
        Vector2 startH = {0.f, i * 50.f};
        Vector2 endH = {800.f, i * 50.f};
        DrawLineEx(startH, endH, 3.f, BLACK);
    }
}

int main(void)
{
    InitializeGame();

    while (!WindowShouldClose())
    {
        UpdateDirection();
        if(elapsedTime + TICK_DELAY < GetTime())
        {
            elapsedTime = GetTime();
            UpdateSnake();
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawSnake();
            DrawRectangle((apple % 16) * 50.f, (apple / 16) * 50.f, 50.f, 50.f, RED); // Draws apple
            DrawBoard();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

