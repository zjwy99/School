///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	educational.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Implementation file for the educational class
// Date: Feb. 21, 2016
//

#include "film.h"
#include "educational.h"

Educational::Educational(QString id, QString title, QString dir, quint32 len, QDate relDate, QString subject, Grade grade) :
    Film(id, title, dir, len, relDate), m_subject(subject), m_GradeLevel(grade) {}

Educational::Educational(QStringList propList) : Film(propList) {}

QString Educational::toString(bool labeled, QString sepchar) {
    if (labeled)
        return "Educational" + sepchar + QString("%1%2%3%4%5").arg(Film::toString(labeled,sepchar))
                .arg(sepchar).arg(m_subject).arg(sepchar).arg(getGradeString());
    else
        return QString("%1%2%3%4%5").arg(Film::toString(labeled,sepchar))
            .arg(sepchar).arg(m_subject).arg(sepchar).arg(getGradeString());
}

QString Educational::getGradeString() {
    switch (m_GradeLevel) {
        case Elementary: return "Elementary";
        case Intermediate: return "Intermediate";
        case HighSchool: return "HighSchool";
        case College: return "College";
        default:
            return "None";
    }
}
