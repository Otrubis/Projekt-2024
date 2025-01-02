#include "structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void tga_print_info(const TGAImage *image) {
    printf("Image Info:\n");
    printf("  Width: %d\n", image->width);
    printf("  Height: %d\n", image->height);
    printf("  Channels: %d\n", image->channels);
    printf("  Bits per pixel: %d\n", image->header.depth);
    printf("  Image type: %d\n", image->header.image_type);
}

int tga_bytes_to_int(const byte *bytes) {
    return bytes[0] | (bytes[1] << 8);}

void tga_int_to_bytes(int value, byte *bytes) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;}

bool tga_load(const char *filepath, TGAImage *image) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        return false; }

    if (fread(&image->header, sizeof(TGAHeader), 1, file) != 1) {
        fclose(file);
        return false; }

    image->width = tga_bytes_to_int(image->header.width);
    image->height = tga_bytes_to_int(image->header.height);
    image->channels = (image->header.depth == 24) ? 3 : 4;

    size_t data_size = image->width * image->height * image->channels;
    image->data = (byte *)malloc(data_size);
    if (!image->data || fread(image->data, 1, data_size, file) != data_size) {
        free(image->data);
        fclose(file);
        return false;}

    fclose(file);
    return true;}
void tga_copy(TGAImage *image, int x1, int y1, int w, int h, int x2, int y2) {
    if (x1 < 0 || y1 < 0 || x1 + w > image->width || y1 + h > image->height ||
        x2 < 0 || y2 < 0 || x2 + w > image->width || y2 + h > image->height) {
        printf("Error: Invalid dimensions for copy.\n");
        return;
    }


    size_t region_size = w * h * image->channels;
    byte *buffer = (byte *)malloc(region_size);
    if (!buffer) {
        printf("Error: Failed to allocate memory for copy.\n");
        return;
    }

 
    for (int ny = 0; ny < h; ny++) {
        for (int nx = 0; nx < w; nx++) {
            for (int c = 0; c < image->channels; c++) {
                buffer[(ny * w + nx) * image->channels + c] =
                    image->data[((y1 + ny) * image->width + (x1 + nx)) * image->channels + c];}}
                    }


    for (int ny = 0; ny < h; ny++) {
        for (int nx = 0; nx < w; nx++) {
            for (int c = 0; c < image->channels; c++) {
                image->data[((y2 + ny) * image->width + (x2 + nx)) * image->channels + c] =
                    buffer[(ny * w + nx) * image->channels + c];}}
    }

    free(buffer);}


bool tga_save(const char *filepath, const TGAImage *image) {
    FILE *file = fopen(filepath, "wb");
    if (!file) {
        return false;}

    if (fwrite(&image->header, sizeof(TGAHeader), 1, file) != 1) {
        fclose(file);
        return false;}

    size_t data_size = image->width * image->height * image->channels;
    if (fwrite(image->data, 1, data_size, file) != data_size) {
        fclose(file);
        return false;}

    fclose(file);
    return true;}


void tga_free(TGAImage *image) {
    if (image->data) {
        free(image->data);
        image->data = NULL;}}


void tga_to_bw(TGAImage *image) {
    for (int i = 0; i < image->width * image->height; i++) {
        int r = image->data[i * image->channels + 2];
        int g = image->data[i * image->channels + 1];
        int b = image->data[i * image->channels + 0];
        byte gray = (r + g + b) / 3;

        for (int c = 0; c < image->channels; c++) {
            image->data[i * image->channels + c] = gray;}}
            }


void tga_resize(TGAImage *image, int new_width, int new_height) {
    size_t new_size = new_width * new_height * image->channels;
    byte *new_data = (byte *)malloc(new_size);
    if (!new_data) {
        printf("Error: Failed to allocate memory for resize.\n");
        return;}

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int src_x = (x * image->width) / new_width;
            int src_y = (y * image->height) / new_height;

            for (int c = 0; c < image->channels; c++) {
                new_data[(y * new_width + x) * image->channels + c] =
                    image->data[(src_y * image->width + src_x) * image->channels + c];}}
    }

    free(image->data);
    image->data = new_data;
    image->width = new_width;
    image->height = new_height;

    tga_int_to_bytes(new_width, image->header.width);
    tga_int_to_bytes(new_height, image->header.height);}


void tga_crop(TGAImage *image, int x, int y, int width, int height) {
    if (x < 0 || y < 0 || x + width > image->width || y + height > image->height) {
        printf("Error: Invalid crop dimensions.\n");
        return;}

    size_t new_size = width * height * image->channels;
    byte *new_data = (byte *)malloc(new_size);
    if (!new_data) {
        printf("Error: Failed to allocate memory for crop.\n");
        return;
    }

    for (int ny = 0; ny < height; ny++) {
        for (int nx = 0; nx < width; nx++) {
            for (int c = 0; c < image->channels; c++) {
                new_data[(ny * width + nx) * image->channels + c] =
                    image->data[((y + ny) * image->width + (x + nx)) * image->channels + c];}}
    }

    free(image->data);
    image->data = new_data;
    image->width = width;
    image->height = height;

    tga_int_to_bytes(width, image->header.width);
    tga_int_to_bytes(height, image->header.height);}


void tga_move(TGAImage *image, int dx, int dy) {
    int width = image->width;
    int height = image->height;
    int channels = image->channels;
    byte *new_data = malloc(width * height * channels);
    if (!new_data) {
        printf("Error: Failed to allocate memory for move.\n");
        return;}

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int src_x = (x - dx + width) % width;
            int src_y = (y - dy + height) % height;

            for (int c = 0; c < channels; c++) {
                new_data[(y * width + x) * channels + c] =
                    image->data[(src_y * width + src_x) * channels + c];}}
    }

    free(image->data);
    image->data = new_data;}
