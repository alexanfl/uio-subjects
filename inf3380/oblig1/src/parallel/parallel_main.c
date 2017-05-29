#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#define TAG 1

/*!
 * Program:    parallel_main.c
 * ---------------------------
 * Takes in a JPEG image 'input_jpeg_filename', turns it into a matrix u, 
 * and denoises it into matrix v, using the so-called isotropic diffusion.
 *
 *
 * This denoising process is done a number of iterations, given by 'iters',
 * and gives a JPEG image 'output_jpeg_filename'.
 *
 */

void import_JPEG_file(const char* filename, unsigned char** image_chars, 
        int* image_height, int* image_width, 
        int* num_components);

void export_JPEG_file(const char* filename, unsigned char* image_chars, 
        int image_height, int image_width, 
        int num_components, int quality);

typedef struct{
    /*!
     * struct:  image
     * --------------
     * 2D array representing images.
     * Has size m*n, which are attributes of 'image'.
     */

    float** image_data;
    int m, n;
} image;

void allocate_image(image *u, int m, int n){
    /*!
     * Function:    allocate_image
     * ---------------------------
     *  Allocates memory for an image u.
     *
     *  u: matrix of size.
     *  m, n: rows, columns of u.
     */

    u->image_data = (float**)malloc(m*sizeof(float*));
    u->m = m;
    u->n = n;

    int i;
    for (i = 0; i < m; i++) {
        (u->image_data)[i] = (float*)malloc(n*sizeof(float));
    }
}

void deallocate_image(image *u){
    /*!
     * Function:    deallocate_image
     * -----------------------------
     * Deallocates an image u.
     */

    int i;
    for (i = 0; i < u->m; i++) {
        free((u->image_data)[i]);
    }
    free(u->image_data);
}

void convert_jpeg_to_image(const unsigned char* image_chars, image *u){
    /*
     * Function:    convert_jpeg_to_image
     * ----------------------------------
     * Takes a 1D array and converts it into a 2D array representing an image.
     *
     * image_chars: 1D array storing JPEG image.
     * u: 2D array representing image.
     */

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
    /*
     * Function: convert_image_to_jpeg
     * -------------------------------
     * Converts 2D matrix to 1D array.
     *
     * u: 2D array.
     * image_chars: 1D array.
     */

    int cnt = 0;
    int i, j;
    for (i = 0; i < (u->m); i++) {
        for (j = 0; j < (u->n); j++) {
            image_chars[cnt] = (u->image_data)[i][j];
            cnt++;
        }
    }

}

void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa){
    /*
     * Function: iso_diffusion_denoising
     * ---------------------------------
     * Takes an image represented by the 2D array u, 
     * and denoises it into the 2D array v using:
     * v[i][j] = u[i][j]+K*(u[i-1][j]+u[i][j-1]-4*u[i][j]+u[i+1][j]+u[i][j+1])
     * for 1<=i<=m-2 and 1<=j<=n-2.
     * 
     * u: 2D image array.
     * u_bar: denoised image array. Called 'v' above.
     * kappa: Constant decimal number of iso diffusion. Called 'K' above.
     * iters: Number of times to perform the iso diffusion. 
     *
     * kappa = 0.1 gives good results.
     */

    int m = u->m;
    int n = u->n;

    int i, j, k;

    for (i = 1; i < m-2; i++) {
        for (j = 1; j < n-2; j++) {
            (u_bar->image_data)[i][j] = (u->image_data)[i][j] + 
                kappa*( (u->image_data)[i-1][j] + 
                        (u->image_data)[i][j-1] -
                        4*(u->image_data)[i][j] +
                        (u->image_data)[i+1][j] + 
                        (u->image_data)[i][j+1]);
        }
    }
    for (i = 1; i < m-2; i++) {
        for (j = 1; j < n-2; j++) {
            u->image_data[i][j] = u_bar->image_data[i][j];
        }
    }
}

