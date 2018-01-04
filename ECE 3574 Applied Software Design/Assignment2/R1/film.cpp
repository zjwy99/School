///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	film.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Implementation file for the film class
// Date: Feb. 21, 2016
//

#include "film.h"
Film::Film(QString id, QString title, QString dir, quint32 length, QDate relDate) : m_FilmID(id), m_Title(title), m_Director(dir),
    m_FilmLength(length), m_ReleaseDate(relDate) {}

Film::Film(QStringList& propList) : m_FilmID(propList.takeFirst()), m_Title(propList.takeFirst()), m_Director(propList.takeFirst()),
    m_FilmLength(propList.takeFirst().toInt()){}

Film::~Film(){}

QString Film::toString(bool labeled, QString sepchar) {
    if (labeled)
        return "Film" + sepchar + QString("%1%2%3%4%5%6%7%8%9%10%11").arg(m_FilmID).arg(sepchar).arg(m_Title)
                .arg(sepchar).arg(m_Director).arg(sepchar)
                .arg(m_FilmID).arg(sepchar).arg(m_FilmLength).arg(sepchar).arg(m_ReleaseDate.toString());
    else
        return QString("%1%2%3%4%5%6%7%8%9%10%11").arg(m_FilmID).arg(sepchar).arg(m_Title)
            .arg(sepchar).arg(m_Director).arg(sepchar)
            .arg(m_FilmID).arg(sepchar).arg(m_FilmLength).arg(sepchar).arg(m_ReleaseDate.toString());
}

QString Film::getFilmID() {
    return m_FilmID;
}

QString Film::getDirector() {
    return m_Director;
}

QString Film::getTitle() {
    return m_Title;
}

quint32 Film::getFilmLength() {
    return m_FilmLength;
}

QDate Film::getReleaseDate() {
    return m_ReleaseDate;
}
