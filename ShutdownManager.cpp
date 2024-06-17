#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <filesystem>
#include "coutColors.h"
using namespace std;

void buildTask(string duration, int unitm, string mode);
void createTask();
void cancelSameDayTask();
void buildXML();
int inputUnit();
int calcTime(int hourInput, int minInput, int secInput);
string inputRelTime();
string inputSpecificTime();
string getUserSID();
string getCurrentUser();

int main()
{ 
    buildXML();
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
        case 1: buildTask(inputRelTime(), inputUnit(), " -s -d p:0:0"); break;
        case 2: buildTask(inputSpecificTime() , 1, " -s -d p:0:0"); break;
        case 3: buildTask(inputRelTime(), inputUnit(), " -r -d p:0:0"); break;
        case 4: buildTask(inputSpecificTime(), 1, " -r -d p:0:0"); break;
        default: buildTask(inputRelTime(), inputUnit(), " -s -d p:0:0");
    }

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

string getUserSID() {

    char buffer[128];
    string result;
    FILE* pipe = _popen("whoami /user", "r");
    if (!pipe) throw runtime_error("popen() failed!");

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    _pclose(pipe);

    size_t pos = result.find("S-");
    if (pos != string::npos) {
        string SID = result.substr(pos);
        SID = SID.substr(0, SID.find('\n'));
        return SID;
    }
    else {
        throw runtime_error("SID not found in whoami output");
    }
}

string getCurrentUser() {

    char buffer[128];
    string result;
    FILE* pipe = _popen("whoami", "r");
    if (!pipe) throw runtime_error("popen() failed!");

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    _pclose(pipe);

    size_t pos = result.find('\n');
    if (pos != string::npos) {
        result.erase(pos);
    }

    return result;
}

void buildXML() {
    string date = "2024-06-17T12:11:54.6558387";
    string author = getCurrentUser();
    string startBoundary = "2024-06-17T13:10:43+02:00";
    string endBoundary = "2025-06-17T14:10:43+02:00";
    string userId = getUserSID();
    string command = "shutdown";
    string arguments = "-s -t 60";

    ofstream file("D:\\Downloads\\task.xml");

    if (!file) {
        cerr << "Error creating file!" << endl;
        system("pause");
    }

    file << R"(<?xml version="1.0" encoding="UTF-8"?>)" << endl;
    file << R"(<Task xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task" version="1.4">)" << endl;
    file << R"(  <RegistrationInfo>)" << endl;
    file << "    <Date>" << date << "</Date>" << endl;
    file << "    <Author>" << author << "</Author>" << endl;
    file << R"(    <URI>\Atest</URI>)" << endl;
    file << R"(  </RegistrationInfo>)" << endl;
    file << R"(  <Triggers>)" << endl;
    file << R"(    <TimeTrigger>)" << endl;
    file << "      <StartBoundary>" << startBoundary << "</StartBoundary>" << endl;
    file << "      <EndBoundary>" << endBoundary << "</EndBoundary>" << endl;
    file << R"(      <Enabled>true</Enabled>)" << endl;
    file << R"(    </TimeTrigger>)" << endl;
    file << R"(  </Triggers>)" << endl;
    file << R"(  <Principals>)" << endl;
    file << R"(    <Principal id="Author">)" << endl;
    file << "      <UserId>" << userId << "</UserId>" << endl;
    file << R"(      <LogonType>S4U</LogonType>)" << endl;
    file << R"(      <RunLevel>LeastPrivilege</RunLevel>)" << endl;
    file << R"(    </Principal>)" << endl;
    file << R"(  </Principals>)" << endl;
    file << R"(  <Settings>)" << endl;
    file << R"(    <MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>)" << endl;
    file << R"(    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>)" << endl;
    file << R"(    <StopIfGoingOnBatteries>true</StopIfGoingOnBatteries>)" << endl;
    file << R"(    <AllowHardTerminate>true</AllowHardTerminate>)" << endl;
    file << R"(    <StartWhenAvailable>true</StartWhenAvailable>)" << endl;
    file << R"(    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>)" << endl;
    file << R"(    <IdleSettings>)" << endl;
    file << R"(      <StopOnIdleEnd>true</StopOnIdleEnd>)" << endl;
    file << R"(      <RestartOnIdle>false</RestartOnIdle>)" << endl;
    file << R"(    </IdleSettings>)" << endl;
    file << R"(    <AllowStartOnDemand>true</AllowStartOnDemand>)" << endl;
    file << R"(    <Enabled>true</Enabled>)" << endl;
    file << R"(    <Hidden>false</Hidden>)" << endl;
    file << R"(    <RunOnlyIfIdle>false</RunOnlyIfIdle>)" << endl;
    file << R"(    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>)" << endl;
    file << R"(    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>)" << endl;
    file << R"(    <WakeToRun>false</WakeToRun>)" << endl;
    file << R"(    <ExecutionTimeLimit>PT72H</ExecutionTimeLimit>)" << endl;
    file << R"(    <Priority>7</Priority>)" << endl;
    file << R"(    <RestartOnFailure>)" << endl;
    file << R"(      <Interval>PT1M</Interval>)" << endl;
    file << R"(      <Count>3</Count>)" << endl;
    file << R"(    </RestartOnFailure>)" << endl;
    file << R"(  </Settings>)" << endl;
    file << R"(  <Actions Context="Author">)" << endl;
    file << R"(    <Exec>)" << endl;
    file << "      <Command>" << command << "</Command>" << endl;
    file << "      <Arguments>" << arguments << "</Arguments>" << endl;
    file << R"(    </Exec>)" << endl;
    file << R"(  </Actions>)" << endl;
    file << R"(</Task>)" << endl;

    file.close();
    cout << "XML file created successfully!" << endl;
}