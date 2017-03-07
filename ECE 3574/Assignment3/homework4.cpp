///////////////////////////////////////////
// ECE 3574, Homework 4, Zachary Yee
// File name:	homework4.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: implementation file for homework4.
// Date: Mar. 15, 2016
//
#include "homework4.h"

/*
 * Default Constructor for Homework4 class. Initalizes the window and
 * pushbuttons for the GUI.
 */
Homework4::Homework4() {

    QWidget::setWindowTitle("Advisor");
    advice = new QPushButton("Advice", this);
    weather = new QPushButton("Weather", this);
    reminder = new QPushButton("Reminder", this);
    quit = new QPushButton("Quit", this);
    text = new QTextEdit(this);

    text->setGeometry(QRect(QPoint(0, 0),QSize(400, 300)));
    advice->setGeometry(QRect(QPoint(0, 300),QSize(400, 50)));
    weather->setGeometry(QRect(QPoint(0, 350),QSize(400, 50)));
    reminder->setGeometry(QRect(QPoint(0, 400),QSize(400, 50)));
    quit->setGeometry(QRect(QPoint(0, 450),QSize(400, 50)));

    connect(advice, SIGNAL (released()), this, SLOT (handleAdvice()));
    connect(weather, SIGNAL (released()), this, SLOT (handleWeather()));
    connect(reminder, SIGNAL (released()), this, SLOT (handleReminder()));
    connect(quit, SIGNAL (released()), this, SLOT (handleQuit()));

    fileSetUp();


}

//Gets information from files and formats them if necessary
void Homework4::fileSetUp() {


    //Puts text into string list
    QFile file("advice.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            adviceText.push_back(line);
        }
    }
    file.close();

    //Checks for possible error in file
    QString check = adviceText.back();
    if (check.at(check.length() - 1) == '\\')
        errorPopUp("advice.dat");

    //Puts text into string list
    file.setFileName("weather.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            weatherText.push_back(line);
        }
    }
    file.close();

    //Checks for possible error in file
    check = weatherText.back();
    if (check.at(check.length() - 1) == '\\')
        errorPopUp("weather.dat");

    //Puts text into string list
    file.setFileName("reminder.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            reminderText.push_back(line);
        }
    }
    file.close();

    //Checks for possible error in file
    if (check.at(check.length() - 1) == '\\')
        errorPopUp("reminder.dat");

    //Formats the advice input
    QStringList tempList;
    for (int i = 0; i < adviceText.length(); i++) {
        QString temp = adviceText.at(i);
        while (temp.at(temp.length() - 1) == '\\') {
            temp.remove(temp.length() - 1, 1);
            i++;
            temp.append(adviceText.at(i));

        }
        tempList.push_back(temp);
    }
    adviceText = tempList;

    //Formats the weather input
    tempList.clear();
    for (int i = 0; i < weatherText.length(); i++) {
        QString temp = weatherText.at(i);
        while (temp.at(temp.length() - 1) == '\\') {
            temp.remove(temp.length() - 1, 1);
            i++;
            temp.append(weatherText.at(i));

        }
        tempList.push_back(temp);
    }
    weatherText = tempList;

    //Formats the reminder input
    tempList.clear();
    for (int i = 0; i < reminderText.length(); i++) {
        QString temp = reminderText.at(i);
        while (temp.at(temp.length() - 1) == '\\') {
            temp.remove(temp.length() - 1, 1);
            i++;
            temp.append(reminderText.at(i));

        }
        tempList.push_back(temp);
    }
    reminderText = tempList;
}

//Destructor
Homework4::~Homework4(){

}

//Message box for handling errors
void Homework4::errorPopUp(QString filename) {
    QString q1(("There is an blacklash on the last line in your " +
                  filename + " file."));
    QMessageBox::warning(this, tr("ERROR"), q1, "OK");
    exit(0);

}

//handles the advice pushbutton
void Homework4::handleAdvice() {

    int index = rand() % (adviceText.length() - 1);
    QString temp = adviceText.at(index);
    text->append("Advice: " + temp);


}

//Handles the weather pushbutton
void Homework4::handleWeather() {
    int index = rand() % (weatherText.length() - 1);
    QString temp = weatherText.at(index);
    text->append("Weather: " + temp);

}

//Handles the reminder pushbutton
void Homework4::handleReminder() {

    //Sets up the message box with a check box
    QCheckBox *message = new QCheckBox("Show this message again");
    QMessageBox *box = new QMessageBox();
    QPushButton *okButton = new QPushButton("OK", box);
    box->setWindowTitle("Advisor");
    box->setCheckBox(message);
    box->setIcon(QMessageBox::Information);
    box->addButton(okButton,QMessageBox::AcceptRole);

    //Sets up output to send to message box or text edit box
    QString temp;
    int day = rand() % 31 + 1;
    int month = rand() % 12 + 1;
    int year = rand() % 16 + 2000;
    int hour = rand() % 24;
    int min = rand() % 60;
    QDate date(year, month, day);
    QTime time(hour, min, 0);
    QDateTime both(date, time);

    //Handles various scenarios for the reminder pushbutton
    if (reminderText.length() == 0) {
        text->append("There are no more reminders.");
    }
    else if (reminderText.length() == 1) {
        temp = reminderText.at(0);
        box->setText(temp + " at " + both.toString("hh:mm ap") + ", " +
                     both.toString("dddd MM/d/yyyy"));
        box->exec();
        if (message->isChecked() == false) {
            reminderText.removeAt(0);
        }
    }
    else {
        int index = rand() % (reminderText.length() - 1);
        temp = reminderText.at(index);
        box->setText(temp + " at " + both.toString("hh:mm ap") + ", " +
                     both.toString("dddd MM/d/yyyy"));
        box->exec();
        if (message->isChecked() == false) {
            reminderText.removeAt(index);
        }
    }




}

//Handles the message box and push button for quit
void Homework4::handleQuit() {
    QString q1(tr("Are you sure you want to quit?")), a1("No"), a2("Yes");
    int ans = QMessageBox::question(this, tr("Advisor"), q1, a1, a2);
    if (ans == 1)
        exit(0);
}
