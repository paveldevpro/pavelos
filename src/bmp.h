#ifndef BMP_H
#define BMP_H

typedef struct
{
    UINT8   bfType[2];      // The characters "BM"
    UINT32  bfSize;         // size of the file in bytes
    UINT16  bfReserved1;    // unused - must be zero
    UINT16  bfReserved2;    // unused - must be zero
    UINT32  bfOffBits;      // offset to the start of Pixel Data

} __attribute__((packed)) BMP_FileHeader;

typedef struct
{
    UINT32 biSize;              // Header Size - Must be at least 40
    UINT32 biWidth;             // Image width in pixels
    UINT32 biHeight;            // Image height in pixels
    UINT16 biPlanes;            // Must be 1
    UINT16 biBitCount;          // Bits per pixel - 1, 4, 8, 16, 24, 32
    UINT32 biCompression;       // Compression type (0 = uncompressed)
    UINT32 biSizeImage;         // Image Size - may be zero for uncompressed images
    UINT32 biXPelsPerMeter;     // Preferred resolution in pixels per meter
    UINT32 biYPelsPerMeter;     // Preferred resolution in pixels per meter
    UINT32 biClrUsed;           // Number Color Map entries that are actually used
    UINT32 biClrImportant;      // Number of significant colors

} __attribute__((packed)) BMP_ImageHeader;

void printFileHeader(BMP_FileHeader* fileHeader)
{
    Print(L"\nType: %c%c.\n", fileHeader->bfType[0],fileHeader->bfType[1]);
    Print(L"Size: %u.\n", fileHeader->bfSize);
    Print(L"Verify (Must be 0 0): %d %d.\n", fileHeader->bfReserved1, fileHeader->bfReserved2);
    Print(L"Offset Hex : %x.\n", fileHeader->bfOffBits);
    Print(L"Offset : %u.\n", fileHeader->bfOffBits);
}

void printImageHeader(BMP_ImageHeader* imageHeader)
{
    Print(L"\nSize of header: %d.\n", imageHeader->biSize);
    Print(L"Width: %d.\n", imageHeader->biWidth);
    Print(L"Height: %d.\n", imageHeader->biHeight);
    Print(L"Color Planes: %d.\n", imageHeader->biPlanes);
    Print(L"Bits per Pixel: %d.\n", imageHeader->biBitCount);
    Print(L"Compression: %d.\n", imageHeader->biCompression);
    Print(L"Image size: %d.\n", imageHeader->biSizeImage);
    Print(L"Preferred resolution in pixels per meter (X-Y): %d-%d.\n", imageHeader->biXPelsPerMeter, imageHeader->biYPelsPerMeter);
    Print(L"Number color map: %d.\n", imageHeader->biClrUsed);
    Print(L"Number of significant colors: %d.\n", imageHeader->biClrImportant);
}

void drawBmpFile(EFI_FILE_PROTOCOL* file)
{
    EFI_STATUS status;

    if (file != NULL)
    {
        BMP_FileHeader* fileHeader;
        BMP_ImageHeader* imageHeader;
        void* ReservedBuffer;

        UINT64 fileHeaderSz = sizeof(BMP_FileHeader);
        gBS->AllocatePool(EfiLoaderData, fileHeaderSz, (void**)&fileHeader);
        status = file->Read(file, &fileHeaderSz, fileHeader);
        if (status != EFI_SUCCESS)
        {
            PrintErr(L"[!] Error: Error when reading the file header !\n");
            PrintStatus(status);
            return;
        }

        UINT64 imageHeaderSz = sizeof(BMP_ImageHeader);
        gBS->AllocatePool(EfiLoaderData, imageHeaderSz, (void**)&imageHeader);
        status = file->Read(file, &imageHeaderSz, imageHeader);
        if (status != EFI_SUCCESS)
        {
            PrintErr(L"[!] Error: Error when reading the image header !\n");
            PrintStatus(status);
            return;
        }

        UINT64 ReservedBufferSz = fileHeader->bfOffBits - (fileHeaderSz + imageHeaderSz);
        gBS->AllocatePool(EfiLoaderData, ReservedBufferSz, (void**)&ReservedBuffer);
        status = file->Read(file, &ReservedBufferSz, ReservedBuffer);
        if (status != EFI_SUCCESS)
        {
            PrintErr(L"[!] Error: Error when reading the reserved buffer !\n");
            PrintStatus(status);
            return;
        }
        gBS->FreePool(ReservedBuffer);

        UINT64 PixelsSz = imageHeader->biSizeImage;
        UINT8 BytesPerPixel = imageHeader->biBitCount / 8;

        UINT8 (*Pixels)[BytesPerPixel];
        gBS->AllocatePool(EfiLoaderData, PixelsSz, (void**)&Pixels);
        status = file->Read(file, &PixelsSz, Pixels);
        if (status != EFI_SUCCESS)
        {
            PrintErr(L"[!] Error: Error when reading pixel data !\n");
            PrintStatus(status);
            return;
        }

        closeFile(file);

        for (int y = imageHeader->biHeight; y > 0 ; y--)
        {
            for (int x = 0; x < imageHeader->biWidth; x++)
            {
                UINT8* g = *Pixels;

                SetGraphicsColor(COLOR(g[3], g[2], g[1], g[0])); // ABGR
                SetPixel(x, y);

                Pixels++;
            }
        }

        gBS->FreePool(fileHeader);
        gBS->FreePool(imageHeader);
        gBS->FreePool(Pixels);
    }
}

#endif // BMP_H