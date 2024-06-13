#pragma once
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void build(string duration, int unitm, string mode);
void createTask();
void cancelSameDayTask();
int unit();
int calcTime(int hourInput, int minInput, int secInput);
string time();
string inputTime();

int main()
{ 
    int input;
    cout << "main mode:\n";
    cout << "[1] create new task\n";
    cout << "[2] cancel existing task\n";
    cin >> input;

    if (input == 2) {
        cancelSameDayTask();
        return 0;
    }

    createTask();
    return 0;
}


void createTask() {
    system("cls");
    int input; 
    cout << "task mode:\n";
    cout << "[1] new shutdown timer\n";
    cout << "[2] new shutdown task at specific time\n";
    cout << "[3] new reboot timer\n";
    cout << "[4] new reboot task at specific time\n";
    cin >> input;

    switch (input) {
        case 1: build(time(), unit(), " -s -d p:0:0"); break;
        case 2: build(inputTime() , 1, " -s -d p:0:0"); break;
        case 3: build(time(), unit(), " -r -d p:0:0"); break;
        case 4: build(inputTime(), 1, " -r -d p:0:0"); break;
        default: build(time(), unit(), " -s -d p:0:0");
    }

    cout << "created task successfully\n";
    
}

int unit() {
    int unitm;
    system("cls");
    int input;
    cout << "unit mode:\n";
    cout << "[1] seconds\n";
    cout << "[2] minutes\n";
    cout << "[3] hours\n";
    cin >> input;

    switch (input) {
    case 1: unitm = 1; break;
    case 2: unitm = 60; break;
    case 3: unitm = 3600; break;
    default: unitm = 60;
    }

    return unitm;
}

string time() {
    string duration;
    system("cls");
    cout << "duration: ";
    cin >> duration;
    return duration;
}

void build(string duration, int unitm, string mode) {

    duration = " -t " + to_string(stoi(duration) * unitm);
    string command = "shutdown" + mode + duration;
    system(command.c_str());
}

void cancelSameDayTask() {
    string s = "shutdown -a";
    int output = system(s.c_str());
    system("cls");
    switch (output) {
    case 0: cout << "canceled task successfully\n"; break;
    case 1116: cout << "no task found\n"; break;
    default: cout << "unkown error\n";
    }
    system("pause");
}

string inputTime() {
    int hourInput, minInput, secInput;
    system("cls");
    cout << "hour: ";
    cin >> hourInput;
    system("cls");
    cout << "minute: ";
    cin >> minInput;
    system("cls");
    cout << "second: ";
    cin >> secInput;
    system("cls");

    return to_string(calcTime(hourInput, minInput, secInput));
}

int calcTime(int hourInput, int minInput, int secInput) {

    time_t now = time(NULL);

    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    int result = (3600 * (hourInput - timeinfo.tm_hour)) + (60 * (minInput - timeinfo.tm_min)) + (secInput - timeinfo.tm_sec);

    if (result <= 0) {
        cout << "Error: given time is in the past. Please restart the programm\n";
        system("pause");
        exit(-1);
    }

    return result;
}
