#include "pch.h"
#include "detours.h"
//#include "customRoomLoader.h"
#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <map>
#include <vector>

enum class ROOM_TYPE {
    ENTRANCE = 1100,
    ENTRANCE_DROP = 1101,
    EXIT = 1102,
    EXIT_NOTOP = 1103,
    IDOL = 1200,
    ALTAR = 1201,
    PATH_DROP_NOTOP = 1202,

    SIDE = 0x0,
    PATH = 0x1,
    PATH_DROP = 0x2,
    PATH_NOTOP = 0x3,
    SHOP_RIGHT_FACING = 0x4,
    SHOP_LEFT_FACING = 0x5,
    SNAKE_PIT_TOP = 0x6,
    SNAKE_PIT_MIDDLE = 0x7,
    SNAKE_PIT_BOTTOM = 0x8,
    RUSHING_WATER_ISLANDS = 0x9,
    RUSHING_WATER_LAKE = 0xa,
    RUSHING_WATER_LAKE_BITEY = 0xb,
    PSYCHIC_PRESENCE_LEFT = 0xc,
    PSYCHIC_PRESENCE_CENTER = 0xd,
    PSYCHIC_PRESENCE_RIGHT = 0xe,
    MOAI = 0xf,
    KALI_PIT_TOP = 0x10,
    KALI_PIT_MIDDLE = 0x11,
    KALI_PIT_BOTTOM = 0x12,
    VLADS_TOWER_TOP = 0x13,
    VLADS_TOWER_MIDDLE = 0x14,
    VLADS_TOWER_BOTTOM = 0x15,
    BEEHIVE_LEFT_RIGHT_OPEN = 0x16,
    BEEHIVE_LEFT_UP_OPEN = 0x18,
    BEEHIVE_LEFT_DOWN_OPEN = 0x19,
    NECRONOMICON_LEFT = 0x1a,
    NECRONOMICON_RIGHT = 0x1b,
    MOTHERSHIP_ENTRANCE_TOP = 0x1c,
    MOTHERSHIP_ENTRANCE_BOTTOM = 0x1d,
    CASTLE_1 = 0x1e,
    CASTLE_2 = 0x1f,
    CASTLE_3 = 0x20,
    CASTLE_4 = 0x21,
    CASTLE_5 = 0x22,
    CASTLE_6 = 0x23,
    CASTLE_7 = 0x24,
    CASTLE_8 = 0x25,
    CASTLE_9 = 0x26,
    CASTLE_10 = 0x27,
    CASTLE_11 = 0x28,
    CRYSKNIFE_PIT_LEFT = 0x29,
    CRYSKNIFE_PIT_RIGHT = 0x2a,
    COFFIN = 0x2b,
    COFFIN_DROP = 0x2c,
    COFFIN_NOTOP = 0x2d,
    ALIEN_QUEEN = 0x2e,
    DAR_CASTLE_ENTRANCE = 0x2f,
    DAR_CRYSTAL_IDOL = 0x30,
    YAMA_0_0 = 0x31,
    YAMA_0_1 = 0x32,
    YAMA_0_2 = 0x33,
    YAMA_0_3 = 0x34,
    YAMA_THRONE_TOP_LEFT = 0x35,
    YAMA_THRONE_TOP_RIGHT = 0x36,
    YAMA_MIDDLE_LEFT = 0x37,
    YAMA_THRONE_BOTTOM_LEFT = 0x38,
    YAMA_THRONE_BOTTOM_RIGHT = 0x39,
    YAMA_MIDDLE_RIGHT = 0x3a,
    YAMA_3_0 = 0x3b,
    YAMA_3_1 = 0x3c,
    YAMA_3_2 = 0x3d,
    YAMA_3_3 = 0x3e,
    SPIDER_LAIR_1 = 0x3f,
    SPIDER_LAIR_2 = 0x40,
    SPIDER_LAIR_3 = 0x41,
    SPIDER_LAIR_4 = 0x42,
    VAULT = 0x43,
    ICE_CAVES_POOL_SINGLE_ROOM = 0x44,
    ICE_CAVES_POOL_TOP = 0x45,
    ICE_CAVES_POOL_BOTTOM = 0x46,
    TEMPLE_ALTAR = 0x48,
    COFFIN_EXIT_RIGHT = 0x49,
    COFFIN_EXIT_LEFT = 0x4a,
    WORM_REGEN_STRUCTURE = 0x4b,
    WET_FUR_1 = 0x4c,
    WET_FUR_2 = 0x4d,
    WET_FUR_3 = 0x4e,
    SHOP_GENERAL_LEFT = 0x64,
    SHOP_SPECIALTY = 0x65,
    SHOP_CLOTHING = 0x66,
    SHOP_BOMBS = 0x67,
    SHOP_WEAPONS = 0x68,
    SHOP_KISSING = 0x69,
    SHOP_HIREDHAND = 0x6a,
    SHOP_PRIZE_OR_ANKH = 0x6b,
    TUTORIAL_0_0 = 0x3e8,
    TUTORIAL_0_1 = 0x3e9,
    TUTORIAL_0_2 = 0x3ea,
    TUTORIAL_0_3 = 0x3eb,
    TUTORIAL_1_0 = 0x3ec,
    TUTORIAL_1_1 = 0x3ed,
    TUTORIAL_1_2 = 0x3ee,
    TUTORIAL_1_3 = 0x3ef,
    TUTORIAL_2_0 = 0x3f0,
    TUTORIAL_2_1 = 0x3f1,
    TUTORIAL_2_2 = 0x3f2,
    TUTORIAL_2_3 = 0x3f3,
    TUTORIAL_3_0 = 0x3f4,
    TUTORIAL_3_1 = 0x3f5,
    TUTORIAL_3_2 = 0x3f6,
    TUTORIAL_3_3 = 0x3f7,

