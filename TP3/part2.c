#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288419716

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum { false, true } bool;

double Q[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99},
};

typedef struct RGB_t {
    uchar R, G, B;
} RGB;

typedef struct picture_t
{
    uchar type; /* Type de l’image (0 ou 1) */
    uint height, width;
    uchar value_max; /* Valeur max utilisee pour representer les pixels */
    union
    {
        uchar **pixels; // un pointeur vers un tableau 2D d’unsigned char (pour les pgm)
        RGB **pixels_rgb; // un pointeur vers un tableau 2D de pixels_rgb (pour les ppm)
    };
} picture;

picture *get_picture(char *file_name) {
    picture *pic = malloc(sizeof(picture));

    FILE *fp = fopen(file_name, "r");
    getc(fp);
    char specification = getc(fp);
    if (specification == '2') {
        pic->type = 0;
        getc(fp);

        while (getc(fp) == '#') 
            while (getc(fp) != '\n');
        
        int width, length, max;
        fseek(fp, -1, SEEK_CUR);
        fscanf(fp, "%d %d", &width, &length);
        
        fscanf(fp, "%d", &max);

        pic->height = length;
        pic->width = width;
        pic->value_max = max;
        

        pic->pixels = malloc(sizeof(uchar*) * length);
        if (pic->pixels == NULL) {
            perror("error while malloc pic->pixels");
            exit(1);
        }
        
        for (int i = 0; i < length; i++) {
            pic->pixels[i] = malloc(sizeof(uchar)*width);
            if (pic->pixels[i] == NULL) {
                perror("error while malloc pic->pixels[i]");
                exit(1);
            }


            for (int j = 0; j < width; j++) {
                uint c;
                fscanf(fp, "%u", &c);
                pic->pixels[i][j] = (uchar)c;
            }
        }
        
    }
    if (specification == '3') {
        pic->type = 1;
        char c = getc(fp);

        while ((c = getc(fp)) == '#') 
            while ((c = getc(fp)) != '\n');
        
        uint width, length, max;
        fseek(fp, -1, SEEK_CUR);
        fscanf(fp, "%u %u", &width, &length);
        
        fscanf(fp, "%u", &max);

        pic->height = length;
        pic->width = width;
        pic->value_max = max;
        

        pic->pixels = malloc(sizeof(uchar*) * length);
        if (pic->pixels == NULL) {
            perror("error while malloc pic->pixels");
            exit(1);
        }
        
        for (int i = 0; i < length; i++) {
            pic->pixels[i] = malloc(sizeof(RGB)*width);
            if (pic->pixels[i] == NULL) {
                perror("error while malloc pic->pixels[i]");
                exit(1);
            }

            
            for (int j = 0; j < width; j++) {
                uint R, G, B;
                fscanf(fp, "%u %u %u", &R, &G, &B);
                RGB pix;
                pix.R = (uchar)R;
                pix.G = (uchar)G; 
                pix.B = (uchar)B;
                pic->pixels_rgb[i][j] = pix;
            }
        }


    }
    if (specification == '5') {
        pic->type = 0;
        getc(fp);

        while (getc(fp) == '#') 
            while (getc(fp) != '\n');
        
        int width, length, max;
        fseek(fp, -1, SEEK_CUR);
        fscanf(fp, "%d %d", &width, &length);
        
        fscanf(fp, "%d", &max);

        pic->height = length;
        pic->width = width;
        pic->value_max = max;
        

        pic->pixels = malloc(sizeof(uchar*) * length);
        if (pic->pixels == NULL) {
            perror("error while malloc pic->pixels");
            exit(1);
        }
        fgetc(fp); // newline
        for (int i = 0; i < length; i++) {
            pic->pixels[i] = malloc(sizeof(uchar)*width);
            if (pic->pixels[i] == NULL) {
                perror("error while malloc pic->pixels[i]");
                exit(1);
            }
        fread(pic->pixels[i], 1, width, fp);
        }
    }
    if (specification == '6') {
        pic->type = 1;
        getc(fp);

        while (getc(fp) == '#') 
            while (getc(fp) != '\n');
        
        int width, length, max;
        fseek(fp, -1, SEEK_CUR);
        fscanf(fp, "%d %d", &width, &length);
        
        fscanf(fp, "%d", &max);

        pic->height = length;
        pic->width = width;
        pic->value_max = max;
        

        pic->pixels = malloc(sizeof(uchar*) * length);
        if (pic->pixels == NULL) {
            perror("error while malloc pic->pixels");
            exit(1);
        }
        fgetc(fp); // newline
        for (int i = 0; i < length; i++) {
            pic->pixels[i] = malloc(sizeof(RGB)*width * length);
            if (pic->pixels[i] == NULL) {
                perror("error while malloc pic->pixels[i]");
                exit(1);
            }
            fread(pic->pixels_rgb[i], 3, width, fp);
        }
    }
    fclose(fp);
    return pic;
}

void free_pic(picture *pic) {
    if (pic->type) {
        for (int i = 0; i < pic->height; i++)
            free(pic->pixels_rgb[i]);
        free(pic->pixels_rgb);
        free(pic);
    }
    else {
        for (int i = 0; i < pic->height; i++)
            free(pic->pixels[i]);
        free(pic->pixels);
        free(pic);
    }
}

