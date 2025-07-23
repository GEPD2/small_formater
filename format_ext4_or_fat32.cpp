#include <iostream>
#include <cstring>
#include <boost/predef.h>

// Qt headers
#include <QApplication>  // Application running
#include <QPushButton>   // Buttons
#include <QLabel>        // Labels
#include <QTextEdit>     // For scrollable output
#include <QLineEdit>     // Line edits, for scrollable events
#include <QInputDialog>  // Inputs
#include <QMainWindow>   // Main Window for the app
#include <QStatusBar>    // Status bar
#include <QProcess>      // To run shell commands
#include "window.h"      // Made header for window managment
#include "os.h"          // Made header for os running

int main(int argc, char **argv)
{
    // Checking running OS
    Search_os osChecker;
    int osCode = osChecker.check_running_os();
    QApplication app(argc, argv);
    // Creating main window
    QMainWindow window;
    window.setFixedSize(720, 480);  // Set window size
    // Set central widget for QMainWindow
    QWidget *centralWidget = new QWidget();
    window.setCentralWidget(centralWidget);
    // Create status bar
    QStatusBar *statusBar = window.statusBar();
    statusBar->showMessage("Ready");
    // Creating UI elements with centralWidget as parent
    QPushButton *button = new QPushButton("Search Devices", centralWidget);
    button->setGeometry(40, 40, 150, 40);  // Position and size
    QTextEdit *textEdit = new QTextEdit(centralWidget);
    textEdit->setGeometry(40, 100, 640, 300);  // Size and position
    textEdit->setReadOnly(true);  // Prevent user editing
    textEdit->setStyleSheet(
        "QTextEdit { "
        "background: #eee; "
        "padding: 10px; "
        "border: 1px solid #ccc; "
        "font-family: monospace; "
        "font-size: 12px; "
        "}"
    );
    QLabel *inputLabel = new QLabel("Enter partition (e.g., sda1):", centralWidget);
    inputLabel->setGeometry(40, 390, 200, 30);
    QLineEdit *partitionInput = new QLineEdit(centralWidget);
    partitionInput->setGeometry(230, 390, 150, 30);
    QPushButton *ext4Button = new QPushButton("EXT4", centralWidget);
    ext4Button->setGeometry(200, 420, 100, 30);
    QPushButton *fat32Button = new QPushButton("FAT32", centralWidget);
    fat32Button->setGeometry(320, 420, 100, 30);
    if (osCode == 0) // Linux
    {
        // Actions for search button
        QObject::connect(button, &QPushButton::clicked, [&]() {
            // execution of command
            QProcess process;
            process.start("lsblk", QStringList());
            process.waitForFinished();
            QString output = process.readAllStandardOutput();
            textEdit->setPlainText(output);
            statusBar->showMessage("Device list updated.");
        });
        // EXT4 Button
        QObject::connect(ext4Button, &QPushButton::clicked, [&]() {
            QString partition = partitionInput->text().trimmed();
            if (partition.isEmpty()) {
                textEdit->append("Please enter a partition name (e.g., sda1).");
                return;
            }
            bool ok;
            QString password = QInputDialog::getText(
                centralWidget,
                "Sudo Password",
                "Enter your password:",
                QLineEdit::Password,
                "", &ok
            );
            if (!ok || password.isEmpty()) {
                textEdit->append("Formatting cancelled.");
                statusBar->showMessage("Ready");
                return;
            }
            // Path for the device
            QString devicePath = "/dev/" + partition;
            textEdit->append("Formatting " + devicePath + " as EXT4...");
            statusBar->showMessage("Formatting as EXT4...");
            // Command for linux
            QProcess process;
            QString command = QString("echo %1 | sudo -S mkfs.ext4 %2").arg(password, devicePath);
            process.start("bash", QStringList() << "-c" << command);
            process.waitForFinished();
            // Command execution
            QString output = process.readAllStandardOutput();
            QString error = process.readAllStandardError();
            if (!output.isEmpty())
                textEdit->append("Output:\n" + output);
            if (!error.isEmpty())
                textEdit->append("Error:\n" + error);
            statusBar->showMessage("Formatting completed.");
        });
        // FAT32 Button
        QObject::connect(fat32Button, &QPushButton::clicked, [&]() {
            QString partition = partitionInput->text().trimmed();

            if (partition.isEmpty()) {
                textEdit->append("Please enter a partition name (e.g., sdb1).");
                return;
            }
            // Path for the device
            QString devicePath = "/dev/" + partition;
            textEdit->append("Formatting " + devicePath + " as FAT32...");
            statusBar->showMessage("Formatting as FAT32...");
            // execution of command
            QProcess process;
            process.start("sudo", QStringList() << "mkfs.vfat" << "-F" << "32" << devicePath);
            process.waitForFinished();
            QString output = process.readAllStandardOutput();
            QString error = process.readAllStandardError();

            if (!output.isEmpty())
                textEdit->append("Output:\n" + output);
            if (!error.isEmpty())
                textEdit->append("Error:\n" + error);

            statusBar->showMessage("Formatting completed.");
        });
    }
    else if (osCode == 10) // Windows
    {
        // Actions for search button
        QObject::connect(button, &QPushButton::clicked, [&]() {
            // execution of command
            QProcess process;
            process.start("cmd", QStringList() << "/c" << "wmic diskdrive list brief");
            process.waitForFinished();
            QString output = process.readAllStandardOutput();
            textEdit->setPlainText(output);
            statusBar->showMessage("Device list updated.");
        });
        // EXT4 Button
        ext4Button->setEnabled(false);  // Disabling EXT4 formatting because you can't do this specific format using windows nor you can read it
        // FAT32 Button (Windows version)
        QObject::connect(fat32Button, &QPushButton::clicked, [&]() {
            QString partition = partitionInput->text().trimmed().toUpper();
            if (partition.isEmpty()) {
                textEdit->append("Please enter a drive letter (e.g., E).");
                return;
            }
            // Adding the necesary if not given
            if (!partition.endsWith(":"))
                partition += ":";
            QString devicePath = partition;
            textEdit->append("Formatting " + devicePath + " as FAT32...");
            statusBar->showMessage("Formatting as FAT32...");
            // Command to format the device for windows
            QString formatCommand = QString("format %1 /FS:FAT32 /Q /Y").arg(devicePath);
            // execution of command
            QProcess process;
            process.start("cmd", QStringList() << "/c" << formatCommand);
            process.waitForFinished(-1);  // Wait indefinitely until done
            QString output = process.readAllStandardOutput();
            QString error = process.readAllStandardError();
            if (!output.isEmpty())
                textEdit->append("Output:\n" + output);
            if (!error.isEmpty())
                textEdit->append("Error:\n" + error);
            statusBar->showMessage("Formatting completed.");
        });
    }
    else if (osCode == 11) // iPhone/iOS unsupported
    {
        // Actions for search button
        QObject::connect(button, &QPushButton::clicked, [=]() {
            textEdit->setPlainText("Device search is not supported on iPhone/iOS yet.");
            statusBar->showMessage("Unsupported OS");
        });
        // EXT4 Button
        ext4Button->setEnabled(false);  // Disabling EXT4 formatting because you can't do this specific format using Mac os by deault nor you can read it
        QObject::connect(fat32Button, &QPushButton::clicked, [&]() {
            QString partition = partitionInput->text().trimmed();
            if (partition.isEmpty()) {
                textEdit->append("Please enter the disk identifier (e.g., disk2s1).");
                return;
            }
            QString devicePath = "/dev/" + partition;
            // Appending the data in the correct formatting for Mac
            textEdit->append("Formatting " + devicePath + " as FAT32...");
            statusBar->showMessage("Formatting as FAT32...");
            // diskutil command for FAT32 formatting (MS-DOS FAT)
            QStringList args = {"eraseVolume", "MS-DOS FAT32", "FAT32", devicePath};
            // execution of command
            QProcess process;
            process.start("diskutil", args);
            process.waitForFinished(-1);  // wait indefinitely
            QString output = process.readAllStandardOutput();
            QString error = process.readAllStandardError();
            if (!output.isEmpty())
                textEdit->append("Output:\n" + output);
            if (!error.isEmpty())
                textEdit->append("Error:\n" + error);
            statusBar->showMessage("Formatting completed.");
        });
    }
    else if (osCode == 12) // MacOS
    {
        // Actions for search button
        QObject::connect(button, &QPushButton::clicked, [&]() {
            QProcess process;
            process.start("diskutil", QStringList() << "list");
            process.waitForFinished();
            QString output = process.readAllStandardOutput();
            textEdit->setPlainText(output);
            statusBar->showMessage("Device list updated.");
        });

    }
    else if (osCode == 14) // Android
    {
        // Actions for search button
        QObject::connect(button, &QPushButton::clicked, [&]() {
            QProcess process;
            process.start("adb", QStringList() << "shell" << "lsblk");
            process.waitForFinished();
            QString output = process.readAllStandardOutput();
            textEdit->setPlainText(output);
            statusBar->showMessage("Device list updated.");
        });
    }
    else
    {
        QLabel *label = new QLabel("Your OS is not supported for device listing.", centralWidget);
        label->setGeometry(40, 420, 640, 30);
        label->setStyleSheet("color: red; font-weight: bold;");
        label->show();
        statusBar->showMessage("Unsupported OS");
    }
    window.show();
    return app.exec();
}
