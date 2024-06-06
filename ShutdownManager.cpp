#pragma once
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string timeInSeconds;
    cout << "Enter desired time in seconds. Enter \"stop\" to cancel all running shutdown timers: ";
    cin >> timeInSeconds;

    if (timeInSeconds == "stop") {
       string s = "shutdown -a";
       system(s.c_str());
       return 0;
    }

    string s = "shutdown -s -t " + timeInSeconds;
    system(s.c_str());
    return 0;
}

