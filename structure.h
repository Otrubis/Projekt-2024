#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdbool.h>
#include <stdint.h> // Pro uint8_t

// Definice datového typu byte
typedef uint8_t byte;

// Struktura TGA hlavičky
typedef struct {
    byte id_length;         // Délka ID pole
    byte color_map_type;    // Typ barevné mapy (0 = žádná, 1 = přítomná)
    byte image_type;        // Typ obrázku (např. 2 = true-color image)
    byte color_map[5];      // Barevná mapa (volitelná)
    byte x_origin[2];       // X počáteční bod (little-endian)
    byte y_origin[2];       // Y počáteční bod (little-endian)
    byte width[2];          // Šířka obrázku (little-endian)
    byte height[2];         // Výška obrázku (little-endian)
    byte depth;             // Počet bitů na pixel (např. 24, 32)
    byte descriptor;        // Deskriptor obrázku (bitové pole)
} TGAHeader;

// Struktura TGA obrázku
typedef struct {
    TGAHeader header;       // Hlavička obrázku
    byte *data;             // Dynamicky alokované pole pixelů
    int width;              // Šířka obrázku (odvozeno z hlavičky)
    int height;             // Výška obrázku (odvozeno z hlavičky)
    int channels;           // Počet barevných kanálů (např. 3 = RGB, 4 = RGBA)
} TGAImage;

// Deklarace funkcí pro práci s obrázky
int tga_bytes_to_int(const byte *bytes);               // Převod z little-endian na int
void tga_int_to_bytes(int value, byte *bytes);         // Převod z int na little-endian
bool tga_load(const char *filepath, TGAImage *image);  // Načtení TGA souboru
bool tga_save(const char *filepath, const TGAImage *image); // Uložení TGA souboru
void tga_free(TGAImage *image);                        // Uvolnění paměti obrázku
void tga_print_info(const TGAImage *image);            // Výpis informací o obrázku
void tga_resize(TGAImage *image, int new_width, int new_height); // Změna velikosti
void tga_crop(TGAImage *image, int x, int y, int width, int height); // Ořezání
void tga_move(TGAImage *image, int dx, int dy);        // Posunutí obrázku
void tga_to_bw(TGAImage *image);                       // Převod na černobílý obrázek
void tga_copy(TGAImage *image, int x1, int y1, int w, int h, int x2, int y2);


#endif // STRUCTURE_H