    SHOP_GENERAL_FLIPPED = 0x2774,
    SHOP_SPECIALTY_FLIPPED = 0x2775,
    SHOP_CLOTHING_FLIPPED = 0x2776,
    SHOP_BOMBS_FLIPPED = 0x2777,
    SHOP_WEAPONS_FLIPPED = 0x2778,
    SHOP_KISSING_FLIPPED = 0x2779,
    SHOP_HIREDHAND_FLIPPED = 0x277a,
    SHOP_PRIZE_OR_ANKH_FLIPPED = 0x277b,
    UNSET = -1,
};

const std::map<std::string, ROOM_TYPE> roomMap = { {
    {"entrance", ROOM_TYPE::ENTRANCE},
    {"entrance_drop", ROOM_TYPE::ENTRANCE_DROP},
    {"exit", ROOM_TYPE::EXIT},
    {"exit_notop", ROOM_TYPE::EXIT_NOTOP},
    {"idol", ROOM_TYPE::IDOL},
    {"altar", ROOM_TYPE::ALTAR},
    {"path_drop_notop", ROOM_TYPE::PATH_DROP_NOTOP},

    {"side", ROOM_TYPE::SIDE},
    {"path", ROOM_TYPE::PATH},
    {"path_drop", ROOM_TYPE::PATH_DROP},
    {"path_notop", ROOM_TYPE::PATH_NOTOP},
    {"shop_right_facing", ROOM_TYPE::SHOP_RIGHT_FACING},
    {"shop_left_facing", ROOM_TYPE::SHOP_LEFT_FACING},
    {"snake_pit_top", ROOM_TYPE::SNAKE_PIT_TOP},
    {"snake_pit_middle", ROOM_TYPE::SNAKE_PIT_MIDDLE},
    {"snake_pit_bottom", ROOM_TYPE::SNAKE_PIT_BOTTOM},
    {"rushing_water_islands", ROOM_TYPE::RUSHING_WATER_ISLANDS},
    {"rushing_water_lake", ROOM_TYPE::RUSHING_WATER_LAKE},
    {"rushing_water_lake_bitey", ROOM_TYPE::RUSHING_WATER_LAKE_BITEY},
    {"psychic_presence_left", ROOM_TYPE::PSYCHIC_PRESENCE_LEFT},
    {"psychic_presence_center", ROOM_TYPE::PSYCHIC_PRESENCE_CENTER},
    {"psychic_presence_right", ROOM_TYPE::PSYCHIC_PRESENCE_RIGHT},
    {"moai", ROOM_TYPE::MOAI},
    {"kali_pit_top", ROOM_TYPE::KALI_PIT_TOP},
    {"kali_pit_middle", ROOM_TYPE::KALI_PIT_MIDDLE},
    {"kali_pit_bottom", ROOM_TYPE::KALI_PIT_BOTTOM},
    {"vlads_tower_top", ROOM_TYPE::VLADS_TOWER_TOP},
    {"vlads_tower_middle", ROOM_TYPE::VLADS_TOWER_MIDDLE},
    {"vlads_tower_bottom", ROOM_TYPE::VLADS_TOWER_BOTTOM},
    {"beehive_left_right_open", ROOM_TYPE::BEEHIVE_LEFT_RIGHT_OPEN},
    {"beehive_left_up_open", ROOM_TYPE::BEEHIVE_LEFT_UP_OPEN},
    {"beehive_left_down_open", ROOM_TYPE::BEEHIVE_LEFT_DOWN_OPEN},
    {"necronomicon_left", ROOM_TYPE::NECRONOMICON_LEFT},
    {"necronomicon_right", ROOM_TYPE::NECRONOMICON_RIGHT},
    {"mothership_entrance_top", ROOM_TYPE::MOTHERSHIP_ENTRANCE_TOP},
    {"mothership_entrance_bottom", ROOM_TYPE::MOTHERSHIP_ENTRANCE_BOTTOM},
    {"castle_1", ROOM_TYPE::CASTLE_1},
    {"castle_2", ROOM_TYPE::CASTLE_2},
    {"castle_3", ROOM_TYPE::CASTLE_3},
    {"castle_4", ROOM_TYPE::CASTLE_4},
    {"castle_5", ROOM_TYPE::CASTLE_5},
    {"castle_6", ROOM_TYPE::CASTLE_6},
    {"castle_7", ROOM_TYPE::CASTLE_7},
    {"castle_8", ROOM_TYPE::CASTLE_8},
    {"castle_9", ROOM_TYPE::CASTLE_9},
    {"castle_10", ROOM_TYPE::CASTLE_10},
    {"castle_11", ROOM_TYPE::CASTLE_11},
    {"crysknife_pit_left", ROOM_TYPE::CRYSKNIFE_PIT_LEFT},
    {"crysknife_pit_right", ROOM_TYPE::CRYSKNIFE_PIT_RIGHT},
    {"coffin", ROOM_TYPE::COFFIN},
    {"coffin_drop", ROOM_TYPE::COFFIN_DROP},
    {"coffin_notop", ROOM_TYPE::COFFIN_NOTOP},
    {"alien_queen", ROOM_TYPE::ALIEN_QUEEN},
    {"dar_castle_entrance", ROOM_TYPE::DAR_CASTLE_ENTRANCE},
    {"dar_crystal_idol", ROOM_TYPE::DAR_CRYSTAL_IDOL},
    {"yama_0_0", ROOM_TYPE::YAMA_0_0},
    {"yama_0_1", ROOM_TYPE::YAMA_0_1},
    {"yama_0_2", ROOM_TYPE::YAMA_0_2},
    {"yama_0_3", ROOM_TYPE::YAMA_0_3},
    {"yama_throne_top_left", ROOM_TYPE::YAMA_THRONE_TOP_LEFT},
    {"yama_throne_top_right", ROOM_TYPE::YAMA_THRONE_TOP_RIGHT},
    {"yama_middle_left", ROOM_TYPE::YAMA_MIDDLE_LEFT},
    {"yama_throne_bottom_left", ROOM_TYPE::YAMA_THRONE_BOTTOM_LEFT},
    {"yama_throne_bottom_right", ROOM_TYPE::YAMA_THRONE_BOTTOM_RIGHT},
    {"yama_middle_right", ROOM_TYPE::YAMA_MIDDLE_RIGHT},
    {"yama_3_0", ROOM_TYPE::YAMA_3_0},
    {"yama_3_1", ROOM_TYPE::YAMA_3_1},
    {"yama_3_2", ROOM_TYPE::YAMA_3_2},
    {"yama_3_3", ROOM_TYPE::YAMA_3_3},
    {"spider_lair_1", ROOM_TYPE::SPIDER_LAIR_1},
    {"spider_lair_2", ROOM_TYPE::SPIDER_LAIR_2},
    {"spider_lair_3", ROOM_TYPE::SPIDER_LAIR_3},
    {"spider_lair_4", ROOM_TYPE::SPIDER_LAIR_4},
    {"vault", ROOM_TYPE::VAULT},
    {"ice_caves_pool_single_room", ROOM_TYPE::ICE_CAVES_POOL_SINGLE_ROOM},
    {"ice_caves_pool_top", ROOM_TYPE::ICE_CAVES_POOL_TOP},
    {"ice_caves_pool_bottom", ROOM_TYPE::ICE_CAVES_POOL_BOTTOM},
    {"temple_altar", ROOM_TYPE::TEMPLE_ALTAR},
    {"coffin_exit_right", ROOM_TYPE::COFFIN_EXIT_RIGHT},
    {"coffin_exit_left", ROOM_TYPE::COFFIN_EXIT_LEFT},
    {"worm_regen_structure", ROOM_TYPE::WORM_REGEN_STRUCTURE},
    {"wet_fur_1", ROOM_TYPE::WET_FUR_1},
    {"wet_fur_2", ROOM_TYPE::WET_FUR_2},
    {"wet_fur_3", ROOM_TYPE::WET_FUR_3},
    {"shop_general_left", ROOM_TYPE::SHOP_GENERAL_LEFT},
    {"shop_specialty", ROOM_TYPE::SHOP_SPECIALTY},
    {"shop_clothing", ROOM_TYPE::SHOP_CLOTHING},
    {"shop_bombs", ROOM_TYPE::SHOP_BOMBS},
    {"shop_weapons", ROOM_TYPE::SHOP_WEAPONS},
    {"shop_kissing", ROOM_TYPE::SHOP_KISSING},
    {"shop_hiredhand", ROOM_TYPE::SHOP_HIREDHAND},
    {"shop_prize_or_ankh", ROOM_TYPE::SHOP_PRIZE_OR_ANKH},
    {"tutorial_0_0", ROOM_TYPE::TUTORIAL_0_0},
    {"tutorial_0_1", ROOM_TYPE::TUTORIAL_0_1},
    {"tutorial_0_2", ROOM_TYPE::TUTORIAL_0_2},
    {"tutorial_0_3", ROOM_TYPE::TUTORIAL_0_3},
    {"tutorial_1_0", ROOM_TYPE::TUTORIAL_1_0},
    {"tutorial_1_1", ROOM_TYPE::TUTORIAL_1_1},
    {"tutorial_1_2", ROOM_TYPE::TUTORIAL_1_2},
    {"tutorial_1_3", ROOM_TYPE::TUTORIAL_1_3},
    {"tutorial_2_0", ROOM_TYPE::TUTORIAL_2_0},
    {"tutorial_2_1", ROOM_TYPE::TUTORIAL_2_1},
    {"tutorial_2_2", ROOM_TYPE::TUTORIAL_2_2},
    {"tutorial_2_3", ROOM_TYPE::TUTORIAL_2_3},
    {"tutorial_3_0", ROOM_TYPE::TUTORIAL_3_0},
    {"tutorial_3_1", ROOM_TYPE::TUTORIAL_3_1},
    {"tutorial_3_2", ROOM_TYPE::TUTORIAL_3_2},
    {"tutorial_3_3", ROOM_TYPE::TUTORIAL_3_3},

    {"shop_general_flipped", ROOM_TYPE::SHOP_GENERAL_FLIPPED},
    {"shop_specialty_flipped", ROOM_TYPE::SHOP_SPECIALTY_FLIPPED},
    {"shop_clothing_flipped", ROOM_TYPE::SHOP_CLOTHING_FLIPPED},
    {"shop_bombs_flipped", ROOM_TYPE::SHOP_BOMBS_FLIPPED},
    {"shop_weapons_flipped", ROOM_TYPE::SHOP_WEAPONS_FLIPPED},
    {"shop_kissing_flipped", ROOM_TYPE::SHOP_KISSING_FLIPPED},
    {"shop_hiredhand_flipped", ROOM_TYPE::SHOP_HIREDHAND_FLIPPED},
    {"shop_prize_or_ankh_flipped", ROOM_TYPE::SHOP_PRIZE_OR_ANKH_FLIPPED},
} };

