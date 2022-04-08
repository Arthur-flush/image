#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1

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

picture *Picture(uint width, uint height, uchar value_max, uchar type, bool initialize_as_zero) {
    picture *new_pic = malloc(sizeof(picture));
    new_pic->width = width;
    new_pic->height = height;
    new_pic->value_max = value_max;
    new_pic->type = type;
    new_pic->pixels = malloc(sizeof(uchar*) * height);
    if (new_pic->pixels == NULL) {
        perror("error while malloc new_pic->pixels");
        exit(1);
    }
    for (int i = 0; i < height; i++) {
        new_pic->pixels[i] = malloc(sizeof(uchar)*width);
        if (new_pic->pixels[i] == NULL) {
            perror("error while malloc new_pic->pixels[i]");
            exit(1);
        }
        if (initialize_as_zero) {
            for (int j = 0; j < width; j++) {
                new_pic->pixels[i][j] = 0;
            }
        }
    }

    return new_pic;
}

picture *naive_x(picture *pic) {
    picture *new_pic = Picture(pic->width, pic->height, pic->value_max, pic->type, false);
    for (int i = 1; i < new_pic->height-1; i++) {
        for (int j = 1; j < new_pic->width-1; j++) {
            new_pic->pixels[i][j] = abs(pic->pixels[i][j] - pic->pixels[i][j-1]);
        }
    }
    return new_pic;
}

picture *naive_y(picture *pic) {
    picture *new_pic = Picture(pic->width, pic->height, pic->value_max, pic->type, false);
    for (int i = 1; i < new_pic->height-1; i++) {
        for (int j = 1; j < new_pic->width-1; j++) {
            new_pic->pixels[i][j] = abs(pic->pixels[i][j] - pic->pixels[i-1][j]);
        }
    }
    return new_pic;
}

picture *naive_edge_detector(picture *pic) {
    picture *new_pic = Picture(pic->width, pic->height, pic->value_max, pic->type, false);
    picture *x_pic = naive_x(pic);
    picture *y_pic = naive_y(pic);

    for (int i = 1; i < new_pic->height-1; i++) {
        for (int j = 1; j < new_pic->width-1; j++) {
            new_pic->pixels[i][j] = sqrt(x_pic->pixels[i][j] * x_pic->pixels[i][j] + y_pic->pixels[i][j] * y_pic->pixels[i][j]);
        }
    }

    free_pic(x_pic);
    free_pic(y_pic);
    return new_pic;
}


picture *sobel_edge_detector(picture *pic) {
    const int c = 2;

    picture *new_img = Picture(pic->width, pic->height, pic->value_max, pic->type, false);
    //for (int i = 1; i < new_img->height-1; i++) {
    //    for (int j = 1; j < new_img->width-1; j++) {
    //        new_img->pixels[i][j-1] = 1/(2+c) * (pic->pixels[i-1][j-1] + c*pic->pixels[i][j-1] + pic->pixels[i+1][j-1]);
    //        new_img->pixels[i-1][j] = 1/(2+c) * (pic->pixels[i-1][j-1] + c*pic->pixels[i-1][j] + pic->pixels[i-1][j+1]);
    //    }
    //}
    const int m1[3] = { 1, c, 1};

    const int m2[3] = {-1, 0, 1};


    for (int i = 1; i < pic->height-1; i++) {
        for (int j = 1; j < pic->width-1; j++) {
            int valx = 0;

            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    valx += m1[1-k] * m2[1-l] * pic->pixels[i+k][j+l];
                }
            }
            
            valx = abs((int)round((float) valx * 0.25));

            // clamping time
            //if (valx > new_img->value_max) {
            //    valx = new_img->value_max;
            //}

            int valy = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    valy += m2[1-k] * m1[1-l] * pic->pixels[i+k][j+l];
                }
            }

            valy = abs((int)round((float) valy * 0.25));

            // clamping time
            //if (valy > new_img->value_max) {
            //    valy = new_img->value_max;
            //}

            new_img->pixels[i][j] = (uchar)round(sqrt(valx * valx + valy * valy));
        }
    }
    return new_img;
}

float **gradiant_angle(picture *grad_x, picture *grad_y) {
    float **angles = malloc(sizeof(float*) * grad_x->height);
    if (angles == NULL) {
        perror("error while malloc angles");
        exit(1);
    }
    for (int i = 0; i < grad_x->height; i++) {
        angles[i] = malloc(sizeof(float) * grad_x->width);
        if (angles[i] == NULL) {
            perror("error while malloc angles[i]");
            exit(1);
        }
    }

    for (int i = 0; i < grad_x->height; i++) {
        for (int j = 0; j < grad_x->width; j++) {
            angles[i][j] = atan2(grad_y->pixels[i][j], grad_x->pixels[i][j]);
        }
    }

    return angles;
}

uchar interpolation(float x, float y, picture *pic) {
    int x_floor = (int)floor(x);
    int y_floor = (int)floor(y);
    int x_ceil = (int)ceil(x);
    int y_ceil = (int)ceil(y);

    float x_diff = x - x_floor;
    float y_diff = y - y_floor;

    float val = (1-x_diff) * (1-y_diff) * pic->pixels[y_floor][x_floor] +
                x_diff * (1-y_diff) * pic->pixels[y_floor][x_ceil] +
                (1-x_diff) * y_diff * pic->pixels[y_ceil][x_floor] +
                x_diff * y_diff * pic->pixels[y_ceil][x_ceil];

    return (uchar)round(val);
}

void non_maxima_suppression(picture *norm, float **angle) {
    for (int i = 1; i < norm->height-1; i++) {
        for (int j = 1; j < norm->width-1; j++) {
            float M1 = cos(angle[i][j]);
            float M2 = sin(angle[i][j]);

            uchar val = interpolation(j+M1, i+M2, norm);

            if (val > norm->pixels[i][j]) {
                norm->pixels[i][j] = 0;
            }
        }
    }
}


// function that applies a 5 x 5 gaussian filter to a picture
picture *gaussian_filter(picture *pic) {
    picture *new_pic = Picture(pic->width, pic->height, pic->value_max, pic->type, false);
    const int m[5][5] = {
        {1, 4, 6, 4, 1},
        {4, 16, 24, 16, 4},
        {6, 24, 36, 24, 6},
        {4, 16, 24, 16, 4},
        {1, 4, 6, 4, 1}
    };

    for (int i = 2; i < pic->height-2; i++) {
        for (int j = 2; j < pic->width-2; j++) {
            int val = 0;
            for (int k = -2; k <= 2; k++) {
                for (int l = -2; l <= 2; l++) {
                    val += m[k+2][l+2] * pic->pixels[i+k][j+l];
                }
            }
            new_pic->pixels[i][j] = (uchar)round((float)val * 0.00626);
        }
    }
    return new_pic;
}


int main() {
    picture *img = get_picture("Monarch.pgm");
    picture *new_pic = gaussian_filter(img);
    write_picture(new_pic, "Monarch_blured.pgm", false);
    free(img);
    img = new_pic;
    float** angles = gradiant_angle(naive_x(img), naive_y(img));
    non_maxima_suppression(img, angles);
    write_picture(img, "Monarch_non_maxima.pgm", false);
    free(angles);
    free_pic(img);
}