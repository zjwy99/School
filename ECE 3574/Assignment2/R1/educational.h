///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	educational.h
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Header file for the educational class.
//
// Date:	Feb. 21, 2016
//

#ifndef EDUCATIONAL
#define EDUCATIONAL
#include "film.h"
#include <QString>
#include <QDate>
#include <QStringList>
class Educational : public Film {
public:
    enum Grade {Elementary = 0, Intermediate, HighSchool, College};
    QString m_subject;
    Grade m_GradeLevel;
    Educational (QString id, QString title, QString dir, quint32 len, QDate relDate, QString subject, Grade grade);
    Educational (QStringList propList);
    virtual QString toString(bool labeled, QString sepchar);
    QString getGradeString();
};

#endif // EDUCATIONAL

