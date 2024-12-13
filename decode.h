#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char image_data[MAX_IMAGE_BUF_SIZE];
    char *magic_data;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char *d_extn_secret_file;

    int size_secret_file;
    FILE *fptr_d_dest_image;

    char *secret_fname;
    FILE *fptr_secret;
} DecodeInfo;

/* Decoding Function Prototypes */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the deoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_defiles(DecodeInfo *decInfo);

/* check capacity */
Status check_decodecapacity(DecodeInfo *decInfo);

/* Decode data from image */
Status decode_extension_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo);

/*Decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo);

/*decode byte from lsb*/
Status decode_byte_from_lsb(char *data, char *image_buffer);

/*decode secret file extension size*/
Status decode_file_extn_size(int size, FILE *fptr_stego_image);

/*Decode size from lsb*/
Status decode_size_from_lsb(char *buffer, int *size);

/*Decode secret file size*/
Status decode_secret_file_size(int file_size, DecodeInfo *decInfo);

/*Decode Secret file Extension*/
Status decode_secret_file_extn(char *file_ext, DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/*Decode Data from image*/
Status decode_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo);

#endif
