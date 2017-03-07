///////////////////////////////////////////
// ECE 3574, Homework 1, Zachary Yee
// File name:	Birthdays.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Implementation file for the birthday class
// Date: Feb. 7, 2015
//
#include "Birthdays.h"

Birthdays::Birthdays() {

    QFileInfo check("birthdays.dat");

    //does nothing if file already exists, makes new file if it does not exist
    if (check.exists() && check.isFile()) {

    }
    else {
        QFile file;
        file.setFileName("birthdays.dat");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream stream(&file);
        file.close();
    }



}


void Birthdays::add(QDate date, QString name) {
     QFile file("birthdays.dat"); //opens file
     file.open(QIODevice::Append | QIODevice::Text);
     QTextStream out(&file);

     //adds name and date to file
     out << name << "\t" <<date.toString(Qt::ISODate) << "\n";
     file.close();
}


bool Birthdays::update(QDate date, QString name) {
    QFile file("birthdays.dat"); //opens file


    //finds name and replaces birthday in file
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString temp;
        QTextStream out(&file);
        while(!out.atEnd())
        {
            QString line = out.readLine();
            QString first = line.split("\t").at(0);
            if(first !=  name)
                temp.append(line + "\n");
            else {
                temp.append(name + "\t" + date.toString(Qt::ISODate)+ "\n");

            }
        }
        file.resize(0);
        out << temp;
        file.close(); //closes file
    }

    return true;

  }

bool Birthdays::removeByName(QString name) {
    QFile file("birthdays.dat"); //opens file

    //finds name and removes name and birthday from file
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString temp;
        QTextStream out(&file);
        while(!out.atEnd())
        {
            QString line = out.readLine();
            QString first = line.split("\t").at(0);
            if(!(first == name)) {
                temp.append(line + "\n");
            }
        }
        file.resize(0);
        out << temp;
        file.close();
    }
    return true;


}

bool Birthdays::removeByDate(QString date) {
    QFile file("birthdays.dat");

    //find date and removes any occurances of the name and date
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString temp;
        QTextStream out(&file);
        while(!out.atEnd())
        {
            QString line = out.readLine();
            if(!line.contains(date))
                temp.append(line + "\n");
        }
        file.resize(0);
        out << temp;
        file.close();
    }
    return true;

}


QStringList Birthdays::showNamesContaining(QString name) {
    QFile file("birthdays.dat");
    QStringList list;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if(line.contains(name)) {
                 list.push_back(line);
            }

        }
    }

    file.close();
    return list;
}

bool Birthdays::ifNameExists(QString name) {
    QFile file("birthdays.dat");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QString first = line.split("\t").at(0);
            QTextStream(stdout) << first << endl;
            if (name == first) {
                file.close();
                return true;
            }

        }

    }
    file.close();
    return false;
}

QStringList Birthdays::printNextBirthdays(int days) {
    QFile file("birthdays.dat");
    QDate current = QDate::currentDate();
    QStringList list;
    if (file.open(QIODevice::ReadOnly))
    {

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QString name = line.split("\t").at(0);
            QString date = line.split("\t").at(1);
            QString year = date.split("-").at(0);
            QString month = date.split("-").at(1);
            QString day = date.split("-").at(2);
            QDate temp(year.toInt(), month.toInt(), day.toInt());
            int daysTo = current.daysTo(temp);
            if (daysTo <= days) {
                list.push_back(line);
            }
        }
    }
    file.close();
    return list;

}

QStringList Birthdays::printBirthdaysFrom(QString search, int days) {
    QFile file("birthdays.dat");
    QString name;
    QStringList list;
    QDate searchDate;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd()) {
           QString line = in.readLine();
           name = line.split("\t").at(0);
           if (name == search) {
               QString date = line.split("\t").at(1);
               QString year = date.split("-").at(0);
               QString month = date.split("-").at(1);
               QString day = date.split("-").at(2);
               searchDate.setDate(year.toInt(), month.toInt(), day.toInt());
           }

        }
        file.close();

    }
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QString date = line.split("\t").at(1);
            QString year = date.split("-").at(0);
            QString month = date.split("-").at(1);
            QString day = date.split("-").at(2);
            QDate temp(year.toInt(), month.toInt(), day.toInt());
            int daysTo = searchDate.daysTo(temp);
            if (daysTo <= days) {
                list.push_back(line);
            }
        }
    }

    return list;
}

void Birthdays::updateDates() {
    QFile file("birthdays.dat");
    QDate current = QDate::currentDate();
    QString temp;
    QString list;
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream out(&file);
        while (!out.atEnd()) {

            QString line = out.readLine();
            QString name = line.split("\t").at(0);
            QString date = line.split("\t").at(1);
            QString year = date.split("-").at(0);
            QString month = date.split("-").at(1);
            QString day = date.split("-").at(2);
            QDate tempDate(year.toInt(), month.toInt(), day.toInt());
            int daysTo = current.daysTo(tempDate);
            while(daysTo < 0) {
                tempDate = tempDate.addYears(1);
                daysTo = current.daysTo(tempDate);
            }
            list.push_back(name + "\t" + tempDate.toString(Qt::ISODate) + "\n");
            temp.append(name + "\t" + tempDate.toString(Qt::ISODate) + "\n");

        }

        file.resize(0);
        out << temp;
        file.close();

    }

}
