#include <SFML\Graphics.hpp>
#include <vector>
#include <iostream>
using namespace sf;
using namespace std;
int WhichCell(int x, int y, int width, int height) {
    int cell;
    if (y < 305 * height/310) {//we clicked on the board might need to be removed
        //adjust the size to 310/310 window
        x = x*310 / width;
        y = y*310 / height;
        x = x/33.33;
        y = y/33.33;
        return x + y * 9;
    }
    return -1;//negative one makes it so the selected cell wont show up on the cell
}
bool inRow(vector<int> a, int location, int number) {
    location = location - location % 9;
    for (int i = 0; i < 9; i++) {
        if (a.at(location + i) == number) {
            return true;
        }
    }
    return false;
}
bool inColumn(vector<int> a, int location, int number) {
    location = location % 9;
    for (int i = 0; i < 9; i++) {
        if (a.at(location + i * 9) == number) {
            return true;
        }
    }
    return false;
}
bool inSquare(vector<int> a, int location, int number) {
    //top left box in 3x3 square
    location = location / 27 * 3 + location % 9 / 3;//get which square you are in indexed at zero
    location = location / 3 * 27 + location % 3 * 3;//get us the top left box exactly.
    for (int i = 0; i < 21; i++) {
        if (a.at(location + i) == number) {
            return true;
        }
        if ((i + 1) % 3 == 0) {
            i = i + 6;
        }
    }
    return false;
}
bool complete(vector<int> a) {
    int i = 0;
    int j = 0;
    //numbers are acceptable
    for (i = 0; i < 81; i++) {
        if (a.at(i) < 1 || a.at(i) > 9) {
            return false;
        }
    }
    //sum rows
    int sum = 0;
    for (j = 0; j < 9; j++) {
        for (i = 0; i < 9; i++) {
            sum = sum + a.at(j * 9 + i);
        }
        if (sum != 45) {
            return false;
        }
        sum = 0;
    }
    //sum colums
    sum = 0;
    for (j = 0; j < 9; j++) {
        for (i = 0; i < 9; i++) {
            sum = sum + a.at(i + 9 * j);
        }
        if (sum != 45) {
            return false;
        }
        sum = 0;
    }
    //squares
    sum = 0;
    for (j = 0; j < 9; j++) {
        for (i = 0; i < 21; i++) {
            sum = sum + a.at(i + j / 3 * 27 + j % 3 * 3);
            if ((i + 1) % 3 == 0) {
                i = i + 6;
            }
        }
        if (sum != 45) {
            return false;
        }
        sum = 0;
    }
    return true;
}
vector<int> newBoard(vector<int> a, int i) {
    //check if we are done
    if (i > 80) {
        return a;
    }
    int j = 0, random = 0, temp;
    random = rand() % 8 + 1;
    for (j = 0; j < 9; j++) {
        temp = (random + j) % 9 + 1;
        if ((inColumn(a, i, temp) || inRow(a, i, temp) || inSquare(a, i, temp)) == false) {
            a.at(i) = temp;
            a = newBoard(a, i + 1);
            if (a.at(80)>0) { return a; }
            else {//clear the current board
                for (temp = i; temp < 81; temp++) {
                    a.at(temp) = 0;
                }
            }
        }
    }
    return a;
}
int miniMatrix(vector<int> a, int square, int number) {
    int topLeft = square / 3 * 27 + square % 3 * 3;//top left cell number of said square//added for simplicity
    if(inSquare(a,topLeft,number)){
    return -1;}
    vector<bool> mini={1,1,1,1,1,1,1,1,1};
    int i;
    //remove spots where a number exists there already
        for (i = 0; i < 9; i++) {
            if (a.at(topLeft + i + i / 3 * 6) > 0) {//top left of square and every 3 interations plus six so we go to the next row
                mini.at(i) = 0;
            }
        }
    //remove the rows
        if (inRow(a, topLeft, number)) {
            mini.at(0) = 0;
            mini.at(1) = 0;
            mini.at(2) = 0;
        }
        if (inRow(a, topLeft+9, number)) {
            mini.at(3) = 0;
            mini.at(4) = 0;
            mini.at(5) = 0;
        }
        if (inRow(a, topLeft+18, number)) {
            mini.at(6) = 0;
            mini.at(7) = 0;
            mini.at(8) = 0;
        }
        //columns
        if (inColumn(a, topLeft, number)) {
            mini.at(0) = 0;
            mini.at(3) = 0;
            mini.at(6) = 0;
        }
        if (inColumn(a, topLeft+1, number)) {
            mini.at(1) = 0;
            mini.at(4) = 0;
            mini.at(7) = 0;
        }
        if (inColumn(a, topLeft+2, number)) {
            mini.at(2) = 0;
            mini.at(5) = 0;
            mini.at(8) = 0;
        }
        topLeft = 0;//use topleft as sum
        for (i = 0; i < 9; i++) {
            topLeft = topLeft + mini.at(i);
        }
        //if we have only one option return the value
        if (topLeft == 1) {
            for (i = 0; i < 9; i++) {
                if (mini.at(i) == true) {
                    return i;
                }
            }
        }
        else {
            return -1;
        }
}
bool solvable(vector<int> a) {
    int additions = 1;
    int position;
    //PrintBoard(a);
    while (additions > 0) {
        additions = 0;
        for (int number = 1; number < 10; number++) {
            for (int square = 0; square < 9; square++) {
                position = miniMatrix(a, square, number);
                if(position > -1) {
                    additions=additions+1;
                    //PrintBoard(a);
                   a.at(square / 3 * 27 + square % 3 * 3 + position + position / 3 * 6) = number;
                }
            }
        }
    }
    //PrintBoard(a);
    return complete(a);
}
vector<int> makespaces(vector<int> a) {
    int random, temp1, temp2, test=0;
    bool check=true;//this way we don't have to do solvable every single time.
    random = rand() % 40;
    a.at(random) = 0;
    a.at(80 - random) = 0;
    for (temp1 = 0; temp1 < 11; temp1++) {
        random = rand() % 40;
        a.at(random) = 0;
        a.at(80 - random) = 0;

    }
    temp1 = 0; temp2 = 0;
    check = solvable(a);
    while (check&&test<15) {
        test = test + 1;
        random = rand() % 40;
        if (a.at(random != 0)) {
            temp1 = a.at(random);
            temp2 = a.at(80 - random);
            a.at(random) = 0;
            a.at(80 - random) = 0;
            check = solvable(a);
        }
    }
    a.at(random) = temp1;
    a.at(80 - random) = temp2;
    return a;
}
std::string BoardToString(vector<int> a) {
    std::string board=" 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9\n 1 2 3 4 5 6 7 8 9";
    int i = 0;
    while(i<81){
        if (a.at(i) == 0) {
            board.at(2 * i + 1 + i / 9) = ' ';
        }
        else {
            board.at(2 * i + 1 + i / 9) = a.at(i) + '0';
        }
        i = i + 1;   
    }
    return board;
}
vector<bool> MakeMutableCells(vector<int> a) {
    vector<bool> temp(81);
    for (int i = 0; i < 81; i++) {
        if (a.at(i)>0) {
            temp.at(i) = 0;
        }
        else {
            temp.at(i) = 1;
        }
    }
    return temp;
}

