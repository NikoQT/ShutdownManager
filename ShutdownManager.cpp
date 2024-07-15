#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include "coutColors.h"
#include "XML_TaskController.h"

using namespace std;

void buildTask(string duration, int unitm, string mode);
void createTask();
void deleteTask();
void cancelSameDayTask();
int inputUnit();
int calcTime(int hourInput, int minInput, int secInput);
string inputRelTime();
string inputSpecificTime();


int main()
{ 
    int input;
    cout << "main mode:\n";
    cout << "[1] create new task\n";
    cout << "[2] cancel existing task\n";
    cin >> input;

    if (input == 2) {
        
        deleteTask();
        return 0;
    }

    createTask();
    return 0;
}

void deleteTask() {
    int input;
    system("cls");
    cout << "What do you want to cancel?\n";
    cout << "[1] same day shutdown\n";
    cout << "[2] other day shutdown\n";
    cin >> input;

    switch (input) {
      case 1: cancelSameDayTask(); break;
      case 2: deleteOtherDayTask(); break;
      default: cancelSameDayTask();
    }
}

void createTask() {
    system("cls");
    int input; 
    cout << "task mode:\n";
    cout << "[1] new shutdown timer\n";
    cout << "[2] new shutdown task at specific time\n";
    cout << "[3] (WOP) new shutdown timer far future\n";
    cout << "[4] new reboot timer\n";
    cout << "[5] new reboot task at specific time\n";
    cout << "[6] (WOP) new reboot timer far future\n";
    cin >> input;

    switch (input) {
        case 1: buildTask(inputRelTime(), inputUnit(), " -s -d p:0:0"); break;
        case 2: buildTask(inputSpecificTime() , 1, " -s -d p:0:0"); break;
        case 3: buildXML("-s"); break;
        case 4: buildTask(inputRelTime(), inputUnit(), " -r -d p:0:0"); break;
        case 5: buildTask(inputSpecificTime(), 1, " -r -d p:0:0"); break;
        case 6: buildXML("-r"); break;
        default: buildTask(inputRelTime(), inputUnit(), " -s -d p:0:0");
    }
    system("cls");
    cout << GREEN << "created task successfully\n" << RESET;
    system("pause");
    
}

int inputUnit() {
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

string inputRelTime() {
    string duration;
    system("cls");
    cout << "duration: ";
    cin >> duration;
    return duration;
}

void buildTask(string duration, int unitm, string mode) {

    duration = " -t " + to_string(stoi(duration) * unitm);
    string command = "shutdown" + mode + duration;
    system(command.c_str());
}

void cancelSameDayTask() {
    string s = "shutdown -a";
    int output = system(s.c_str());
    system("cls");
    switch (output) {
    case 0: cout << GREEN << "canceled task successfully\n" << RESET; break;
    case 1116: cout << RED << "no task found\n" << RESET; break;
    default: cout << RED << "unkown error\n" << RESET;
    }
    system("pause");
}

string inputSpecificTime() {
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
        cout << RED << "Error: given time is in the past. Please restart the programm\n" << RESET;
        system("pause");
        exit(-1);
    }

    return result;
}