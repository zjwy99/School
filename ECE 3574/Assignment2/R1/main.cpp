///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	main.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: client file for film program
// Date: Feb. 21, 2016
//

#include <stdio.h>
#include <typeinfo>
#include <QString>
#include <QTextStream>
#include "film.h"
#include "filmlist.h"
#include "educational.h"
#include "entertainment.h"

QTextStream cout(stdout);
QTextStream cin(stdin);
enum Choices {ADD = 1, DELETE, FINDID, FINDFILM, EXIT};
FilmList list;
Choices task() {
    int choice;
    QString response;
    cout << ADD << ". Add film to list.\n"
         << DELETE << ". Deletes a film with the given ID.\n"
         << FINDID << ". Prints all IDs with given film title.\n"
         << FINDFILM << ". Prints film information given an ID.\n"
         << EXIT << ". Exits the program.\n"
            "Your Choice: " << flush;
    response = cin.readLine();
    choice = response.toInt();
    return static_cast<Choices>(choice);
}

void addEducational(QString ID, QString title, QString director, quint32 length, QDate tempDate) {
    Film* film;
    QString subject;
    cout << "What is the subject for the film:\n" << flush;
    subject = cin.readLine();
    bool goodInput = false;
    int choice;
    QString response;
    Educational::Grade tempGrade;
    while (goodInput == false) {
        cout << "Select Grade Level for film\n" <<
                "1. Elementary\n" <<
                "2. Intermediate\n" <<
                "3. HighSchool\n" <<
                "4. College\n" <<
                "Your Choice: " << flush;
        response = cin.readLine();
        choice = response.toInt();

        switch (choice) {
            case 1:
                tempGrade = Educational::Elementary;

                goodInput = true;
                break;
            case 2:
                tempGrade = Educational::Intermediate;

                goodInput = true;
                break;
            case 3:
                tempGrade = Educational::HighSchool;

                goodInput = true;
                break;
            case 4:
                tempGrade = Educational::College;

                goodInput = true;
                break;
            default:
                cout << "Input Error try again" << endl;
                break;
        }



    }
    film = new Educational(ID, title, director, length, tempDate, subject, tempGrade);
    bool check = true;
    for (int i = 0; i < list.size(); i++) {
        QString subString1 = list[i]->toString(true,"").mid(0,4);
        QString subString2 = film->toString(true,"").mid(0,4);
        if( subString1 == subString2) {
            if (film->getTitle() == list[i]->getTitle() && film->getFilmLength() == list[i]->getFilmLength()
                   && film->getReleaseDate().toString() == list[i]->getReleaseDate().toString()) {


                check = false;
                break;
            }
        }

    }

    if (check) {
        list.addFilm(film);
        cout << "Film sucessfully added" << endl;
        cout << film->toString(false, "[::]") << endl;
    }
    else {
        cout << "Similar film cannot add" << endl;
    }

}

void addRegularFilm(QString ID, QString title, QString director, quint32 length, QDate tempDate) {
    Film* film;
    film = new Film(ID, title, director, length, tempDate);
    bool check = true;
    for (int i = 0; i < list.size(); i++) {
        QString subString1 = list[i]->toString(true,"").mid(0,4);
        QString subString2 = film->toString(true,"").mid(0,4);
        if( subString1 == subString2) {
            if (film->getTitle() == list[i]->getTitle() && film->getFilmLength() == list[i]->getFilmLength()
                   && film->getReleaseDate().toString() == list[i]->getReleaseDate().toString()) {


                check = false;
                break;
            }
        }

    }

    if (check) {
        list.addFilm(film);
        cout << "Film sucessfully added" << endl;
        cout << film->toString(false, "[::]") << endl;
    }
    else {
        cout << "Similar film cannot add" << endl;
    }
}

void addEntertainment(QString ID, QString title, QString director, quint32 length, QDate tempDate) {
    Film* film;
    Entertainment::FilmTypes type;
    Entertainment::MPAARatings rating;
    int choice;
    QString response;
    bool goodInput = false;
    while (goodInput == false) {
        cout << "Select film type:\n" <<
                "1. Action\n" <<
                "2. Comedy\n" <<
                "3. SciFi\n" <<
                "4. Romance\n" <<
                "5. Horror\n" <<
                "Your Choice: " << flush;
        response = cin.readLine();
        choice = response.toInt();
        switch (choice) {
            case 1:
                type = Entertainment::Action;
                goodInput = true;
                break;
            case 2:
                type = Entertainment::Comedy;
                goodInput = true;
                break;
            case 3:
                type = Entertainment::SciFi;
                goodInput = true;
                break;
            case 4:
                type = Entertainment::Romance;
                goodInput = true;
                break;
            case 5:
                type = Entertainment::Horror;
                goodInput = true;
                break;
            default:
                cout << "Input Error try again" << endl;
                break;
        }

    }

    goodInput = false;

    while (goodInput == false) {
        cout << "Select film type:\n" <<
                "1. G\n" <<
                "2. PG\n" <<
                "3. PG13\n" <<
                "4. R\n" <<
                "Your Choice: " << flush;
        response = cin.readLine();
        choice = response.toInt();
        switch (choice) {
            case 1:
                rating = Entertainment::G;
                goodInput = true;
                break;
            case 2:
                rating = Entertainment::PG;
                goodInput = true;
                break;
            case 3:
                rating = Entertainment::PG13;
                goodInput = true;
                break;
            case 4:
                rating = Entertainment::R;
                goodInput = true;
                break;
            default:
                cout << "Input Error try again" << endl;
                break;
        }

    }

    bool check = true;
    film = new Entertainment(ID, title, director, length, tempDate, type, rating);
    for (int i = 0; i < list.size(); i++) {
        QString subString1 = list[i]->toString(true,"").mid(0,13);
        QString subString2 = film->toString(true,"").mid(0,13);
        if( subString1 == subString2) {
            if (film->getTitle() == list[i]->getTitle() && film->getFilmLength() == list[i]->getFilmLength()
                   && film->getReleaseDate().toString() == list[i]->getReleaseDate().toString()) {


                check = false;
                break;
            }
        }

    }
    if (check) {
        list.addFilm(film);
        cout << "Film sucessfully added" << endl;
        cout << film->toString(false, "[::]") << endl;
    }
    else {
        cout << "Similar film cannot add" << endl;
    }
}

