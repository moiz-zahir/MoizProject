#include "user.h"
using namespace std;

User::User(string userName) {
    name = userName;
    subjectCount = 0;
}

void User::addSubject(string subjectName) {
    if (subjectCount < MAX_SUBJECTS) {
        subjects[subjectCount] = Subject(subjectName);
        subjectCount++;
    }
}

Subject* User::getSubject(string subjectName) {
    for (int i = 0; i < subjectCount; i++) {
        if (subjects[i].getName() == subjectName) {
            return &subjects[i];
        }
    }
    return nullptr;
}

Subject* User::getAllSubjects() {
    return subjects;
}

int User::getSubjectCount() const {
    return subjectCount;
}

string User::getName() const {
    return name;
}