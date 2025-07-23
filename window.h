#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>

class Window : public QWidget
{
    Q_OBJECT  // Required for signal/slot mechanism

public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void onButtonClicked();  // <-- SLOT: where your button logic goes

private:
    QPushButton *m_button;         // "Search Devices" button
    QTextEdit *m_outputArea;       // Scrollable output area
    QLabel *m_inputLabel;          // Label for the partition input
    QLineEdit *m_partitionInput;   // User input for partition name (e.g., sda1)
    QPushButton *m_ext4Button;     // EXT4 format button
    QPushButton *m_fat32Button;    // FAT32 format button
};

#endif // WINDOW_H