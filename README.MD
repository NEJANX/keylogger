# Cross-Platform Keylogger in C

## Overview

This project is a cross-platform keylogger written in C that works on **Windows**, **Linux**, and **macOS**. The keylogger captures keystrokes on each platform using appropriate system APIs and logs them into a file named `keylog.txt`.

### Platforms Supported:
- **Windows**: Uses `SetWindowsHookEx` to intercept keyboard events.
- **Linux**: Utilizes the `X11` library to capture keystrokes in graphical environments.
- **macOS**: Employs the `CoreGraphics` framework to monitor keyboard events.

This is a minimal implementation aimed at security professionals and ethical hackers for testing and educational purposes.

## Features
- Logs keystrokes to a file (`keylog.txt`) across different operating systems.
- Separate implementations for each platform, ensuring compatibility and leveraging native system APIs.
- Minimal code structure that can be extended for more complex logging (e.g., handling special characters or different keyboard layouts).

## Requirements

### Windows
- A C compiler such as MinGW or Visual Studio.
  
### Linux
- X11 development headers (Install via: `sudo apt-get install libx11-dev` on Debian/Ubuntu-based systems).

### macOS
- Xcode or a working Clang installation, which should be available by default on macOS.

## Compilation Instructions

### Windows
To compile the keylogger on Windows using MinGW:

```bash
gcc -o keylogger keylogger.c -luser32
```

### Linux
Ensure you have the X11 development libraries installed, then compile with:

```bash
gcc -o keylogger keylogger.c -lX11
```

### macOS
To compile the keylogger on macOS using the default Clang compiler:

```bash
gcc -o keylogger keylogger.c -framework ApplicationServices
```

## Running the Keylogger

### Windows
Just run the compiled binary `keylogger.exe`. It will start logging keystrokes to `keylog.txt` in the same directory.

### Linux
Run the program with elevated permissions (if necessary) to capture global keystrokes:

```bash
sudo ./keylogger
```

### macOS
Run the compiled binary:

```bash
./keylogger
```

Make sure to grant permissions if required (macOS may prompt you to allow the application to monitor input).

## File Output
The keylogger will write all captured keystrokes to a file named `keylog.txt` in the same directory as the executable.

## Disclaimer
This keylogger is provided for <b>educational purposes only</b>. Use it only in environments where you have explicit permission. Misuse of this software can result in legal consequences.

### <center> © 2024 - NEJAN </center>
