#ifndef TEXTURES_H
#define TEXTURES_H

#define TEXTURESIZE 16
#define LETTERSIZE 8

/// @brief all 16x16 textures that are used in the game
extern const int TEXTURES[25*16*16];
/// @brief all letters, digits etc, can be mapped to from chars
extern const int LETTERS[43*8*8];

extern const int PROJECTILETEXTURES[3];
extern const int ENEMYTEXTURES[6];

extern const int FPSHAND[25*15];
extern const int FPSBOOK[35*15];

extern const int COMPASSDIRECTIONS[8*16*16];


#endif //TEXTURES_H