#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "fbdrt.h"

const unsigned char description[] = {0x0A, 0x00, 0x10, 0x10, 0x94, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xE5, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD1, 0xC0, 0xD2, 0x2D, 0x35, 0x30, 0x30, 0x00, 0x31, 0x00, 0x31, 0x35, 0x2D, 0x31, 0x32, 0x2D, 0x32, 0x30, 0x31, 0x37, 0x20, 0x31, 0x33, 
0x3A, 0x30, 0x38, 0x3A, 0x35, 0x33, 0x00};

#define MEM_SIZE 100

char memory[MEM_SIZE];

tSignal FBDgetProc(char type, tSignal index)
{
    switch(type) {
    case 0:
        printf(" request InputPin(%ld)\n", index);
        return 0;
    case 1:
        printf(" request NVRAM(%ld)\n", index);
        return 0;
    }
    return 0;
}

void FBDsetProc(char type, tSignal index, tSignal *value)
{
    switch(type)
    {
    case 0:
        printf(" set OutputPin(%ld) to value %ld\n", index, *value);
        break;
    case 1:
        printf(" set NVRAM(%ld) to value %ld\n", index, *value);
        break;
    }
}

int main(void)
{
    int res,i;
    tNetVar netvar;
    //
    res = fbdInit(description);
    if(res <= 0) {
        printf("result = %d\n", res);
        return 0;
    }
    if(res > MEM_SIZE) {
        printf("not enough memory\n");
        return 0;
    }
    //
    fbdSetMemory(memory, true);
    printf("memory request size = %d\n", res);
    //
    printf("FBD_OPT_REQ_VERSION: %ld\n", fbdGetGlobalOptions(FBD_OPT_REQ_VERSION));
    printf("FBD_NETVAR_USE: %ld\n", fbdGetGlobalOptions(FBD_OPT_NETVAR_USE));
    printf("FBD_NETVAR_PORT: %ld\n", fbdGetGlobalOptions(FBD_OPT_NETVAR_PORT));
    printf("FBD_NETVAR_GROUP: %ld\n", fbdGetGlobalOptions(FBD_OPT_NETVAR_GROUP));

    // main loop
    i=0;
    while(1) {
        printf("step %d\n", i++);

        netvar.index = 5;
        netvar.value = i;
        fbdSetNetVar(&netvar);

        netvar.index = 7;
        netvar.value = i-1;
        fbdSetNetVar(&netvar);

        Sleep(1000);
        fbdDoStep(1000);
        //
        while(fbdGetNetVar(&netvar)) {
            printf("                        Send netvar(%ld) value %ld\n", netvar.index, netvar.value);
        }

    }
    return 0;
}
