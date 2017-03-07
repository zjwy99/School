///////////////////////////////////////////
// ECE 3574, Homework 2, Zachary Yee
// File name:	filmlist.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Implementation file for the filmlist class
// Date: Feb. 21, 2016
//

#include "filmlist.h"

FilmList::~FilmList() {
    qDeleteAll(*this);
    clear();
}

FilmList::FilmList(){}

QString FilmList::toString() {
    QStringList list;
    for (int i = 0; i < size(); i++) {
        list << at(i)->toString(false, "[::]");
    }
    return list.join("\n");
}

void FilmList::addFilm(Film*& film) {
    append(film);
}

void FilmList::removeFilm(QString filmID) {
    Film* film(findFilm(filmID));
    if (film != NULL) {
        removeAll(film);
        delete film;
    }

}

QStringList FilmList::getID(QString title) {
    QStringList list;
    for (int i = 0; i < size(); i++) {
        if (at(i)->getTitle() == title)
            list << at(i)->getFilmID();
    }
    return list;
}

Film* FilmList::findFilm(QString filmID) {
    for (int i = 0; i < size(); i++) {
        if (at(i)->getFilmID() == filmID)
            return at(i);
    }

    return NULL;
}


