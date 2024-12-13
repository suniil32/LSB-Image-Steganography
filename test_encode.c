/***************************************************************************
 * Name: Sunil kumar
 * Date: 2024-06-12
 * Project: LSB Image Stegenography
 * Description:Steganography is the practice of hiding private or sensitive information
     within something that appears to be nothing out to the usual.


 *Input: ./a.out -e beautiful.bmp secret.txt
 *Output:--------------------------------------------------------------
 <---------------Selected Encoding------------>
Read and valiate argument is successful
Opened all required file successfully
width = 1024
height = 768
Possible to encode the secret data in image
copied header successfully
MAGIC STRING encoded successfully
Encoded secret file extention size successfully
Encoded secret file extension successfully
Encoded secret file size successfully
Encoded secret data successfully
Copied remaining bytes successfully
<------------Completed Encoding------------>

Input: ./a.out -d stego.bmp
Output:---------------------------------------------
<---------------Selected Decoding--------------->
Read and Validate argument is successfully
Opened all required file successfully
Decoded Magic string : #*
Decode magic string successfully
Secret file extension size : 4
Decode Secret file extension size successfully
Decoded Secret file extension : .txt
Decoded secret file extension successfully
Decoded Secret file size : 25
Decoded secret file size Successfully
Decoded secret file data Succuessfully
<---------------Completed Decoding--------------->

 ***********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

void print_usage();

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    if (check_operation_type(argv) == e_encode)
    {
        if (argc < 4)
        {
            print_usage();
            return 1;
        }

        EncodeInfo encInfo;
        printf("<---------------Selected Encoding------------>\n");
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and valiate argument is successful\n");
            if (do_encoding(&encInfo) == e_success)
            {
                printf("<------------Completed Encoding------------>\n");
            }
            else
            {
                printf("Faile to Encode The Secret data\n");
            }
        }
        else
        {
            printf("Failed to validate the arguments\n");
        }
    }

    else if (check_operation_type(argv) == e_decode)
    {
        if (argc < 3)
        {
            print_usage();
            return 1;
        }

        DecodeInfo decInfo;
        printf("<---------------Selected Decoding--------------->\n");
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("Read and Validate argument is successfully\n");
            if (do_decoding(&decInfo) == e_success)
            {
                printf("<---------------Completed Decoding--------------->\n");
            }
            else
            {
                printf("Faile to decode The stego data\n");
            }
        }
        else
        {
            printf("Failed to validate the arguments\n");
        }
    }
    else
    {
        print_usage();
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

void print_usage()
{
    printf("Invalid Option\nUSAGE\n");
    printf("Encoding : ./a.out -e beautiful.bmp secret.txt\n");
    printf("Decoding : ./a.out -d stego.bmp\n");
}