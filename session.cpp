#include "session.h"

Session::Session(std::string subject, float durationMinutes)
    : subjectName(subject), elapsedMinutes(0.0f), sessionLength(durationMinutes), completed(false) {}

void Session::update() {
    elapsedMinutes += 1.0f / 60.0f; // Assuming 60 FPS
    if (elapsedMinutes >= sessionLength) {
        completed = true;
    }
}

bool Session::isFinished() const {
    return completed;
}

std::string Session::getSubjectName() const {
    return subjectName;
}

float Session::getElapsedMinutes() const {
    return elapsedMinutes;
}

float Session::getSessionLength() const {
    return sessionLength;
}
