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
    InitAudioDevice();

    Sound session_begin = LoadSound("resources/menu.ogg");
    SetSoundVolume(session_begin, 0.7f);

    Sound session_end = LoadSound("resources/ping.ogg");
    SetSoundVolume(session_end, 0.7f);

    Sound bonk = LoadSound("resources/bonk.ogg");

    SetExitKey(KEY_NULL); // makes sure you don't accidentally hit esc and close the program

    int framesCounter = 0;
    Font fontTtf = LoadFontEx("resources/PixAntiqua.ttf", 48, 0, 250);
    GameScreen currentScreen = MAIN;

    int study_mins = STUDY_TIMER;
    int break_mins = BREAK_TIMER;
    int session_counter = 0;
    struct t_time time;
    bool sound_played = false;
    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose()) // Detects window close
    {
        switch (currentScreen) {
            case MAIN:
            {
                if (sound_played == false) {
                    PlaySound(bonk);
                    sound_played = true;
                }

                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = STUDY;
                    PlaySound(session_begin);
                }
            } break;
            case STUDY:
            {
                framesCounter++;
                if (framesCounter == 60) {
                    framesCounter = 0;
                    study_mins--;
                    conversion(study_mins, &time);
                }
                if (study_mins == 0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = BREAK;
                    study_mins = STUDY_TIMER;
                    conversion(study_mins, &time);
                    PlaySound(session_end);
                }
            } break;
            case BREAK:
            {
                framesCounter++;
                if (framesCounter == 60) {
                    framesCounter = 0;
                    break_mins--;
                    conversion(break_mins, &time);
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = MAIN;
                    sound_played = false;
                    break_mins = BREAK_TIMER;
                    conversion(break_mins, &time);
                } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    PlaySound(session_begin);
                    currentScreen = STUDY;
                    break_mins = BREAK_TIMER;
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
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "New session!", (Vector2){ 20.0f, 40.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                } break;
                case STUDY:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "Study time!", (Vector2){ 20.0f, 40.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                    DrawTextEx(fontTtf, TextFormat("%.2d:%.2d", time.mins, time.secs), (Vector2){ 20.0f, 100.0f }, (float)fontTtf.baseSize, 2, GOLD);
                } break;
                case BREAK:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "Break time!", (Vector2){ 20.0f, 40.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                    DrawTextEx(fontTtf, TextFormat("%.2d:%.2d", time.mins, time.secs), (Vector2){ 20.0f, 100.0f }, (float)fontTtf.baseSize, 2, GOLD);
                } break;
                default: break;
            }
        /*DrawFPS(10, 10);*/
        EndDrawing();
    }
    UnloadFont(fontTtf);
    UnloadSound(session_begin);
    UnloadSound(session_end);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

static void conversion (int seconds, struct t_time *h){
    h -> hrs  =  seconds / 3600;
    h -> mins = (seconds / 60) % 60;
    h -> secs =  seconds % 60;
}
