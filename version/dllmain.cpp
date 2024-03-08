// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"
#include "version.h"
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include "Memory.h"
#include "GameData.h"
#include <conio.h> // For _getch() on Windows
#include "xorstr.hpp"


HANDLE hMainThread;
HMODULE g_hModule;

// Function to initialize and show the console window
void InitializeConsole() {
    FILE* pFile = nullptr;
    AllocConsole();
    //SetConsoleTitle(_XOR_(L"666"));
    freopen_s(&pFile, _XOR_("CONOUT$"), _XOR_("w"), stdout);
}

void DestroyConsole()
{
    DestroyWindow(GetConsoleWindow());
    FreeConsole();
}

//Check Key Pressed
bool keyPressed(int vKey)
{
    return (GetAsyncKeyState(vKey) & 1);
}

//Console Menu using checkbox
struct Checkbox {
    std::string title;
    bool checked;
};

// Function to display checkboxes
void displayCheckboxes(const std::vector<Checkbox>& checkboxes, size_t selectedCheckbox) {
    system("cls"); // Clear the console (Windows specific)


    std::cout << _XOR_("Checkboxes:\n");
    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (i == selectedCheckbox) {
            std::cout << "> ";
        }
        else {
            std::cout << "  ";
        }

        std::cout << "[" << (checkboxes[i].checked ? 'X' : ' ') << "] " << checkboxes[i].title << "\n";
    }
    std::cout << "\n";
}