struct GlobalState {
    char off[4457940];
    uint32_t level;
    uint32_t level_track;
    char off1[6];
    bool undef_flag1;
    bool undef_flag2;
    char dark_level;
    char altar_spawned;
    char idol_spawned;
    char damsel_spawned;
    char unknown_flag;
    char moai_unopened;
    char moai_broke_in;
    char ghost_spawned;
    char rescued_damsel;
    char shopkeeper_triggered;
    char area_had_dark_level;
    char level_has_udjat;
    char has_spawned_udjat;
    char unused_flag;
    char vault_spawned_in_area;
    char flooded_mines;
    char skin_is_crawling;
    char dead_are_restless;
    char rushing_water;
    char is_haunted_castle;
    char at_haunted_castle_exit;
    char tiki_village;
    char has_spawned_bm_entrance;
    char unused_flag2;
    char haunted_castle_entrance_spawned;
    char mothership_spawned;
    char moai_spawned;
    char is_blackmarket;
    char at_blackmarket_exit;
    char is_wet_fur;
    char is_mothership;
    char at_mothership_exit;
    char is_city_of_gold;
    char at_city_of_gold_exit;
    char is_worm;
};

struct LevelState {
    DWORD entity_backgrounds[512];
    uint32_t entity_backgrounds_count;
    uint32_t unknown_count;
    DWORD entity_items[720];
    uint32_t entity_items_count;
    DWORD entity_floors[4692];
    DWORD entity_floors2[4692];
    ROOM_TYPE room_types[48];
    char unknown[51888];//(52080-200)+8];
    GlobalState* globalState;
};

