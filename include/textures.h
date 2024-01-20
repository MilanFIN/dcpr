#ifndef TEXTURES_H
#define TEXTURES_H

const int TEXTURESIZE = 16;
const int LETTERSIZE = 8;

const int TEXTURES[20*16*16] = {

    //stone wall
    3,3,1,3,3,3,3,3,3,3,1,3,3,3,3,3,
    3,3,1,3,3,3,3,3,3,3,1,3,3,3,3,3,
    3,3,1,2,3,3,3,3,3,3,1,2,3,3,3,3,
    3,3,1,2,2,2,3,3,3,3,1,2,2,2,3,3,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3,3,3,3,3,1,3,3,3,3,3,3,3,1,3,3,
    3,3,3,3,3,1,3,3,3,3,3,3,3,1,3,3,
    3,3,3,3,3,1,3,3,3,3,3,3,3,1,3,3,
    3,3,3,3,3,1,2,3,3,3,3,3,3,1,2,3,
    2,3,3,3,3,1,2,2,2,3,3,3,3,1,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,
    1,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,
    1,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,
    1,2,3,3,3,3,3,3,1,2,3,3,3,3,3,3,
    1,2,2,2,3,3,3,3,1,2,2,2,3,3,3,3,

    //key
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    00,00,39,39,00,00,00,00,00,00,00,00,00,00,00,00,
    00,39,39,39,39,39,00,00,00,00,00,00,00,00,00,00,
    39,39,39,39,39,39,39,00,00,00,00,00,00,00,00,00,
    39,39,00,00,39,39,39,39,39,39,39,39,39,39,39,39,
    39,39,00,00,39,39,39,39,39,39,39,39,39,39,39,39,
    39,39,39,39,39,39,39,00,00,00,39,36,39,36,39,00,
    00,39,39,39,39,39,00,00,00,00,39,36,39,36,39,00,
    00,00,39,39,00,00,00,00,00,00,00,00,00,00,00,00,



    //color reference tile
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,16,17,18,19,20,21,22,23,
    24,25,26,27,28,29,30,31,24,25,26,27,28,29,30,31,
    32,33,34,35,36,37,38,39,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,40,41,42,43,44,45,46,47,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,
    0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,



    
    //wooden door
    3,3,1,3,132,132,132,132,132,132,132,132,3,3,3,3,
    3,3,1,132,132,132,132,132,132,132,132,132,132,3,3,3,
    3,3,132,132,132,132,132,132,132,132,132,132,132,132,3,3,
    3,132,132,132,132,132,132,132,132,132,132,132,132,132,132,3,
    1,132,132,132,132,132,132,132,132,132,132,132,132,132,132,1,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,
    132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,

    //ball
    0,0,0,0,0,0,46,46,46,46,0,0,0,0,0,0,
    0,0,0,0,46,46,46,46,46,46,46,46,0,0,0,0,
    0,0,0,46,46,46,46,46,46,46,46,46,46,0,0,0,
    0,0,46,46,46,46,46,46,46,46,46,46,46,46,0,0,
    0,46,46,46,46,46,46,46,46,46,46,46,46,46,46,0,
    0,46,46,46,46,46,46,46,46,46,46,46,46,46,46,0,
    46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
    46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
    46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
    46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
    0,46,46,46,46,46,46,46,46,46,46,46,46,46,46,0,
    0,46,46,46,46,46,46,46,46,46,46,46,46,46,46,0,
    0,0,46,46,46,46,46,46,46,46,46,46,46,46,0,0,
    0,0,0,46,46,46,46,46,46,46,46,46,46,0,0,0,
    0,0,0,0,46,46,46,46,46,46,46,46,0,0,0,0,
    0,0,0,0,0,0,46,46,46,46,0,0,0,0,0,0,
    
    //cylinder
    0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,
    0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,
    0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,15,15,15,15,15,15,15,15,15,15,15,15,0,0,
    0,0,0,15,15,15,15,15,15,15,15,15,15,0,0,0,
    0,0,0,0,15,15,15,15,15,15,15,15,0,0,0,0,
    0,0,0,0,0,0,15,15,15,15,0,0,0,0,0,0,

    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,

    //ball2
    0,0,0,0,0,0,30,30,30,30,0,0,0,0,0,0,
    0,0,0,0,30,30,30,30,30,30,30,30,0,0,0,0,
    0,0,0,30,30,30,30,30,30,30,30,30,30,0,0,0,
    0,0,30,30,30,30,30,30,30,30,30,30,30,30,0,0,
    0,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0,
    0,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    0,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0,
    0,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0,
    0,0,30,30,30,30,30,30,30,30,30,30,30,30,0,0,
    0,0,0,30,30,30,30,30,30,30,30,30,30,0,0,0,
    0,0,0,0,30,30,30,30,30,30,30,30,0,0,0,0,
    0,0,0,0,0,0,30,30,30,30,0,0,0,0,0,0,

    //ball3
    0,0,0,0,0,0,39,39,39,39,0,0,0,0,0,0,
    0,0,0,0,39,39,39,39,39,39,39,39,0,0,0,0,
    0,0,0,39,39,39,39,39,39,39,39,39,39,0,0,0,
    0,0,39,39,39,39,39,39,39,39,39,39,39,39,0,0,
    0,39,39,39,39,39,39,39,39,39,39,39,39,39,39,0,
    0,39,39,39,39,39,39,39,39,39,39,39,39,39,39,0,
    39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
    39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
    39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
    39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
    0,39,39,39,39,39,39,39,39,39,39,39,39,39,39,0,
    0,39,39,39,39,39,39,39,39,39,39,39,39,39,39,0,
    0,0,39,39,39,39,39,39,39,39,39,39,39,39,0,0,
    0,0,0,39,39,39,39,39,39,39,39,39,39,0,0,0,
    0,0,0,0,39,39,39,39,39,39,39,39,0,0,0,0,
    0,0,0,0,0,0,39,39,39,39,0,0,0,0,0,0,

    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,

    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,

};

