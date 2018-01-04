///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	filmlist.h
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Header file for the filmlist class.
//
// Date:	Feb. 21, 2016
//


#ifndef FILMLIST
#define FILMLIST
#include "film.h"
#include <QString>
#include <QDate>
#include <QStringList>
class FilmList : public QList<Film*> {
public:
    FilmList();
    ~FilmList();
    virtual QString toString();
    QStringList getID(QString title);
    void addFilm(Film*& film);
    void removeFilm(QString filmID);
    Film* findFilm(QString filmID);
};
#endif // FILMLIST

