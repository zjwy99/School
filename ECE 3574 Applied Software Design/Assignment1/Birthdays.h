///////////////////////////////////////////
// ECE 3574, Homework 1, Zachary Yee
// File name:	Birthdays.h
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Header file for the birthday class.
//
// Date:	Feb. 7, 2015
//

#ifndef BIRTHDAYS_H
#define BIRTHDAYS_H
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <iostream>
#include <QDate>
#include <QDebug>
#include <QStringList>

class Birthdays {
public:
    Birthdays();
    bool update(QDate date, QString name); //updates existing name with new birthday
    void add(QDate date, QString name); //adds a birthday to the file
    bool removeByName(QString name); //removes name and birthday from birthday.dat by name
    bool removeByDate(QString date); //removes name and birthday from birthday.dat by date
    QStringList showNamesContaining(QString name); //returns list names and birthdays of the given substring
    QStringList printNextBirthdays(int days); //return list of the next birthdays by the given dates
    QStringList printBirthdaysFrom(QString name, int days);//return list of birthdays from the given person birthday
    void updateDates(); //updates birthdays so that they are current
    bool ifNameExists(QString name); //checks a person's name is in the birthday.dat file
private:
    QString fileName;


};

#endif // COMMANDS_H
