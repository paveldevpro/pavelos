#ifndef EFIERRS_H
#define EFIERRS_H

unsigned short int* CheckStandardEFIError(unsigned long long s)
{
    switch (s)
    {
        case EFI_LOAD_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: Load Error\r\n";
        }
        case EFI_INVALID_PARAMETER:
        {
            return (unsigned short int*)L"[!] EFI Status: Invalid Parameter !\r\n";
        }
        case EFI_UNSUPPORTED:
        {
            return (unsigned short int*)L"[!] EFI Status: Unsupported !\r\n";
        }
        case EFI_BAD_BUFFER_SIZE:
        {
            return (unsigned short int*)L"[!] EFI Status: Bad Buffer Size !\r\n";
        }
        case EFI_BUFFER_TOO_SMALL:
        {
            return (unsigned short int*)L"[!] EFI Status: Buffer Too Small !\r\n";
        }
        case EFI_NOT_READY:
        {
            return (unsigned short int*)L"[!] EFI Status: Not Ready !\r\n";
        }
        case EFI_DEVICE_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: Device Error !\r\n";
        }
        case EFI_WRITE_PROTECTED:
        {
            return (unsigned short int*)L"[!] EFI Status: Write Protected !\r\n";
        }
        case EFI_OUT_OF_RESOURCES:
        {
            return (unsigned short int*)L"[!] EFI Status: Out of Resources !\r\n";
        }
        case EFI_VOLUME_CORRUPTED:
        {
            return (unsigned short int*)L"[!] EFI Status: Volume Corrupted !\r\n";
        }
        case EFI_VOLUME_FULL:
        {
            return (unsigned short int*)L"[!] EFI Status: Volume Full !\r\n";
        }
        case EFI_NO_MEDIA:
        {
            return (unsigned short int*)L"[!] EFI Status: No Media !\r\n";
        }
        case EFI_MEDIA_CHANGED:
        {
            return (unsigned short int*)L"[!] EFI Status: Media Changed !\r\n";
        }
        case EFI_NOT_FOUND:
        {
            return (unsigned short int*)L"[!] EFI Status: File Not Found !\r\n";
        }
        case EFI_ACCESS_DENIED:
        {
            return (unsigned short int*)L"[!] EFI Status: Access Denied !\r\n";
        }
        case EFI_NO_RESPONSE:
        {
            return (unsigned short int*)L"[!] EFI Status: No Response !\r\n";
        }
        case EFI_NO_MAPPING:
        {
            return (unsigned short int*)L"[!] EFI Status: No Mapping !\r\n";
        }
        case EFI_TIMEOUT:
        {
            return (unsigned short int*)L"[!] EFI Status: Timeout !\r\n";
        }
        case EFI_NOT_STARTED:
        {
            return (unsigned short int*)L"[!] EFI Status: Not Started !\r\n";
        }
        case EFI_ALREADY_STARTED:
        {
            return (unsigned short int*)L"[!] EFI Status: Already Started !\r\n";
        }
        case EFI_ABORTED:
        {
            return (unsigned short int*)L"[!] EFI Status: Aborted !\r\n";
        }
        case EFI_ICMP_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: ICMP Error !\r\n";
        }
        case EFI_TFTP_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: TFTP Error !\r\n";
        }
        case EFI_PROTOCOL_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: Protocol Error !\r\n";
        }
        case EFI_INCOMPATIBLE_VERSION:
        {
            return (unsigned short int*)L"[!] EFI Status: Incompatible Version !\r\n";
        }
        case EFI_SECURITY_VIOLATION:
        {
            return (unsigned short int*)L"[!] EFI Status: Security Violation !\r\n";
        }
        case EFI_CRC_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: CRC Error !\r\n";
        }
        case EFI_END_OF_MEDIA:
        {
            return (unsigned short int*)L"[!] EFI Status: End of Media !\r\n";
        }
        case EFI_END_OF_FILE:
        {
            return (unsigned short int*)L"[!] EFI Status: End of File !\r\n";
        }
        case EFI_INVALID_LANGUAGE:
        {
            return (unsigned short int*)L"[!] EFI Status: Invalid Language !\r\n";
        }
        case EFI_COMPROMISED_DATA:
        {
            return (unsigned short int*)L"[!] EFI Status: Compromised Data !\r\n";
        }
        case EFI_IP_ADDRESS_CONFLICT:
        {
            return (unsigned short int*)L"[!] EFI Status: IP Address Invalid !\r\n";
        }
        case EFI_HTTP_ERROR:
        {
            return (unsigned short int*)L"[!] EFI Status: HTTP Error !\r\n";
        }
        case EFI_WARN_UNKNOWN_GLYPH:
        {
            return (unsigned short int*)L"[!] EFI Warning: Unknown Glyph !\r\n";
        }
        case EFI_WARN_DELETE_FAILURE:
        {
            return (unsigned short int*)L"[!] EFI Warning: Delete Failure !\r\n";
        }
        case EFI_WARN_WRITE_FAILURE:
        {
            return (unsigned short int*)L"[!] EFI Warning: Write Failure !\r\n";
        }
        case EFI_WARN_BUFFER_TOO_SMALL:
        {
            return (unsigned short int*)L"[!] EFI Warning: Buffer Too Small !\r\n";
        }
        case EFI_WARN_STALE_DATA:
        {
            return (unsigned short int*)L"[!] EFI Warning: Stale Data !\r\n";
        }
        case EFI_WARN_FILE_SYSTEM:
        {
            return (unsigned short int*)L"[!] EFI Warning: File System !\r\n";
        }
        case EFI_WARN_RESET_REQUIRED:
        {
            return (unsigned short int*)L"[!] EFI Warning: Reset Required !\r\n";
        }
        case EFI_SUCCESS:
        {
            return (unsigned short int*)L"[!] EFI Status: Successful !\r\n";
        }
    }

    return (unsigned short int*)L"[!] EFI Status: Unknown Error !\r\n";
}

#endif // EFIERRS_H
