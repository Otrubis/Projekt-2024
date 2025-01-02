#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structure.h"

#define COMMAND_BUFFER_SIZE 256

void print_help() {
    printf("Supported commands:\n");
    printf("  resize w,h        - Resize the image to the specified width and height.\n");
    printf("  crop x,y,w,h      - Crop the image to a rectangular region.\n");
    printf("  move x,y          - Move the image by x pixels horizontally and y pixels vertically.\n");
    printf("  copy x1,y1,w,h x2,y2 - Copy a rectangular region from one location to another.\n");
    printf("  bw                - Convert the image to grayscale.\n");
    printf("  info              - Print image dimensions and details.\n");
    printf("  save              - Save the current image to the output file.\n");
    printf("  exit              - Save the image and exit the program.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: %s --input <input_file> --output <output_file>\n", argv[0]);
        return 1;
    }

    char *input_path = NULL;
    char *output_path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            input_path = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_path = argv[++i];}}

    if (!input_path || !output_path) {
        printf("Error: Both --input and --output arguments are required.\n");
        return 1;}

    TGAImage image;


    if (!tga_load(input_path, &image)) {
        printf("Error: Could not load the input file '%s'.\n", input_path);
        return 1;}

    printf("Image loaded successfully.\n");
    tga_print_info(&image);
    

    char command[COMMAND_BUFFER_SIZE];
    while (1) {
        printf("\n> Type a command ('help' for a list of commands): ");
        if (!fgets(command, sizeof(command), stdin)) {
            printf("\nEOF detected. Exiting.\n");
            break;}


        command[strcspn(command, "\n")] = 0;


        if (strncmp(command, "resize", 6) == 0) {
            int w, h;
            if (sscanf(command, "resize %d,%d", &w, &h) == 2) {
                tga_resize(&image, w, h);
                printf("Image resized to %dx%d.\n", w, h);
            } else {
                printf("Invalid parameters for resize. Usage: resize w,h\n");
            }
        } else if (strncmp(command, "crop", 4) == 0) {
            int x, y, w, h;
            if (sscanf(command, "crop %d,%d,%d,%d", &x, &y, &w, &h) == 4) {
                tga_crop(&image, x, y, w, h);
                printf("Image cropped to region (%d,%d,%d,%d).\n", x, y, w, h);
            } else {
                printf("Invalid parameters for crop. Usage: crop x,y,w,h\n");
            }
        } else if (strncmp(command, "move", 4) == 0) {
            int dx, dy;
            if (sscanf(command, "move %d,%d", &dx, &dy) == 2) {
                tga_move(&image, dx, dy);
                printf("Image moved by (%d,%d).\n", dx, dy);
            } else {
                printf("Invalid parameters for move. Usage: move x,y\n");
            }
        } else if (strncmp(command, "copy", 4) == 0) {
            int x1, y1, w, h, x2, y2;
            if (sscanf(command, "copy %d,%d,%d,%d %d,%d", &x1, &y1, &w, &h, &x2, &y2) == 6) {
                tga_copy(&image, x1, y1, w, h, x2, y2);
                printf("Region (%d,%d,%d,%d) copied to (%d,%d).\n", x1, y1, w, h, x2, y2);
            } else {
                printf("Invalid parameters for copy. Usage: copy x1,y1,w,h x2,y2\n");
            }
        } else if (strcmp(command, "bw") == 0) {
            tga_to_bw(&image);
            printf("Image converted to grayscale.\n");
        } else if (strcmp(command, "info") == 0) {
            tga_print_info(&image);
        } else if (strcmp(command, "save") == 0) {
            if (!tga_save(output_path, &image)) {
                printf("Error: Could not save the file to '%s'.\n", output_path);
            } else {
                printf("Image saved to '%s'.\n", output_path);
            }
        } else if (strcmp(command, "exit") == 0) {
            if (!tga_save(output_path, &image)) {
                printf("Error: Could not save the file to '%s'.\n", output_path);
            } else {
                printf("Image saved to '%s'. Exiting.\n", output_path);
            }
            break;
        } else if (strcmp(command, "help") == 0) {
            print_help();
        } else {
            printf("Unknown command: %s. Type 'help' for a list of commands.\n", command);}}


    tga_free(&image);
    return 0;}
