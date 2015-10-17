//
//  main.cpp
//  Snake
//
//  Created by Phillip Booth on 10/15/15.
//  Copyright © 2015 Phillip Booth. All rights reserved.
//
//  Simple Snake Game:
//        Use 'l key' to turn left relative to your current position,
//        Use 'r key' to turn right relative to your current position.
//        Hold down the 'm key' to move your snake.
//
//        Dont hit your tail and dont let your energy level reach zero or else you're
//        finished.


#include <iostream>
#include "termfuncs.hpp"
using namespace std;

// constants
const int WIDTH = 30;
const int HEIGHT = 30;

//structs
struct Snake {
    
    char currHead;
    int xPos;
    int yPos;
    int EU;
    int score;
    bool dead;
    
};

//function declarations
void setupBoard(char board[WIDTH][HEIGHT]);
void fillInnerBoard(char board[WIDTH][HEIGHT]);
void drawRepresentationOfBoard(char board[WIDTH][HEIGHT]);
void drawBoard(char board[WIDTH][HEIGHT], Snake phil);
void checkSnack(char board[WIDTH][HEIGHT]);
void insertSnack(char board[WIDTH][HEIGHT]);
void showScore(int score);


int playGame(char board[WIDTH][HEIGHT]);

bool isDead(char board[WIDTH][HEIGHT], Snake phil);

Snake init();
Snake changeHeadDirection(char input, Snake phil);
Snake foundSnack(char board[WIDTH][HEIGHT], Snake phil);
Snake updateInfoOnMove(char board[WIDTH][HEIGHT], Snake phil);
Snake updateTail(char board[WIDTH][HEIGHT], Snake phil);


//Interface
int main() {
    char board[WIDTH][HEIGHT];
    int score;
    setupBoard(board);
    score = playGame(board);
    showScore(score);
    return 0;
    
}

void setupBoard(char board[WIDTH][HEIGHT]){
    screen_clear();
    fillInnerBoard(board);
}

void fillInnerBoard(char board[WIDTH][HEIGHT]){
    for(int h = 0; h < HEIGHT; h++){
        for(int w = 0; w < WIDTH; w++) {
            board[w][h] = ' ';
        }
    }
    drawRepresentationOfBoard(board);
    
}
void drawRepresentationOfBoard(char board[WIDTH][HEIGHT]){
    cout << endl;
    //Print board itself
    for(int h = -1; h < HEIGHT + 1; h++){
        for(int w = -1; w < WIDTH + 1; w++) {
            if(h == -1 || h == HEIGHT || w == -1 || w == WIDTH ){
                cout << '#';
            }else{
                cout << board[w][h];
            }
        }
        cout << endl;
    }
}

void drawBoard(char board[WIDTH][HEIGHT], Snake phil){
    screen_clear();
    board[phil.xPos][phil.yPos] = phil.currHead;
    drawRepresentationOfBoard(board);
    cout << "Energy Level: " << phil.EU << " Score: " ;
    cout << phil.score << endl;
    
}

void checkSnack(char board[WIDTH][HEIGHT]){
    bool found  = false;
    for(int h = 0; h < HEIGHT; h++){
        for(int w = 0; w < WIDTH; w++ ){
            if(board[w][h] == '*'){
                found = true;
            }
        }
    }
    if(!found){
        insertSnack(board);
    }
}

void insertSnack(char board[WIDTH][HEIGHT]){
    int xTar, yTar;
    bool placed = false;
    while(placed == false){
        xTar = random_int(0, 30);
        yTar = random_int(0, 30);
        if(board[xTar][yTar] == ' '){
            board[xTar][yTar] = '*';
            placed = true;
        }
    }
}

