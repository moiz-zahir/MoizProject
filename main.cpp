#include "raylib.h"
#include "user.h"
#include "BarChart.h"
#include "session.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

Texture2D starTexture;
enum Screen { MENU, SESSION };

void DrawStatCard(int x, int y, int w, int h, const char* label, const char* value, Color color) {
    DrawRectangleRounded({ (float)x, (float)y, (float)w, (float)h }, 0.2f, 10, color);
    DrawText(label, x + 15, y + 10, 18, WHITE);
    DrawText(value, x + 15, y + 40, 24, WHITE);
}

string GetTodayDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%Y%m%d", now);
    return string(buffer);
}

int ReadIntFromFile(const string& filename, int def) {
    ifstream fin(filename);
    int val = def;
    if (fin >> val) return val;
    return def;
}

void WriteIntToFile(const string& filename, int value) {
    ofstream fout(filename);
    fout << value;
}

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "StudyBuddy");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/classroom.jpg");
    starTexture = LoadTexture("assets/star.png");

    Screen currentScreen = MENU;
    int selectedSubjectIndex = 0;
    int selectedTaskIndex = 0;
    Session* session = nullptr;
    bool logged = false;

    float timerOptions[] = {10, 15, 25, 30, 45};
    int selectedTimerIndex = 2; // default: 25 minutes

    int streak = ReadIntFromFile("streakCount.txt", 0);
    string lastDate = "";
    ifstream f("lastStudyDate.txt"); if (f) f >> lastDate;
    string todayDate = GetTodayDate();

    User user("Ahad");
    user.addSubject("Math");
    user.addSubject("Physics");
    user.addSubject("CS");

    user.getSubject("Math")->setGoal(75);
    user.getSubject("Physics")->setGoal(75);
    user.getSubject("CS")->setGoal(75);

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);

        Subject* subjects = user.getAllSubjects();
        int subjectCount = user.getSubjectCount();

        int totalH = 0, totalTasks = 0, completed = 0;
        for (int i = 0; i < subjectCount; i++) {
            totalH += (int)subjects[i].getHoursStudied();
            totalTasks += subjects[i].getTaskCount();
            if (subjects[i].getHoursStudied() >= subjects[i].getGoal()) completed++;
        }

        DrawStatCard(40, 20, 220, 80, "Total Hours", TextFormat("%d", totalH), DARKGREEN);
        DrawStatCard(280, 20, 220, 80, "Active Tasks", TextFormat("%d", totalTasks), DARKBLUE);
        DrawStatCard(520, 20, 220, 80, "Subjects Completed", TextFormat("%d", completed), DARKPURPLE);
        DrawText(TextFormat("Streak: %d Days", streak), 800, 30, 20, ORANGE);
        DrawText("🔥", 765, 30, 28, RED);

        if (currentScreen == MENU) {
            DrawText("Select Subject to Start Session", 220, 110, 22, DARKGRAY);
            for (int i = 0; i < subjectCount; i++) {
                Color col = (i == selectedSubjectIndex) ? DARKBLUE : BLACK;
                DrawText(subjects[i].getName().c_str(), 350, 150 + i * 35, 20, col);
            }

            // Timer selection
            DrawText("Select Timer:", 750, 300, 20, DARKGRAY);
            for (int i = 0; i < 5; i++) {
                Color col = (i == selectedTimerIndex) ? BLUE : BLACK;
                DrawText(TextFormat("%.0f min", timerOptions[i]), 750, 330 + i * 30, 20, col);
            }

            if (IsKeyPressed(KEY_DOWN)) selectedSubjectIndex = (selectedSubjectIndex + 1) % subjectCount;
            if (IsKeyPressed(KEY_UP)) selectedSubjectIndex = (selectedSubjectIndex - 1 + subjectCount) % subjectCount;
            if (IsKeyPressed(KEY_RIGHT)) selectedTimerIndex = (selectedTimerIndex + 1) % 5;
            if (IsKeyPressed(KEY_LEFT)) selectedTimerIndex = (selectedTimerIndex - 1 + 5) % 5;

            if (IsKeyPressed(KEY_ENTER)) {
                session = new Session(subjects[selectedSubjectIndex].getName(), timerOptions[selectedTimerIndex]);
                logged = false;
                currentScreen = SESSION;
            }

            BarChart chart(subjects, subjectCount);
            chart.draw(100, 220, 800, 300);

        } else if (currentScreen == SESSION) {
            session->update();
        
            float sessionLen = session->getSessionLength();
            float elapsed = session->getElapsedMinutes();
        
            DrawText(TextFormat("Studying: %s", session->getSubjectName().c_str()), 270, 110, 25, DARKBLUE);
            DrawText(TextFormat("Time Left: %.2f min", sessionLen - elapsed), 300, 150, 30, DARKBLUE);
        
            float progress = elapsed / sessionLen;
            int barX = 100, barY = 90, barWidth = 800, barHeight = 20;
            DrawRectangle(barX, barY, barWidth, barHeight, GRAY);
            Color progColor = (progress < 0.5f) ? DARKGREEN : (progress < 0.8f ? ORANGE : RED);
            DrawRectangle(barX, barY, (int)(barWidth * progress), barHeight, progColor);
            DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
        

            if (session->isFinished() && !logged) {
                Subject* subj = user.getSubject(session->getSubjectName());
                if (subj) subj->logStudyTime(sessionLen);
                logged = true;

                if (todayDate != lastDate) {
                    int todayInt = stoi(todayDate);
                    int lastInt = lastDate == "" ? 0 : stoi(lastDate);
                    streak = (todayInt - lastInt == 1) ? streak + 1 : 1;
                    WriteIntToFile("streakCount.txt", streak);
                    ofstream fout("lastStudyDate.txt"); fout << todayDate;
                    lastDate = todayDate;
                }
            }

            BarChart chart(subjects, subjectCount);
            chart.draw(100, 220, 800, 300);

            if (session->isFinished()) {
                DrawText("Session Complete! Press [ENTER] to return to menu.", 200, 550, 20, DARKGRAY);
                if (IsKeyPressed(KEY_ENTER)) {
                    delete session;
                    session = nullptr;
                    currentScreen = MENU;
                }
            }
        }

        EndDrawing();
    }

    if (session) delete session;
    UnloadTexture(background);
    UnloadTexture(starTexture);
    CloseWindow();
    return 0;
}
