#pragma once
#include "Subject.h"
using namespace std;

const int MAX_SUBJECTS = 10;

class User {
private:
    string name;
    Subject subjects[MAX_SUBJECTS];
    int subjectCount;

public:
    User(string userName);
    void addSubject(string subjectName);
    Subject* getSubject(string subjectName);
    Subject* getAllSubjects();
    int getSubjectCount() const;
    string getName() const;
};