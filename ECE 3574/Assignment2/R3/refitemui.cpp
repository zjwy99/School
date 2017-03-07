#include "libraryui.h"
#include "library.h"
#include <QTextStream>
#include <QDate>


QStringList RefItemUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    const int MAXCOPIES(10);
    const int ISBNLEN(13);
    int copies;
    QString str;
    QStringList retval;
    while(1) {
       cout << "ISBN ("<< ISBNLEN << " digits): " << flush;
       str = cin.readLine();
       if(str.length() == ISBNLEN) {
          retval << str;
          break;
       }
    }
    cout << "Title: " << flush;
    retval << cin.readLine();
    while(1) {
       cout << "Number of copies: " << flush;
       copies = cin.readLine().toInt();
       if(copies > 0 and copies <= MAXCOPIES) {
          str.setNum(copies);
          break;
       }
    }
    retval << str;
    return retval;

}

RefItemUI::~RefItemUI() {}

QStringList BookUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    static const int MINYEAR(1900),
                     MAXYEAR(QDate::currentDate().year());
    int year;
    RefItemUI* tempUI;
    tempUI = new RefItemUI();
    QStringList retval(tempUI->prompt());
    QString str;
    cout << "Author: " << flush;
    retval << cin.readLine();
    cout << "Publisher: " << flush;
    retval << cin.readLine();
    while(1) {
       cout << "Copyright year: " << flush;
       year = cin.readLine().toInt();
       if(year >= MINYEAR and year <= MAXYEAR) {
          str.setNum(year);
          break;
       }
    }
    retval << str;
    return retval;

}

QStringList ReferenceBookUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    int idx(0);
    bool ok;
    QString str;
    RefItemUI* tempUI;
    tempUI = new BookUI();
    QStringList retval(tempUI->prompt());
    QStringList cats(ReferenceBook::getRefCategories());
    while(1) {
       cout << "Enter the index of the correct Reference Category: ";
       for(int i = 0; i < cats.size(); ++i)
          cout << "\n\t(" << i << ") " << cats.at(i);
       cout << "\n\t(-1)None of these\t:::" << flush;
       idx = cin.readLine().toInt(&ok);
       if(ok) {
          retval << str.setNum(idx);
          break;
       }
    }
    return retval;

}

QStringList TextBookUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    RefItemUI* tempUI;
    tempUI = new TextBookUI();
    QStringList retval(tempUI->prompt());
    cout << "Course: " << flush;
    retval << cin.readLine();
    return retval;

}

QStringList DvdUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    bool ok;
    QString str, ans;
    int dnum;
    const int MAXDISKS(10);
    RefItemUI* tempUI;
    tempUI = new DvdUI();
    QStringList retval(tempUI->prompt());
    while(1) {
       cout << "Number of disks (no more than " << MAXDISKS
            << " please): " << flush;
       dnum = cin.readLine().toInt(&ok);
       if(ok and dnum > 0 and dnum <= MAXDISKS)
          break;
    }
    str.setNum(dnum);
    retval << str;
    cout << "Two-sided disks (Y/N)? " << flush;
    ans = cin.readLine().toUpper();
    if(ans.startsWith("Y"))
       str.setNum(1);
    else
       str.setNum(0);
    retval << str;
    return retval;

}

QStringList FilmUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    int min;
    const int MAXTIME(1000);
    bool ok;
    QString str, ans;
    RefItemUI* tempUI;
    tempUI = new FilmUI();
    QStringList retval(tempUI->prompt());
    cout << "Star of film: " << flush;
    retval << cin.readLine();
    cout << "Director of film: " << flush;
    retval << cin.readLine();
    while(1) {
       cout << "Length of film (minutes): " << flush;
       min = cin.readLine().toInt(&ok);
       if(ok and min > 0 and min <= MAXTIME)
          break;
    }
    str.setNum(min);
    retval << str;
    cout << "BlueRay (Y/N)? " << flush;
    ans = cin.readLine().toUpper();
    if(ans.startsWith("Y"))
       str.setNum(1);
    else
       str.setNum(0);
    retval << str;
    return retval;

}

QStringList DataBaseUI::prompt() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    RefItemUI* tempUI;
    tempUI = new DataBaseUI();
    QStringList retval(tempUI->prompt());
    cout << "Database protocol: " << flush;
    retval << cin.readLine();
    return retval;

}
