#pragma once
#include <string>
#include "Task.h"

const int MAX_TASKS = 10;

class Subject {
private:
    std::string name;
    float hoursStudied;
    float goal;
    Task tasks[MAX_TASKS];
    int taskCount;

public:
    Subject(std::string n = "");

    void logStudyTime(float hrs);
    float getHoursStudied() const;

    std::string getName() const;
    void setName(std::string subjectName);

    void setGoal(float g);
    float getGoal() const;

    void addTask(const std::string& desc);
    Task* getTasks();
    int getTaskCount() const;
};