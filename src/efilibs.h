#ifndef EFILIBS_H
#define EFILIBS_H

//#define COLOR(r,g,b) ((b) | (g << 8) | (r << 16))

#define COLOR(a,r,g,b) ((a << 24) | (r << 16) | (g << 8) | (b))

EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL GraphicsColor;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* Volume;
UINTN TextColor = EFI_WHITE;

void SetTextPosition(UINT32 Col, UINT32 Row)
{
    gST->ConOut->SetCursorPosition(gST->ConOut, Col, Row);
}

// don't use for the moment
void ResetScreen()
{
    gST->ConOut->Reset(gST->ConOut, 1);
}

void ClearScreen()
{
    gST->ConOut->ClearScreen(gST->ConOut);
}

void SetColor(UINTN Attribute)
{
    gST->ConOut->SetAttribute(gST->ConOut, Attribute);
    TextColor = Attribute;
}

void PrintErr(CHAR16* str)
{
    UINTN lastColor = TextColor;
    SetColor(EFI_LIGHTRED);
    Print(str);
    SetColor(lastColor);
}

void ResetKeyboard()
{
    gST->ConIn->Reset(gST->ConIn, 1);
}

void HitAnyKey()
{
    ResetKeyboard();

    EFI_INPUT_KEY Key;

    while ((gST->ConIn->ReadKeyStroke(gST->ConIn, &Key)) == EFI_NOT_READY);
}

EFI_INPUT_KEY CheckKeystroke;
BOOLEAN GetKey(CHAR16 key)
{
    if (CheckKeystroke.UnicodeChar == key)
        return 1;
    return 0;
}

BOOLEAN GetKeyEx(UINT16 key)
{
    if (CheckKeystroke.ScanCode == key)
        return 1;
    return 0;
}

EFI_STATUS CheckKey()
{
    return gST->ConIn->ReadKeyStroke(gST->ConIn, &CheckKeystroke);
    gST->ConIn->WaitForKey;
}

void SetGraphicsColor(UINT32 color)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL GColor;
    GColor.Reserved = color >> 24;
    GColor.Red      = color >> 16;
    GColor.Green    = color >> 8;
    GColor.Blue     = color;
    GraphicsColor = GColor;
}

void SetPixel(UINT32 xPos, UINT32 yPos)
{
    // TODO : Add in a choice instead of defaulting to EfiBltVideoFill.
    gop->Blt(gop, &GraphicsColor, EfiBltVideoFill, 0, 0, xPos, yPos, 1, 1, 0);
}

void CreateFilledBox(UINT32 xPos, UINT32 yPos, UINT32 w, UINT32 h)
{
    // TODO : Add in a choice instead of defaulting to EfiBltVideoFill.
    gop->Blt(gop, &GraphicsColor, EfiBltVideoFill, 0, 0, xPos, yPos, w, h, 0);
}

void PrintStatus(unsigned long long s)
{
    PrintErr(CheckStandardEFIError(s));
}

void Delay(UINTN d)
{
    gBS->Stall(d * 1000); // in microseconds
}

void Delay1()
{
    gBS->Stall(1);
}

void COLD_REBOOT()
{
    // Hardware Reboot
    gST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, 0);
}

void WARM_REBOOT()
{
    // Software Reboot
    gST->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, 0);
}

void SHUTDOWN()
{
    // Shuts off the computer
    // NOTE : This does not work in VirtualBox.
    // WORKS in QEMU !!!
    gST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, 0);
}

EFI_STATUS GOP_Initialize()
{
    // We initialize the Graphics Output Protocol.
    // This is used instead of the VGA interface.
    SetColor(EFI_YELLOW);
    Print(L"[>] Loading Graphics Output Protocol...\n");
    EFI_STATUS status = gBS->LocateProtocol(&((EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID), 0, (void**)&gop);
    PrintStatus(status);
    return status;
}

void FS_Initialize()
{
    EFI_STATUS status;
    // EFI takes advantage of the FAT32 file system
    Print(L"[>] Initializing the Loaded Image...\n");
    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
    status = gBS->HandleProtocol(gImageHandle, &((EFI_GUID)EFI_LOADED_IMAGE_PROTOCOL_GUID), (void**)&LoadedImage);

    PrintStatus(status);

    Print(L"[>] Initializing the Device Path...\n");
    EFI_DEVICE_PATH_PROTOCOL* DevicePath;
    status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &((EFI_GUID)EFI_DEVICE_PATH_PROTOCOL_GUID), (void**)&DevicePath);

    PrintStatus(status);

    Print(L"[>] Initializing the Volume...\n");
    status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &((EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID), (void**)&Volume);
    
    PrintStatus(status);
}

EFI_FILE_PROTOCOL* openFile(CHAR16* FileName)
{
    // works only with files in root
    // full path seems to not work on real hardware
    // but in QEMU, it worked
    EFI_STATUS status;
    EFI_FILE_PROTOCOL* RootFS;
    Print(L"[>] Opening the volume...\n");
    status = Volume->OpenVolume(Volume, &RootFS);
    PrintStatus(status);

    Print(L"[>] Opening the file...\n");
    EFI_FILE_PROTOCOL* FileHandle = NULL;
    status = RootFS->Open(RootFS, &FileHandle, FileName, 0x0000000000000001, 0);
    PrintStatus(status);

    return FileHandle;
}

void closeFile(EFI_FILE_PROTOCOL* FileHandle)
{
    EFI_STATUS status;
    SetColor(EFI_YELLOW);
    Print(L"[>] Closing the file...\n");
    status = FileHandle->Close(FileHandle);
    PrintStatus(status);
}

#endif // EFILIBS_H
