#include "BarChart.h"
#include "raylib.h"
#include <string>
#include <cmath>
using namespace std;

struct ConfettiParticle {
    float x, y;
    float speedY;
    Color color;
    bool active;
};

ConfettiParticle confetti[100];
int lastCelebratedLevel[10] = { 0 };

void InitConfetti(int barX, int barY, int barWidth) {
    for (int i = 0; i < 100; i++) {
        confetti[i].x = barX + GetRandomValue(0, barWidth);
        confetti[i].y = barY;
        confetti[i].speedY = -((float)GetRandomValue(50, 150) / 100.0f);
        confetti[i].color = Color{ (unsigned char)GetRandomValue(100,255), (unsigned char)GetRandomValue(100,255), (unsigned char)GetRandomValue(100,255), 200 };
        confetti[i].active = true;
    }
}

void UpdateAndDrawConfetti() {
    for (int i = 0; i < 100; i++) {
        if (confetti[i].active) {
            confetti[i].y += confetti[i].speedY;
            confetti[i].speedY += 0.2f;
            DrawCircle((int)confetti[i].x, (int)confetti[i].y, 3, confetti[i].color);
            if (confetti[i].y > GetScreenHeight()) {
                confetti[i].active = false;
            }
        }
    }
}

BarChart::BarChart(Subject* subjectList, int count) {
    subjects = subjectList;
    subjectCount = count;
    maxBarHeight = 300;
    for (int i = 0; i < subjectCount; i++) {
        currentHeights[i] = 0;
        lastCelebratedLevel[i] = 0;
    }
}

void BarChart::draw(int x, int y, int width, int height) {
    Color barColors[10] = { BLUE, RED, GREEN, ORANGE, PURPLE, MAROON, VIOLET, DARKGREEN, GOLD, PINK };
    int barWidth = width / subjectCount - 20;
    int spacing = 20;
    int adjustedY = y + 60;

    for (int i = 0; i < subjectCount; i++) {
        float studied = subjects[i].getHoursStudied();
        float goal = subjects[i].getGoal();
        float ratio = studied / goal;
        ratio *= 1.5f;
        if (ratio > 1.0f) ratio = 1.0f;

        int targetHeight = (int)(ratio * maxBarHeight);
        if (studied >= goal) {
            targetHeight += (int)(5.0f * sin(GetTime() * 8));
        }

        // Fast growth animation
        currentHeights[i] += (targetHeight - currentHeights[i]) * 0.4f;

        int barX = x + i * (barWidth + spacing);
        int barY = adjustedY + (maxBarHeight - currentHeights[i]);

        // Glow effect
        float alphaPulse = 100 + 50 * sin(GetTime() * 3);
        Color glowColor = barColors[i % 10];
        glowColor.a = (unsigned char)alphaPulse;
        DrawRectangleLinesEx(Rectangle{ (float)barX - 2, (float)adjustedY - 2, (float)barWidth + 4, (float)maxBarHeight + 4 }, 4, glowColor);

        DrawRectangle(barX, adjustedY, barWidth, maxBarHeight, LIGHTGRAY);
        DrawRectangle(barX, barY, barWidth, (int)currentHeights[i], barColors[i % 10]);

        // Labels
        DrawText(TextFormat("%.0f/%.0f", studied, goal), barX + 5, barY - 20, 16, DARKGRAY);
        DrawText(subjects[i].getName().c_str(), barX, adjustedY + maxBarHeight + 25, 16, BLACK);

        // Flash and star when goal complete
        if (studied >= goal) {
            Color flash = WHITE;
            flash.a = (unsigned char)(80 + 40 * sin(GetTime() * 12));
            DrawRectangle(barX, barY, barWidth, (int)currentHeights[i], flash);

            float pulse = 5.0f * sin(GetTime() * 5.0f) + 25.0f;
            DrawText("*", barX + barWidth / 2 - pulse / 2, adjustedY - 30, pulse, GOLD);
        }

        int currentLevel = (int)(studied / goal);
        if (currentLevel > lastCelebratedLevel[i]) {
            InitConfetti(barX, barY, barWidth);
            lastCelebratedLevel[i] = currentLevel;
        }
    }

    UpdateAndDrawConfetti();
}