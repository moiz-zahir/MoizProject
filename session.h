#pragma once
#include <string>
using namespace std;

class Session {
private:
    std::string subjectName;
    float elapsedMinutes;
    float sessionLength;
    bool completed;

public:
    Session(std::string subject, float durationMinutes);
    void update();
    bool isFinished() const;
    std::string getSubjectName() const;
    float getElapsedMinutes() const;
    float getSessionLength() const;
};
