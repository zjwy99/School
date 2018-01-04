///////////////////////////////////////////
// ECE 3574, Homework 4, Zachary Yee
// File name:	main.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Client file for program. This main simply starts
//              the program.
// Date: Mar. 15, 2016
//
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QAction>
#include <QDebug>
#include <QApplication>

#include "homework4.h"
int main (int argc, char** argv) {

    QApplication a(argc, argv);
    Homework4* test = new Homework4();
    test->resize(400,500);
    test->show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();

}
