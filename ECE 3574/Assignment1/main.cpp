///////////////////////////////////////////
// ECE 3574, Homework 1, Zachary Yee
// File name:	main.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: client file for birthday program
// Date: Feb. 7, 2015
//
#include <iostream>
#include "Birthdays.h"
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char *argv[])
{


    Birthdays test =  Birthdays();
    QString secondArgument;
    QString name;
    QString date;
    if(argc == 1) {
        QStringList list = test.printNextBirthdays(30);
        QTextStream(stdout) << "Name\t\t" << "Birthday" << endl;
        QTextStream(stdout) << "====\t\t" << "========" << endl;
        for (int i = 0; i < list.size(); i++) {
            QString line = list[i];
            QString name = line.split("\t").at(0);
            QString date = line.split("\t").at(1);
            QTextStream(stdout) << name << "\t\t" << date << endl;

        }
    }

    if (argc >= 2)
        secondArgument = argv[1];
    if(secondArgument == "-d") {
        QString thirdArgument = argv[2];
        QChar check = thirdArgument[0];
        if (check.isDigit()) {
            test.removeByDate(argv[2]);
        }
        else {
            name = thirdArgument;
            test.removeByName(name);
        }
        return 0;
    }
    if (secondArgument == "-a") {
        date = argv[2];
        QString year = date.split("-").at(0);
        QString month = date.split("-").at(1);
        QString day = date.split("-").at(2);
        QDate tempDate(year.toInt(), month.toInt(), day.toInt());
        name = argv[3];
        if(test.ifNameExists(name)) {
            test.update(tempDate, name);
        }
        else
            test.add(tempDate, name);
        return 0;
    }

    if (secondArgument == "-n") {
        QString number = argv[2];
        QStringList list = test.printNextBirthdays(number.toInt());
        QTextStream(stdout) << "Name\t\t" << "Birthday" << endl;
        QTextStream(stdout) << "====\t\t" << "========" << endl;
        for(int i = 0; i < list.size(); i ++) {
            QString line = list[i];
            QString name = line.split("\t").at(0);
            QString date = line.split("\t").at(1);
            QTextStream(stdout) << name << "\t\t" << date << endl;

        }
        return 0;


    }

    if (secondArgument == "-m") {
        name = argv[2];
        QString number = argv[3];
        QStringList list = test.printBirthdaysFrom(name, number.toInt());
        QTextStream(stdout) << "Name\t\t" << "Birthday" << endl;
        QTextStream(stdout) << "====\t\t" << "========" << endl;
        for(int i = 0; i < list.size(); i ++) {
            QString line = list[i];
            QString name = line.split("\t").at(0);
            QString date = line.split("\t").at(1);
            QTextStream(stdout) << name << "\t\t" << date << endl;

        }
        return 0;

    }

    if (secondArgument == "-u") {
        test.updateDates();
        return 0;
    }

    if (secondArgument.length() > 2) {
        QStringList list = test.showNamesContaining(argv[1]);
        list.sort(Qt::CaseSensitive);
        QTextStream(stdout) << "Name\t\t" << "Birthday" << endl;
        QTextStream(stdout) << "====\t\t" << "========" << endl;
        for(int i = 0; i < list.size(); i ++) {
            QString line = list[i];
            QString name = line.split("\t").at(0);
            QString date = line.split("\t").at(1);
            QTextStream(stdout) << name << "\t\t" << date << endl;
        }

        return 0;
    }

}
