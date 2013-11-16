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
    bool autoCount;
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
    void readFile();
    void writeFile();
    bool checkLineValue(int i, char orientation);
};

Nonogram::Nonogram() {
    width = 20;
    height = 10;
    percentage = 50;
    autoCount = true;
}

int Nonogram::getHeight() {
    return height;
}

int Nonogram::getWidth() {
    return width;
}

bool Nonogram::checkLineValue(int i, char orientation) {
    int counter = 0;
    int indexCount = 0;
    int tempValues[maxDimension] = {0};
    switch (orientation) {
        case 'h':
            for (int j = 0; j < width; j++) {
                if (grid[i][j]) {
                    counter++;
                } else {
                    if (counter > 0) {
                        tempValues[indexCount] = counter;
                        counter = 0;
                        indexCount++;
                    }
                }
            }
            for (int j = 0; j < width; j++) {
                if (tempValues[j] != horizontalValues[i][j]) {
                    return false;
                }
            }
            return true;
        case 'v':
            for (int j = 0; j < height; j++) {
                if (grid[j][i]) {
                    counter++;
                } else {
                    if (counter > 0) {
                        tempValues[indexCount] = counter;
                        counter = 0;
                        indexCount++;
                    }
                }
            }
            for (int j = 0; j < height; j++) {
                //cout << "tempValues[" << j << "]: " << tempValues[j] << endl;
                //cout << "verticalValues[" << i << "][" << j << "]: " << verticalValues[i][j] << endl;
                if (tempValues[j] != verticalValues[i][j]) {
                    return false;
                }
            }
            return true;
        default:
            return false;
    }
}

void Nonogram::countValues() {
    resetValues();
    int counter = 0;
    int indexCount = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j]) {
                counter++;
            } else {
                if (counter > 0) {
                    horizontalValues[i][indexCount] = counter;
                    counter = 0;
                    indexCount++;
                }
            }
        }
        horizontalValues[i][indexCount] = counter;
        counter = 0;
        indexCount = 0;
    }
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            if (grid[i][j]) {
                counter++;
            } else {
                if (counter > 0) {
                    verticalValues[j][indexCount] = counter;
                    counter = 0;
                    indexCount++;
                }
            }
        }
        verticalValues[j][indexCount] = counter;
        counter = 0;
        indexCount = 0;
    }
}

void Nonogram::writeFile() {
    ofstream output;
    string outputFile;
    cout << "Please enter the requested file path of the output file:" << endl;
    cin >> outputFile;
    output.open(outputFile.c_str(), ios::out);
    output << height << ' ' << width << '\n';
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (horizontalValues[i][j] != 0) {
                output << horizontalValues[i][j] << ' ';
            }
        }
        output.put('0');
        output.put('\n');
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (verticalValues[i][j] != 0) {
                output << verticalValues[i][j] << ' ';
            }
        }
        output.put('0');
        output.put('\n');
    }
    output.close();
}

void Nonogram::readFile() {
    ifstream input;
    string inputFile;
    int indexI = 0;
    int indexJ = 0;
    int currentLine = 0;
    int number = 0;
    cout << "Please enter the file path of the input file:" << endl;
    cin >> inputFile;
    input.open(inputFile.c_str(), ios::in);
    if (input.fail()) {
        cout << "Error while loading file. Returning to main menu." << endl;
    }
    input >> height;
    input.ignore(1);
    input >> width;
    input.ignore(1);
    clean();
    autoCount = false;
    resetValues();
    while (!input.eof()) {
        input >> number;
        cout << "number: " << number << endl;
        while (number != 0) {
            if (currentLine < height) {
                horizontalValues[indexI][indexJ] = number;
            } else {
                verticalValues[indexI][indexJ] = number;
                cout << "Vertical number" << endl;
            }
            indexJ++;
            input.ignore(1);
            input >> number;
            if (currentLine == height) {
                indexI = 0;
                verticalValues[indexI][indexJ] = number;
            }
        }
        if (number == 0) {
            currentLine++;
            indexI++;
            indexJ = 0;
        }
    }
}

void Nonogram::print() {
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
        if (!autoCount) {
            if (checkLineValue(i, 'h')) {
                cout << " " << 'V';
            } else {
                cout << "  ";
            }
        }
        j = 0;
        while (horizontalValues[i][j] > 0) {
            cout << " " << horizontalValues[i][j];
            j++;
        }
        cout << endl;
    }
    printHorizontalLine();
    if (!autoCount) {
        cout << ' ';
        for (i = 0; i < width; i++) {
            if (checkLineValue(i, 'v')) {
                cout << " V";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
    for (i = 0; i < height; i++) {
        if (!emptyLine(i)) {
            cout << ' ';
            for (j = 0; j < width; j++) {
                if (verticalValues[j][i] != 0) {
                    cout << ' ' << verticalValues[j][i];
                } else {
                    if (verticalValues[j][i] == 0) {
                        cout << "  ";
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
    if (autoCount) {
        countValues();
    }
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
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
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
    for (i = 0; i < maxDimension; i++) {
        for (j = 0; j < maxDimension; j++) {
            verticalValues[i][j] = 0;
        }
    }
}

void Nonogram::clean() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            grid[i][j] = false;
        }
    }
    autoCount = true;
}

void Nonogram::submenu(char selection) {
    while (selection != 'b') {
        cout << "[S]change size | [P]change percentage | [M]back to mainmenu | [I]import | [E]export" << endl;
        cin >> selection;
        switch (char(tolower(selection))) {
            case 's':
                changeSize();
                break;
            case 'p':
                changePercentage();
                break;
            case 'r':
                readFile();
                break;
            case 'm':
                return;
            case 'e':
                writeFile();
        }
    }
}

void mainMenu() {
    char selection;
    Nonogram nono;
    nono.clean();
    nono.resetValues();
    while (selection != 'q' || selection != 'Q') {
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

        switch (char(tolower(selection))) {
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
