/* 
 * File:   main.cpp
 * Author: nelssalminen
 *
 * Created on November 13, 2013, 2:47 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Hello World!" << endl;
    char optie;
    cin.get(optie);
    cout << "optie:" << optie << endl;
    return 0;
}

void nonogram::print() {
    int i = 0, j = 0, counter;
    cout << "  ";
    for (i = 0; i < columns + 2; i++) cout << "+";
    cout << endl;
    for (i = 0; i < rows; i++) {
        cout << " + ";
        for (j = 0; j < columns; j++) {
            if (mat[i][j] == true) {
                if (i == cursur[0] && j == cursur[1]) {
                    cout << "*";
                } else {
                    cout << "X";
                }
            } else {
                if (i == cursur[0] && j == cursur[1]) {
                    cout << "_";
                } else {
                    cout << ".";
                }
            }
        }
        cout << " + ";
        j = 0;
        /*horizontale getalen*/
        do {
            if (h_num[i][j] > 9) {
                cout << char(h_num[i][j] - 10 + 'A');
            } else {
                cout << h_num[i][j];
            }
            j++;
        } while (h_num[i][j] != 0);
        cout << endl;
    }
    cout << "   ";
    for (i = 0; i < columns; i++) cout << "+";
    /* printing the vertical numbers */
    for (i = 0; i < rows; i++) {
        counter = 0;
        for (j = 0; j < columns; j++) {
            if (v_num[i][j] > 0) {
                counter = 1;
            }
        }
        if (counter == 1) {
            cout << endl << "   ";
            for (j = 0; j < columns; j++) {
                if (i == 0 || v_num[i][j] != 0) {
                    if (v_num[i][j] > 9) {
                        cout << " " << char(v_num[i][j] - 10 + 'A');
                    } else {
                        cout << v_num[i][j];
                    }
                } else {
                    if (v_num[i][j] == 0) {
                        cout << " ";
                    }
                }
            }
        }
    }
    cout << endl << "Size  : " << rows << "X" << columns << "(" << rows * columns << ")";
    cout << endl << "Black : " << t_black;
    cout << endl << "Speelen: " << speel;
    cout << endl;
}

