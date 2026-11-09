#include "Renderer/Text.h"

#include <glad/glad.h>

#include <ft2build.h>
#include <zlib.h>
#include FT_FREETYPE_H

#define RGP_MAX_LOADED_CHARACTER_COUNT 128

struct RGP_Font_s
{
    FT_Face face;
    RGP_Character_t characters[RGP_MAX_LOADED_CHARACTER_COUNT];
    Bool isCharLoaded[RGP_MAX_LOADED_CHARACTER_COUNT];
    FT_UInt extractedSize;
};

struct RGP_FontLoader_s
{
    FT_Library ft;
    FT_Long fontAmount;
};

void RGP_FontLoaderCreate(RGP_FontLoader_t** outFontLoader)
{
    RGP_ASSERT(*outFontLoader != NULL, "*outFontLoader cannot be NULL");
    *outFontLoader = malloc(sizeof(**outFontLoader));
    RGP_FontLoader_t* fontLoader = *outFontLoader;

    RGP_ASSERT(outFontLoader != NULL, "failed to allocate memory for font loader");

    fontLoader->fontAmount = 0;

    if(FT_Init_FreeType(&fontLoader->ft))
    {
        RGP_ASSERT(0, "failed to init font loader");
        return;
    }

    RGP_LOG_SUCCESS("fontloader created");
}

void RGP_FontLoaderDestroy(RGP_FontLoader_t* fontontLoader)
{
    RGP_ASSERT(fontontLoader != NULL, "font loader must not be NULL");

    FT_Done_FreeType(fontontLoader->ft);

    free(fontontLoader);

    RGP_LOG_SUCCESS("fontloader destroyed");
}

void RGP_FontGetCharacter(const RGP_Character_t** outCharacter, const RGP_Font_t* font, uint32_t id)
{
    RGP_ASSERT(outCharacter != NULL && font != NULL && id < RGP_MAX_LOADED_CHARACTER_COUNT,  "the given *character, font must not be NULL and id must be below %d, current id : %d", RGP_MAX_LOADED_CHARACTER_COUNT, id);
    *outCharacter = &font->characters[id];
}

ILDA_vector2f RGP_FontGetTextTotalSize(const RGP_FontLoader_t* fontLoader, const RGP_Font_t* font, const char* text, float size)
{
    RGP_ASSERT(fontLoader != NULL && font != NULL,  "the given fontLoader, font must not be NULL");

    ILDA_vector2f position = { 0.0f, 0.0f };
    size_t textLen = strlen(text);
    const RGP_Character_t* ch;
    for(size_t c = 0; c < textLen; ++c)
    {
        RGP_FontGetCharacter(&ch, font, text[c]);

        position.x += (ch->advance >> 6) * size;

        float ySize = ch->size.y * size;
        if(position.y < ySize)
        {
            position.y = ySize;
        }
    }

    return position;
}

void RGP_LoadFont(RGP_Font_t** outFont, RGP_FontLoader_t* fontLoader, const char* path, uint32_t extractedSize)
{
    RGP_ASSERT(*outFont != NULL && fontLoader != NULL, "*outFont and font loader and must not be NULL");

    *outFont = calloc(1, sizeof(**outFont));
    RGP_Font_t* font = *outFont;
    RGP_ASSERT(outFont != NULL, "failed to load font");

    if(FT_New_Face(fontLoader->ft, path, fontLoader->fontAmount, &font->face))
    {
        RGP_ASSERT(0, "failed to load face");
    }

    FT_Set_Pixel_Sizes(font->face, 0, extractedSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(uint8_t c = 0; c < RGP_MAX_LOADED_CHARACTER_COUNT; ++c)
    {
        if(FT_Load_Char(font->face, c, FT_LOAD_RENDER))
        {
            RGP_LOG_WARNING("failed to load character %c", c);
            continue;
        }
        font->isCharLoaded[c] = 1;

        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        FT_Bitmap* bitmap = &font->face->glyph->bitmap;
        FT_GlyphSlot glyph = font->face->glyph;
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED, bitmap->width,
            bitmap->rows, 0, GL_RED, GL_UNSIGNED_BYTE,
            bitmap->buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        font->characters[c] = (RGP_Character_t){
            .texID = texID,
            .size = { bitmap->width, bitmap->rows },
            .bearing = { glyph->bitmap_left, glyph->bitmap_top },
            .advance = font->face->glyph->advance.x,
        };
    }

    ++fontLoader->fontAmount;

    RGP_LOG_SUCCESS("font %s created", path);
}

void RGP_UnloadFont(RGP_Font_t* font, RGP_FontLoader_t* fontLoader)
{
    RGP_ASSERT(fontLoader != NULL && font != NULL, "font loader and font must not be NULL");

    for(uint8_t c = 0; c < RGP_MAX_LOADED_CHARACTER_COUNT; ++c)
    {
        if(font->isCharLoaded[c])
        {
            glDeleteTextures(1, &font->characters[c].texID);
        }
    }

    FT_Done_Face(font->face);
    --fontLoader->fontAmount;
    RGP_LOG_SUCCESS("font unloaded");
}
