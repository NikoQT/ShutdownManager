#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <codecvt>

using namespace std;

string getUserSID();
string getAppDataFolder();
string getCurrentUser();
string to_stringd(int i);
void inputXML();
void buildXML(string type);

int year, moth, day, hour, minute, second, hour_corrected, minute_corrected;

void inputXML() {
    cout << "year: ";
    cin >> year;
    cout << "moth: ";
    cin >> moth;
    cout << "day: ";
    cin >> day;
    cout << "hour: ";
    cin >> hour;
    cout << "minute: ";
    cin >> minute;
    cout << "second: ";
    cin >> second;


    if (minute >= 5) {
        minute_corrected = minute - 5;
        hour_corrected = hour;
    }
    else {
        hour_corrected = hour - 1;

        minute_corrected = 60 - (5 - minute);
    }
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

    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

string getAppDataFolder() {
    char buffer[128];
    string result;
    FILE* pipe = _popen("echo %appdata%", "r");
    if (!pipe) throw runtime_error("popen() failed!");

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    _pclose(pipe);

    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

string to_stringd(int i) {
    string str = to_string(i);

    if (str.length() == 1) {
        return "0" + str;
    }

    return str;
}

void buildXML(string type) {

    time_t now = time(NULL);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    string date = to_stringd(timeinfo.tm_year + 1900) + "-" + to_stringd(timeinfo.tm_mon + 1) + "-" + to_stringd(timeinfo.tm_mday) + "T" + to_stringd(timeinfo.tm_hour) + ":" + to_stringd(timeinfo.tm_min) + ":" + to_stringd(timeinfo.tm_sec); //schema: "2024-06-17T12:11:54.6558387"
    inputXML();
    string startBoundary = to_stringd(year) + "-" + to_stringd(moth) + "-" + to_stringd(day) + "T" + to_stringd(hour_corrected) + ":" + to_stringd(minute_corrected) + ":" + to_stringd(second); //schema: "2024-06-17T13:10:43+02:00"
    string endBoundary = to_stringd(year) + "-" + to_stringd(moth) + "-" + to_stringd(day) + "T" + to_stringd(hour) + ":" + to_stringd(minute) + ":" + to_stringd(second); //schema: "2025-06-17T14:10:43"
    string userId = getUserSID();
    string command = "shutdown";
    string arguments = type + " -t 300 -d p:0:0";
    string author = getCurrentUser();

    string path = getAppDataFolder() + "\\ShutdownManager.xml";

    wofstream file(path, ios::out | ios::trunc | ios::binary);

    //BOM for UTF-16LE
    file.put(0xFF);
    file.put(0xFE);
    file.imbue(locale(file.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));


    file << L"<?xml version=\"1.0\" encoding=\"UTF-16\"?>" << endl;
    file << L"<Task xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\" version=\"1.4\">" << endl;
    file << L"  <RegistrationInfo>" << endl;
    file << L"    <Date>" << wstring(date.begin(), date.end()) << L"</Date>" << endl;
    file << L"    <Author>" << wstring(author.begin(), author.end()) << L"</Author>" << endl;
    file << L"    <URI>\\Atest</URI>" << endl;
    file << L"  </RegistrationInfo>" << endl;
    file << L"  <Triggers>" << endl;
    file << L"    <TimeTrigger>" << endl;
    file << L"      <StartBoundary>" << wstring(startBoundary.begin(), startBoundary.end()) << L"</StartBoundary>" << endl;
    file << L"      <EndBoundary>" << wstring(endBoundary.begin(), endBoundary.end()) << L"</EndBoundary>" << endl;
    file << L"      <Enabled>true</Enabled>" << endl;
    file << L"    </TimeTrigger>" << endl;
    file << L"  </Triggers>" << endl;
    file << L"  <Principals>" << endl;
    file << L"    <Principal id=\"Author\">" << endl;
    file << L"      <UserId>" << wstring(userId.begin(), userId.end()) << L"</UserId>" << endl;
    file << L"      <LogonType>S4U</LogonType>" << endl;
    file << L"      <RunLevel>LeastPrivilege</RunLevel>" << endl;
    file << L"    </Principal>" << endl;
    file << L"  </Principals>" << endl;
    file << L"  <Settings>" << endl;
    file << L"    <MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>" << endl;
    file << L"    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>" << endl;
    file << L"    <StopIfGoingOnBatteries>true</StopIfGoingOnBatteries>" << endl;
    file << L"    <AllowHardTerminate>true</AllowHardTerminate>" << endl;
    file << L"    <StartWhenAvailable>true</StartWhenAvailable>" << endl;
    file << L"    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>" << endl;
    file << L"    <IdleSettings>" << endl;
    file << L"      <StopOnIdleEnd>true</StopOnIdleEnd>" << endl;
    file << L"      <RestartOnIdle>false</RestartOnIdle>" << endl;
    file << L"    </IdleSettings>" << endl;
    file << L"    <AllowStartOnDemand>true</AllowStartOnDemand>" << endl;
    file << L"    <Enabled>true</Enabled>" << endl;
    file << L"    <Hidden>false</Hidden>" << endl;
    file << L"    <RunOnlyIfIdle>false</RunOnlyIfIdle>" << endl;
    file << L"    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>" << endl;
    file << L"    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>" << endl;
    file << L"    <WakeToRun>false</WakeToRun>" << endl;
    file << L"    <ExecutionTimeLimit>PT72H</ExecutionTimeLimit>" << endl;
    file << L"    <Priority>7</Priority>" << endl;
    file << L"    <RestartOnFailure>" << endl;
    file << L"      <Interval>PT1M</Interval>" << endl;
    file << L"      <Count>3</Count>" << endl;
    file << L"    </RestartOnFailure>" << endl;
    file << L"  </Settings>" << endl;
    file << L"  <Actions Context=\"Author\">" << endl;
    file << L"    <Exec>" << endl;
    file << L"      <Command>" << wstring(command.begin(), command.end()) << L"</Command>" << endl;
    file << L"      <Arguments>" << wstring(arguments.begin(), arguments.end()) << L"</Arguments>" << endl;
    file << L"    </Exec>" << endl;
    file << L"  </Actions>" << endl;
    file << L"</Task>" << endl;

    file.close();
}
