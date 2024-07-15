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
    cout << "[1] same day task\n";
    cout << "[2] other (future) day task\n";
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
    cout << "shutdowns:\n";
    cout << "   [1] same day shutdown - countdown\n";
    cout << "   [2] same day shutdown - specific time\n";
    cout << "   [3] other day shutdown - specific time\n\n";
    cout << "reboots:\n";
    cout << "   [4] same day reboot - countdown\n";
    cout << "   [5] same day reboot - specific time\n";
    cout << "   [6] other day reboot - specific time\n";
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
    system("cls");
    cout << GREEN << "created task successfully\n" << RESET;
}

void cancelSameDayTask() {
    string s = "shutdown -a";
    int output = system(s.c_str());
    system("cls");
    switch (output) {
    case 0: cout << GREEN << "canceled task successfully\n" << RESET; break;
    case 1116: cout << RED << "Error: " << RESET << "no task found\n"; break;
    default: cout << RED << "Error: " << RESET << "unkown error\n";
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