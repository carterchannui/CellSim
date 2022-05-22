#include "/opt/homebrew/Cellar/raylib/4.0.0/include/raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Definitions
//----------------------------------------------------------------------------------
#define SQUARE_SIZE 31

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Cell {
    Vector2 position;
    Vector2 size;
    Vector4 environment;
} Cell;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
const int screenHeight = 800;
const int screenWidth = 800;
static Cell cell;
static Vector2 offset = { screenWidth%SQUARE_SIZE, screenHeight%SQUARE_SIZE};
static Vector2 foodPosition = (Vector2){ 0 };
static Vector2 foodSize = { (float)SQUARE_SIZE, (float)SQUARE_SIZE };

//------------------------------------------------------------------------------------
// Function Declarations (local)
//------------------------------------------------------------------------------------
static void InitSim(void);
static void DrawSim(void);
static void UpdateSim(void);
static void scan_env(Cell);
//------------------------------------------------------------------------------------
// Program entry point
//------------------------------------------------------------------------------------
int main(void)
{
    InitWindow(screenWidth, screenHeight, "game");
    InitSim();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        DrawSim();
        UpdateSim();
            // Check for adjacent food.
            if (cell.position.x + offset.x/2 == foodPosition.x + offset.x/2 && cell.position.y + offset.y/2 == foodPosition.y + offset.y/2) {
                foodPosition = (Vector2){
                    (floor(((screenWidth/(float)((rand() % 5) + 1)) / SQUARE_SIZE) - 1) * SQUARE_SIZE) + offset.x/2, 
                    (floor(((screenHeight/(float)((rand() % 5) + 1)) / SQUARE_SIZE) - 1) * SQUARE_SIZE) + offset.y/2
                };
            }
    }

    CloseWindow();

    return 0;
}

void InitSim(void) {
    cell.position = (Vector2){ offset.x/2, offset.y/2 };
    cell.size = (Vector2){ (float)SQUARE_SIZE, (float)SQUARE_SIZE };
    cell.environment = (Vector4){ 0 };
    foodPosition = (Vector2){
        (floor((screenWidth/2.00) / SQUARE_SIZE) * SQUARE_SIZE) + offset.x/2,
        (floor((screenHeight/2.00) / SQUARE_SIZE) * SQUARE_SIZE) + offset.y/2 
    };
}

void UpdateSim(void) {
    if (IsKeyDown(KEY_RIGHT)) {
        cell.position.x += SQUARE_SIZE;
        scan_env(cell);
    } else if (IsKeyDown(KEY_LEFT)) {
        cell.position.x -= SQUARE_SIZE;
        scan_env(cell);
    } else if (IsKeyDown(KEY_UP)) {
        cell.position.y -= SQUARE_SIZE;
        scan_env(cell);
    } else if (IsKeyDown(KEY_DOWN)) {
        cell.position.y += SQUARE_SIZE;
        scan_env(cell);
    }
}

void DrawSim(void)
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
            // Render simulation grid.
            for (int i = 0; i < screenWidth/SQUARE_SIZE + 1; i++)
            {
                DrawLineV(
                        (Vector2){SQUARE_SIZE*i + offset.x/2, offset.y/2},
                        (Vector2){SQUARE_SIZE*i + offset.x/2, screenHeight - offset.y/2}, 
                        LIGHTGRAY
                        );
            }

            for (int i = 0; i < screenHeight/SQUARE_SIZE + 1; i++)
            {
                DrawLineV(
                        (Vector2){offset.x/2, SQUARE_SIZE*i + offset.y/2}, 
                        (Vector2){screenWidth - offset.x/2, SQUARE_SIZE*i + offset.y/2}, 
                        LIGHTGRAY
                        );
            }
            DrawRectangleV(cell.position, cell.size, MAROON);
            DrawRectangleV(foodPosition, foodSize, BLUE);
            EndDrawing();
}

void scan_env(Cell cell) {
    // Scan a cell's environment.
    if (cell.position.x + offset.x/2 == foodPosition.x + offset.x/2 && cell.position.y + offset.y/2 == foodPosition.y + offset.y/2 + SQUARE_SIZE) {
        cell.environment.x = 1;
        printf("Object detected above\n");
    }
    if (cell.position.x + offset.x/2 == foodPosition.x + offset.x/2 - SQUARE_SIZE && cell.position.y + offset.y/2 == foodPosition.y + offset.y/2) {
        cell.environment.y = 1;
        printf("Object detected to the right\n");
    }
    if (cell.position.x + offset.x/2 == foodPosition.x + offset.x/2 && cell.position.y + offset.y/2 == foodPosition.y + offset.y/2 - SQUARE_SIZE) {
        cell.environment.z = 1;
        printf("Object detected below\n");
    }
    if (cell.position.x + offset.x/2 == foodPosition.x + offset.x/2 + SQUARE_SIZE && cell.position.y + offset.y/2 == foodPosition.y + offset.y/2) {
        cell.environment.w = 1;
        printf("Object detected to the left\n");
    }
}

