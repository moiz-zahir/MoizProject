#pragma once
#include <string>
using namespace std;

class Task {
private:
    string description;
    bool completed;

public:
    Task(string desc = "") : description(desc), completed(false) {}

    string getDescription() const { return description; }
    bool isCompleted() const { return completed; }
    void toggle() { completed = !completed; }
};
