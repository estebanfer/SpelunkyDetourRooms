#include "pch.h"
#include "detours.h"

const DWORD GETROOM_OFFSET = 0xd6690;
const DWORD GETROOM_GET_ROOM_OFF = GETROOM_OFFSET + 0x29;
const DWORD GETROOM_SPAWN_ROOM_OFF = GETROOM_OFFSET + 0x174;

void* roomGenFunc{nullptr};

DWORD GetBaseAddress()
{
    return (DWORD)GetModuleHandle(NULL);
}

DWORD getRoomGet = {GETROOM_GET_ROOM_OFF+GetBaseAddress()};
DWORD getRoomSpawn = {GETROOM_SPAWN_ROOM_OFF + GetBaseAddress()};

bool customRoomGet(int doorRoomType, int roomIndex, char* roomOut, DWORD levelState) {
    if (doorRoomType == 1)
    {
        return false;
        //strcpy_s(roomOut, 0x51, "011111111001111111100vvvvvvvv00vv0000vv0000009000001v====v1001111111101111111111");
    }
    else if (doorRoomType == 2)
    {
        return false;
        //strcpy_s(roomOut, 0x51, "00000000000111200100011110010021111011000000002109011111111102111111121111111111");
    }
    else {
        for (int i = 0; i < 0x50; i++)
        {
            roomOut[i] = rand() % 3 == 0 ? '1' : '0';
        }
        //strcpy_s(roomOut, 0x51, "00000000000010111100000000000000011010000050000000000000000000000000001111111111");
    };
    return true;
}

void __declspec(naked) hookThing() {
    __asm {
        //original func code {
        sub esp, 0xbc
        push ebx
        mov ebx, dword ptr ss : [esp + 0xD0]
        push ebp
        mov ebp, dword ptr ss : [esp + 0xC8]
        mov eax, dword ptr ss : [ebp + ebx * 0x4 + 0xA5EC]
        push esi
        mov esi, dword ptr ss : [ebp + 0x1715C]
        push edi
        //}
        lea edi, dword ptr ss : [esp + 0x74] //room_dest
        push eax

        push ebp //level_state
        push edi //room_dest
        push ebx //room_index
        push edx //entrance_or_exit
        call customRoomGet
        add esp, 0x10
        cmp eax, 0x0
        pop eax
        jz ifZero
        jmp getRoomSpawn
        ifZero:
        cmp dword ptr ds : [esi + 0x5C] , 0x16 //original func code
        jmp getRoomGet
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        roomGenFunc = (void*)(GetBaseAddress() + GETROOM_OFFSET);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach((void**)&roomGenFunc, hookThing);

        const LONG error = DetourTransactionCommit();
    }

    return TRUE;
}