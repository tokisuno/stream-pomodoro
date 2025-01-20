#include <raylib.h>
/*#include <stdio.h>*/

/*
 * Functionality:
 * + Set amount of sessions and breaks in config.h
 * + Countdown timer runs for allotted amount of time
 * + Break runs for allotted amount of time
 * + Set keys similar to tomatoshell to control the program when focused
 * + MM:SS time format
 * Features to maybe add:
 * + Streamer mode (sets background to green perma for chroma key)
 */

const static int SCREEN_W = 640;
const static int SCREEN_H = 480;

typedef enum GameScreen {
    MAIN,
    STUDY,
    BREAK
} GameScreen;

int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "pomodoro");
    SetExitKey(KEY_NULL); // makes sure you don't accidentally hit esc and close the program

    int framesCounter = 0;
    GameScreen currentScreen = MAIN;

    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose()) // Detects window close
    {
        switch (currentScreen) {
            case MAIN:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = STUDY;
                }
            } break;
            case STUDY:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = BREAK;
                }
            } break;
            case BREAK:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = MAIN;
                }
            } break;
            default: break;
        }

        // Drawing to screen
        BeginDrawing();
            ClearBackground(GREEN);

            switch (currentScreen) {
                case MAIN:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLUE);
                    DrawText("Ready?", 20, 20, 40, MAROON);
                } break;
                case STUDY:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, WHITE);
                    DrawText("Time to study!", 20, 20, 40, MAROON);
                } break;
                case BREAK:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, RED);
                    DrawText("Break time!", 20, 20, 40, WHITE);
                } break;
                default: break;
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
