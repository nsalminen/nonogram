#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>

using namespace std;

class Cursor {
private:
    int x, y;
public:
    Cursor();
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    void move(char direction, int width, int height);
};

Cursor::Cursor() {
    x = y = 0;
}

void Cursor::move(char direction, int width, int height) {
    switch (direction) {
        case 'u':
            if (y > 0) {
                y--;
            } else {
                y = height - 1;
            }
            break;
        case 'l':
            if (x > 0) {
                x--;
            } else {
                x = width - 1;
            }
            break;
        case 'r':
            if (x < width - 1) {
                x++;
            } else {
                x = 0;
            }
            break;
        case 'd':
            if (y < height - 1) {
                y++;
            } else {
                y = 0;
            }
            break;
    }
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

class Nonogram {
private:
    const static int maxDimension = 50;
    int width, height;
    int percentage;
    bool grid[maxDimension][maxDimension];
    int horizontalValues[maxDimension][maxDimension];
    int verticalValues[maxDimension][maxDimension];
public:
    Cursor cursor;
    Nonogram();
    int getRandomNumber();
    void fillRandomly();
    void countValues();
    void print();
    void changePercentage();
    void changeSize();
    void toggle();
    void submenu(char);
    void clean();
    void resetValues();
    bool emptyLine(int i);
    void printHorizontalLine();
    int getHeight();
    int getWidth();
};

Nonogram::Nonogram() {
    width = 20;
    height = 10;
    percentage = 50;
}

int Nonogram::getHeight() {
    return height;
}

int Nonogram::getWidth() {
    return width;
}

void Nonogram::countValues() {
    int counter = 0;
    int indexCount = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j]) {
                counter++;
            } else {
                horizontalValues[i][indexCount] = counter;
                indexCount++;
                counter = 0;
            }
        }
        indexCount = 0;
    }
    counter = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[j][i]) {
                counter++;
            } else {
                verticalValues[i][indexCount] = counter;
                indexCount++;
                counter = 0;
            }
        }
        indexCount = 0;
        counter = 0;
    }
}

void Nonogram::print() {
//    verticalValues[0][0] = 1;
//    verticalValues[0][1] = 2;
//    verticalValues[1][0] = 3;
//    verticalValues[1][1] = 4;
//    verticalValues[1][2] = 5;
    printHorizontalLine();
    int i, j;
    for (i = 0; i < height; i++) {
        cout << '+';
        for (j = 0; j < width; j++) {
            if (cursor.getX() == j && cursor.getY() == i) {
                if (!grid[i][j]) {
                    cout << " " << '-';
                } else {
                    cout << " " << '*';
                }
            } else if (!grid[i][j]) {
                cout << " " << 'O';
            } else if (grid[i][j]) {
                cout << " " << 'X';
            }
        }
        cout << " " << '+';
        j = 0;
        while (horizontalValues[i][j] > 0) {
            cout << " " << horizontalValues[i][j];
            j++;
        }
        cout << endl;
    }
    printHorizontalLine();
    for (i = 0; i < height; i++) {
        if (!emptyLine(i)) {
            cout << " ";
            for (j = 0; j < width; j++) {
                if (verticalValues[i][j] != 0) {
                    cout << " " << verticalValues[i][j];
                } else {
                    if (verticalValues[i][j] == 0) {
                        cout << " ";
                    }
                }
            }
            cout << endl;
        }
    }
}

void Nonogram::printHorizontalLine() {
    for (int i = 0; i < width + 2; i++) {
        cout << '+' << ' ';
    }
    cout << endl;
}

bool Nonogram::emptyLine(int i) {
    for (int j = 0; j < width; j++) {
        if (verticalValues[j][i] > 0) {
            return false;
        }
    }
    return true;
}

void Nonogram::changeSize() {
    cout << "Please enter width (< 50):" << endl;
    cin >> width;
    cout << "Please enter height (< 50):" << endl;
    cin >> height;
    clean();
    resetValues();
}

void Nonogram::toggle() {
    if (grid[cursor.getY()][cursor.getX()]) {
        grid[cursor.getY()][cursor.getX()] = false;
    } else {
        grid[cursor.getY()][cursor.getX()] = true;
    }
    countValues();
}

int Nonogram::getRandomNumber() {
    static int number = 42;
    number = ((221 * number + 1) % 1000) / 10;
    return number;
}

void Nonogram::changePercentage() {
    cout << "Choose a percentage between 0 and 100:" << endl;
    cin >> percentage;
}

void Nonogram::fillRandomly() {
    int number;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            number = getRandomNumber();
            if (number < percentage) {
                grid[i][j] = true;
            } else {
                grid[i][j] = false;
            }
        }
    }
}

void Nonogram::resetValues() {
    int i = 0;
    int j = 0;
    for (i = 0; i < maxDimension; i++) {
        for (j = 0; j < maxDimension; j++) {
            horizontalValues[i][j] = 0;
        }
    }
    //int verticalValues[maxDimension][maxDimension / 2];
    //int horizontalValues[maxDimension / 2][maxDimension];
    for (i = 0; i < maxDimension; i++) {
        for (j = 0; j < maxDimension; j++) {
            verticalValues[i][j] = 0;
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
        cout << "[S]change size | [P]change percentage | [M]back to mainmenu" << endl;
        cin >> selection;

        switch (selection) {
            case 's':
                changeSize();
                break;
            case 'p':
                changePercentage();
                break;
            case 'm':
                return;
        }
    }
}

void mainMenu() {
    char selection;
    Nonogram nono;
    nono.clean();
    nono.resetValues();
    while (selection != 'q') {
        cout << "      _   __" << endl;
        cout << "     / | / /___  ____  ____  ____  _________  ____ ___" << endl;
        cout << "    /  |/ / __ \\/ __ \\/ __ \\/ __ `/ ___/ __ `/ __ `__ \\ " << endl;
        cout << "   / /|  / /_/ / / / / /_/ / /_/ / /  / /_/ / / / / / / " << endl;
        cout << "  /_/ |_/\\____/_/ /_/\\____/\\__, /_/   \\__,_/_/ /_/ /_/ " << endl;
        cout << "                          /____/ " << endl << endl;
        nono.print();
        cout << "[W]up | [A]left | [D]right | [S]down | [T]toggle node" << endl;
        cout << "[C]clean | [R]fill randomly | [M]more options | [Q]quit" << endl;
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
            case 't':
                nono.toggle();
                break;
            case 'w':
                nono.cursor.move('u', nono.getWidth(), nono.getHeight());
                break;
            case 'a':
                nono.cursor.move('l', nono.getWidth(), nono.getHeight());
                break;
            case 's':
                nono.cursor.move('d', nono.getWidth(), nono.getHeight());
                break;
            case 'd':
                nono.cursor.move('r', nono.getWidth(), nono.getHeight());
                break;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
