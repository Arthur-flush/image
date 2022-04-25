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
        
        uint width, length, max;
        fseek(fp, -1, SEEK_CUR);

        if (fscanf(fp, "%u %u", &width, &length) != 2) {
            perror("cannot read width and length in file\n");
            exit(1);
        }
        
        if (fscanf(fp, "%u", &max) != 1) {
            perror("cannot read maximum value in file\n");
            exit(1);
        }

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
                if (fscanf(fp, "%u", &c) != 1) {
                    perror("cannot read value in file\n");
                    exit(1);
                }
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
        if (fscanf(fp, "%u %u", &width, &length) != 2) {
            perror("cannot read width and length in file\n");
            exit(1);
        }
        
        if (fscanf(fp, "%u", &max) != 1) {
            perror("cannot read maximum value in file\n");
            exit(1);
        }

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
                if (fscanf(fp, "%u %u %u", &R, &G, &B) != 3) {
                    perror("cannot read value of rgb pixel");
                    exit(1);
                }
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
        if (fscanf(fp, "%u %u", &width, &length) != 2) {
            perror("cannot read width and length in file\n");
            exit(1);
        }
        
        if (fscanf(fp, "%u", &max) != 1) {
            perror("cannot read maximum value in file\n");
            exit(1);
        }

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
            if (fread(pic->pixels[i], 1, width, fp) != width) {
                perror("cannot read pixels in file");
                exit(1);
            }
        }
    }
    if (specification == '6') {
        pic->type = 1;
        getc(fp);

        while (getc(fp) == '#') 
            while (getc(fp) != '\n');
        
        int width, length, max;
        fseek(fp, -1, SEEK_CUR);
        if (fscanf(fp, "%u %u", &width, &length) != 2) {
            perror("cannot read width and length in file\n");
            exit(1);
        }
        
        if (fscanf(fp, "%u", &max) != 1) {
            perror("cannot read maximum value in file\n");
            exit(1);
        }

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
            if (fread(pic->pixels_rgb[i], 3, width, fp) != width) {
                perror("cannot read pixels in file");
                exit(1);
            }
            
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
    if (pic->height - x >= 8) {
        for (int i = 0; i < 8; i++) {
            if (pic->width - y >= 8) {
                for (int j = 0; j < 8; j++) {
                    RGB pix = pic->pixels_rgb[x+i][y+j];
                    double Y = get_Y_component_from_RGB(pix);
                    block[i][j] = Y;
                }
            }
            else {
                int y_to_8 = 8 - (pic->width - y);
                for (int j = 0; j < y_to_8; j++) {
                    RGB pix = pic->pixels_rgb[x+i][y+j];
                    double Y = get_Y_component_from_RGB(pix);
                    block[i][j] = Y;
                }
                for (int j = 0; j < 8-y_to_8; j++) {
                    block[i][j] = 0;
                }
            }
        }
    } 
    else {
        int x_to_8 = 8 - (pic->width - x);
        for (int i = 0; i < x_to_8; i++) {
            if (pic->width - y >= 8) {
                for (int j = 0; j < 8; j++) {
                    RGB pix = pic->pixels_rgb[x+i][y+j];
                    double Y = get_Y_component_from_RGB(pix);
                    block[i][j] = Y;
                }
            }
            else {
                int y_to_8 = 8 - (pic->width - y);
                for (int j = 0; j < y_to_8; j++) {
                    RGB pix = pic->pixels_rgb[x+i][y+j];
                    double Y = get_Y_component_from_RGB(pix);
                    block[i][j] = Y;
                }
                for (int j = 0; j < 8-y_to_8; j++) {
                    block[i][j] = 0;
                }
            }
        }
        for (int i = 0; i < 8 - x_to_8; i++) {
            if (pic->width - y >= 8) {
                for (int j = 0; j < 8; j++) {
                    block[i][j] = 0;
                }
            }
            else {
                int y_to_8 = 8 - (pic->width - y);
                for (int j = 0; j < y_to_8; j++) {
                    block[i][j] = 0;
                }
                for (int j = 0; j < 8-y_to_8; j++) {
                    block[i][j] = 0;
                }
            }
        }
    }
}

double C(int z) {
    if (z == 0) { 
        return 0.70711; 
    }
    else
        return 1;
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
            data[i][j] = 0.25 * C(i) * C(j) * sum;
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
    int zeros = 0;
    for (int i = 0; i < 64; i++) {
        if (zeros < 2) {
            if (zigzag[i] != 0) {
                if (zeros == 1) {
                    fprintf(f, "0\n");
                    zeros = 0;
                }
                fprintf(f, "%d\n", zigzag[i]);
            }
            else {
                zeros++;
            }
        }
        else {
            int k = 0;
            while (zigzag[i + k - 2] == 0) 
                k++;
            fprintf(f, "@%d\n", k);
            i += k-3;
            zeros = 0;
        }
    }
}

