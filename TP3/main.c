#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum { false, true } bool;



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

double get_Y_component_from_RGB(RGB pixel, uint max) { 

    double Y = .299 * pixel.R + .587 * pixel.G + .114 * pixel.B;

    return Y;
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
            double v = get_Y_component_from_RGB(pic->pixels_rgb[i][j], pic->value_max);
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


    picture *pic = get_picture(filename);
    picture *new_pic = ppm_to_pgm(pic);

    char *new_filename = malloc(0);
    int new_filename_length = 0;
    for (int i = 0; i < period_pos; i++) {
        new_filename = realloc(new_filename, ++new_filename_length);
        new_filename[new_filename_length-1] = filename[i];   
    }
    
    new_filename = realloc(new_filename, new_filename_length+4);
    new_filename[new_filename_length] = '.';
    new_filename[new_filename_length+1] = 'p';
    new_filename[new_filename_length+2] = 'g';
    new_filename[new_filename_length+3] = 'm';

    write_picture(new_pic, new_filename, false);

    free_pic(pic);
}