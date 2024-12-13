#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"
#include "types.h"

/* Function Definitions */

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_decfiles(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }

    // Secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    // Do Error handling
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);

        return e_failure;
    }

    return e_success;
}

/*Read and validate decode arguments*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /*validate .bmp file*/
    if (argv[2] != NULL && strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    // output image file creation
    if (argv[3] != NULL)
    {
        decInfo->secret_fname = argv[3];
    }
    else
    {
        decInfo->secret_fname = "decode.txt";
    }

    return e_success;
}
/*Decode byte from lsb*/
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    int bit = 7;
    unsigned char ch = 0x00;
    for (int i = 0; i < 8; i++)
    {
        ch = ((image_buffer[i] & 0x01) << bit--) | ch;
    }
    *data = ch;
    return e_success;
}

// Function definition for decoding data fom image
Status decode_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    int i;
    char str[8];
    for (i = 0; i < size; i++)
    {
        fread(str, 8, sizeof(char), fptr_stego_image);
        decode_byte_from_lsb(&decInfo->magic_data[i], str);
    }
    return e_success;
}
/*decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    int i = strlen(MAGIC_STRING);
    decInfo->magic_data = malloc(strlen(MAGIC_STRING) + 1);
    decode_data_from_image(strlen(MAGIC_STRING), decInfo->fptr_stego_image, decInfo);
    decInfo->magic_data[i] = '\0';
    if (strcmp(decInfo->magic_data, MAGIC_STRING) == 0)
    {
        printf("Decoded Magic string : %s\n", decInfo->magic_data);
        return e_success;
    }

    else
    {
        return e_failure;
    }
        
}
/*decode size from lsb*/
Status decode_size_from_lsb(char *buffer, int *size)
{
    if (buffer == NULL || size == NULL)
    {
        return e_failure; // Handle null pointers
    }

    int j = 31;
    int num = 0x00;
    for (int i = 0; i < 32; i++)
    {
        num = ((buffer[i] & 0x01) << j--) | num;
    }
    *size = num;
    return e_success;
}
/*decode file extension size*/
Status decode_file_extn_size(int size, FILE *fptr_stego_image)
{
    char str[32];
    int length;
    fread(str, 32, sizeof(char), fptr_stego_image);
    decode_size_from_lsb(str, &length);
    if (length == size)
    {
        printf("Secret file extension size : %d\n", length);
        return e_success;
    }

    else
    {
        return e_failure;
    }
        
}

// Function definition decode extension data from image
Status decode_extension_data_from_image(int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        fread(decInfo->stego_image_fname, 8, 1, fptr_stego_image);
        decode_byte_from_lsb(&decInfo->d_extn_secret_file[i], decInfo->stego_image_fname);
    }
    return e_success;
}

/*function definition decoe secret file extension*/
Status decode_secret_file_extn(char *file_ext, DecodeInfo *decInfo)
{
    file_ext = ".txt";
    int i = strlen(file_ext);
    decInfo->d_extn_secret_file = malloc(i + 1);
    decode_extension_data_from_image(strlen(file_ext), decInfo->fptr_stego_image, decInfo);
    decInfo->d_extn_secret_file[i] = '\0';
    if (strcmp(decInfo->d_extn_secret_file, file_ext) == 0)
    {
        printf("Decoded Secret file extension : %s\n", decInfo->d_extn_secret_file);
        return e_success;
    }

    else
    {
        return e_failure;
    }
    
}

// Function definition for decode secret file size
Status decode_secret_file_size(int file_size, DecodeInfo *decInfo)
{
    char str[32];
    fread(str, 32, sizeof(char), decInfo->fptr_stego_image);
    decode_size_from_lsb(str, &file_size);
    decInfo->size_secret_file = file_size;
    printf("Decoded Secret file size : %d\n", decInfo->size_secret_file);
    return e_success;
}

/*function definition decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(decInfo->stego_image_fname, 8, sizeof(char), decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, decInfo->stego_image_fname);
        fputc(ch, decInfo->fptr_secret);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    /*all the remaining encode function should be called here*/
    /*open all the required files*/
    if (open_decfiles(decInfo) == e_success)
    {
        printf("Opened all required file successfully\n");
        if (decode_magic_string(decInfo) == e_success)
        {
            printf("Decode magic string successfully\n");
            if (decode_file_extn_size(strlen(".txt"), decInfo->fptr_stego_image) == e_success)
            {
                printf("Decode Secret file extension size successfully\n");
                if (decode_secret_file_extn(decInfo->d_extn_secret_file, decInfo) == e_success)
                {
                    printf("Decoded secret file extension successfully\n");
                    if (decode_secret_file_size(decInfo->size_secret_file, decInfo) == e_success)
                    {
                        printf("Decoded secret file size Successfully\n");
                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Decoded secret file data Succuessfully\n");
                        }
                        else
                        {
                            printf("Decoding of secret file data is a failure\n");
                        }
                    }
                    else
                    {
                        printf("Failed\n");
                    }
                }
                else
                {
                    printf("Failed to decode secret file extension\n");
                }
            }
            else
            {
                printf("Failed to decode secret file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Decode magic string failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open the file\n");
        return e_failure;
    }
    return e_success;
}