#pragma once
#include "Subject.h"

class BarChart {
private:
    Subject* subjects;
    int subjectCount;
    int maxBarHeight;
    float currentHeights[10];

public:
    BarChart(Subject* subjectList, int count);
    void draw(int x, int y, int width, int height);
};
