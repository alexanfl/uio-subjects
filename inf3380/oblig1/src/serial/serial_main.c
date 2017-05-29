#include <stdio.h>
#include <stdlib.h>

typedef struct{
    float** image_data;
    int m;
    int n;
} image;

void import_JPEG_file(const char* filename, unsigned char** image_chars, 
        int* image_height, int* image_width, 
        int* num_components);

void export_JPEG_file(const char* filename, unsigned char* image_chars, 
        int image_height, int image_width, 
        int num_components, int quality);


void allocate_image(image *u, int m, int n){
    u->image_data = (float**)malloc(m*sizeof(float*));
    u->m = m;
    u->n = n;

    int i;
    for (i = 0; i < m; i++) {
        (u->image_data)[i] = (float*)malloc(n*sizeof(float));
    }
}

void deallocate_image(image *u){
    int i;
    for (i = 0; i < u->m; i++) {
        free((u->image_data)[i]);
    }
    free(u->image_data);
}

void convert_jpeg_to_image(const unsigned char* image_chars, image *u){
    int cnt = 0;
    int i, j;
    for (i = 0; i < (u->m); i++) {
        for (j = 0; j < (u->n); j++) {
            (u->image_data)[i][j] = image_chars[cnt];
            cnt++;
        }
    }
}

void convert_image_to_jpeg(const image *u, unsigned char* image_chars){
    int cnt = 0;
    int i, j;
    for (i = 0; i < (u->m); i++) {
        for (j = 0; j < (u->n); j++) {
            image_chars[cnt] = (u->image_data)[i][j];
            cnt++;
        }
    }

}

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters){
    int i, j, k;

    for (k = 0; k < iters; k++) {
        for (i = 1; i < (u->m)-2; i++) {
            for (j = 1; j < u->n-2; j++) {
                (u_bar->image_data)[i][j] = (u->image_data)[i][j] + 
                    kappa*( (u->image_data)[i-1][j] + 
                            (u->image_data)[i][j-1] -
                            4*(u->image_data)[i][j] +
                            (u->image_data)[i+1][j] + 
                            (u->image_data)[i][j+1]);
            }
        }
    }
}

int main (int argc, char *argv){
    // Create matrix dimensions, c value and number of iterations
    int m, n, c, iters;

    // Set kappa value for image smoothening
    float kappa;

    // Create 2D image array
    image u, u_bar;
    
    // Create image chars
    unsigned char *image_chars;
    
    // Create input and output arrays
    char *input_jpeg_filename, *output_jpeg_filename;

    // Initiate pointers for char arrays
    input_jpeg_filename = malloc(255);
    output_jpeg_filename = malloc(255);

    // Retrieve values from command line
    printf("\nInput kappa value: ");
    scanf("%f", &kappa);
    printf("\nInput number of iterations value: ");
    scanf("%d", &iters);
    printf("\n../path/to/input_file: ");
    scanf("%s", input_jpeg_filename);
    printf("\n../path/to/output_file: ");
    scanf("%s", output_jpeg_filename);

    // Import JPEG file
    import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);

    //Allocate memory for image matrices
    allocate_image(&u, m, n);
    allocate_image(&u_bar, m, n);

    // Convert 1D array to 2D image array
    convert_jpeg_to_image(image_chars, &u);

    // Do ISO diffusion on image
    iso_diffusion_denoising(&u, &u_bar, kappa, iters);

    // Convert 2D array back to 1D
    convert_image_to_jpeg(&u_bar, image_chars);
    
    // Export JPEG file
    export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
   
    // Deallocate 2D image arrays 
    deallocate_image(&u);
    deallocate_image(&u_bar);

    return 0;
}