void printblock(double block[8][8]) {

    for (int i = 0; i < 8; i++) {
        printf("[");
        for (int j = 0; j < 7; j++) {
            printf("%lf, ", block[i][j]);
        }
        printf("%lf]\n", block[i][7]);
    }
    printf("\n");
}

void jpeg_compression(picture *image, char *filename) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp ,"JPEG\n");
    if (image->width % 8 == 0) 
        fprintf(fp, "%d ", image->width);
    else 
        fprintf(fp, "%d ", ((image->width % 8) + 1) * 8);

    if (image->height % 8 == 0) 
        fprintf(fp, "%d\n", image->height);
    else 
        fprintf(fp, "%d\n", ((image->height % 8) + 1) * 8);
    
    double block[8][8] = {0};
    int data[64] = {0};
    for (int i = 0; i < image->height; i+=8) {
        for (int j = 0; j < image->width; j+=8) {
            extract_block(image, block, i, j);
            DCT(block);
            quantify(block);
            zigzag_extraction(block, data);
            compress_RLE(fp, data);
        }
    }    
    fclose(fp);
}


uint file_size(char *filename) {
    FILE* fp = fopen(filename, "r");

    fseek(fp, 0L, SEEK_END);
    uint size = ftell(fp); // might cause problems if file size is bigger than 2^32

    fclose(fp);
    return size;
}

picture *ppm_to_pgm(picture *pic) {
    picture *new_pic = malloc(sizeof(picture));
    new_pic->height = pic->height;
    new_pic->width = pic->width;
    new_pic->type = 0;
    new_pic->value_max = pic->value_max;
    new_pic->pixels = malloc(sizeof(uchar*) * new_pic->height);
    for (int i = 0; i < pic->height; i++) {
        new_pic->pixels[i] = malloc(new_pic->width);
        for (int j = 0; j < pic->width; j++) {
            double v = get_Y_component_from_RGB(pic->pixels_rgb[i][j]);
            v = round(v);
            new_pic->pixels[i][j] = (uchar)v;
        }
    }

    return new_pic;
}

int main(int argc, char** argv) {
    char * filename;
    if (argc == 2) 
        filename = argv[1];
    else {
        perror("wrong argument ammount");
        exit(1);
    }

    char c;
    int period_pos = 0; // get position of last period in file name
    for (int i = 0; (c = filename[i]) != '\0'; i++) {
        if (filename[i] == '.')
        period_pos = i;
    }

    // check if extension is ppm
    if (!((filename[period_pos+1] == 'p') && (filename[period_pos+2] == 'p') && (filename[period_pos+3] =='m'))) {
        perror("file extension must be .ppm");
        exit(1);
    }

    char *jpeg_filename = malloc(0);
    jpeg_filename = malloc(period_pos+5);
    for (int i = 0; i < period_pos; i++) {
        jpeg_filename[i] = filename[i];   
    }
    
    jpeg_filename[period_pos] = '.';
    jpeg_filename[period_pos+1] = 'j';
    jpeg_filename[period_pos+2] = 'p';
    jpeg_filename[period_pos+3] = 'e';
    jpeg_filename[period_pos+4] = 'g';


    picture *pic = get_picture(filename);

    jpeg_compression(pic, jpeg_filename);

    picture* pgm_pic = ppm_to_pgm(pic);
    char *pgm_filename = malloc(period_pos+4);
    for (int i = 0; i < period_pos; i++) {
        pgm_filename[i] = filename[i];
    }
    pgm_filename[period_pos]   = '.';
    pgm_filename[period_pos+1] = 'p';
    pgm_filename[period_pos+2] = 'g';
    pgm_filename[period_pos+3] = 'm';
    write_picture(pgm_pic, pgm_filename, false);

    free_pic(pic);

    uint pgm_file_size = file_size(pgm_filename);
    uint jpeg_file_size = file_size(jpeg_filename);
    printf("le fichier %s fait %u octets\n", pgm_filename, pgm_file_size);
    printf("le fichier %s fait %u octets\n", jpeg_filename, jpeg_file_size);

    printf("le taux de compression de ce fichier est de %.2f\n", (float)jpeg_file_size/(float)pgm_file_size);
}