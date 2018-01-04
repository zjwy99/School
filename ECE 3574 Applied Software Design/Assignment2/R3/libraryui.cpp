#include "libraryui.h"



LibraryUI::LibraryUI(Library *lib) {
    m_Lib = lib;
}

void LibraryUI::prepareToQuit(bool saved) {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    QString ans;
    if(not saved) {
       cout << "Save data first (Y/N)? " << flush;
       ans = cin.readLine().toUpper();
       if(ans.startsWith("Y")) {
          save();
          exit(0);
       }
    }
    exit(1);

}

void LibraryUI::save() {
    QFile outf("libfile");
    outf.open(QIODevice::WriteOnly);
    QTextStream outstr(&outf);
    outstr << m_Lib->toString();
    outf.close();
}

void LibraryUI::add(QStringList objdata) {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    cout << objdata.join("[::]") << endl;
    QString type = objdata.first();
    RefItem* ref;
    switch(static_cast<Types>(TYPES.indexOf(type))) {
    case BOOK:
       ref = new Book(objdata);
       m_Lib->addRefItem(ref);
       break;
    case REFERENCEBOOK:
       ref = new ReferenceBook(objdata);
       m_Lib->addRefItem(ref);
          break;
 //end
    case TEXTBOOK:
       ref = new TextBook(objdata);
       m_Lib->addRefItem(ref);
       break;
    case DVD:
       ref = new Dvd(objdata);
       m_Lib->addRefItem(ref);
       break;
    case FILM:
       ref = new Film(objdata);
       m_Lib->addRefItem(ref);
          break;
    case DATADVD:
       ref = new DataDvd(objdata);
       m_Lib->addRefItem(ref);
       break;
 //start id=lclmenu
    default: qDebug() << "Bad type in add() function";
    }

}

void LibraryUI::read() {
    const QString sep("[::]");
    const int BADLIMIT(5); //max number of bad lines
    QString line, type;
    QStringList objdata;
    QFile inf("libfile");
    inf.open(QIODevice::ReadOnly);
    QTextStream instr(&inf);
    int badlines(0);
    while(not instr.atEnd()) {
       if(badlines >= BADLIMIT) {
          qDebug() << "Too many bad lines! Aborting.";
          return;
       }
       line = instr.readLine();
       objdata = line.split(sep);
       if(objdata.isEmpty()) {
          qDebug() << "Empty Line in file!";
          ++badlines;
       }
       else if(not TYPES.contains(objdata.first())) {
          qDebug() << "Bad type in line: " << objdata.join(";;;");
          ++badlines;
       }
       else
          add(objdata);
     }

}

void LibraryUI::enterData() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    QString typestr;
    while(1) {
       cout << "Library item type: " << flush;
       typestr = cin.readLine();
       if(not TYPES.contains(typestr)) {
          cout << "Please enter one of the following types:\n"
               << TYPES.join(" ,") << endl;
          continue;
       }
       break;
    }
    QStringList objdata;
    RefItemUI* tempUI;
    switch (TYPES.indexOf(typestr)) {
    case BOOK:
          tempUI = new BookUI;
          objdata = tempUI->prompt();
          break;
    case REFERENCEBOOK:
          tempUI = new ReferenceBookUI;
          objdata = tempUI->prompt();
          break;
 //end
    case TEXTBOOK:
          tempUI = new TextBookUI;
          objdata = tempUI->prompt();
          break;
    case DVD:
          tempUI = new DvdUI;
          objdata = tempUI->prompt();
          break;
    case FILM:
          tempUI = new FilmUI;
          objdata = tempUI->prompt();
          break;
    case DATADVD:
         tempUI = new DataBaseUI;
          objdata = tempUI->prompt();
          break;
 //start id=lclprompt
    default:
          qDebug() << "Bad type in enterData()";
    }
    objdata.prepend(typestr);
    add(objdata);

}

QString LibraryUI::find() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    QString isbn, item;
    cout << "Search ISBN: " << flush;
    isbn = cin.readLine();
    item = m_Lib->getItemString(isbn);
    if(item == QString()) {
       cout << isbn << " not found." << endl;
       return item;
    }
    else {
       cout << item << endl;
       return isbn;
    }

}

void LibraryUI::remove() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    QString isbn(find()), ans;
    if(isbn == QString()) {
       cout << isbn << " not found." << endl;
       return;
    }
    cout << "Remove this item (Y/N)? " << flush;
    ans = cin.readLine().toUpper();
    if(ans.startsWith("N"))
       return;
    int numCopiesLeft = m_Lib->removeRefItem(isbn);
    cout << "There are now  " << numCopiesLeft
         << " copies left in the library." << endl;

}

void LibraryUI::list() {
    QTextStream cout(stdout);
    cout << m_Lib->toString() << endl;
}


LibraryUI::Choices LibraryUI::nextTask() {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    int choice;
    QString response;
    do {
       cout << READ << ". Read data from a file.\n"
            << ADD << ". Add items to the Library.\n"
            << FIND << ". Find and display an item.\n"
            << REMOVE << ". Remove an item from the Library.\n"
            << SAVE << ". Save the Library list to a file.\n"
            << LIST << ". Brief listing of Library items.\n"
            << QUIT << ". Exit from this program.\n"
            << "Your choice: " << flush;
      response = cin.readLine();
      choice = response.toInt();
    } while(choice < READ or choice > QUIT);
    return static_cast<Choices>(choice);

}
