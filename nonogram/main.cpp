#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>

using namespace std;

class Cursor {
private:
    int x, y;
    char cursorIcon;
public:
    Cursor();
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    char getCursorIcon();
    void setCursorIcon(char cursorIcon);
};

Cursor::Cursor() {
    x = y = 0;
    cursorIcon = '-';
}

int Cursor::getX() {
    return x;
}

void Cursor::setX(int x) {
    this->x = x;
}

int Cursor::getY() {
    return y;
}

void Cursor::setY(int y) {
    this->y = y;
}

char Cursor::getCursorIcon() {
    return cursorIcon;
}

void Cursor::setCursorIcon(char cursorIcon) {
    this->cursorIcon = cursorIcon;
}

class Nonogram {
private:
    const static int maxDimension = 1000;
    int width, height;
    int percentage;
    bool grid[maxDimension][maxDimension];
    Cursor cursor;
public:
    Nonogram();
    int randomNumber();
    void fillRandomly();
    void print();
    void changePercentage();
    void changeSize();
    void toggle();
    void submenu(char);
    void clean();
    Cursor getCursor();
};

Nonogram::Nonogram() {
    width = 15;
    height = 15;
    percentage = 50;
}

Cursor Nonogram::getCursor() {
    return cursor;
}

void Nonogram::print() {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            if (getCursor().getX() == j && getCursor().getY() == i) {
                cout << " " << getCursor().getCursorIcon();
            } else if (!grid[i][j]) {
                cout << " " << '0';
            } else if (grid[i][j]) {
                cout << " " << 'X';
            }
        }
        cout << endl;
    }
}

void Nonogram::changeSize() {
    cout << "Please enter width:" << endl;
    cin >> width;
    cout << "Please enter height:" << endl;
    cin >> height;
    clean();
}

void Nonogram::toggle() {
    if (grid[cursor.getY()][cursor.getX()]) {
        grid[cursor.getY()][cursor.getX()] = false;
    } else {
        grid[cursor.getY()][cursor.getX()] = true;
    }
}

int Nonogram::randomNumber() {
    static int number = 42;
    number = ((221 * number + 1) % 1000) / 10;
    return number;
}

void Nonogram::changePercentage() {
    cout << "Choose random percentage between 0 and 100." << endl;
    cin >> percentage;
}

void Nonogram::fillRandomly() {
    int number;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            number = randomNumber();
            cout << number << endl;
            if (number < percentage) {
                grid[i][j] = true;
            } else {
                grid[i][j] = false;
            }
        }
    }
}

void Nonogram::clean() {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            grid[i][j] = false;
        }
    }
}

void Nonogram::submenu(char selection) {
    while (selection != 'b') {
        cout << "change (s)ize, change (p)ercentage, (t)oggle node, back to (m)ainmenu" << endl;
        cin >> selection;

        switch (selection) {
            case 's':
                changeSize();
                break;
            case 'p':
                changePercentage();
                break;
            case 't':
                toggle();
                break;
            case 'm':
                return;
        }
    }
}

void mainMenu() {
    char selection;
    Nonogram nono;
    cout << "      _   __" << endl;
    cout << "     / | / /___  ____  ____  ____  _________  ____ ___" << endl;
    cout << "    /  |/ / __ \\/ __ \\/ __ \\/ __ `/ ___/ __ `/ __ `__ \\ " << endl;
    cout << "   / /|  / /_/ / / / / /_/ / /_/ / /  / /_/ / / / / / / " << endl;
    cout << "  /_/ |_/\\____/_/ /_/\\____/\\__, /_/   \\__,_/_/ /_/ /_/ " << endl;
    cout << "                          /____/ " << endl << endl;

    nono.clean();

    while (selection != 'q') {
        nono.print();
        cout << "(c)lean, fill (r)andomly, (m)ore options, (q)uit" << endl;
        cin >> selection;

        switch (selection) {
            case 'm':
                nono.submenu(selection);
                break;
            case 'c':
                nono.clean();
                break;
            case 'r':
                nono.fillRandomly();
                break;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
