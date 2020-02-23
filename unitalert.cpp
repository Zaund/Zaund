#include "pch.h"
#include "windows.h"
#include "memoryAPI.h"
#include <iostream>

using namespace std;

const int playerNumberADDR = 0x4FC734; // Player number address
const int jumpRet = 0x40EF63; // Jump back address

// Hack state
int unitAlertstate = 1;

// Unit type variables
int unitState; // 1 for created ?? for killed
int playerNumber; // Your player number
int buildingPlayernumb; // Player building the unit
int unitType; // Type of unit Maybe needs to be hex?
int totalUnit; // Total amount of x unit commanded

// Terran buildings
char commandCenter[] = "Command Center";
char supplyDepot[] = "Supply Depot";
char extractor[] = "Extractor";
char barracks[] = "Barracks";
char engineeringBay[] = "Engineering Bay";
char missleTurret[] = "Missle Turret";
char academy[] = "Academy";
char bunker[] = "Bunker";
char factory[] = "Factory";
char machineShop[] = "Machine Shop";
char controlTower[] = "Control Tower";
char scienceFacility[] = "Science Facility";
char covertOps[] = "Covert Ops";
char physicsLab[] = "Physics Lab";
char armory[] = "Armory";

// Terran units
char SCV[] = "SCV";
char marine[] = "Marine";
char firebat[] = "Firebat";
char ghost[] = "Ghost";
char medic[] = "Medic";
char vulture[] = "Vulture";
char stSiege[] = "Siege Tank(Siege Mode)";
char stTank[] = "Siege Tank(Tank mode)";
char goliath[] = "Goliath";
char wraith[] = "Wraith";
char dropship[] = "Dropship";
char scienceVessel[] = "Science Vessel";
char battleCruiser[] = "Battlecruiser";
char vulcan[] = "Vulcan";
char nuke[] = "Nuke";

// Protoss buildings
char nexus[] = "Nexus";
char pylon[] = "Pylon";
char assimilator[] = "Assimilator";
char gateway[] = "Gateway";
char forge[] = "Forge";
char cannon[] = "Cannon";
char cyberneticsCore[] = "Cybernetics Core";
char shieldBattery[] = "Shield Battery";
char roboticsFacility[] = "Robotics Facility";
char stargate[] = "Stargate";
char citadelofAdun[] = "Citadel of Adun";
char roboticsSupportbay[] = "Robotics Support Bay";
char fleetBeacon[] = "Fleet Beacon";
char templarArchives[] = "Templar Archives";
char observatory[] = "Observatory";
char arbiterTribunal[] = "Arbiter Tribunal";

// Protoss units
char probe[] = "Probe";
char zealot[] = "Zealot";
char dragoon[] = "Dragoon";
char highTemplar[] = "High Templar";
char darkTemplar[] = "Dark Templar";
char reaver[] = "Reaver";
char shuttle[] = "Shuttle";
char observer[] = "Observer";
char scout[] = "Scout";
char arbiter[] = "Arbiter";
char carrier[] = "Carrier";
char corsair[] = "Corsair";

// Zerg buildings
char hatchery[] = "Hatchery";
char lair[] = "Lair";
char hive[] = "Hive";
char infestedCC[] = "infested Command Center";
char evolutionChamber[] = "Evolution Chamber";
char spawningPool[] = "Spawning Pool";
char hydraliskDen[] = "Hydralisk Den";
char spire[] = "Spire";
char greaterSpire[] = "Greater Spire";
char queensLair[] = "Queens Lair";
char nydusCanal[] = "Nydus Canal";
char ultraliskCavern[] = "Ultralisk Cavern";
char defilerMound[] = "Defiler Mound";

// Zerg units
char drone[] = "Drone";
char zergling[] = "Zergling";
char hydralisk[] = "Hydralisk";
char lurker[] = "Lurker";
char mutalisk[] = "Mutalisk";
char guardian[] = "Guardian";
char ultralisk[] = "Ultralisk";
char queen[] = "Queen";
char defiler[] = "Defiler";

