#include "raylib.h"
#include "config.h"

/*
 * Functionality:
 * + Set amount of sessions and breaks in config.h
 * + Countdown timer runs for allotted amount of time
 * + Break runs for allotted amount of time
 * + Set keys similar to tomatoshell to control the program when focused
 * + MM:SS time format
 *
 * Features to maybe add:
 * + Streamer mode (sets background to green perma for chroma key)
 *
 * Remaps:
 *  Mouse1 -> Next session
 *  Mouse2 -> Previous session
 *  r      -> Reset timer
 */

// Screen states
typedef enum GameScreen {
    MAIN,
    STUDY,
    BREAK
} GameScreen;

struct t_time {
    int hrs;
    int mins;
    int secs;
};

static void conversion (int seconds, struct t_time *h);

int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "pomodoro");
    SetExitKey(KEY_NULL); // makes sure you don't accidentally hit esc and close the program

    int framesCounter = 0;
    Font fontTtf = LoadFontEx("resources/PixAntiqua.ttf", 32, 0, 250);
    GameScreen currentScreen = MAIN;

    int countdown = STUDY_TIMER * 60;
    struct t_time time;

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
                framesCounter++;
                if (framesCounter == 60) {
                    framesCounter = 0;
                    countdown--;
                    conversion(countdown, &time);
                }
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = BREAK;
                    countdown = STUDY_TIMER * 60;
                    conversion(countdown, &time);
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
            ClearBackground(WHITE);
            switch (currentScreen) {
                case MAIN:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, WHITE);
                    DrawTextEx(fontTtf, "Ready?", (Vector2){ 20.0f, 100.0f }, (float)fontTtf.baseSize, 2, MAROON);
                } break;
                case STUDY:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, WHITE);
                    DrawTextEx(fontTtf, "Study time!", (Vector2){ 20.0f, 40.0f }, (float)fontTtf.baseSize, 2, MAROON);
                    DrawTextEx(fontTtf, TextFormat("%.2d:%.2d", time.mins, time.secs), (Vector2){ 20.0f, 100.0f }, (float)fontTtf.baseSize, 2, MAROON);
                } break;
                case BREAK:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "Break time!", (Vector2){ 20.0f, 100.0f }, (float)fontTtf.baseSize, 2, WHITE);
                } break;
                default: break;
            }
        /*DrawFPS(10, 10);*/
        EndDrawing();
    }
    UnloadFont(fontTtf);
    CloseWindow();

    return 0;
}

static void conversion (int seconds, struct t_time *h){
    h -> hrs  =  seconds / 3600;
    h -> mins = (seconds / 60) % 60;
    h -> secs =  seconds % 60;
}