void write_picture(picture *image, char *filename, bool binary) {
    FILE *fp = fopen(filename, "w");
    if (image->type == 0) {
        if (binary)
            fprintf(fp, "P5\n");
        else 
            fprintf(fp, "P2\n");
        
        fprintf(fp, "%d %d\n", image->width, image->height);
        fprintf(fp, "%d\n", image->value_max);
        if (binary) {
            for (int i = 0; i < image->height; i++) {
                fwrite(image->pixels[i], 1, image->width, fp);
            }
        }
        else {
            for (int i = 0; i < image->height; i++) {
                for (int j = 0; j < image->width; j++) {
                    fprintf(fp, "%u\n", image->pixels[i][j]);
                }
            }
        }
    }
    else {
        if (binary)
            fprintf(fp, "P6\n");
        else 
            fprintf(fp, "P3\n");
        
        fprintf(fp, "%d %d\n", image->width, image->height);
        fprintf(fp, "%d\n", image->value_max);
        if (binary) {
            for (int i = 0; i < image->height; i++) {
                fwrite(image->pixels_rgb[i], 3, image->width, fp);
            }
        }
        else {
            for (int i = 0; i < image->height; i++) {
                for (int j = 0; j < image->width; j++) {
                    RGB pix = image->pixels_rgb[i][j];
                    fprintf(fp, "%u %u %u\n", pix.R, pix.G, pix.B);
                }
            }
        }
    }


    fclose(fp);
}

double get_Y_component_from_RGB(RGB pixel) { 

    double Y = .299 * pixel.R + .587 * pixel.G + .114 * pixel.B;

    return Y;
}

void extract_block(picture *pic, double block[8][8], int x, int y) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            RGB pix = pic->pixels_rgb[x+i][y+j];
            double Y = get_Y_component_from_RGB(pix);
            block[i][j] = Y;
        }
    }
}

double C(int z) {
    if (z > 0) { // what ??
        return 0.353553390593; 
    }
    else
        return 0.25;
}

void DCT(double block[8][8]) {
    double data[8][8];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            double sum = 0;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    float dct1 = block[x][y] *
                           cos((2 * x + 1) * i * M_PI / (2 * 8)) *
                           cos((2 * y + 1) * j * M_PI / (2 * 8));
                    sum += dct1;
                }
            }
            data[i][j] = 2 * C(i) * C(j) * sum;
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            block[i][j] = data[i][j];
        }
    }
}

void quantify(double block[8][8]) {
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j++) {
            block[i][j] /= Q[i][j];
        }
    }
}
void zigzag_extraction(double block[8][8], int zigzag[64]) {
    int x = 0;
    int y = 1;
    int i = 1;
    zigzag[0] = round(block[0][0]);
    while ((x != 8) && (y != 8)) {
        while (1) {
            zigzag[i] = round(block[x][y]);
            i++;
            if (x + 1 >= 8) {
                y++;
                break;
            }
            if (y - 1 < 0) {
                x++;
                break;
            }
            x++;
            y--;
        }
        while(1) {
            zigzag[i] = round(block[x][y]);
            i++;
            if (x - 1 < 0) {
                y++;
                break;
            }
            if (y + 1 >= 8) {
                x++;
                break;
            }
            x--;
            y++;
        }
    }
}

void compress_RLE(FILE *f, int zigzag[64]) {
    for (int i = 0; i < 64; i++) {
        if (zigzag[i] != 0)
            fprintf(f, "%d\n", zigzag[i]);
        else {
            int k = 0;
            while (zigzag[i + k] == 0) 
                k++;
            fprintf(f, "@%d\n", k);
            i += k-1;
        }
    }
}

void jpeg_compression(picture *image, char *filename) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp ,"JPEG\n");
    fprintf(fp, "%d %d", image->width, image->height);

    

    fclose(fp);
}

int main(int argc, char** argv) {
    char * filename;
    if (argc == 2) 
        filename = argv[1];
    else {
        // perror("wrong argument ammount");
        // exit(1);
        filename = "exemple.ppm";
    }

    picture *pic = get_picture(filename);

    double pixels[8][8] = {
        {139, 144, 149, 153, 155, 155, 155, 155},
        {144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156},
        {159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155},
        {161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157},
        {162, 162, 161, 161, 163, 158, 158, 158}
    };

    DCT(pixels);
    quantify(pixels);
    int zigzag[64] = {0};
    zigzag_extraction(pixels, zigzag);

    FILE *fp = fopen("test.txt", "w");

    compress_RLE(fp, zigzag);

    fclose(fp);

    printf("{\n");
    for (int i = 0; i < 8; i++) {
        printf("{");
        for (int j = 0; j < 7; j++) {
            printf("%.0lf, ", round(pixels[i][j]));
        }
        printf("%.0lf}\n", round(pixels[i][7]));
    }
    printf("}\n");

    for (int i = 0; i < 64; i++) {
        printf("%d ", zigzag[i]);
    }
    printf("\n");

    free_pic(pic);
}