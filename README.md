# SimpleInstaller

**SimpleInstaller** is a cross-platform Java GUI application that automatically checks for and installs the required dependencies (`qmake`, `g++`) to build a Qt-based C++ project: **`format_ext4_or_fat32`**.

✅ Supported Platforms:
- Linux (APT-based)
- Windows (via PowerShell and Winget)
- macOS (via Homebrew and Xcode)
- Android (via Termux, with or without root)

❌ Unsupported:
- iOS (GUI build and execution unsupported)

---

## 📦 Features

- Detects the user's operating system
- Verifies if `qmake` and `g++` are installed
- Automatically installs missing dependencies
- Builds the Qt project: `format_ext4_or_fat32.pro`
- Displays installation progress via a status bar
- Uses `make` (or `mingw32-make`) to compile the app
- Java Swing interface with a single-button installer

---

## 🛠 Included Files

- `SimpleInstaller.java` - Java Swing installer logic (this project)
- `format_ext4_or_fat32.cpp` - Qt-based C++ app that handles partition formatting
- `os.h` - Platform-specific detection logic for the Qt app

---

## 🚀 How to Run

### ✅ Compile and Launch the Installer

```bash
javac SimpleInstaller.java
java SimpleInstaller

