#include "windows.h"
#include <psapi.h>
#include "string"
#include "iostream"

using namespace std;

void print_memory_info(DWORD &processID) {
  PROCESS_MEMORY_COUNTERS pmc;

  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                FALSE,
                                processID);
  if (hProcess == NULL) {
    return;
  }

  string process_name;

  TCHAR Buffer[MAX_PATH];

  if (GetModuleFileNameEx(hProcess, 0, Buffer, MAX_PATH)) {
    process_name = Buffer;
  }

  if (GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc))) {
    cout << "Process: " << process_name << " Memory: " << (pmc.WorkingSetSize / 1024.0) << " KB" << endl;
  }

  CloseHandle(hProcess);
}

int main(int argc, char const *argv[]) {
  // DWORD aProcesses[1024], cbNeeded;
  //
  // if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
  //     return 1;
  // }
  //
  // for (auto &processID: aProcesses) {
  //   print_memory_info(processID);
  // }

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

  print_memory_info(pi.dwProcessId);

  WaitForSingleObject(pi.hProcess, INFINITE);

  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );

  delete[] c_command;

  return 0;
}
