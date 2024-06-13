#pragma once
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void build(int unitm, string duration, string mode);
void task(string& mode);
void unit(int& unitm);
void time(string& duration);
void cancelSameDayTask();
int calcTime(int hourInput, int minInput, int secInput);

int main()
{ 
    string duration, mode;
    int input, unitm;
    cout << "main mode:\n";
    cout << "[1] create new task\n";
    cout << "[2] cancel existing task\n";
    cin >> input;

    if (input == 2) {
        cancelSameDayTask();
        return 0;
    }

    task(mode);
    unit(unitm);
    time(duration);
    build(unitm, duration, mode);
    return 0;
}


void task(string& mode) {
    system("cls");
    int input; 
    cout << "task mode:\n";
    cout << "[1] new shutdown timer\n";
    cout << "[2] new reboot timer\n";
    cin >> input;

    switch (input) {
        case 1: mode = " -s -d p:0:0"; break;
        case 2: mode = " -r -d p:0:0"; break;
        default: mode = " -s -d p:0:0";
    }
}

void unit(int& unitm) {
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
}

void time(string& duration) {
    system("cls");
    cout << "duration: ";
    cin >> duration;
}

void build(int unitm, string duration, string mode) {

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