void buildMsg(char* unitIs, int totalUnit, int playerNumber) {
    int colorGreenint = 07;

    char colorGreen = colorGreenint;
    char msgBuf[50];

    sprintf_s(msgBuf, 50, "%chas created %s (%d total)", colorGreen, unitIs, totalUnit);
    playerText(msgBuf, playerNumber);
}

void unitAlertstateMsg(int unitAlertstate) {
    int colorYellowint = 03;
    int colorGreyint = 05;
    int colorGreenint = 07;
    int colorRedint = 06;

    char colorYellow = colorYellowint;
    char colorGrey = colorGreyint;
    char colorGreen = colorGreenint;
    char colorRed = colorRedint;

    char msgBuf[50];
    __asm { PUSHAD }
    if (unitAlertstate == 0) { sprintf_s(msgBuf, 50, "%cUnit Alert: %cOFF", colorYellow, colorGrey); }
    else if (unitAlertstate == 1) { sprintf_s(msgBuf, 50, "%cUnit Alert: %cLIGHT", colorYellow, colorGreen); }
    else if (unitAlertstate == 2) { sprintf_s(msgBuf, 50, "%cUnit Alert: %cFULL", colorYellow, colorRed); }
    __asm { POPAD }
    centerText(msgBuf);
}

void pickState() {
    //__asm { pushad }
    if (unitAlertstate == 0) { unitAlertstate = 1; unitAlertstateMsg(unitAlertstate); }
    else if (unitAlertstate == 1) { unitAlertstate = 2; unitAlertstateMsg(unitAlertstate); }
    else if (unitAlertstate == 2) { unitAlertstate = 0; unitAlertstateMsg(unitAlertstate); }
    //__asm { popad }
}