int main (int argc, char *argv[]){
    if( argc == 5) {
        printf("Arguments supplied:\n\t Iterations = %d Kappa = %f Input: "
                "%s Output: %s\n", 
                atoi(argv[1]), atof(argv[2]), argv[3], argv[4]);
    }
    else if (argc > 5) {
        printf("Too many arguments supplied.\n");
    }
    else{
        printf("Four arguments expected:\n\t"
                "'number_of_iterations' 'kappa_value' 'infile' 'outfile'");
    } 

    int m, n, c, iters;
    int my_m, my_n, my_rank, num_procs;
    int average_m_per_proc, rest_m_per_proc, rows;
    float kappa;

    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename;

    iters = atoi(argv[1]);
    kappa = atof(argv[2]);
    input_jpeg_filename = argv[3];
    output_jpeg_filename = argv[4];

    // Parallel initiation
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (my_rank == 0) {
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        allocate_image(&whole_image, m, n);
    }

    MPI_Status status;

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


    // Divide m*n pixels among MPI processes
    my_m = m/num_procs;
    my_n = n;

    int num_workers = num_procs - 1;
    int end_m;

    if (my_rank == num_workers) {
        end_m = m - my_m*num_workers;
        my_m = end_m;
    }
    MPI_Bcast(&end_m, 1, MPI_INT, num_workers, MPI_COMM_WORLD);
    my_image_chars = malloc(my_m*my_n*sizeof(MPI_UNSIGNED_CHAR));

    if (my_rank == 0) {
        int i, j;
        for (i = 1; i < num_workers; i++) {
            MPI_Send(&image_chars[i*my_m*my_n], my_m*my_n, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
            j = i;

        }
        MPI_Send(&image_chars[j*my_m*my_n], end_m*my_n, MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);
        memcpy(my_image_chars, image_chars, my_m*my_n*sizeof(unsigned char));
    }
    if (my_rank != 0) {
        MPI_Recv(my_image_chars, my_m*my_n, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    // Halt until all processes have reached this part
    MPI_Barrier(MPI_COMM_WORLD);

    //Allocate memory for image matrices
    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_n, my_n);

    // Convert 1D array to 2D image array
    convert_jpeg_to_image(my_image_chars, &u);

    // Do ISO diffusion on image
    int i;
    for (i = 0; i < iters; i++) {
        iso_diffusion_denoising_parallel(&u, &u_bar, kappa);

        if (my_rank < num_workers) {
            MPI_Sendrecv(&u.image_data[my_m-2], 
                    my_n, MPI_FLOAT, my_rank + 1, 0, 
                    &u.image_data[my_m-1], 
                    my_n, MPI_FLOAT, my_rank + 1, MPI_ANY_TAG, 
                    MPI_COMM_WORLD, &status);
        }
        else if (my_rank > 0) {
            MPI_Sendrecv(&u.image_data[0], 
                    my_n, MPI_FLOAT, my_rank - 1, 0, 
                    &u.image_data[1], 
                    my_n, MPI_FLOAT, my_rank - 1, MPI_ANY_TAG,
                    MPI_COMM_WORLD, &status);
        }
    }
    // Convert 2D array back to 1D
    convert_image_to_jpeg(&whole_image, image_chars);

    if (my_rank != 0) {
        MPI_Send(my_image_chars, my_m*my_n, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        memcpy(image_chars, my_image_chars, my_m*my_n*sizeof(unsigned char));
        int i, j;
        for (i = 1; i < num_workers; i++) {
            MPI_Recv(&image_chars[i*my_m*my_n], my_m*my_n, MPI_UNSIGNED_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            j = i;
        }

       MPI_Recv(&image_chars[j*my_m*my_n], end_m*my_n, MPI_UNSIGNED_CHAR, j, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0) {
        // Export JPEG file
        export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);

        // Deallocate 2D image arrays
        deallocate_image(&whole_image);
    }
   

    // Deallocate 2D image arrays 
    deallocate_image(&u);
    deallocate_image(&u_bar);

    MPI_Finalize();
    return 0;
}
