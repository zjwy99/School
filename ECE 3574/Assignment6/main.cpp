///////////////////////////////////////////
// ECE 3574, Homework 6, Zachary Yee
// File name:	main.cpp
// ID: 905796206
// Email: zjwy99@vt.edu
// Description: Multiplies two maticies together. Getting the values of the two matricies ran in parallel.
//              Computing each value of the output matrix was also done in parallel.
// Date: April 25, 2016
//
#include <QVector>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
pthread_mutex_t my_lock;
QVector<string> matrixResult;


struct matrix {
    char  *name;
    int row;
    int col;
    QVector<QVector<int> > decVector;
};

struct multiplyArgs {
    int index1;
    int index2;
};

struct matrix matrixA;
struct matrix matrixB;

void *getMatrix(void *m) {

    long result = 0;
    int row = 0;
    int col = 0;
    struct matrix *data;
    data = (struct matrix *) m;
    std::string line;
    char *filename = data->name;
    std::ifstream file (filename);
    while (std::getline(file, line)) {

        std::istringstream iss(line);
        std::string word;
        row++;
        QVector<int> tempVector;

        while (iss >> word) {

            if (word.find('.') != std::string::npos) {
                std::cerr << "ERROR: Data format disagreement of matricies\n";
                exit(-1);
            }

            int temp = std::stoi(word);
            col++;
            tempVector.append(temp);


        }
        data->decVector.append(tempVector);
        if (data->col != 0 && data->col != col) {
            std::cerr << "Error: Multiple-length rows for single matrix\n";
            exit(-1);
        }
        data->col = col;
        col = 0;

    }
    data->row = row;
    pthread_exit((void *) result);
}

void *multiply(void *m) {
    struct multiplyArgs *args;
    args = (struct multiplyArgs *) m;
    int result = 0;
    int index1 = args->index1 ;

    int index2 = args->index2 ;


    for (int i = 0; i < matrixA.col; i++) {
        result += matrixA.decVector[index1][i] * matrixB.decVector[i][index2];
    }
    index1++;
    index2++;
    char temp[50];
    sprintf(temp, "C%d,%d=%d",index1,index2,result);

    matrixResult.append (temp);



    pthread_exit(NULL);
}


//Gets the two matricies from the files
void initMatrix () {
    pthread_t thread1;
    pthread_t thread2;

    pthread_attr_t attr;
    int rc;
    long status;


    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    rc = pthread_create(&thread1, &attr, getMatrix,(void *)&matrixA);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    rc = pthread_create(&thread2, &attr, getMatrix,(void *)&matrixB);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    pthread_attr_destroy(&attr);

    rc = pthread_join(thread1, (void **)&status);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_join(thread2, (void **)&status);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    if (matrixA.row != matrixB.col) {
        std::cerr << "ERROR: matrix inner dimension mismatch\n";
        exit(-1);
    }



}


int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cerr << "ERROR: Not enough command line arguements" << "\n";
        exit(-1);
    }


    pthread_attr_t attr;
    int rc;
    long status;

    matrixA.name = argv[1];
    matrixB.name = argv[2];

    initMatrix();

    std::ofstream tempOut("temp.txt");

    int numOfMul = matrixA.row * matrixB.col;
    pthread_t mulThreads[numOfMul];

    pthread_mutex_init(&my_lock, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int count = 0;
    QVector<QVector<int> > final(matrixA.row);
    struct multiplyArgs tempArgs[numOfMul];
    for (int x = 0; x < matrixA.row; x++) {

        for (int j = 0; j < matrixB.col; j++) {

            tempArgs[count].index1 = x;
            tempArgs[count].index2 = j;

            rc = pthread_create(&mulThreads[count], &attr, multiply, (void *)&tempArgs[count]);
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);

            }

            count++;


        }
        final[x].resize(matrixB.col);

    }

    pthread_attr_destroy(&attr);

    for (int i = 0; i < numOfMul; i++) {
        rc = pthread_join(mulThreads[i], (void **)&status);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        tempOut << matrixResult[i] << "\n";

    }

    if (pthread_mutex_destroy(&my_lock) == -1)
        exit(-1);

    tempOut.close();
    std::ofstream outFile(argv[3]);



    std::ifstream tempIn("temp.txt");

    //Puts results from temp file into an array
    for(int i = 0; i < numOfMul; i++) {
        string index1, index2, finalResult;
        int num1, num2, numResult;
        getline(tempIn, index1, ',');
        getline(tempIn, index2, '=');
        getline(tempIn, finalResult);
        index1.erase(0,1);
        num1 = std::stoi(index1) - 1;
        num2 = std::stoi(index2) - 1;
        numResult = std::stoi(finalResult);
        final[num1][num2] = numResult;
    }


    for (int i = 0; i < matrixA.row; i++) {
        for (int j = 0; j < matrixB.col; j++) {
            outFile << final[i][j] << " ";
        }
        outFile<< "\n";
    }

    pthread_exit(NULL);

}