const DWORD GETROOM_OFFSET = 0xd6690;
const DWORD GETROOM_GET_ROOM_OFF = GETROOM_OFFSET + 0x29;
const DWORD GETROOM_SPAWN_ROOM_OFF = GETROOM_OFFSET + 0x174;
//const DWORD GLOBAL_STATE_OFF = 0x15446C;
const DWORD SPAWN_BORDERTILES_ROOMS = 0xdd760;

void* roomGenFunc{nullptr};

DWORD GetBaseAddress()
{
    return (DWORD)GetModuleHandle(NULL);
}

using PreSpawnRoomsFunc = void* __stdcall (LevelState* levelState);
using RoomList = std::map<ROOM_TYPE, std::vector<std::string>>;

PreSpawnRoomsFunc* spawnBordertilesRoomsEtc = {nullptr};
DWORD getRoomGet = { GETROOM_GET_ROOM_OFF + GetBaseAddress() };
DWORD getRoomSpawn = { GETROOM_SPAWN_ROOM_OFF + GetBaseAddress() };
//GlobalState* globalState = { (GlobalState*)(*(DWORD*)(GLOBAL_STATE_OFF + GetBaseAddress())) };

RoomList rooms;

void readFileLevel(std::string filename, RoomList& ret) {
    ret.clear();
    std::ifstream levelFile(filename);
    std::string line_str;
    ROOM_TYPE current_room_id;
    std::string current_room = "";
    uint32_t room_line = 0;
    while (std::getline(levelFile, line_str))
    {
        line_str.erase(std::remove_if(line_str.begin(), line_str.end(), isspace), line_str.end());
        std::regex remove_comment("//.*");
        line_str = std::regex_replace(line_str, remove_comment, "");
        //std::cout << line_str << "\n";
        if (line_str.rfind("\\.", 0) == 0) {
            std::string room_template = line_str.substr(2, line_str.length());
            auto search = roomMap.find(room_template);
            if (search != roomMap.end()) {
                current_room_id = search->second;
            }
            else {
                //std::cout << "ERROR\n";
            }
        }
        else if (line_str.length() == 10) {
            current_room.append(line_str);
            ++room_line;
            if (room_line == 8) {
                ret[current_room_id].push_back(current_room);
                current_room = "";
                room_line = 0;
            }
        }
    }
    levelFile.close();
    /*std::cout << "\n----\n";
    for (const auto& p : ret)
    {
        std::cout << p.first << "\n";
        for (const auto s : p.second) {
            std::cout << s << "\n";
        }
    }*/
}

