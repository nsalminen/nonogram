//Authors: Leroy Klotz and Nels Numan
//Stdnr: Leroy: 1414372, Nels: 1459929
//File: KlotzNuman.cpp
//Description:
//Dit programma biedt de mogelijkheid om een nonogram puzzel
//te spelen.
//Compiler: g++ 4.2.1 IDE: Netbeans, Mac OS X
//Date: 15 november 2013

#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>

using namespace std;

//De klasse Cursor bevat de coordinaten van de cursor op het speelveld. 
//Het bestaan van deze klasse vereenvoudigt de interactie tussen de
//cursor en het speelveld en zorgt voor ruimte voor uitbreiding van het programma

class Cursor {
private:
    int x, y;
public:
    Cursor();
    int getX();
    int getY();
    void centerCursor(int width, int height);
    void move(char direction, int width, int height);
};

Cursor::Cursor() {
    x = y = 0;
}

//Deze methode verandert de positie van de cursor op basis van parameters.
//Deze methode heeft de parameters: direction;om de richting 
//van de verplaatsing aan te geven, width en height; 
//om de grootte van het speelveld aan te geven.

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

//Een eenvoudige methode voor het opvragen van de x-positie van de Cursor

int Cursor::getX() {
    return x;
}

//Een eenvoudige methode voor het opvragen van de y-positie van de Cursor

int Cursor::getY() {
    return y;
}

//Deze methode centreert de cursor op het speelveld

void Cursor::centerCursor(int width, int height) {
    x = width / 2;
    y = height / 2;
}

class Nonogram {
private:
    const static int maxDimension = 50;
    int width, height;
    int percentage;
    //Het percentage wat gebruikt wordt voor het willekeurig vullen
    bool grid[maxDimension][maxDimension];
    //Het speelveld
    int horizontalValues[maxDimension][maxDimension];
    //De horizontale beschrijvingen
    int verticalValues[maxDimension][maxDimension];
    //De verticale beschrijvingen
    bool autoCount; //Een boolean om aan te geven of de beschrijving...
    //...automatisch moeten meetellen bij het veranderen 
    //van de inhoud van het speelveld
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
    width = 15;
    height = 15;
    percentage = 50;
    autoCount = true;
}

//Een eenvoudige methode voor het opvragen van de hoogte van het Nonogram

int Nonogram::getHeight() {
    return height;
}

//Een eenvoudige methode voor het opvragen van de breedte van het Nonogram

int Nonogram::getWidth() {
    return width;
}

//In deze methode worden het aantal X'en in een specifieke 
//regel van het Nonogram geteld. Hierna wordt dit aantal 
//vergeleken met de beschrijving.

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
                if (tempValues[j] != verticalValues[i][j]) {
                    return false;
                }
            }
            return true;
        default:
            return false;
    }
}

//Deze methode itereert door het volledige speelveld van het Nonogram 
//en telt per regel het aantal X'en en schrijft dit naar de beschrijvingsarrays.

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

//Verandert getallen van karakters naar integers

int readNumber() {
    int getal;
    char temp;
    temp = ' ';
    getal = 0;

    while (temp != '\n') {
        temp = cin.get();
        if (temp >= '0' && temp <= '9') {
            getal = getal * 10 + (temp - '0');
        }
    }
    return getal;
}

//Deze methode leest karakters in en geeft deze terug aan de functie
//zonder overbodige enters

void readSelection(char &output) {
    output = cin.get();
    char temp;
    temp = output;
    while (temp != '\n') {
        temp = cin.get();
    }
}

//Deze methode schrijft de huidige beschrijvingen en hoogte en breedte van het spel 
//naar een bestand met een door de gebruiker gespecificeerde naam.

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

