#ifndef LIBRARYUI
#define LIBRARYUI
#include "library.h"
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QDate>
#include <QDebug>

class LibraryUI {


public:
    enum Choices {READ=1, ADD, FIND, REMOVE, SAVE, LIST, QUIT};
    enum Types {BOOK, REFERENCEBOOK, TEXTBOOK, DVD, FILM, DATADVD};
    const QStringList TYPES = (QStringList() << "BOOK" << "REFERENCEBOOK"
       << "TEXTBOOK" << "DVD" << "FILM" << "DATADVD");
    Library* m_Lib;
    LibraryUI (Library* lib);
    void add(QStringList objdata);
    void read();
    void enterData();
    QString find();
    void remove();
    void save();
    void list();
    Choices nextTask();
    void prepareToQuit(bool saved);

};

class RefItemUI {
public:
    ~RefItemUI();
    QStringList prompt();

};

class BookUI : public RefItemUI {
public:
    QStringList prompt();

};

class DvdUI : public RefItemUI {
public:
    QStringList prompt();

};

class FilmUI : public DvdUI {
public:
    QStringList prompt();

};

class DataBaseUI : public DvdUI {
public:
    QStringList prompt();

};

class ReferenceBookUI : public BookUI {
public:
    QStringList prompt();

};

class TextBookUI : public BookUI {
public:
    QStringList prompt();

};


#endif // LIBRARYUI