bool trySetRoom(ROOM_TYPE roomType, char* roomOut) {
    auto search = rooms.find(roomType);
    if (search != rooms.end()) {
        auto room_vec = search->second;
        int i = rand() % room_vec.size();
        room_vec[i].copy(roomOut, 80);
        roomOut[80] = '\0';
        return true;
    }
    return false;
}

bool customRoomGet(int doorRoomType, int roomIndex, char* roomOut, LevelState* levelState) {
    //readFileLevel("level.lvl", rooms);
    ROOM_TYPE roomType = levelState->room_types[roomIndex];
    ROOM_TYPE aboveRoom = roomIndex > 3 ? levelState->room_types[roomIndex - 4] : ROOM_TYPE::UNSET;
    if (doorRoomType == 1) {
        if (roomType == ROOM_TYPE::PATH_DROP) {
            return trySetRoom(ROOM_TYPE::ENTRANCE_DROP, roomOut);
        }
        else {
            return trySetRoom(ROOM_TYPE::ENTRANCE, roomOut);
        }
    }
    else if (doorRoomType == 2) {
        if (aboveRoom == ROOM_TYPE::PATH_DROP) {
            return trySetRoom(ROOM_TYPE::EXIT_NOTOP, roomOut);
        }
        else {
            return trySetRoom(ROOM_TYPE::EXIT, roomOut);
        }
    }
    else {
        //todo: test
        GlobalState* globalState = levelState->globalState;
        if (roomType == ROOM_TYPE::SIDE) {
            if (globalState->level > 1)
            {
                if (globalState->altar_spawned == 0 && rand() % 14 == 0) {
                    globalState->altar_spawned = 1;
                    return trySetRoom(ROOM_TYPE::ALTAR, roomOut);
                }
                else if (globalState->idol_spawned == 0 && 11 < roomIndex && rand() % 10 == 0)
                {
                    globalState->idol_spawned = 1;
                    return trySetRoom(ROOM_TYPE::IDOL, roomOut);
                }
            }
            return trySetRoom(ROOM_TYPE::SIDE, roomOut);
        }
        else if (roomType == ROOM_TYPE::PATH_DROP &&
            (aboveRoom == ROOM_TYPE::PATH_DROP || aboveRoom == ROOM_TYPE::COFFIN_DROP)) {
            return trySetRoom(ROOM_TYPE::PATH_DROP_NOTOP, roomOut);
        }
        else {
            return trySetRoom(roomType, roomOut);
        }
    }
    //char chr[81];
    //rooms[-1][0].copy(chr, 80);
    //chr[80] = '\0';
    //std::cout << chr;
    return false;
}

void __stdcall preSpawnRoomsHook(LevelState *levelState) {
    readFileLevel("level.lvl", rooms);
    spawnBordertilesRoomsEtc(levelState);
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
        test al, al
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
        spawnBordertilesRoomsEtc = (PreSpawnRoomsFunc*)(SPAWN_BORDERTILES_ROOMS + GetBaseAddress());
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach((void**)&roomGenFunc, hookThing);
        DetourAttach((void**)&spawnBordertilesRoomsEtc, preSpawnRoomsHook);

        const LONG error = DetourTransactionCommit();
    }

    return TRUE;
}