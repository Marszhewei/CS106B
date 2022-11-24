#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <experimental/random>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

/**
 * 1 or 0, if there is a cell, it will die
 * 2, not change
 * 3, if is empty, add 1, otherwise not change
 * more than 3, if there is a cell, it will die
 */

void welcome_msg();
bool read_file(ifstream &fin);
void random_map(Grid<bool> &pre_flag, Grid<bool> &curr_flag, LifeGUI &gui);
void read(ifstream &fin, Grid<bool> &pre_flag, Grid<bool> &curr_flag, LifeGUI &gui);
void life_game(Grid<bool> &pre_flag, Grid<bool> &curr_flag, LifeGUI &gui);
void next(Grid<bool> &pre_flag, Grid<bool> &curr_flag);
void display(const Grid<bool> &flag, LifeGUI &gui);

int main()
{
    welcome_msg();
    ifstream fin;
    bool is_random = read_file(fin);
    Grid<bool> pre_flag;  // true means there is a cell
    Grid<bool> curr_flag;
    LifeGUI gui;
    if (is_random) {
        random_map(pre_flag, curr_flag, gui);
    } else {
        read(fin, pre_flag, curr_flag, gui);
    }

    life_game(pre_flag, curr_flag, gui);

    fin.clear();
    fin.close();

    cout << "Have a nice Life!" << endl;
    return 0;
}

/**
 * @brief welcome message before life game
 */
void welcome_msg()
{
    cout << "Welcome to the CS 106B Game of Life,\n"
    "a simulation of the lifecycle of a bacteria colony.\n"
    "Cells (X) live and die by the following rules:\n"
    "- A cell with 1 or fewer neighbors dies.\n"
    "- Locations with 2 neighbors remain stable.\n"
    "- Locations with 3 neighbors will create life.\n"
    "- A cell with 4 or more neighbors dies.\n\n\n";
}

/**
 * @brief get file name
 * @param fin -> ifstream object
 */
bool read_file(ifstream &fin)
{
    string file_name;
    while (true) {
        file_name = getLine("Please input a valid file name or r)andom for rand map: ");
        if (file_name == "r") {
            return true;
        } else if (!fileExists(file_name)) {
            cout << "Can't locate the file, try again.\n";
        } else {
            openFile(fin, file_name);
            return false;
        }
    }
}


void random_map(Grid<bool> &pre_flag, Grid<bool> &curr_flag, LifeGUI &gui)
{
    srand(time(0));
    int m = randomInteger(80, 100);
    int n = randomInteger(80, 100);
    pre_flag.resize(m, n);
    curr_flag.resize(m, n);
    gui.resize(m, n);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            pre_flag[i][j] = randomInteger(0, 1);
        }
    }
    display(pre_flag, gui);
    curr_flag.deepCopy(pre_flag);
}


/**
 * @brief read data from file
 * @param fin -> ifstream object
 * @param pre_flag -> previous map
 * @param curr_flag -> current map
 */
void read(ifstream &fin, Grid<bool> &pre_flag, Grid<bool> &curr_flag, LifeGUI &gui)
{
    int m, n;
    fin >> m >> n;
    pre_flag.resize(m, n);
    curr_flag.resize(m, n);
    gui.resize(m, n);

    char ch;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fin >> ch;
            if (ch == '-') {
                pre_flag[i][j] = false;
            } else {
                pre_flag[i][j] = true;
            }
        }
    }
    display(pre_flag, gui);
    curr_flag.deepCopy(pre_flag);
}

/**
 * @brief life_game main loop
 * @param pre_flag -> previous map
 * @param curr_flag -> current map
 */
void life_game(Grid<bool> &pre_flag, Grid<bool> &curr_flag, LifeGUI &gui)
{
    while (true) {
        string s = getLine("a)nimate, t)ick, q)uit? ");
        if (s == "a") {
            int t = getInteger("How many frames? ", "Illegal integer format. Try again.\n");
            for (int i = 0; i < t; ++i) {
                clearConsole();
                next(pre_flag, curr_flag);
                display(curr_flag, gui);
                pause(50);
            }
        } else if (s == "t") {
            next(pre_flag, curr_flag);
            display(curr_flag, gui);
        } else if (s == "q") {
            break;
        } else {
            continue;
        }
    }
}

/**
 * @brief the next step of life game
 * @param pre_flag -> previous map
 * @param curr_flag -> current map
 */
void next(Grid<bool> &pre_flag, Grid<bool> &curr_flag)
{
    int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int m = pre_flag.numRows();
    int n = pre_flag.numCols();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int cnt = 0;
            for (int k = 0; k < 8; ++k) {
                cnt += pre_flag[(i+dx[k]+m)%m][(j+dy[k]+n)%n];
            }
            if (cnt == 0 || cnt == 1 || cnt > 3) {
                curr_flag[i][j] = false;
            } else if (cnt == 3) {
                curr_flag[i][j] = true;
            } else {
                continue;
            }
        }
    }
    pre_flag.deepCopy(curr_flag);
}

/**
 * @brief display game map
 * @param flag -> game map
 */
void display(const Grid<bool> &flag, LifeGUI &gui)
{
    int m = flag.numRows();
    int n = flag.numCols();
    for (int i  = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (flag[i][j]) {
                cout << 'x';
                gui.drawCell(i, j, true);
            } else {
                cout << '-';
                gui.drawCell(i, j, false);
            }
        }
        cout << endl;
    }
}
