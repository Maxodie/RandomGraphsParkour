#pragma once
#include "Core.h"

typedef struct RGP_Font_s RGP_Font_t;
typedef struct RGP_FontLoader_s RGP_FontLoader_t;
typedef struct RGP_Character_s RGP_Character_t;

struct RGP_Character_s
{
    uint32_t texID;
    uint32_t advance;
    ILDA_vector2f size;
    ILDA_vector2f bearing;
};

void RGP_FontLoaderCreate(RGP_FontLoader_t** outFontLoader);
void RGP_FontLoaderDestroy(RGP_FontLoader_t* fontLoader);

void RGP_FontGetCharacter(const RGP_Character_t** outCharacter, RGP_Font_t* font, uint32_t id);

void RGP_LoadFont(RGP_Font_t** outFont, RGP_FontLoader_t* fontLoader, const char* path, uint32_t extractedSize);
void RGP_UnloadFont(RGP_Font_t* font, RGP_FontLoader_t* fontLoader);
