#include "Subject.h"
#include "Task.h"

Subject::Subject(std::string n) : name(n), hoursStudied(0), goal(100), taskCount(0) {}

float Subject::getHoursStudied() const {
    return hoursStudied;
}

std::string Subject::getName() const {
    return name;
}

void Subject::setName(std::string subjectName) {
    name = subjectName;
}

void Subject::setGoal(float g) {
    goal = g;
}

float Subject::getGoal() const {
    return goal;
}

void Subject::logStudyTime(float hrs) {
    hoursStudied += hrs;
}

void Subject::addTask(const std::string& desc) {
    if (taskCount < MAX_TASKS) {
        tasks[taskCount++] = Task(desc);
    }
}

Task* Subject::getTasks() {
    return tasks;
}

int Subject::getTaskCount() const {
    return taskCount;
}