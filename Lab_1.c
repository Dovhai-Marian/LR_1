#include <windows.h>
#include <iostream>

void ManageProcess(const char* appName, int timeoutMs) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcess(appName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Error: " << GetLastError() << std::endl;
        return;
    }

    if (WaitForSingleObject(pi.hProcess, timeoutMs) == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1);
        std::cout << appName << " terminated due to timeout.\n";
    } else {
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        std::cout << appName << " exited with code: " << exitCode << "\n";
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const char* apps[] = { "notepad.exe", "calc.exe" };
    for (const char* app : apps) ManageProcess(app, 10000);
    return 0;
}