void __declspec(naked) unitAlert() {
    __asm {pushad} // Push all registers to the stack
    if (unitAlertstate == 0) {
        __asm {
            popad // Restore all registers from the stack
            MOV DWORD PTR DS : [EDI * 4 + 0x50B94C] , EBX
            JMP jumpRet
        }
    }
    __asm {
        mov totalUnit, ebx // Get total unit count
        mov unitState, ecx // Get killed or created
        mov buildingPlayernumb, esi // Building player number
        push esi
        mov esi, DWORD PTR DS : [0x4FC734] // Your player number
        mov playerNumber, esi
        pop esi
    }

    if (unitState == 1) { // check if unit is created or killed
        __asm push eax

        if (playerNumber != buildingPlayernumb) { // check if it is your unit
            __asm {
                pop eax
                mov unitType, eax
            }
            // Terran buildings
            if (unitType == 0x6A) { buildMsg(commandCenter, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xD6 && unitAlertstate == 2) { buildMsg(supplyDepot, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xBC && unitAlertstate == 2) { buildMsg(extractor, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x6F) { buildMsg(barracks, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x7A && unitAlertstate == 2) { buildMsg(engineeringBay, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x7C && unitAlertstate == 2) { buildMsg(missleTurret, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x70) { buildMsg(academy, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x7D) { buildMsg(bunker, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x71 && unitAlertstate == 2) { buildMsg(factory, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x78) { buildMsg(machineShop, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x72) { buildMsg(stargate, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x73 && unitAlertstate == 2) { buildMsg(controlTower, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x74 && unitAlertstate == 2) { buildMsg(scienceFacility, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x75) { buildMsg(covertOps, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x76) { buildMsg(physicsLab, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x7B && unitAlertstate == 2) { buildMsg(armory, totalUnit, buildingPlayernumb); }

            // Terran units
            else if (unitType == 0x07) { buildMsg(SCV, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x00) { buildMsg(marine, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x20) { buildMsg(firebat, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x01) { buildMsg(ghost, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x22 && unitAlertstate == 2) { buildMsg(medic, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x02 && unitAlertstate == 2) { buildMsg(vulture, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x1E) { buildMsg(stSiege, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x03) { buildMsg(goliath, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x08 && unitAlertstate == 2) { buildMsg(wraith, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x0B && unitAlertstate == 2) { buildMsg(dropship, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x09 && unitAlertstate == 2) { buildMsg(scienceVessel, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x05) { buildMsg(stTank, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x0C) { buildMsg(battleCruiser, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x3A && unitAlertstate == 2) { buildMsg(vulcan, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x0E) { buildMsg(nuke, totalUnit, buildingPlayernumb); }

            // Protoss buildings
            else if (unitType == 0x9A) { buildMsg(nexus, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x9C && unitAlertstate == 2) { buildMsg(pylon, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x9D && unitAlertstate == 2) { buildMsg(assimilator, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA0) { buildMsg(gateway, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA6 && unitAlertstate == 2) { buildMsg(forge, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA2 && unitAlertstate == 2) { buildMsg(cannon, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA4 && unitAlertstate == 2) { buildMsg(cyberneticsCore, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xAC && unitAlertstate == 2) { buildMsg(shieldBattery, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x9B) { buildMsg(roboticsFacility, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA7) { buildMsg(stargate, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA3 && unitAlertstate == 2) { buildMsg(citadelofAdun, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xAB && unitAlertstate == 2) { buildMsg(roboticsSupportbay, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA9 && unitAlertstate == 2) { buildMsg(fleetBeacon, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xA5) { buildMsg(templarArchives, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x9F && unitAlertstate == 2) { buildMsg(observatory, totalUnit, buildingPlayernumb); }
            else if (unitType == 0xAA) { buildMsg(arbiterTribunal, totalUnit, buildingPlayernumb); }

            // Protoss units
            else if (unitType == 0x40) { buildMsg(probe, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x41) { buildMsg(zealot, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x42) { buildMsg(dragoon, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x43) { buildMsg(highTemplar, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x3D) { buildMsg(darkTemplar, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x53) { buildMsg(reaver, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x45 && unitAlertstate == 2) { buildMsg(shuttle, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x54) { buildMsg(observer, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x46 && unitAlertstate == 2) { buildMsg(scout, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x47) { buildMsg(arbiter, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x48) { buildMsg(carrier, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x3C && unitAlertstate == 2) { buildMsg(corsair, totalUnit, buildingPlayernumb); }

            // Zerg buildings
            else if (unitType == 0x82) { buildMsg(infestedCC, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x83) { buildMsg(hatchery, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x84) { buildMsg(lair, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x85) { buildMsg(hive, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x8B && unitAlertstate == 2) { buildMsg(evolutionChamber, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x8E) { buildMsg(spawningPool, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x87) { buildMsg(hydraliskDen, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x8D) { buildMsg(spire, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x89) { buildMsg(greaterSpire, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x8A) { buildMsg(queensLair, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x86) { buildMsg(ultraliskCavern, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x8C) { buildMsg(nydusCanal, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x88) { buildMsg(defilerMound, totalUnit, buildingPlayernumb); }

            // Zerg units
            else if (unitType == 0x29) { buildMsg(drone, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x25) { buildMsg(zergling, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x26) { buildMsg(hydralisk, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x67) { buildMsg(lurker, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x2D) { buildMsg(mutalisk, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x2C) { buildMsg(guardian, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x53) { buildMsg(reaver, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x2D) { buildMsg(queen, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x27) { buildMsg(ultralisk, totalUnit, buildingPlayernumb); }
            else if (unitType == 0x2E) { buildMsg(defiler, totalUnit, buildingPlayernumb); }
        }
        else { __asm pop eax }
    }
    __asm {
        popad // Restore all registers from the stack
        MOV DWORD PTR DS : [EDI * 4 + 0x50B94C] , EBX
        JMP jumpRet
    }
}