DWORD WINAPI Payload(LPVOID lpParam)
{
    // Initialize the proxy for the DLL
    HMODULE dllfwd = dllforward::setup();
    if (dllfwd)
        CloseHandle(dllfwd);

    //Console Menu
    std::vector<Checkbox> checkboxes = { 
          {_XOR_("1"), false}
        , {_XOR_("2"), false}
        , {_XOR_("3"), false}
        , {_XOR_("4"), false}
        , {_XOR_("5"), false}
    
    }; // Initialize all checkboxes to unchecked
    const int numCheckboxes = checkboxes.size();
    size_t selectedCheckbox = 0;
    char userInput;

    HMODULE moduleHandle = nullptr;
    GameData gData;

    do
    {
        moduleHandle = GetModuleHandle(_XOR_(L"game.dll"));
        Sleep(1000);
    } while (!moduleHandle);
    Sleep(100);


    //Show Console
    InitializeConsole();
    
    do {

        displayCheckboxes(checkboxes, selectedCheckbox);

        // Get user input
        userInput = _getch(); // Use _getch() for reading a single character without pressing Enter

        // Handle arrow key input
        switch (userInput) {
        case 72: // Up arrow key
            selectedCheckbox = (selectedCheckbox == 0) ? numCheckboxes - 1 : selectedCheckbox - 1;
            break;

        case 80: // Down arrow key
            selectedCheckbox = (selectedCheckbox == numCheckboxes - 1) ? 0 : selectedCheckbox + 1;
            break;

        case ' ': // Space bar
            // Toggle the state of the selected checkbox
            checkboxes[selectedCheckbox].checked = !checkboxes[selectedCheckbox].checked;
            break;

        case 13: // Enter key
            std::cout << _XOR_("Activate Cheat Feature.\n");
            break;

        case 27: // Esc key

            break;

        default:
            // Ignore other key presses
            break;
        }

    } while (userInput != 13); // Loop until the Esc key is pressed

    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (checkboxes[i].checked)
        {


            if (checkboxes[i].title == _XOR_("1"))
            {
                if (!gData.InfGrenadesLegit && !gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("4D 03 C6 41 FF 08"));
                    BYTE GrenadeBytes[] =
                    {
                        0x4D, 0x01, 0xF0,                               //add r8,r14
                        0x41, 0x83, 0x38, 0x01,                         //cmp dword ptr [r8],01
                        0x74, 0x03,                                     //je short @f
                        0x41, 0xFF, 0x08,                               // dec [r8]
                        0x4A, 0x8B, 0x84, 0xED, 0x30, 0x20, 0x00, 0x00, // mov rax,[rbp+r13*8+00002030]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Grenades
                    };
                    LPVOID memory = Memory::AllocateMemory(GrenadesAddress, sizeof(GrenadeBytes));
                    Memory::CreateTrampoline(GrenadesAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, GrenadesAddress + 14, GrenadeBytes, Memory::ArrayLength(GrenadeBytes));
                    gData.InfGrenadesLegit = !gData.InfGrenadesLegit;
                    printf(_XOR_("[Active] Infinite Grenades(Legit)\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("2"))
            {
                if (!gData.ShowAllMapIcons)
                {
                    BYTE ShowAllMapIconsByte[] =
                    {
                        0xB8, 0x01, 0x00, 0x00, 0x00, 0x90
                    };

                    BYTE ShowAllMapIconsByte1[] =
                    {
                        0xF8
                    };

                    BYTE ShowAllMapIconsByte2n4[] =
                    {
                        0x90, 0xE9
                    };

                    BYTE ShowAllMapIconsByte3[] =
                    {
                        0xEB, 0x04
                    };

                    uintptr_t ShowAllMapIconsAddr = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 0F B6 44 97 23"));
                    uintptr_t aob_CheckIfAlienHivesAreObstructed = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 80 BE 3C BA 07 00 00"));
                    uintptr_t aob_CheckIfMinorInterestBlipIsDiscovered = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 85 ?? ?? ?? ?? 48 8B 44 24 ?? 80 78 29 00"));
                    uintptr_t aob_GetMinorInterestBlipIcon = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 84 ?? ?? ?? ?? 48 8B 4C 24 ?? F3 41 0F 10 4F"));
                    uintptr_t aob_CheckMissionBlip = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 85 59 02 00 00 49 8D"));
                     
                    Memory::Patch((LPVOID)(ShowAllMapIconsAddr), ShowAllMapIconsByte, 6);
                    Memory::Patch((LPVOID)(aob_CheckIfAlienHivesAreObstructed), ShowAllMapIconsByte1, 1);
                    Memory::Nop((LPVOID)(aob_CheckIfAlienHivesAreObstructed+1), 7);
                    Memory::Patch((LPVOID)(aob_CheckIfMinorInterestBlipIsDiscovered), ShowAllMapIconsByte2n4, 2);
                    Memory::Patch((LPVOID)(aob_GetMinorInterestBlipIcon), ShowAllMapIconsByte3, 2);
                    Memory::Patch((LPVOID)(aob_CheckMissionBlip), ShowAllMapIconsByte2n4, 2);
                    gData.ShowAllMapIcons = !gData.ShowAllMapIcons;
                    printf(_XOR_("[Active] Show All Map Icons\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("3"))
            {
                if (!gData.AllStratagems)
                {
                    BYTE AllStratagemsByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllStratagems = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 89 5C 24 ?? 48 8B D9 85 D2 75 09"));
                    Memory::Patch((LPVOID)(AllStratagems), AllStratagemsByte, 3);
                    gData.AllStratagems = !gData.AllStratagems;
                    printf(_XOR_("[Active] Unlock All Stratagems\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("4"))
            {
                if (!gData.AllEquipment)
                {
                    BYTE AllEquipmentByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllEquipment = Memory::FindPattern(_XOR_("game.dll"), _XOR_("83 B9 ?? ?? ?? ?? ?? 75 ?? 85 D2 74 ?? 44 8B 89 ?? ?? ?? ?? 45 33 C0 45 85 C9 74 ?? 48 8D 81 ?? ?? ?? ?? 39 50 ?? 74 ?? 41 FF C0 48 83 C0 ?? 45 3B C1 72 ?? 32 C0 C3 8B 00 48 69 C8"));
                    Memory::Patch((LPVOID)(AllEquipment+11), AllEquipmentByte, 3);
                    gData.AllEquipment = !gData.AllEquipment;
                    printf(_XOR_("[Active] Unlock All Equipment\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("5"))
            {
                if (!gData.AllArmor)
                {
                    BYTE AllArmorByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllArmor = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 83 EC ?? 44 8B 49 ?? 45 33 C0"));
                    Memory::Patch((LPVOID)(AllArmor), AllArmorByte, 3);
                    gData.AllArmor = !gData.AllArmor;
                    printf(_XOR_("[Active] Unlock All Armor\n"));
                }
            }

            /*if (checkboxes[i].title == "One / Two Hit Kill ( Bile Titan Bug, Aim Only Head )")
            {
                if (!gData.OHK)
                {
                    BYTE OHKByte[] =
                    {
                        0x83, 0xBF, 0x38, 0x0B, 0x00, 0x00, 0x0A,
                        0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,
                        0xE9, 0x18, 0x00, 0x00, 0x00,
                        0xC7, 0x87, 0x44, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x66, 0x4A, 0x6B, 0x80, 0x01, 0x00, 0x00, 0x00,
                        0x89, 0x87, 0x44, 0x64, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    uintptr_t OHK = Memory::FindPattern("game.dll", "89 87 44 64 00 00");
                    LPVOID memory = Memory::AllocateMemory(OHK, 0x100);
                    Memory::CreateTrampoline(OHK, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, OHK + 15, OHKByte, Memory::ArrayLength(OHKByte));
                    gData.OHK = !gData.OHK;
                    printf("[Active] Instant Railgun\n");
                }
            }*/
            


        }
    }
    printf(_XOR_("[Exit] Unload\n"));
    FreeConsole();
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        g_hModule = hModule;
        hMainThread = CreateThread(NULL, 0, Payload, hModule, 0, NULL);
        if (hMainThread)
            CloseHandle(hMainThread);
    }break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}

