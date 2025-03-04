#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include "efierrs.h"
#include "efilibs.h"
#include "bmp.h"

#include <Library/DebugLib.h>
#include <Guid/FileInfo.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>

#define MAX_FILE_INFO_SIZE 1024

EFI_STATUS EFIAPI PerFileFunc(IN EFI_FILE_HANDLE Dir, IN EFI_DEVICE_PATH *DirDp, IN EFI_FILE_INFO *FileInfo, IN EFI_DEVICE_PATH *Dp)
{
    EFI_STATUS       Status;
    EFI_FILE_HANDLE  File;

    Print (L"Path = %s FileName = %s\n", ConvertDevicePathToText(DirDp, TRUE, TRUE), FileInfo->FileName);

    Status = Dir->Open (Dir, &File, FileInfo->FileName, EFI_FILE_MODE_READ,0);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // reset position just in case
    File->SetPosition (File, 0);

    // ****Do stuff on the file here****

    Dir->Close (File);

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI ProcessFilesInDir(IN EFI_FILE_HANDLE Dir, IN EFI_DEVICE_PATH *DirDp)
{
    EFI_STATUS       Status;
    EFI_FILE_INFO    *FileInfo;
    CHAR16           *FileName;
    UINTN            FileInfoSize;
    EFI_DEVICE_PATH  *Dp;

    FileInfo = AllocatePool (MAX_FILE_INFO_SIZE);
    if (FileInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    for (;;)
    {
        // get the next file's info. there's an internal position
        // that gets incremented when you read from a directory
        // so that subsequent reads gets the next file's info
        FileInfoSize = MAX_FILE_INFO_SIZE;
        Status = Dir->Read (Dir, &FileInfoSize, (VOID *) FileInfo);
        if (EFI_ERROR (Status) || FileInfoSize == 0) { //this is how we eventually exit this function when we run out of files
            if (Status == EFI_BUFFER_TOO_SMALL) {
                Print (L"EFI_FILE_INFO > MAX_FILE_INFO_SIZE. Increase the size\n");
            }
            FreePool (FileInfo);
            return Status;
        }

        FileName = FileInfo->FileName;

        // skip files named . or ..
        if (StrCmp (FileName, L".") == 0 || StrCmp (FileName, L"..") == 0) {
            continue;
        }

        // so we have absolute device path to child file/dir
        Dp = FileDevicePath (DirDp, FileName);
        if (Dp == NULL) {
            FreePool (FileInfo);
            return EFI_OUT_OF_RESOURCES;
        }

        // Do whatever processing on the file
        PerFileFunc (Dir, DirDp, FileInfo,  Dp);

        if (FileInfo->Attribute & EFI_FILE_DIRECTORY) {
            //
            // recurse
            //

            EFI_FILE_HANDLE NewDir;

            Status = Dir->Open(Dir, &NewDir, FileName, EFI_FILE_MODE_READ, 0);
            if (Status != EFI_SUCCESS) {
                FreePool(FileInfo);
                FreePool(Dp);
                return Status;
            }
        }

        FreePool(Dp);
    }
}

EFI_STATUS test()
{
    EFI_STATUS  Status;
    UINTN       NumHandles;
    EFI_HANDLE  *Handles;
    UINTN       Index;

    Status = gBS->LocateHandleBuffer (
        ByProtocol,
        &((EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID),
        NULL,
        &NumHandles,
        &Handles
    );

    if (EFI_ERROR (Status)) {
        return Status;
    }

    for (Index = 0; Index < NumHandles; Index++) {
        EFI_FILE_HANDLE                         Root;
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL        *Fs;
        EFI_DEVICE_PATH                        *Dp;

        // get simple file system protocol instance
        // from current handle
        Status = gBS->OpenProtocol (
            Handles[Index],
            &gEfiSimpleFileSystemProtocolGuid,
            (void**)&Fs,
            NULL,
            gImageHandle,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        );

        if (EFI_ERROR (Status)) {
            DEBUG ((EFI_D_ERROR, "Missing EFI_SIMPLE_FILE_SYSTEM_PROTOCOL on handle.\n"));
            continue;
        }

        // get device path instance from current handle
        Status = gBS->OpenProtocol (
            Handles[Index],
            &gEfiDevicePathProtocolGuid,
            (void**)&Dp,
            NULL,
            gImageHandle,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        );

        if (EFI_ERROR (Status)) {
            DEBUG ((EFI_D_ERROR, "Missing EFI_DEVICE_PATH_PROTOCOL on handle.\n"));
            continue;
        }

        // open root dir from current simple file system
        Status = Fs->OpenVolume (Fs, &Root);
        if (EFI_ERROR (Status)) {
            DEBUG ((EFI_D_ERROR, "Unable to open volume.\n"));
            continue;
        }

        // recursively process files in root dir
        Status = ProcessFilesInDir(Root, Dp);
        Root->Close(Root);

        if (EFI_ERROR (Status)) {
            DEBUG ((EFI_D_ERROR, "ProcessFilesInDir error. Continuing with next volume...\n"));
            continue;
        }
    }

    Print(L"Done successfully\n");

    return EFI_SUCCESS;
}

/*Print(L"MaxMode = %u\n", gop->Mode->MaxMode);
    Print(L"Current mode = %u\n", gop->Mode->Mode);
    Print(L"Resolution = %ux%u\n", gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution);
    
    UINT32 numModes = gop->Mode->MaxMode;//, nativeMode = gop->Mode->Mode;
    UINTN SizeOfInfo = gop->Mode->SizeOfInfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;

    for (int i = 0; i < numModes; i++)
    {
        gop->QueryMode(gop, i, &SizeOfInfo, &info);
        Print(L"mode %03d width %d height %d format %x%s\n",
            i,
            info->HorizontalResolution,
            info->VerticalResolution,
            info->PixelFormat
        );
        HitAnyKey();
    }

    gop->SetMode(gop, 6);

    Print(L"Current mode = %u\n", gop->Mode->Mode);
    Print(L"Resolution = %ux%u\n", gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution);*/

void GetCharacter(UINT32 character, UINT32 xPos, UINT32 yPos, UINT32 fs)
{
    
}

EFI_STATUS EFIAPI UefiEntry(IN EFI_HANDLE imgHandle, IN EFI_SYSTEM_TABLE* sysTable)
{
    gST = sysTable;
    gBS = sysTable->BootServices;
    gImageHandle = imgHandle;
    gEfiSimpleFileSystemProtocolGuid = (EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    
    gBS->SetWatchdogTimer(0, 0, 0, NULL);

    ClearScreen();

    SetColor(EFI_LIGHTGREEN);
    Print(L"Hello Pavel from UEFI !\n\n");

    EFI_STATUS status = GOP_Initialize();
    if (status != EFI_SUCCESS)
    {
        PrintErr(L"[!] Error: GOP Initialization failed !\n");
        PrintStatus(status);
        goto end;
    }

    FS_Initialize();



    // EFI_FILE_PROTOCOL* txt = openFile(L"img.bmp");
    // drawBmpFile(txt);

end:
    while (1)
    {
        Delay1();

        EFI_STATUS status = CheckKey();
        if (status == EFI_SUCCESS)
        {
            Print(L"Unicode : %x\n", CheckKeystroke.UnicodeChar);


            /*if (GetKey('\b'))
            {
                Print(L"Backspace !\n");
            }*/
            if (GetKey('q'))
            {
                SHUTDOWN();
                break;
            }
            if (GetKey('r'))
            {
                WARM_REBOOT();
                break;
            }
        }
    }

    WARM_REBOOT();

    return EFI_SUCCESS;
}