void addFilm() {
    QString temp;
    QString ID;
    QString title;
    QString director;
    int num;
    bool check;
    QDate tempDate;
    quint32 length = 0;
    quint32 month = 0;
    quint32 day = 0;
    quint32 year = 0;
    Film* exists;
    do {
        cout << "Enter ID for film:\n" << flush;
        ID = cin.readLine();
        exists = list.findFilm(ID);
        if (exists != NULL)
            cout << "ID Exists in list, please enter new ID" << endl;
    } while (exists != NULL);

    cout << "Enter Title for film:\n" << flush;
    title = cin.readLine();

    cout << "Enter Director name for film:\n" << flush;
    director = cin.readLine();

    do {
        cout << "Enter length for film (please enter value as an integer):\n" << flush;
        temp = cin.readLine();
        length = temp.toInt(&check);
        if (check == false)
            cout << "Input error please enter an integer" << endl;
    } while (check == false);


    do {



        do {
            cout << "Enter year film was released (please enter value as an integer):\n" << flush;
            temp = cin.readLine();
            year = temp.toInt(&check);
            if (check == false)
                cout << "Input error please enter an integer" << endl;
        } while (check == false);

        do {
            cout << "Enter month film was released (please enter value as an integer):\n" << flush;
            temp = cin.readLine();
            month = temp.toInt(&check);
            if (check == false)
                cout << "Input error please enter an integer" << endl;
        } while (check == false);

        do {
            cout << "Enter day film was released (please enter value as an integer):\n" << flush;
            temp = cin.readLine();
            day = temp.toInt(&check);
            if (check == false)
                cout << "Input error please enter an integer" << endl;

        } while (check == false);

        tempDate.setDate(year, month, day);
        check = tempDate.isValid();
        if (check == false)
            cout << "Date is invalid, please enter again" << endl;
    } while (check == false);

    cout << "What type of film is it?\n 1. Educational \n 2. Entertainment \n 3. Regular Film\n Your Choice:\n" << flush;
    temp = cin.readLine();
    num = temp.toInt(&check);


    switch (num) {

        case 1:
            addEducational(ID, title, director, length, tempDate);
            break;
        case 2:
            addEntertainment(ID, title, director, length, tempDate);
            break;
        case 3:
            addRegularFilm(ID, title, director, length, tempDate);
            break;
        default:
            cout << "Input Error try again." << endl;
            break;

    }

}

void removeFilm() {

   QString ID;
   cout << "Type ID for film to remove:\n" << flush;
   ID = cin.readLine();
   Film* film = list.findFilm(ID);
   if (film == NULL)
       cout << "ID not found" << endl;
   else {
       list.removeFilm(ID);
       cout << "Film successfully removed" << endl;
   }




}

void getID() {
    QString title;
    cout << "Type Name of film:\n" << flush;
    title = cin.readLine();

    QStringList tempList = list.getID(title);
    if (tempList.size() == 0)
        cout << "No films found" << endl;

    else {

        cout << "IDs of films with title of " << title << ":\n" << flush;
        for (int i = 0; i < tempList.size(); i++) {
            cout << tempList[i] << endl;
        }
    }


}

void getFilm() {
    QString ID;
    cout << "Type ID for film:\n" << flush;
    ID = cin.readLine();

    Film* tempFilm = list.findFilm(ID);
    if (tempFilm == NULL)
        cout << "No film with ID was found" << endl;
    else {

        cout << tempFilm->toString(true, "\n") << endl;
    }

}

int main( int argc, const char* argv[] )
{

   Film* test;
   QDate temp();

   int myChoice = 0;

    while (1) {
           switch(task()) {
               case 1:
                   addFilm();
                   break;
               case 2:
                   removeFilm();
                   break;
               case 3:
                   getID();
                   break;
               case 4:
                   getFilm();
                   break;
               case 5:
                   exit(0);
                   break;
               default:
                   cout << "Input error try again." << endl;
                   break;
           }

    }
}
