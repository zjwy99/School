///////////////////////////////////////////
// ECE 3574, Homework 4, Zachary Yee
// File name:	homework4.h
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Header file for homework4.
// Date: Mar. 15, 2016
//
#ifndef HOMEWORK4
#define HOMEWORK4

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <time.h>
#include <QDateTime>
#include <QDate>
#include <QTime>

//Homework4 class
class Homework4 : public QMainWindow {
  Q_OBJECT
public:
  Homework4(); //Default Constuctor
  ~Homework4(); //Destructor

//Slots for the 4 push buttons
private slots:
  void handleAdvice();
  void handleWeather();
  void handleReminder();
  void handleQuit();
private:
  QStringList adviceText; //List of all text in the advice.dat file
  QStringList weatherText; //List of all text in the weather.dat file
  QStringList reminderText; //List of all text in the reminder.dat file
  QTextEdit* text; //Text Edit box
  QPushButton* advice; //Push button for advice
  QPushButton* weather; //Push button for weather
  QPushButton* reminder; //Push button for reminder
  QPushButton* quit; //Push button for quit


  //Private method to handle getting information from files and formats them
  //if necessay and places information into StringLists
  void fileSetUp();

  //If there any problems in the advice.dat, weather.dat, or reminder.dat file
  //A pop up message appears detailing which file had failed.
  void errorPopUp(QString filename);

};


#endif // HOMEWORK4