int main()
{
    int selectCell = -1;
    int numberCell = 0;
    vector<int> Board(81);
    vector<bool> MutableCells;//cells which can't be changed, becuase they are part of the original puzzle
    int i = 0;
    //PrintBoard(Board);
    srand(time(0));
    Board=newBoard(Board, 0);
    Board = makespaces(Board);
    MutableCells = MakeMutableCells(Board);

    std::string BoardString= BoardToString(Board);
    //make window
    sf::RenderWindow window(sf::VideoMode(665, 381), "Sudoku9x9");
    window.setFramerateLimit(30);

    //make sprite for instruction page and sprite for board
    Texture boardPic;
    boardPic.loadFromFile("C:\\Users\\Michael\\source\\repos\\SFML\\images\\Instruction.png");
    sf::Sprite sprite(boardPic);
    //set the background
    sf::RectangleShape rectangle(sf::Vector2f(310.f, 310.f));
    rectangle.setFillColor(sf::Color::White);
    //set font
    sf::Font font;

    font.loadFromFile("C:\\Windows\\Fonts\\Consola.ttf");
    //set numbers up
    sf::Text text;
    text.setFont(font);
    text.setString(BoardString);
    text.setCharacterSize(29);
    text.setFillColor(sf::Color::Blue);
    //make select square
    sf::RectangleShape SelectedSquare(sf::Vector2f(32,32));
    SelectedSquare.setPosition(0, 0);
    SelectedSquare.setOutlineThickness(1);
    SelectedSquare.setOutlineColor(sf::Color::Green);
    //view for resizing event

    //instructions for game
    sf::Event event;
    while (window.isOpen()) {
        //tell instructions
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }
        }
        if (event.key.code == sf::Keyboard::Space) {
            break;
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    //get sudoku game setup
    boardPic.loadFromFile("C:\\Users\\Michael\\source\\repos\\SFML\\images\\sudoku9x9.png");
    window.setView(sf::View(sf::FloatRect(0, 0, 310, 310)));
    window.setSize(Vector2u(310, 310));
    sprite.setTexture(boardPic);
    //start game
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2u size = window.getSize();
                    selectCell = WhichCell(event.mouseButton.x, event.mouseButton.y, size.x, size.y);
                    SelectedSquare.setPosition((selectCell % 9 * 33.33), selectCell / 9 * 33.88);
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code)
                {
                case sf::Keyboard::Space:
                    //experimenting to see if rand will mix up the table
                    selectCell = -1;
                    SelectedSquare.setPosition((selectCell % 9 * 33.33), selectCell / 9 * 33.88);
                    Board = newBoard(Board, 0);
                    Board = makespaces(Board);
                    MutableCells = MakeMutableCells(Board);
                    text.setString(BoardToString(Board));
                    break;
                case sf::Keyboard::Num1:
                    numberCell = 1;
                    break;
                case sf::Keyboard::Num2:
                    numberCell = 2;
                    break;
                case sf::Keyboard::Num3:
                    numberCell = 3;
                    break;
                case sf::Keyboard::Num4:
                    numberCell = 4;
                    break;
                case sf::Keyboard::Num5:
                    numberCell = 5;
                    break;
                case sf::Keyboard::Num6:
                    numberCell = 6;
                    break;
                case sf::Keyboard::Num7:
                    numberCell = 7;
                    break;
                case sf::Keyboard::Num8:
                    numberCell = 8;
                    break;
                case sf::Keyboard::Num9:
                    numberCell = 9;
                    break;
                case sf::Keyboard::Numpad0:
                    numberCell = 0;
                    break;
                case sf::Keyboard::Numpad1:
                    numberCell = 1;
                    break;
                case sf::Keyboard::Numpad2:
                    numberCell = 2;
                    break;
                case sf::Keyboard::Numpad3:
                    numberCell = 3;
                    break;
                case sf::Keyboard::Numpad4:
                    numberCell = 4;
                    break;
                case sf::Keyboard::Numpad5:
                    numberCell = 5;
                    break;
                case sf::Keyboard::Numpad6:
                    numberCell = 6;
                    break;
                case sf::Keyboard::Numpad7:
                    numberCell = 7;
                    break;
                case sf::Keyboard::Numpad8:
                    numberCell = 8;
                    break;
                case sf::Keyboard::Numpad9:
                    numberCell = 9;
                    break;
                default:
                    break;
                }
            }
        }
        //change board based on mouse and keypad
        if ((selectCell != -1) && MutableCells.at(selectCell)) {
            Board.at(selectCell) = numberCell;
            text.setString(BoardToString(Board));
        }
        
        //check to see if window is completed
        if (complete(Board) && window.isOpen()) {
            //completed the puzzle start congradulations
            boardPic.loadFromFile("C:\\Users\\Michael\\source\\repos\\SFML\\images\\Congradulations.png");
            window.setView(sf::View(sf::FloatRect(0, 0, 301, 116)));
            sprite.setTexture(boardPic);
            window.setSize(sf::Vector2u(301, 116));
            //they can close the window or press space
            while (window.isOpen() && !(event.key.code == sf::Keyboard::Space)) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) { window.close(); }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }
                }
                window.clear();
                window.draw(sprite);
                window.display();
            }

            if (window.isOpen()) {//if they want another puzzle
                Board = newBoard(Board, 0);
                Board = makespaces(Board);
                MutableCells = MakeMutableCells(Board);
                text.setString(BoardToString(Board));
                window.setView(sf::View(sf::FloatRect(0, 0, 310,310)));
                window.setSize(Vector2u(310, 310));
                boardPic.loadFromFile("C:\\Users\\Michael\\source\\repos\\SFML\\images\\sudoku9x9.png");
                sprite.setTexture(boardPic);
            }
        }

        //render
        window.clear();
        window.draw(rectangle);
        window.draw(sprite);
        window.draw(SelectedSquare);
        window.draw(text);
        window.display();
    }


    return 0;
}