void  showScore(int score) {
    cout << "Game Over. Your Score: " << score <<endl;
    
}

 Snake changeHeadDirection(char input, Snake phil){
    switch (input) {
        case 'l':
            switch (phil.currHead) {
                case '^':
                    phil.currHead = '<';
                    break;
                case '>':
                    phil.currHead = '^';
                    break;
                case 'v':
                    phil.currHead = '>';
                    break;
                case '<':
                    phil.currHead = 'v';
                    break;
                default:
                    break;
            }
            break;
        case 'r':
            switch (phil.currHead) {
                case '^':
                    phil.currHead = '>';
                    break;
                case '>':
                    phil.currHead = 'v';
                    break;
                case 'v':
                    phil.currHead = '<';
                    break;
                case '<':
                    phil.currHead = '^';
                    break;
                default:
                    break;
            }
            break;

    }
     return phil;
}

Snake foundSnack(char board[WIDTH][HEIGHT], Snake phil){
    if (phil.currHead == '^') {
        if (board[phil.xPos][phil.yPos - 1] == '*') {
            phil.EU += 30;
        }
    }
    if (phil.currHead == '>') {
        if (board[phil.xPos + 1][phil.yPos] == '*') {
            phil.EU += 30;
        }
    }
    if (phil.currHead == '<') {
        if (board[phil.xPos - 1 ][phil.yPos] == '*') {
            phil.EU += 30;
        }
    }
    if (phil.currHead == 'v') {
        if (board[phil.xPos][phil.yPos + 1] == '*') {
            phil.EU += 30;
        }
    }
    
    return phil;
}

Snake updateInfoOnMove(char board[WIDTH][HEIGHT], Snake phil){
    phil = updateTail(board, phil);
    phil.score++;
    phil.EU --;
    return phil;
}

Snake updateTail(char board[WIDTH][HEIGHT], Snake phil){
    if (phil.currHead == '^') {
        board[phil.xPos][phil.yPos] = '#';
        if (phil.yPos ==0) {
            phil.yPos = 30;
        }
        phil.yPos--;
    }
    if (phil.currHead == 'v') {
        board[phil.xPos][phil.yPos] = '#';
        if (phil.yPos ==29) {
            phil.yPos = -1;
        }
        phil.yPos++;
    }
    if (phil.currHead == '>') {
        board[phil.xPos][phil.yPos] = '#';
        if (phil.xPos ==29) {
            phil.xPos = 0;
        }
        phil.xPos++;
    }
    if (phil.currHead == '<') {
        board[phil.xPos][phil.yPos] = '#';
        if (phil.yPos ==0) {
            phil.yPos = 29;
        }
        phil.xPos--;
    }
    
    return phil;
}

Snake init(){
    Snake phil;
    phil.xPos = 15;
    phil.yPos = 15;
    phil.currHead = '^';
    phil.EU = 30;
    phil.score = 0;
    
    return phil;
}

bool isDead(char board[WIDTH][HEIGHT], Snake phil){
    if (phil.currHead == '^') {
        if (board[phil.xPos][phil.yPos - 1] == '#') {
            return true;
        }
    }
    if (phil.currHead == '>') {
        if (board[phil.xPos + 1][phil.yPos] == '#') {
            return true;
        }
    }
    if (phil.currHead == '<') {
        if (board[phil.xPos - 1][phil.yPos] == '#') {
            return true;
        }
    }
    if (phil.currHead == 'v') {
        if (board[phil.xPos][phil.yPos + 1] == '#') {
            return true;
        }
    }
    if (phil.EU <1) {
        return true;
    }
    return false;
}

int playGame(char board[WIDTH][HEIGHT]){
    bool dead = false;
    char input;
    Snake phil = init();
    
    while(dead == false){
        checkSnack(board);
        drawBoard(board, phil);
        
        //Actively grabbing user input
        input = getachar();
        if(input == 'l' || input == 'r'){
            //Turn the head in the right direction so movement looks natural
            phil = changeHeadDirection(input, phil);
        }
        if(input == 'm'){
            phil = foundSnack(board, phil);
            dead = isDead(board, phil);
            phil = updateInfoOnMove(board, phil);
        }
    }
    return phil.score - 1;
}


