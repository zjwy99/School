///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	film.h
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Header file for the film class.
//
// Date:	Feb. 21, 2016
//


#ifndef FILM
#define FILM
#include <QString>
#include <QDate>
#include <QStringList>
#include <string>

class Film {
public:
    virtual ~Film();
    QString m_FilmID;
    QString m_Title;
    QString m_Director;
    quint32 m_FilmLength;
    QDate m_ReleaseDate;
    virtual QString toString(bool labeled, QString sepchar);
    QString getFilmID();
    QString getTitle();
    QString getDirector();
    quint32 getFilmLength();
    QDate getReleaseDate();
    Film (QString id, QString title, QString dir, quint32 length, QDate relDate);
    Film (QStringList& propList);
};


#endif // FILM