const int LETTERS[50*8*8] = {

    0,1,1,1,1,1,1,0, //0
    1,0,0,0,0,0,1,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,1,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,1,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,

    0,0,0,1,1,0,0,0, //1
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,

    0,1,1,1,1,1,1,0, //2
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,0, //3
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,

    1,0,0,0,0,0,0,1, //4
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,

    0,1,1,1,1,1,1,1, //5
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,

    0,1,1,1,1,1,1,0, //6
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,0,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,

    0,1,1,1,1,1,1,0, //7
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,1,0,0,
    0,0,1,1,1,1,1,0,
    0,0,0,0,1,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,0,1,0,0,0,

    0,1,1,1,1,1,1,0, //8
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,

    0,1,1,1,1,1,1,0, //9
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,



    1,1,1,1,1,1,1,1, //not in use #1
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, //not in use #2
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, //not in use #3
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, //not in use #4
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,

    1,0,0,0,0,0,0,0, //not in use  (>)
    1,1,1,0,0,0,0,0,
    1,1,1,1,1,0,0,0,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,0,0,0,
    1,1,1,0,0,0,0,0,
    1,0,0,0,0,0,0,0,

    1,1,1,1,1,1,1,1, //not in use #5
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, //not in use #6
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,

    0,1,1,1,1,1,1,0, //a
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    
    1,1,1,1,1,1,1,0, //b
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,

    0,1,1,1,1,1,1,1, //c
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,1,    
    
    1,1,1,1,1,1,1,0, //d
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,

    1,1,1,1,1,1,1,1, //e
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    
    1,1,1,1,1,1,1,1, //f
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,

    0,1,1,1,1,1,1,0, //g
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,    
    
    1,0,0,0,0,0,0,1, //h
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,

    0,0,0,1,1,0,0,0, //i
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,

    0,0,0,0,0,0,0,1, //j
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,

    1,0,0,0,1,1,0,0, //k
    1,0,0,1,0,0,0,0,
    1,0,1,0,0,0,0,0,
    1,1,0,0,0,0,0,0,
    1,1,0,0,0,0,0,0,
    1,0,1,0,0,0,0,0,
    1,0,0,1,0,0,0,0,
    1,0,0,0,1,1,0,0,

    1,0,0,0,0,0,0,0, //l
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,0,
        
    1,0,0,0,0,0,0,1, //m
    1,1,0,0,0,0,1,1,
    1,0,1,0,0,1,0,1,
    1,0,1,1,1,1,0,1,
    1,0,0,1,1,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,

    1,0,0,0,0,0,0,1, //n
    1,1,0,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,1,0,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,1,

    0,1,1,1,1,1,1,0, //o
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,

    0,1,1,1,1,1,1,0, //p
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,

    0,1,1,1,1,1,0,0, //Q
    1,0,0,0,0,0,1,0,
    1,0,0,0,0,0,1,0,
    1,0,0,0,0,0,1,0,
    1,0,0,0,0,0,1,0,
    1,0,0,0,1,0,1,0,
    1,0,0,0,0,1,1,0,
    0,1,1,1,1,1,0,1,

    0,1,1,1,1,1,1,0, //R
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,
    1,0,0,1,0,0,0,0,
    1,0,0,0,1,0,0,0,
    1,0,0,0,0,1,0,0,
    1,0,0,0,0,0,1,0,

    0,1,1,1,1,1,1,0, //s
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,0,

    1,1,1,1,1,1,1,1, //t
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,

    1,0,0,0,0,0,0,1, //u
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,1,1,1,1,1,0,

    1,0,0,0,0,0,0,1, //v
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,

    1,0,0,0,0,0,0,1, //w
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    0,1,0,1,1,0,1,0,
    0,1,0,1,1,0,1,0,
    0,0,1,1,1,1,0,0,

    1,0,0,0,0,0,0,1, //z
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    1,0,0,0,0,0,0,1,

    1,0,0,0,0,0,0,1, //z
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,

    1,1,1,1,1,1,1,1, //z
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,1,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
};

#endif //TEXTURES_H