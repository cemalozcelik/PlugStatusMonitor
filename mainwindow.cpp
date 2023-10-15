#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "ui_mainwindow.h"

#include <QStorageInfo>
#include <QMessageBox>
#include <QCoreApplication>
#include <QProcess>
#include <QTimer>
#include <QRegularExpression>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect slots to buttons
    connect(ui->usbCheckButton, SIGNAL(clicked()), this, SLOT(checkUSB()));
    connect(ui->hdmiCheckButton, SIGNAL(clicked()), this, SLOT(checkHDMI()));
    connect(ui->powerCheckButton, SIGNAL(clicked()), this, SLOT(checkPower()));

    // Add other necessary initializations here
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkUSB()
{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("lsblk");
    process.waitForFinished();
    QString result = process.readAllStandardOutput();
    qDebug() << result;
    // Extract information for connected USB storage devices
    QRegularExpression regex("(sda\\d+)\\s+\\d:\\d\\s+\\d\\s+(\\S+)\\s+.*?\\n");
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(result);

    QString usbStorageInfo;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString usbPartition = match.captured(1);
        QString usbSize = match.captured(2);

        usbStorageInfo += "USB Storage Partition: " + usbPartition + "\n";
        usbStorageInfo += "Size: " + usbSize + "\n\n";
    }

    if (!usbStorageInfo.isEmpty()) {
        QMessageBox::information(this, "USB Storage Status", usbStorageInfo);
    } else {
        QMessageBox::information(this, "USB Storage Status", "No USB storage partitions connected.");
    }
#else
    // Implement USB storage check for other platforms here
    QMessageBox::information(this, "USB Storage Status", "USB storage status check not implemented for this platform.");
#endif
}


void MainWindow::checkHDMI()
{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("xrandr");
    process.waitForFinished();
    QString result = process.readAllStandardOutput();

    // Extract information for all connected HDMI devices
    QRegularExpression regex("HDMI-\\d-\\d (\\w+) (\\d+x\\d+\\+\\d+\\+\\d+) \\(normal left inverted right x axis y axis\\) (\\d+mm x \\d+mm)[\\s\\S]*?([\\s\\S]+?)(?=(HDMI-\\d-\\d|$))");
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(result);

    QString hdmiInfo;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString status = match.captured(1);
        QString resolution = match.captured(2);
        QString physicalSize = match.captured(3);
        QString supportedModes = match.captured(4);

        hdmiInfo += "HDMI Device Status: " + status + "\n";
        hdmiInfo += "Resolution: " + resolution + "\n";
        hdmiInfo += "Physical Size: " + physicalSize + "\n";
        hdmiInfo += "Supported Modes:\n" + supportedModes + "\n\n";
    }

    if (!hdmiInfo.isEmpty()) {
        QMessageBox::information(this, "HDMI Status", hdmiInfo);
    } else {
        QMessageBox::information(this, "HDMI Status", "No HDMI devices connected.");
    }
#else
    // Implement HDMI check for other platforms here
    QMessageBox::information(this, "HDMI Status", "HDMI status check not implemented for this platform.");
#endif
}


void MainWindow::checkPower()
{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("acpi");
    process.waitForFinished();
    QString result = process.readAllStandardOutput();

    // Extract battery information for Battery 0 and Battery 1
    QRegularExpression regex("Battery (\\d): (\\w+), (\\d+%)");
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(result);

    QString batteryInfo;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString batteryNumber = match.captured(1);
        QString batteryStatus = match.captured(2);
        QString batteryPercentage = match.captured(3);

        batteryInfo += "Battery " + batteryNumber + ": Status: " + batteryStatus + ", Level: " + batteryPercentage + "\n";
    }

    if (!batteryInfo.isEmpty()) {
        QMessageBox::information(this, "Battery Status", batteryInfo);
    } else {
        QMessageBox::information(this, "Battery Status", "Battery information not available.");
    }
#else
    // Implement battery check for other platforms here
    QMessageBox::information(this, "Battery Status", "Battery status check not implemented for this platform.");
#endif
}