//Deze methode leest de huidige beschrijvingen en hoogte en breedte van het spel 
//van een bestand met een door de gebruiker gespecificeerde naam.

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
        return;
    }
    input >> height;
    input.ignore(1);
    input >> width;
    input.ignore(1);
    clean();
    autoCount = false;
    resetValues();
    cursor.centerCursor(width, height);
    while (!input.eof()) {
        input >> number;
        cout << "number: " << number << endl;
        while (number != 0) {
            if (currentLine < height) {
                horizontalValues[indexI][indexJ] = number;
            } else {
                verticalValues[indexI][indexJ] = number;
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

//Deze methode print het speelveld, beschrijvingen en V's op het scherm
//van de gebruiker

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
        if (!autoCount) { //Het onderdeel waar de horizontale V's geprint worden
            if (checkLineValue(i, 'h')) {
                cout << " " << 'V';
            } else {
                cout << "  ";
            }
        }
        j = 0;
        //Het onderdeel waar de horizontale beschrijvingen geprint worden
        while (horizontalValues[i][j] > 0) {
            cout << " " << horizontalValues[i][j];
            j++;
        }
        cout << endl;
    }
    printHorizontalLine();
    if (!autoCount) { //Het onderdeel waar de verticale V's geprint worden
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
    //Het onderdeel waar de verticale beschrijvingen geprint worden
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

//Deze methode is een hulpmethode voor Nonogram::print(). 
//Er wordt met deze methode een horizontale lijn gelijk aan de
//breedte van het spel met +-tekens op scherm geprint

void Nonogram::printHorizontalLine() {
    for (int i = 0; i < width + 2; i++) {
        cout << '+' << ' ';
    }
    cout << endl;
}

//Deze methode is een hulpmethode voor Nonogram::print(). Er wordt met deze methode
//nagegaan of een gehele regel van de onderste beschrijvingsarray leeg is

bool Nonogram::emptyLine(int i) {
    for (int j = 0; j < width; j++) {
        if (verticalValues[j][i] > 0) {
            return false;
        }
    }
    return true;
}

//Deze methode verandert de dimensies van het speelveld met behulp van de invoer van
//de gebruiker

void Nonogram::changeSize() {
    int tempWidth = -1;
    int tempHeight = -1;
    while ((tempWidth > 50 || tempWidth < 0)) {
        cout << "Please enter width (< 50):" << endl;
        tempWidth = readNumber();
    }
    width = tempWidth;
    while ((tempHeight > 50 || tempHeight < 0)) {
        cout << "Please enter height (< 50):" << endl;
        tempHeight = readNumber();
    }
    height = tempHeight;
    clean();
    resetValues();
    cursor.centerCursor(width, height);
}

//Deze methode schakelt de toestand van een specifieke cel in het speelveld

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

//Deze methode is een hulpmethode van Nonogram::fillRandomly(). 
//Er wordt in deze methode een willekeurig getal gegenereerd.

int Nonogram::getRandomNumber() {
    static int number = 42;
    number = ((221 * number + 1) % 1000) / 10;
    return number;
}

//Deze methode verandert het percentage gebruikt bij het 
//willekeurig vullen van het speelveld met behulp van invoer van de gebruiker

void Nonogram::changePercentage() {
    int temp = -1;
    while (temp < 0 || temp > 100) {
        cout << "Choose a percentage between 0 and 100:" << endl;
        temp = readNumber();
    }
    percentage = temp;
}

//Deze methode vult het speelveld op een willekeurige manier met behulp van het
//gespecificeerde percentage

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

//Deze methode maakt de beschrijvingsarrays leeg, dat wil 
//zeggen dat de twee arrays volledig gevuld worden met 0

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

//Deze methode maakt het speelveld leeg, dat wil zeggen dat de 
//array van het speelveld volledig op false wordt gezet

void Nonogram::clean() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = false;
        }
    }
    autoCount = true;
    cursor.centerCursor(width, height);
    resetValues();
}

//Deze methode kan vanuit het mainmenu aangeroepen en toont extra opties
//op het scherm van de gebruiker

void submenu(Nonogram &nono) {
    char selection;
    while (true) {
        cout << "[S]change size | [P]change percentage | [I]import | [E]export | [M]mainmenu" << endl;
        readSelection(selection);
        switch (char(tolower(selection))) {
            case 's':
                nono.changeSize();
                break;
            case 'p':
                nono.changePercentage();
                break;
            case 'i':
                nono.readFile();
                break;
            case 'e':
                nono.writeFile();
            case 'm':
                return;
        }
    }
}

//Deze methode print het logo van het programma op het scherm van de gebruiker

void printLogo() {
    cout << "      _   __" << endl;
    cout << "     / | / /___  ____  ____  ____  _________  ____ ___" << endl;
    cout << "    /  |/ / __ \\/ __ \\/ __ \\/ __ `/ ___/ __ `/ __ `__ \\ " << endl;
    cout << "   / /|  / /_/ / / / / /_/ / /_/ / /  / /_/ / / / / / / " << endl;
    cout << "  /_/ |_/\\____/_/ /_/\\____/\\__, /_/   \\__,_/_/ /_/ /_/ " << endl;
    cout << "                          /____/ " << endl << endl;
}

//Deze methode toont de opties van het hoofdmenu aan de gebruiker
//en verzoekt het Nonogram om zichzelf te printen

void mainMenu() {
    char selection;
    bool mainMenu = true;
    Nonogram nono;
    nono.clean();
    nono.resetValues();
    while (mainMenu) {
        printLogo();
        nono.print();
        cout << "[W]up | [A]left | [D]right | [S]down | [T]toggle node" << endl;
        cout << "[C]clean | [R]fill randomly | [M]more options | [Q]quit" << endl;
        readSelection(selection);
        switch (char(tolower(selection))) {
            case 'm':
                submenu(nono);
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
            case 'q':
                mainMenu = false;
        }
    }
}

//Deze methode print het 'infoblokje' van het programma

void printInfo() {
    cout << "Authors: Leroy Klotz and Nels Numan" << endl;
    cout << "Stdnr: Leroy: 1414372, Nels: 1459929" << endl;
    cout << "File: KlotzNuman.cc" << endl;
    cout << "Description:" << endl;
    cout << "Dit programma biedt de mogelijkheid om een nonogram puzzel te spelen" << endl;
    cout << "Compiler: g++ 4.2.1 IDE: Netbeans, Mac OS X" << endl;
    cout << "Date: 15 november 2013" << endl << endl;
}

int main() {
    printInfo();
    mainMenu();
    return 0;
}
