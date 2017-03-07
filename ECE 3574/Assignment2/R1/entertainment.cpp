///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	entertainment.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Implementation file for the entertainment class
// Date: Feb. 21, 2016
//

#include "film.h"
#include "entertainment.h"

Entertainment::Entertainment(QString id, QString title, QString dir, quint32 len, QDate relDate, FilmTypes type, MPAARatings rtng) :
    Film(id, title, dir, len, relDate), m_Type(type), m_Rating(rtng) {}

Entertainment::Entertainment(QStringList propList) : Film(propList){}

QString Entertainment::toString(bool labeled, QString sepchar) {
    if (labeled)
        return "Entertainment" + sepchar + QString("%1%2%3%4%5").arg(Film::toString(labeled, sepchar))
                .arg(sepchar).arg(getFilmTypeString()).arg(sepchar).arg(getRatingString());
    else
        return QString("%1%2%3%4%5").arg(Film::toString(labeled, sepchar))
            .arg(sepchar).arg(getFilmTypeString()).arg(sepchar).arg(getRatingString());
}

QString Entertainment::getFilmTypeString() {
    switch (m_Type) {
        case Action: return "Action";
        case Comedy: return "Comedy";
        case SciFi: return "SciFi";
        case Romance: return "Romance";
        case Horror: return "Horror";
        default:
            return "None";
    }
}

QString Entertainment::getRatingString() {
    switch (m_Rating) {
        case G: return "G";
        case PG: return "PG";
        case PG13: return "PG13";
        case R: return "R";
        default:
            return "None";
    }
}
