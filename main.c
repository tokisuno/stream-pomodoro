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
 */

// define two structs for each study_time method

// Screen states
typedef enum GameScreen {
    STUDY_WAIT,
    STUDY,
    BREAK_WAIT,
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

    Image image = LoadImage("resources/never_give_up.jpg");
    Texture2D inspo = LoadTextureFromImage(image);
    UnloadImage(image);

    Sound session_begin = LoadSound("resources/menu.ogg");
    Sound session_end = LoadSound("resources/ping.ogg");
    Sound bonk = LoadSound("resources/bonk.ogg");

    SetSoundVolume(session_begin, 0.7f);
    SetSoundVolume(session_end, 0.7f);
    SetSoundVolume(session_end, 0.8f);

    struct t_time study_time;
    struct t_time break_time;

    bool sound_played = false;

    int framesCounter = 0;
    int study_mins = STUDY_TIMER;
    int break_mins = BREAK_TIMER;
    int session_counter = 0;

    SetExitKey(KEY_NULL); // makes sure you don't accidentally hit esc and close the program

    Font fontTtf = LoadFontEx("resources/PixAntiqua.ttf", 48, 0, 250);
    GameScreen currentScreen = STUDY_WAIT;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        switch (currentScreen) {
            case STUDY_WAIT:
            {
                if (sound_played == false) {
                    PlaySound(bonk);
                    sound_played = true;
                }

                if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = STUDY;
                    PlaySound(session_begin);
                    sound_played = false;
                }
            } break;
            case STUDY:
            {
                framesCounter++;
                if (framesCounter == 60) {
                    framesCounter = 0;
                    study_mins--;
                    conversion(study_mins, &study_time);
                }
                if (study_mins == 0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = BREAK_WAIT;
                    study_mins = STUDY_TIMER;
                    conversion(study_mins, &study_time);
                    PlaySound(session_end);
                }
            } break;
            case BREAK_WAIT:
            {
                if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = BREAK;
                    PlaySound(session_begin);
                    sound_played = false;
                }
            } break;
            case BREAK:
            {
                framesCounter++;
                if (framesCounter == 60) {
                    framesCounter = 0;
                    break_mins--;
                    conversion(break_mins, &break_time);
                }
                if (break_mins == 0 || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = STUDY_WAIT;
                    break_mins = BREAK_TIMER;
                    conversion(break_mins, &break_time);
                } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    PlaySound(session_begin);
                    currentScreen = STUDY;
                    break_mins = BREAK_TIMER;
                }
            } break;
            default: break;
        }

        BeginDrawing();
            ClearBackground(WHITE);
            switch (currentScreen) {
                case STUDY_WAIT:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "New session!", (Vector2){ 40.0f, 70.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                } break;
                case STUDY:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "Study study_time!", (Vector2){ 40.0f, 70.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                    DrawTextEx(fontTtf, TextFormat("%.2d:%.2d", study_time.mins, study_time.secs), (Vector2){ 40.0f, 125.0f }, (float)fontTtf.baseSize, 2, GOLD);
                    DrawTexture(inspo, SCREEN_W - inspo.width, SCREEN_H - inspo.height, WHITE);
                } break;
                case BREAK_WAIT:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "Breaks are important..", (Vector2){ 40.0f, 70.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                    DrawTextEx(fontTtf, "Please take one now :>", (Vector2){ 40.0f, SCREEN_H - 104.0f }, (float)fontTtf.baseSize, 2, GOLD);
                } break;
                case BREAK:
                {
                    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLACK);
                    DrawTextEx(fontTtf, "Break time!", (Vector2){ 40.0f, 70.0f }, (float)fontTtf.baseSize + 3.0f, 2, YELLOW);
                    DrawTextEx(fontTtf, TextFormat("%.2d:%.2d", break_time.mins, break_time.secs), (Vector2){ 40.0f, 125.0f }, (float)fontTtf.baseSize, 2, GOLD);
                } break;
                default: break;
            }
        EndDrawing();
    }
    UnloadFont(fontTtf);
    UnloadSound(session_begin);
    UnloadSound(session_end);
    UnloadSound(bonk);
    UnloadTexture(inspo);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

static void conversion (int seconds, struct t_time *h){
    h -> hrs  =  seconds / 3600;
    h -> mins = (seconds / 60) % 60;
    h -> secs =  seconds % 60;
}
