
const unsigned char B_TOP = 1;
const unsigned char B_RIGHT = 2;
const unsigned char B_BOTTOM = 4;
const unsigned char B_LEFT = 8;
const unsigned char B_SOLID = B_TOP|B_BOTTOM|B_LEFT|B_RIGHT;

PROGMEM const unsigned char empty [8] = {
    0b10000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b10000000,
    0b01010101
};

PROGMEM const unsigned char solid [8] = {
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111
};

PROGMEM const unsigned char top [8] = {
    0b00000001,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00001111,
    0b00000111,
    0b00000011,
    0b00000001
};
PROGMEM const unsigned char right [8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111
};
PROGMEM const unsigned char bottom [8] = {
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b10000000
};
PROGMEM const unsigned char left [8] = {
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

