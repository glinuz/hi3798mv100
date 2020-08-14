/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_deal_with_texture_data.c
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

/*********************************add include here******************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "hi_texture.h"

/***************************** Macro Definition ******************************/

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_deal_with_texture_data.h *************/

struct TGA_HEADER
{
    unsigned char  IDLength;
    unsigned char  ColorMapType;
    unsigned char  ImageType;
    unsigned short FirstEntryIndex;
    unsigned short ColorMapLength;
    unsigned char  ColorMapEntrySize;
    unsigned short XOriginOfImage;
    unsigned short YOriginOfImage;
    unsigned short ImageWidth;
    unsigned short ImageHeight;
    unsigned char  PixelDepth;
    unsigned char  ImageDescriptor;
} __attribute__((packed));

void *hiLoadTGA(FILE *    File,
                GLenum *  Format,
                GLsizei * Width,
                GLsizei * Height)
{
    struct TGA_HEADER tga;
    size_t bytes;
    unsigned char * bits;

    /* Read the TGA file header. */
    if (fread(&tga, sizeof(tga), 1, File) != 1)
    {
        return NULL;
    }

    /* For now we only support uncompressed true-color images. */
    if (tga.ImageType != 2)
    {
        return NULL;
    }

    /* We only support top-left and bottom-left images. */
    if (tga.ImageDescriptor & 0x10)
    {
        return NULL;
    }

    /* Check pixel depth. */
    switch (tga.PixelDepth)
    {
    case 16:

        /* 16-bpp RGB. */
        *Format = GL_UNSIGNED_SHORT_5_6_5;
        break;

    case 24:

        /* 24-bpp RGB. */
        *Format = GL_RGB;
        break;

    case 32:

        /* 32-bpp RGB. */
        *Format = GL_RGBA;
        break;

    default:

        /* Invalid pixel depth. */
        return NULL;
    }

    /* Return texture dimension. */
    *Width  = tga.ImageWidth;
    *Height = tga.ImageHeight;

    /* Compute number of bytes. */
    bytes = tga.ImageWidth * tga.ImageHeight * tga.PixelDepth / 8;

    /* Skip ID field. */
    if (tga.IDLength)
    {
        fseek(File, tga.IDLength, SEEK_SET);
    }

    /* Allocate the bits. */
    bits = (unsigned char *) malloc(bytes);

    if (bits != NULL)
    {
        if (tga.ImageDescriptor & 0x20)
        {
            /* Read the bits from the TGA file. */
            if (fread(bits, 1, bytes, File) != bytes)
            {
                /* Error reading bits. */
                free(bits);
                bits = NULL;
            }
        }
        else
        {
            GLsizei y;
            GLsizei stride = bytes / tga.ImageHeight;

            /* Bottom up - copy line by line. */
            for (y = *Height - 1; y >= 0; --y)
            {
                if (fread(bits + y * stride, 1, stride, File) != (unsigned int)stride)
                {
                    /* Error reading bits. */
                    free(bits);
                    bits = NULL;
                    break;
                }
            }
        }

        if (bits != NULL)
        {
            size_t i;

            /* Convert all RGB pixels into GL pixels. */
            for (i = 0; i < bytes; i += tga.PixelDepth / 8)
            {
                unsigned char save;

                switch (tga.PixelDepth)
                {
                case 16:

                    /* Swap red and blue channel in 16-bpp. */
                    save = bits[i + 0] & 0x1F;
                    bits[i + 0] = (bits[i + 0] & ~0x1F) | (bits[1] >> 3);
                    bits[i + 1] = (bits[i + 1] & ~0xF8) | (save << 3);
                    break;

                case 24:
                case 32:

                    /* Swap red and blue channel in 24-bpp or 32-bpp. */
                    save = bits[i + 0];
                    bits[i + 0] = bits[i + 2];
                    bits[i + 2] = save;
                    break;
                }
            }
        }
    }

    /* Return the bits. */
    return bits;
}

struct PKM_HEADER
{
    unsigned char Magic[4];
    unsigned char Version[2];
    unsigned char Type[2];
    unsigned char Width[2];
    unsigned char Height[2];
    unsigned char ActiveWidth[2];
    unsigned char ActiveHeight[2];
} __attribute__((packed));

