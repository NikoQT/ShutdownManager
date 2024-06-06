#pragma once
#include <iostream>
#include <string>

using namespace std;

void build(int unitm, string duration, string mode);
void task(string& mode);
void unit(int& unitm);
void time(string& duration);

int main()
{
    string duration, mode;
    int input, unitm;
    cout << "main mode:\n";
    cout << "[1] create new task\n";
    cout << "[2] cancel existing task\n";
    cin >> input;

    if (input == 2) {
       string s = "shutdown -a";
       system(s.c_str());
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
        case 1: mode = " -s"; break;
        case 2: mode = " -r"; break;
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
