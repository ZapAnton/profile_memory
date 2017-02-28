#include "windows.h"
#include <psapi.h>
#include "string"
#include "iostream"

using namespace std;

int main(int argc, char const *argv[]) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  string command = "python test.py";

  char* c_command = new char[command.length() + 1];

  strcpy(c_command, command.c_str());

  if (!CreateProcess(
    NULL,
    c_command,
    NULL,
    NULL,
    FALSE,
    0,
    NULL,
    NULL,
    &si,
    &pi
  )) {
    cout << "Process start failed!" << endl;
    return -1;
  }

  DWORD exit_code;
  while (true) {
    if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
      if (exit_code == STILL_ACTIVE) {
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc))) {
          cout << " Memory: " << (pmc.WorkingSetSize / 1024.0) << " KB" << endl;
        }

        Sleep(500);
      } else {
        cout << "Process has ended\n";
        break;
      }
    } else {
      cout << "Error getting exit code\n";
    }
  }

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  delete[] c_command;

  return 0;
}