void *hiLoadPKM(FILE *    File,
                GLenum *  Format,
                GLsizei * Width,
                GLsizei * Height,
                GLsizei * Bytes)
{
    struct PKM_HEADER pkm;
    size_t bytes;
    unsigned char * bits;
    unsigned short type;
    unsigned short width, height;
    unsigned short activeWidth, activeHeight;

    /* Read the PKM file header. */
    if (fread(&pkm, sizeof(pkm), 1, File) != 1)
    {
        return NULL;
    }

    /* Validate magic data. */
    if ((pkm.Magic[0] != 'P')
        || (pkm.Magic[1] != 'K')
        || (pkm.Magic[2] != 'M')
        || (pkm.Magic[3] != ' ')
    )
    {
        return NULL;
    }

    /* Convert from big endian to numbers. */
    type   = (pkm.Type[0] << 8) | pkm.Type[1];
    width  = (pkm.Width[0] << 8) | pkm.Width[1];
    height = (pkm.Height[0] << 8) | pkm.Height[1];
    activeWidth  = (pkm.ActiveWidth[0] << 8) | pkm.ActiveWidth[1];
    activeHeight = (pkm.ActiveHeight[0] << 8) | pkm.ActiveHeight[1];

    /* For now we only support ETC1_RGB_NO_MIPMAPS. */
    if (type != 0)
    {
        return NULL;
    }

    /* ETC1 RGB texture format. */
    *Format = GL_ETC1_RGB8_OES;

    /* Return texture dimension. */
    *Width  = activeWidth;
    *Height = activeHeight;

    /* Compute number of bytes. */
    bytes = ((width + 3) / 4) * ((height + 3) / 4) * 8;

    *Bytes = bytes;

    /* Allocate the bits. */
    bits = (unsigned char *) malloc(bytes);

    if (bits != NULL)
    {
        /* Read the bits from the PKM file. */
        if (fread(bits, 1, bytes, File) != bytes)
        {
            /* Error reading bits. */
            free(bits);
            bits = NULL;
        }
    }

    /* Return the bits. */
    return bits;
}

/******************************************************************************
func                     : hiLoad2DTexture
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32 hiLoad2DTexture(const HI_CHAR *pFileName, HI_S32 s32DataType)
{
    GLuint texture = 0;
    FILE * f;

    /* Open the file. */
    f = fopen(pFileName, "rb");
    if (f == NULL)
    {
        /* Error opening the file. */
        return 0;
    }

    do
    {
        GLenum format, target = 0;
        void * bits;
        GLsizei width, height, bytes;

        /* Generate a texture. */
        glGenTextures(1, &texture);

        if (glGetError() != GL_NO_ERROR)
        {
            /* Error generating texture. */
            break;
        }

        glBindTexture(target = GL_TEXTURE_2D, texture);

        /* Test for any error. */
        if (glGetError() != GL_NO_ERROR)
        {
            glDeleteTextures(1, &texture);
            texture = 0;
            break;
        }

        /* Dispatch on texture type. */
        switch (s32DataType)
        {
        case HI_TGA:     /* TGA file. */
            bits = hiLoadTGA(f, &format, &width, &height);

            if (bits == NULL)
            {
                glDeleteTextures(1, &texture);
                texture = 0;
                break;
            }

            /* Set unpack alignment. */
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            /* Load texture. */
            glTexImage2D(target,
                         0,
                         format,
                         width,
                         height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         bits);

            /* Generate mipmaps. */

            //glGenerateMipmapOES(GL_TEXTURE_2D);

            /* Free the bits. */
            free(bits);
            break;

        case HI_PKM:     /* PKM file. */
            bits = hiLoadPKM(f, &format, &width, &height, &bytes);

            if (bits == NULL)
            {
                glDeleteTextures(1, &texture);
                texture = 0;
                break;
            }

            /* Set unpack alignment. */
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            /* Load texture. */
            glCompressedTexImage2D(target,
                                   0,
                                   format,
                                   width,
                                   height,
                                   0,
                                   bytes,
                                   bits);

            /* Free the bits. */
            free(bits);
            break;

        default:

            /* Unknown texture type. */
            break;
        }
    } while (0);

    /* Close the file. */
    fclose(f);

    /* Return the texture. */
    return texture;
}
