///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	entertainment.h
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Header file for the entertainment class.
//
// Date:	Feb. 21, 2016
//


#ifndef ENTERTAINMENT
#define ENTERTAINMENT
#include "film.h"
#include <QString>
#include <QDate>
#include <QStringList>
class Entertainment : public Film {
public:
    enum MPAARatings {G, PG, PG13, R};
    enum FilmTypes {Action, Comedy, SciFi, Romance, Horror};
    FilmTypes m_Type;
    MPAARatings m_Rating;
    Entertainment(QString id, QString title, QString dir, quint32 len, QDate relDate, FilmTypes type, MPAARatings rtng);
    Entertainment(QStringList propList);
    virtual QString toString(bool labeled, QString sepchar);
    QString getFilmTypeString();
    QString getRatingString();
};

#endif // ENTERTAINMENT

