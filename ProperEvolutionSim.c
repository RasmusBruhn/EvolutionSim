#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <Random.h>
#include <Settings.h>

// Errors
#define ERR_PREFIX MAIN
#include <Error.h>

enum MAIN_ErrorID {
    MAIN_ERRORID_NONE = 0x0,
    MAIN_ERRORID_LOADSETTINGS_MALLOC = 0x00010200,
    MAIN_ERRORID_LOADSETTINGS_LOAD = 0x00010201,
    MAIN_ERRORID_LOADSETTINGS_TRANSLATE = 0x00010202,
    MAIN_ERRORID_CREATEMAP_MALLOC = 0x00020200,
    MAIN_ERRORID_CREATEMAP_MALLOCTILES = 0x00020201,
    MAIN_ERRORID_CREATEMAP_CREATEPLANT = 0x00020202,
    MAIN_ERRORID_CREATEMAP_TILEENERGY = 0x00020203,
    MAIN_ERRORID_CREATEMAP_WIDTH = 0x00020204,
    MAIN_ERRORID_CREATEMAP_HEIGHT = 0x00020205,
    MAIN_ERRORID_CLEANPLANT_INTILE = 0x00030100,
    MAIN_ERRORID_CLEANPLANT_INMAP = 0x00030101,
    MAIN_ERRORID_CREATEPLANT_MALLOC = 0x00040200,
    MAIN_ERRORID_CREATEPLANT_TILE = 0x00040201,
    MAIN_ERRORID_CREATEPLANT_MAP = 0x00040202,
    MAIN_ERRORID_REMOVEFROMTILE_INTILE = 0x00050200,
    MAIN_ERRORID_REMOVEFROMTILE_PLANTLIST = 0x00050201,
    MAIN_ERRORID_REMOVEFROMMAP_INMAP = 0x00060200,
    MAIN_ERRORID_REMOVEFROMMAP_PLANTLIST = 0x00060201,
    MAIN_ERRORID_ADDTOTILE_REALLOC = 0x00070200,
    MAIN_ERRORID_ADDTOMAP_REALLOC = 0x00080200
};

#define MAIN_ERRORMES_MALLOC "Unable to allocate memory (Size: %llu)"
#define MAIN_ERRORMES_REALLOC "Unable to reallocate memory (Size: %llu)"
#define MAIN_ERRORMES_LOADSETTINGS "Unable to load settings (FileName: %s)"
#define MAIN_ERRORMES_TRANSLATESETTINGS "Unable to translate settings (FileName: %s)"
#define MAIN_ERRORMES_PLANTINTILE "Unable to locate plant in tiles plant list"
#define MAIN_ERRORMES_PLANTINMAP "Unable to locate plant in maps plant list"
#define MAIN_ERRORMES_GENERATEPLANT "Unable to generate plant (ID: %llu)"
#define MAIN_ERRORMES_ENERGYMETHOD "Uknown energy method (Method: %s)"
#define MAIN_ERRORMES_LOWWIDTH "The width of the map is too low"
#define MAIN_ERRORMES_LOWHEIGHT "The height of the map is too low"
#define MAIN_ERRORMES_ADDTOTILE "Unable to add plant to tile"
#define MAIN_ERRORMES_ADDTOMAP "Unable to add plant to map"
#define MAIN_ERRORMES_REMOVEPLANTFROMTILE "Unable to remove plant from tile"
#define MAIN_ERRORMES_REMOVEPLANTFROMMAP "Unable to remove plant from map"
#define MAIN_ERRORMES_NULLPLANTLIST "Plant list is NULL"

// Settings
typedef struct __MAIN_Settings MAIN_Settings;
typedef struct __MAIN_GeneConstraints MAIN_GeneConstraints;
typedef struct __MAIN_UintConstraint MAIN_UintConstraint;
typedef struct __MAIN_IntConstraint MAIN_IntConstraint;
typedef struct __MAIN_FloatConstraint MAIN_FloatConstraint;
typedef struct __MAIN_MapSettings MAIN_MapSettings;
typedef struct __MAIN_InitialSettings MAIN_InitialSettings;
typedef struct __MAIN_EnergySettings MAIN_EnergySettings;
typedef struct __MAIN_Size MAIN_Size;
typedef struct __MAIN_Map MAIN_Map;
typedef struct __MAIN_Tile MAIN_Tile;
typedef struct __MAIN_Plant MAIN_Plant;
typedef struct __MAIN_Gene MAIN_Gene;
typedef struct __MAIN_PlantStats MAIN_PlantStats;

struct __MAIN_UintConstraint {
    uint64_t min;    // The minimum allowed value
    uint64_t max;    // The maximum allowed value
    uint64_t mean;   // The mean starting value
    uint64_t spread; // The spread of the starting value
};

struct __MAIN_IntConstraint {
    int64_t min;    // The minimum allowed value
    int64_t max;    // The maximum allowed value
    int64_t mean;   // The mean starting value
    int64_t spread; // The spread of the starting value
};

struct __MAIN_FloatConstraint {
    double min;    // The minimum allowed value
    double max;    // The maximum allowed value
    double mean;   // The mean starting value
    double spread; // The spread of the starting value
};

struct __MAIN_GeneConstraints {
    MAIN_UintConstraint maxHeight; // The maximum height of the plant
    MAIN_UintConstraint maxSize; // The maximum size of the plant
    MAIN_FloatConstraint efficiency; // The effieciency at collecting energy
    MAIN_FloatConstraint growthRateHeight; // The probability the plant will grow in height if it can
    MAIN_FloatConstraint growthRateSize; // The probability the plant will grow in size if it can
    MAIN_UintConstraint minGrowthEnergyHeight; // The minimum energy required to grow in height
    MAIN_UintConstraint minGrowthEnergySize; // The minimum energy required to grow in size
    MAIN_FloatConstraint spawnRate; // The probability of spawning if it can
    MAIN_UintConstraint minSpawnEnergy; // The minimum energy required to spawn
    MAIN_UintConstraint maxTileEnergy; // The maximum amount of energy allowed to store per tile
    MAIN_UintConstraint spawnEnergy; // The amount of energy to give each seed
    MAIN_UintConstraint spawnSize; // The average number of seeds per spawning
    MAIN_UintConstraint spawnSpread; // The spread of the number of seeds per spawning
    MAIN_FloatConstraint mutationRate; // The probability that a mutation occurs during spawning
    MAIN_UintConstraint mutationAttempts; // The number of attempts to do a mutation during spawning
};

struct __MAIN_MapSettings {
    uint64_t width; // The width of the map
    uint64_t height; // The height of the map
    uint64_t minEnergy; // The minimum energy production per tile
    uint64_t maxEnergy; // The maximum energy production per tile
    char *energyMethod; // The method to calculate the energy
};

struct __MAIN_InitialSettings {
    uint64_t count; // The number of plant to create at the beginning
    uint64_t seed; // The seed for the simulation
    uint64_t energy; // The starting energy of all the initial plants
};

struct __MAIN_EnergySettings {
    double storageRate; // The rate of energy loss due to storage
    double storagePow; // The power of the storage loss
    double baseRate; // The rate of energy loss due to the base loss
    double heightPow; // The power of the height loss
    double sizePow; // The power of the size loss
    double effPow; // The power of the efficiency loss
    double growthBase; // The energy used in base growth
    double growthStorage; // The energy used in storage growth
};

struct __MAIN_Settings {
    MAIN_GeneConstraints geneConstraints; // All the gene constraints
    MAIN_MapSettings map;                 // All the map settings
    MAIN_InitialSettings init;            // The settings for the initial population
    MAIN_EnergySettings energy;           // The settings for the energy usage
    bool killIlligal;                     // If true then it should kill any plant created with illigal genes, if false they should be trunctated
};

struct __MAIN_Size {
    uint64_t w; // The width
    uint64_t h; // The height
};

struct __MAIN_Map {
    const MAIN_Settings *settings; // The settings of the map
    MAIN_Tile *tiles; // All of the tiles for the map
    MAIN_Size size; // The size of the map
    MAIN_Plant **plantList; // A list with all of the plants on the map with the oldest first
    size_t plantCount; // The number of plants in the plant list
    uint64_t time; // The tick counter which keeps going up
    uint64_t random; // A random number used to generate more random numbers
};

struct __MAIN_Tile {
    MAIN_Plant **plantList; // List of all the plants in this tile in order from tallest to lowest
    size_t plantCount; // The number of plants in the plant list
    uint64_t energy; // The energy production of the tile
};

struct __MAIN_Gene {
    uint64_t maxHeight;             // The maximum height of the plant
    uint64_t maxSize;               // The maximum size of the plant
    double efficiency;              // The effieciency at collecting energy
    double growthRateHeight;        // The probability the plant will grow in height if it can
    double growthRateSize;          // The probability the plant will grow in size if it can
    uint64_t minGrowthEnergyHeight; // The minimum energy required to grow in height
    uint64_t minGrowthEnergySize;   // The minimum energy required to grow in size
    double spawnRate;               // The probability of spawning if it can
    uint64_t minSpawnEnergy;        // The minimum energy required to spawn
    uint64_t maxTileEnergy;         // The maximum amount of energy allowed to store per tile
    uint64_t spawnEnergy;           // The amount of energy to give each seed
    uint64_t spawnSize;             // The average number of seeds per spawning
    uint64_t spawnSpread;           // The spread of the number of seeds per spawning
    double mutationRate;            // The probability that a mutation occurs during spawning
    uint64_t mutationAttempts;      // The number of attempts to do a mutation during spawning
};

struct __MAIN_PlantStats {
    uint64_t size; // The size of the plant
    uint64_t height; // The height of the plant
    uint64_t energy; // The energy storage of the plant
    uint64_t age; // At what random tick it was born
    uint64_t energyUsage; // The amount of energy to use per turn
    uint64_t maxEnergy; // The maximum amount of energy the plant can store
    uint64_t biomass; // The biomass of the plant
};

struct __MAIN_Plant {
    MAIN_Gene gene; // The genes for this plant
    MAIN_Tile **tileList; // A list of all the tiles this plant is within
    MAIN_PlantStats stats; // The stats of the plant
    MAIN_Map *map; // The map this plant belongs to
};

// Settings translation tables
#define MAIN_SETTINGSCONSTRAINTCOUNT 4
#define MAIN_SETTINGSGENECONSTRAINTCOUNT 15
#define MAIN_SETTINGSCOUNT 5
#define MAIN_SETTINGSMAPCOUNT 5
#define MAIN_SETTINGSINITCOUNT 3
#define MAIN_SETTINGSENERGYCOUNT 8

SET_TranslationTable MAIN_SettingsTableUintConstraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_UintConstraint, min)},
    {.name = "max", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_UintConstraint, max)},
    {.name = "mean", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_UintConstraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_UintConstraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableIntConstraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_IntConstraint, min)},
    {.name = "max", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_IntConstraint, max)},
    {.name = "mean", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_IntConstraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_IntConstraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableFloatConstraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, min)},
    {.name = "max", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, max)},
    {.name = "mean", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableGeneConstrains[MAIN_SETTINGSGENECONSTRAINTCOUNT] = {
    {.name = "maxHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxHeight), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "maxSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxSize), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "efficiency", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, efficiency), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "growthRateHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, growthRateHeight), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "growthRateSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, growthRateSize), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "minGrowthEnergyHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, minGrowthEnergyHeight), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "minGrowthEnergySize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, minGrowthEnergySize), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "spawnRate", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, spawnRate), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "minSpawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, minSpawnEnergy), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "maxTileEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxTileEnergy), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "spawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, spawnEnergy), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "spawnSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, spawnSize), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "spawnSpread", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, spawnSpread), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "mutationRate", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, mutationRate), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "mutationAttempts", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, mutationAttempts), .size = sizeof(MAIN_UintConstraint), .sub = MAIN_SettingsTableUintConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT}
};

SET_TranslationTable MAIN_SettingsTableMap[MAIN_SETTINGSMAPCOUNT] = {
    {.name = "width", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_MapSettings, width)},
    {.name = "height", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_MapSettings, height)},
    {.name = "minEnergy", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_MapSettings, minEnergy)},
    {.name = "maxEnergy", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_MapSettings, maxEnergy)},
    {.name = "energyMethod", .type = SET_DATATYPE_STR, .depth = 0, .offset = offsetof(MAIN_MapSettings, energyMethod)}
};

SET_TranslationTable MAIN_SettingsTableInit[MAIN_SETTINGSINITCOUNT] = {
    {.name = "count", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_InitialSettings, count)},
    {.name = "seed", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_InitialSettings, seed)},
    {.name = "energy", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_InitialSettings, energy)}
};

SET_TranslationTable MAIN_SettingsTableEnergy[MAIN_SETTINGSENERGYCOUNT] = {
    {.name = "storageRate", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, storageRate)},
    {.name = "storagePow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, storagePow)},
    {.name = "baseRate", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, baseRate)},
    {.name = "heightPow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, heightPow)},
    {.name = "sizePow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, sizePow)},
    {.name = "effPow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, effPow)},
    {.name = "growthBase", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, growthBase)},
    {.name = "growthStorage", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, growthStorage)}
};

SET_TranslationTable MAIN_SettingsTableMain[MAIN_SETTINGSCOUNT] = {
    {.name = "geneConstraints", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, geneConstraints), .size = sizeof(MAIN_GeneConstraints), .sub = MAIN_SettingsTableGeneConstrains, .count = MAIN_SETTINGSGENECONSTRAINTCOUNT},
    {.name = "map", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, map), .size = sizeof(MAIN_MapSettings), .sub = MAIN_SettingsTableMap, .count = MAIN_SETTINGSMAPCOUNT},
    {.name = "init", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, init), .size = sizeof(MAIN_InitialSettings), .sub = MAIN_SettingsTableInit, .count = MAIN_SETTINGSINITCOUNT},
    {.name = "energy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, energy), .size = sizeof(MAIN_EnergySettings), .sub = MAIN_SettingsTableEnergy, .count = MAIN_SETTINGSENERGYCOUNT},
    {.name = "killIlligal", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_Settings, killIlligal)}
};

// Energy method names
#define MAIN_ENERGYMETHOD_CONST "const"
#define MAIN_ENERGYMETHOD_LINEAR "linear"
#define MAIN_ENERGYMETHOD_COS "cos"
#define MAIN_ENERGYMETHOD_COS2 "cos2"

#define MAIN_PI 3.14159265359

// Functions
// Load a settings file
MAIN_Settings *MAIN_LoadSettings(const char *FileName);

// Create a new map
MAIN_Map *MAIN_CreateMap(const MAIN_Settings *Settings);

// Calculates the energy of the tiles using const method
void MAIN_TileEnergyConst(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size);

// Calculates the energy of the tiles using single period cos method
void MAIN_TileEnergyCos(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size);

// Calculates the energy of the tiles using double period cos method
void MAIN_TileEnergyCos2(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size);

// Generates a uint from constraints
uint64_t MAIN_GenerateUint(const MAIN_UintConstraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
int64_t MAIN_GenerateInt(const MAIN_IntConstraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
double MAIN_GenerateFloat(const MAIN_FloatConstraint *Constraint, uint64_t *Random);

// Generate a random gene
void MAIN_GenerateGene(MAIN_Map *Map, MAIN_Gene *Gene);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateUint(const MAIN_UintConstraint *Constraint, uint64_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateInt(const MAIN_IntConstraint *Constraint, int64_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateFloat(const MAIN_FloatConstraint *Constraint, double *Value);

// Truncate all values of a gene so that they are legal, returns true if it trancated anything
bool MAIN_TruncateGene(const MAIN_Settings *Settings, MAIN_Gene *Gene);

// Create a plant and mutates the parent genes for it
bool MAIN_CreatePlant(MAIN_Map *Map, MAIN_Tile *Tile, uint64_t Energy, const MAIN_Gene *ParentGene);

// Adds a plant to a tile
bool MAIN_AddToTile(MAIN_Tile *Tile, MAIN_Plant *Plant);

// Removes a plant from a tile
bool MAIN_RemoveFromTile(MAIN_Tile *Tile, const MAIN_Plant *Plant);

// Adds a plant to the map
bool MAIN_AddToMap(MAIN_Map *Map, MAIN_Plant *Plant);

// Removes a plant from the map
bool MAIN_RemoveFromMap(MAIN_Map *Map, const MAIN_Plant *Plant);

// Calculates the energy usage of a plant
uint64_t MAIN_EnergyUsage(const MAIN_Plant *Plant);

// Calculates the biomass for a plant
uint64_t MAIN_Biomass(const MAIN_Plant *Plant);


// Init functions
void MAIN_InitUintConstraint(MAIN_UintConstraint *Struct);
void MAIN_InitIntConstraint(MAIN_IntConstraint *Struct);
void MAIN_InitFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_InitGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_InitMapSettings(MAIN_MapSettings *Struct);
void MAIN_InitInitialSettings(MAIN_InitialSettings *Struct);
void MAIN_InitEnergySettings(MAIN_EnergySettings *Struct);
void MAIN_InitSettings(MAIN_Settings *Struct);
void MAIN_InitMap(MAIN_Map *Struct);
void MAIN_InitTile(MAIN_Tile *Struct);
void MAIN_InitPlant(MAIN_Plant *Struct);
void MAIN_InitPlantStats(MAIN_PlantStats *Struct);
void MAIN_InitGene(MAIN_Gene *Struct);
void MAIN_InitSize(MAIN_Size *Struct);

// Clean functions
void MAIN_CleanUintConstraint(MAIN_UintConstraint *Struct);
void MAIN_CleanIntConstraint(MAIN_IntConstraint *Struct);
void MAIN_CleanFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_CleanGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_CleanMapSettings(MAIN_MapSettings *Struct);
void MAIN_CleanInitialSettings(MAIN_InitialSettings *Struct);
void MAIN_CleanEnergySettings(MAIN_EnergySettings *Struct);
void MAIN_CleanSettings(MAIN_Settings *Struct);
void MAIN_CleanMap(MAIN_Map *Struct);
void MAIN_CleanTile(MAIN_Tile *Struct);
void MAIN_CleanPlant(MAIN_Plant *Struct);
void MAIN_CleanPlantStats(MAIN_PlantStats *Struct);
void MAIN_CleanGene(MAIN_Gene *Struct);
void MAIN_CleanSize(MAIN_Size *Struct);

// Destroy functions
void MAIN_DestroyUintConstraint(MAIN_UintConstraint *Struct);
void MAIN_DestroyIntConstraint(MAIN_IntConstraint *Struct);
void MAIN_DestroyFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_DestroyGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_DestroyMapSettings(MAIN_MapSettings *Struct);
void MAIN_DestroyInitialSettings(MAIN_InitialSettings *Struct);
void MAIN_DestroyEnergySettings(MAIN_EnergySettings *Struct);
void MAIN_DestroySettings(MAIN_Settings *Struct);
void MAIN_DestroyMap(MAIN_Map *Struct);
void MAIN_DestroyTile(MAIN_Tile *Struct);
void MAIN_DestroyPlant(MAIN_Plant *Struct);
void MAIN_DestroyPlantStats(MAIN_PlantStats *Struct);
void MAIN_DestroyGene(MAIN_Gene *Struct);
void MAIN_DestroySize(MAIN_Size *Struct);

MAIN_Settings *MAIN_LoadSettings(const char *FileName)
{
    // Get memory
    MAIN_Settings *Settings = (MAIN_Settings *)malloc(sizeof(MAIN_Settings));

    if (Settings == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOADSETTINGS_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Settings));
        return NULL;
    }

    MAIN_InitSettings(Settings);

    // Load the settings
    extern SET_TranslationTable MAIN_SettingsTableMain[];

    SET_DataStruct *SettingsDict = SET_LoadSettings(FileName);

    if (SettingsDict == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOADSETTINGS_LOAD, SET_GetError(), MAIN_ERRORMES_LOADSETTINGS, FileName);
        free(Settings);
        return NULL;
    }

    // Translate the settings
    if (!SET_Translate(Settings, SettingsDict, MAIN_SettingsTableMain, MAIN_SETTINGSCOUNT, SET_TRANSLATIONMODE_NONE))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOADSETTINGS_TRANSLATE, SET_GetError(), MAIN_ERRORMES_TRANSLATESETTINGS, FileName);
        SET_DestroyDataStruct(SettingsDict);
        free(Settings);
        return NULL;
    }

    SET_DestroyDataStruct(SettingsDict);

    return Settings;
}

MAIN_Map *MAIN_CreateMap(const MAIN_Settings *Settings)
{
    // Allocate memory
    MAIN_Map *Map = (MAIN_Map *)malloc(sizeof(MAIN_Map));

    if (Map == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_CREATEMAP_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Map));
        return NULL;
    }

    MAIN_InitMap(Map);

    // Insert the settings
    Map->settings = Settings;

    // Copy the size
    Map->size.w = Settings->map.width;
    Map->size.h = Settings->map.height;

    // Make sure height and width are large enough
    if (Map->size.w < 1)
    {
        _MAIN_SetError(MAIN_ERRORID_CREATEMAP_WIDTH, MAIN_ERRORMES_LOWWIDTH);
        MAIN_DestroyMap(Map);
        return NULL;
    }

    if (Map->size.h < 1)
    {
        _MAIN_SetError(MAIN_ERRORID_CREATEMAP_HEIGHT, MAIN_ERRORMES_LOWHEIGHT);
        MAIN_DestroyMap(Map);
        return NULL;
    }

    // Initialise the tiles
    Map->tiles = (MAIN_Tile *)malloc(sizeof(MAIN_Tile) * Map->size.w * Map->size.h);

    if (Map->tiles == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_CREATEMAP_MALLOCTILES, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Tile) * Map->size.w * Map->size.h);
        MAIN_DestroyMap(Map);
        return NULL;
    }

    for (MAIN_Tile *TileList = Map->tiles, *EndTileList = Map->tiles + Map->size.w * Map->size.h; TileList < EndTileList; ++TileList)
        MAIN_InitTile(TileList);

    // Set energy of tiles
    if (strcmp(Settings->map.energyMethod, MAIN_ENERGYMETHOD_CONST) == 0)
        MAIN_TileEnergyConst(Settings, Map->tiles, &Map->size);
            
    else if (strcmp(Settings->map.energyMethod, MAIN_ENERGYMETHOD_COS) == 0)
        MAIN_TileEnergyCos(Settings, Map->tiles, &Map->size);    

    else if (strcmp(Settings->map.energyMethod, MAIN_ENERGYMETHOD_COS2) == 0)
        MAIN_TileEnergyCos2(Settings, Map->tiles, &Map->size);
    
    else
    {
        _MAIN_SetError(MAIN_ERRORID_CREATEMAP_TILEENERGY, MAIN_ERRORMES_ENERGYMETHOD, Settings->map.energyMethod);
        MAIN_DestroyMap(Map);
        return NULL;
    }

    // Set the random number
    Map->random = Settings->init.seed;

    // Populate with random plants
    for (uint64_t Var = 0; Var < Settings->init.count; ++Var)
    {
        // Generate the gene
        MAIN_Gene Gene;

        MAIN_GenerateGene(Map, &Gene);

        MAIN_TruncateGene(Settings, &Gene);

        // Find the tile
        MAIN_Tile *Tile = Map->tiles + RNG_RandS(Map->random) % (Map->size.w * Map->size.h);

        // Create the plant
        if (!MAIN_CreatePlant(Map, Tile, Settings->init.energy, &Gene))
        {
            _MAIN_AddError(MAIN_ERRORID_CREATEMAP_CREATEPLANT, MAIN_ERRORMES_GENERATEPLANT, Var);
            MAIN_DestroyMap(Map);
            return NULL;
        }
    }

    return Map;
}

void MAIN_TileEnergyConst(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size)
{
    for (MAIN_Tile *TileList = Tiles, *EndTileList = Tiles + Size->w * Size->h; TileList < EndTileList; ++TileList)
        TileList->energy = Settings->map.maxEnergy;
}

void MAIN_TileEnergyCos(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size)
{
    for (MAIN_Tile *TileList = Tiles, *EndTileList = Tiles + Size->w * Size->h; TileList < EndTileList; ++TileList)
        TileList->energy = Settings->map.minEnergy + (uint64_t)((double)(Settings->map.maxEnergy - Settings->map.minEnergy) * pow(cos((double)((TileList - Tiles) / Size->w) / (double)Size->h * MAIN_PI), 2.));
}

void MAIN_TileEnergyCos2(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size)
{
    for (MAIN_Tile *TileList = Tiles, *EndTileList = Tiles + Size->w * Size->h; TileList < EndTileList; ++TileList)
        TileList->energy = Settings->map.minEnergy + (uint64_t)((double)(Settings->map.maxEnergy - Settings->map.minEnergy) * pow(cos((double)((TileList - Tiles) / Size->w) / (double)Size->h * 2 * MAIN_PI), 2.));
}

uint64_t MAIN_GenerateUint(const MAIN_UintConstraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

int64_t MAIN_GenerateInt(const MAIN_IntConstraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

double MAIN_GenerateFloat(const MAIN_FloatConstraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + RNG_RandSf(*Random) * (double)(2 * Constraint->spread);
}

void MAIN_GenerateGene(MAIN_Map *Map, MAIN_Gene *Gene)
{
    Gene->maxHeight = MAIN_GenerateUint(&Map->settings->geneConstraints.maxHeight, &Map->random);
    Gene->maxSize = MAIN_GenerateUint(&Map->settings->geneConstraints.maxSize, &Map->random);
    Gene->efficiency = MAIN_GenerateFloat(&Map->settings->geneConstraints.efficiency, &Map->random);
    Gene->growthRateHeight = MAIN_GenerateFloat(&Map->settings->geneConstraints.growthRateHeight, &Map->random);
    Gene->growthRateSize = MAIN_GenerateFloat(&Map->settings->geneConstraints.growthRateSize, &Map->random);
    Gene->minGrowthEnergyHeight = MAIN_GenerateUint(&Map->settings->geneConstraints.minGrowthEnergyHeight, &Map->random);
    Gene->minGrowthEnergySize = MAIN_GenerateUint(&Map->settings->geneConstraints.minGrowthEnergySize, &Map->random);
    Gene->spawnRate = MAIN_GenerateFloat(&Map->settings->geneConstraints.spawnRate, &Map->random);
    Gene->minSpawnEnergy = MAIN_GenerateUint(&Map->settings->geneConstraints.minSpawnEnergy, &Map->random);
    Gene->maxTileEnergy = MAIN_GenerateUint(&Map->settings->geneConstraints.maxTileEnergy, &Map->random);
    Gene->spawnEnergy = MAIN_GenerateUint(&Map->settings->geneConstraints.spawnEnergy, &Map->random);
    Gene->spawnSize = MAIN_GenerateUint(&Map->settings->geneConstraints.spawnSize, &Map->random);
    Gene->spawnSpread = MAIN_GenerateUint(&Map->settings->geneConstraints.spawnSpread, &Map->random);
    Gene->mutationRate = MAIN_GenerateFloat(&Map->settings->geneConstraints.mutationRate, &Map->random);
    Gene->mutationAttempts = MAIN_GenerateUint(&Map->settings->geneConstraints.mutationAttempts, &Map->random);
}

bool MAIN_TruncateUint(const MAIN_UintConstraint *Constraint, uint64_t *Value)
{
    if (*Value < Constraint->min)
    {
        *Value = Constraint->min;
        return true;
    }

    if (*Value > Constraint->max)
    {
        *Value = Constraint->max;
        return true;
    }

    return false;
}

bool MAIN_TruncateInt(const MAIN_IntConstraint *Constraint, int64_t *Value)
{
    if (*Value < Constraint->min)
    {
        *Value = Constraint->min;
        return true;
    }

    if (*Value > Constraint->max)
    {
        *Value = Constraint->max;
        return true;
    }

    return false;
}

bool MAIN_TruncateFloat(const MAIN_FloatConstraint *Constraint, double *Value)
{
    if (*Value < Constraint->min)
    {
        *Value = Constraint->min;
        return true;
    }

    if (*Value > Constraint->max)
    {
        *Value = Constraint->max;
        return true;
    }

    return false;
}

bool MAIN_TruncateGene(const MAIN_Settings *Settings, MAIN_Gene *Gene)
{
    bool Return = false;

    Return |= MAIN_TruncateUint(&Settings->geneConstraints.maxHeight, &Gene->maxHeight);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.maxSize, &Gene->maxSize);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.efficiency, &Gene->efficiency);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.growthRateHeight, &Gene->growthRateHeight);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.growthRateSize, &Gene->growthRateSize);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.minGrowthEnergyHeight, &Gene->minGrowthEnergyHeight);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.minGrowthEnergySize, &Gene->minGrowthEnergySize);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.spawnRate, &Gene->spawnRate);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.minSpawnEnergy, &Gene->minSpawnEnergy);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.maxTileEnergy, &Gene->maxTileEnergy);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.spawnEnergy, &Gene->spawnEnergy);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.spawnSize, &Gene->spawnSize);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.spawnSpread, &Gene->spawnSpread);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.mutationRate, &Gene->mutationRate);
    Return |= MAIN_TruncateUint(&Settings->geneConstraints.mutationAttempts, &Gene->mutationAttempts);

    return Return;
}

bool MAIN_AddToTile(MAIN_Tile *Tile, MAIN_Plant *Plant)
{
    // Allocate new memory
    MAIN_Plant **NewPlantList = (MAIN_Plant **)realloc(Tile->plantList, sizeof(MAIN_Plant *) * (Tile->plantCount + 1));

    if (NewPlantList == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_ADDTOTILE_REALLOC, strerror(errno), MAIN_ERRORMES_REALLOC, sizeof(MAIN_Plant *) * (Tile->plantCount + 1));
        return false;
    }

    Tile->plantList = NewPlantList;

    // Move other plants to find position
    MAIN_Plant **PlantList = NewPlantList + Tile->plantCount++;

    for (; PlantList > NewPlantList && (*(PlantList - 1))->stats.height < Plant->stats.height; --PlantList)
        *PlantList = *(PlantList - 1);

    *PlantList = Plant;

    // Update size
    ++Plant->stats.size;

    // Update energy stats
    Plant->stats.maxEnergy = Plant->stats.size * Plant->gene.maxTileEnergy;
    Plant->stats.energyUsage = MAIN_EnergyUsage(Plant);
    Plant->stats.biomass = MAIN_Biomass(Plant);

    return true;
}

bool MAIN_RemoveFromTile(MAIN_Tile *Tile, const MAIN_Plant *Plant)
{
    // Find it in the plant list
    MAIN_Plant **PlantList = Tile->plantList;

    if (PlantList == NULL)
    {
        _MAIN_SetError(MAIN_ERRORID_REMOVEFROMTILE_PLANTLIST, MAIN_ERRORMES_NULLPLANTLIST);
        return false;
    }

    for (MAIN_Plant **EndPlantList = Tile->plantList + Tile->plantCount, **MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2; PlantList < EndPlantList - 1; MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2)
    {
        if ((*MiddlePlantList)->stats.height > Plant->stats.height)
            PlantList = MiddlePlantList;

        else if ((*MiddlePlantList)->stats.height < Plant->stats.height)
            EndPlantList = MiddlePlantList;

        else
            break;
    }

    // Make sure it found it
    MAIN_Plant **FoundPlant = NULL;

    for (MAIN_Plant **TempPlantList = PlantList, **StartTempPlantList = Tile->plantList; TempPlantList >= StartTempPlantList && (*TempPlantList)->stats.height == Plant->stats.height; --TempPlantList)
        if (*TempPlantList == Plant)
        {
            FoundPlant = TempPlantList;
            break;
        }

    if (FoundPlant == NULL)
        for (MAIN_Plant **TempPlantList = PlantList + 1, **EndTempPlantList = Tile->plantList + Tile->plantCount; TempPlantList < EndTempPlantList && (*TempPlantList)->stats.height == Plant->stats.height; ++TempPlantList)
            if (*TempPlantList == Plant)
            {
                FoundPlant = TempPlantList;
                break;
            }

    if (FoundPlant == NULL)
    {
        _MAIN_SetError(MAIN_ERRORID_REMOVEFROMTILE_INTILE, MAIN_ERRORMES_PLANTINTILE);
        return false;
    }

    // Remove it from the list
    ++FoundPlant;

    for (MAIN_Plant **EndPlantList = Tile->plantList + Tile->plantCount; FoundPlant < EndPlantList; ++FoundPlant)
        *(FoundPlant - 1) = *FoundPlant;

    // Free the list if needed
    if (--Tile->plantCount == 0)
    {
        free(Tile->plantList);
        Tile->plantList = NULL;
    }

    // Realloc
    else
        Tile->plantList = (MAIN_Plant **)realloc(Tile->plantList, sizeof(MAIN_Plant *) * Tile->plantCount);

    return true;
}

bool MAIN_AddToMap(MAIN_Map *Map, MAIN_Plant *Plant)
{
    // Allocate new memory
    MAIN_Plant **NewPlantList = (MAIN_Plant **)realloc(Map->plantList, sizeof(MAIN_Plant *) * (Map->plantCount + 1));

    if (NewPlantList == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_ADDTOMAP_REALLOC, strerror(errno), MAIN_ERRORMES_REALLOC, sizeof(MAIN_Plant *) * (Map->plantCount + 1));
        return false;
    }

    Map->plantList = NewPlantList;
    
    // Move other plants to find position
    MAIN_Plant **PlantList = NewPlantList + Map->plantCount++;

    for (; PlantList > NewPlantList && (*(PlantList - 1))->stats.age > Plant->stats.height; --PlantList)
        *PlantList = *(PlantList - 1);

    *PlantList = Plant;

    return true;
}

bool MAIN_RemoveFromMap(MAIN_Map *Map, const MAIN_Plant *Plant)
{
    MAIN_Plant **PlantList = Map->plantList;

    if (PlantList == NULL)
    {
        _MAIN_SetError(MAIN_ERRORID_REMOVEFROMMAP_PLANTLIST, MAIN_ERRORMES_NULLPLANTLIST);
        return false;
    }

    for (MAIN_Plant **EndPlantList = Map->plantList + Map->plantCount, **MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2; PlantList < EndPlantList - 1; MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2)
    {
        if ((*MiddlePlantList)->stats.age > Plant->stats.age)
            EndPlantList = MiddlePlantList;

        else if ((*MiddlePlantList)->stats.age < Plant->stats.age)
            PlantList = MiddlePlantList;

        else
            break;
    }

    // Make sure it found it
    MAIN_Plant **FoundPlant = NULL;

    for (MAIN_Plant **TempPlantList = PlantList, **StartTempPlantList = Map->plantList; TempPlantList >= StartTempPlantList && (*TempPlantList)->stats.age == Plant->stats.age; --TempPlantList)
        if (*TempPlantList == Plant)
        {
            FoundPlant = TempPlantList;
            break;
        }

    if (FoundPlant == NULL)
        for (MAIN_Plant **TempPlantList = PlantList + 1, **EndTempPlantList = Map->plantList + Map->plantCount; TempPlantList < EndTempPlantList && (*TempPlantList)->stats.age == Plant->stats.age; ++TempPlantList)
            if (*TempPlantList == Plant)
            {
                FoundPlant = TempPlantList;
                break;
            }

    if (FoundPlant == NULL)
    {
        _MAIN_SetError(MAIN_ERRORID_REMOVEFROMMAP_INMAP, MAIN_ERRORMES_PLANTINMAP);
        return false;
    }

    // Remove it from the list
    ++FoundPlant;

    for (MAIN_Plant **EndPlantList = Map->plantList + Map->plantCount; FoundPlant < EndPlantList; ++FoundPlant)
        *(FoundPlant - 1) = *FoundPlant;

    // Free the list if needed
    if (--Map->plantCount == 0)
    {
        free(Map->plantList);
        Map->plantList = NULL;
    }

    // Realloc
    else
        Map->plantList = (MAIN_Plant **)realloc(Map->plantList, sizeof(MAIN_Plant *) * Map->plantCount);

    return true;
}

uint64_t MAIN_EnergyUsage(const MAIN_Plant *Plant)
{
    uint64_t StorageEnergy = (uint64_t)(Plant->map->settings->energy.storageRate * pow((double)Plant->stats.maxEnergy, Plant->map->settings->energy.storagePow));
    uint64_t BaseEnergy = (uint64_t)(Plant->map->settings->energy.baseRate * pow((double)Plant->stats.height, Plant->map->settings->energy.heightPow) * pow((double)Plant->stats.size, Plant->map->settings->energy.sizePow) * exp((double)Plant->gene.efficiency * Plant->map->settings->energy.effPow));
    return StorageEnergy + BaseEnergy;
}

bool MAIN_CreatePlant(MAIN_Map *Map, MAIN_Tile *Tile, uint64_t Energy, const MAIN_Gene *ParentGene)
{
    // Allocate memory
    MAIN_Plant *Plant = (MAIN_Plant *)malloc(sizeof(MAIN_Plant));

    if (Plant == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_CREATEPLANT_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Plant));
        return false;
    }

    MAIN_InitPlant(Plant);

    // Set age
    Plant->stats.age = Map->time;

    // Add map to plant
    Plant->map = Map;

    // Add to map
    if (!MAIN_AddToMap(Map, Plant))
    {
        _MAIN_AddError(MAIN_ERRORID_CREATEPLANT_MAP, MAIN_ERRORMES_ADDTOMAP);
        MAIN_DestroyPlant(Plant);
        return false;
    }

    // Copy genes with mutation
    memcpy((void *)&Plant->gene, (void *)ParentGene, sizeof(MAIN_Gene));

    for (uint64_t Attempt = 0; Attempt < ParentGene->mutationAttempts; ++Attempt)
        if (RNG_RandSf(Map->random) < ParentGene->mutationRate)
            *((uint8_t *)&Plant->gene + (RNG_RandS(Map->random) % sizeof(MAIN_Gene))) ^= 1 << (RNG_RandS(Map->random) % sizeof(uint8_t));

    // Make sure it is legal
    if (MAIN_TruncateGene(Map->settings, &Plant->gene) && Map->settings->killIlligal)
    {
        MAIN_DestroyPlant(Plant);
        return true;
    }

    // Add to tile
    if (!MAIN_AddToTile(Tile, Plant))
    {
        _MAIN_AddError(MAIN_ERRORID_CREATEPLANT_TILE, MAIN_ERRORMES_ADDTOTILE);
        MAIN_DestroyPlant(Plant);
        return false;
    }

    // Set energy usage
    Plant->stats.energyUsage = MAIN_EnergyUsage(Plant);

    // Set energy
    Plant->stats.energy = Energy;

    if (Plant->stats.energy > Plant->stats.maxEnergy)
        Plant->stats.energy = Plant->stats.maxEnergy;

    return true;
}

uint64_t MAIN_Biomass(const MAIN_Plant *Plant)
{
    uint64_t StorageSize = (uint64_t)((double)Plant->stats.maxEnergy * Plant->map->settings->energy.growthStorage);
    uint64_t BaseSize = (uint64_t)((double)(Plant->stats.height * Plant->stats.size) * exp((double)Plant->gene.efficiency * Plant->map->settings->energy.effPow) * Plant->map->settings->energy.growthBase);
    return StorageSize + BaseSize;
}


void MAIN_InitUintConstraint(MAIN_UintConstraint *Struct)
{
    Struct->min = 0;
    Struct->max = 0xFFFFFFFFFFFFFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitIntConstraint(MAIN_IntConstraint *Struct)
{
    Struct->min = -0x8000000000000000;
    Struct->max = -0x7FFFFFFFFFFFFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitFloatConstraint(MAIN_FloatConstraint *Struct)
{
    Struct->min = 0.;
    Struct->max = 1.;
    Struct->mean = 0.5;
    Struct->spread = 0.5;
}

void MAIN_InitGeneConstraints(MAIN_GeneConstraints *Struct)
{
    Struct->maxHeight.min = 1;
    Struct->maxHeight.max = 0xFFFFFFFFFFFFFFFF;
    Struct->maxHeight.mean = 100;
    Struct->maxHeight.spread = 100;

    Struct->maxSize.min = 1;
    Struct->maxSize.max = 0xFFFFFFFFFFFFFFFF;
    Struct->maxSize.mean = 10;
    Struct->maxSize.spread = 10;

    Struct->efficiency.min = 0.;
    Struct->efficiency.max = 1.;
    Struct->efficiency.mean = 0.5;
    Struct->efficiency.spread = 0.5;

    Struct->growthRateHeight.min = 0.;
    Struct->growthRateHeight.max = 1.;
    Struct->growthRateHeight.mean = 0.5;
    Struct->growthRateHeight.spread = 0.5;

    Struct->growthRateSize.min = 0.;
    Struct->growthRateSize.max = 1.;
    Struct->growthRateSize.mean = 0.5;
    Struct->growthRateSize.spread = 0.5;

    Struct->minGrowthEnergyHeight.min = 0;
    Struct->minGrowthEnergyHeight.max = 0xFFFFFFFFFFFFFFFF;
    Struct->minGrowthEnergyHeight.mean = 1000;
    Struct->minGrowthEnergyHeight.spread = 1000;

    Struct->minGrowthEnergySize.min = 0;
    Struct->minGrowthEnergySize.max = 0xFFFFFFFFFFFFFFFF;
    Struct->minGrowthEnergySize.mean = 1000;
    Struct->minGrowthEnergySize.spread = 1000;

    Struct->spawnRate.min = 0.;
    Struct->spawnRate.max = 1.;
    Struct->spawnRate.mean = 0.5;
    Struct->spawnRate.spread = 0.5;

    Struct->minSpawnEnergy.min = 0;
    Struct->minSpawnEnergy.max = 0xFFFFFFFFFFFFFFFF;
    Struct->minSpawnEnergy.mean = 1000;
    Struct->minSpawnEnergy.spread = 1000;

    Struct->maxTileEnergy.min = 1;
    Struct->maxTileEnergy.max = 0xFFFFFFFFFFFFFFFF;
    Struct->maxTileEnergy.mean = 1000;
    Struct->maxTileEnergy.spread = 1000;

    Struct->spawnEnergy.min = 1;
    Struct->spawnEnergy.max = 0xFFFFFFFFFFFFFFFF;
    Struct->spawnEnergy.mean = 100;
    Struct->spawnEnergy.spread = 100;

    Struct->spawnSize.min = 1;
    Struct->spawnSize.max = 0xFFFFFFFFFFFFFFFF;
    Struct->spawnSize.mean = 5;
    Struct->spawnSize.spread = 5;

    Struct->spawnSpread.min = 0;
    Struct->spawnSpread.max = 0xFFFFFFFFFFFFFFFF;
    Struct->spawnSpread.mean = 5;
    Struct->spawnSpread.spread = 5;

    Struct->mutationRate.min = 0.;
    Struct->mutationRate.max = 1.;
    Struct->mutationRate.mean = 0.5;
    Struct->mutationRate.spread = 0.5;

    Struct->mutationAttempts.min = 0;
    Struct->mutationAttempts.max = 0x100;
    Struct->mutationAttempts.mean = 10;
    Struct->mutationAttempts.spread = 10;
}

void MAIN_InitMapSettings(MAIN_MapSettings *Struct)
{
    Struct->height = 1024;
    Struct->width = 1024;
    Struct->minEnergy = 1000;
    Struct->maxEnergy = 10000;
    Struct->energyMethod = (char *)malloc(sizeof(char) * (strlen(MAIN_ENERGYMETHOD_CONST) + 1));
    
    if (Struct->energyMethod != NULL)
        strcpy(Struct->energyMethod, MAIN_ENERGYMETHOD_CONST);
}   

void MAIN_InitInitialSettings(MAIN_InitialSettings *Struct)
{
    Struct->count = 1000;
    Struct->energy = 10000;
    Struct->seed = time(NULL);
}

void MAIN_InitEnergySettings(MAIN_EnergySettings *Struct)
{
    Struct->storageRate = 1e-2;
    Struct->storagePow = 1.;
    Struct->baseRate = 1.;
    Struct->heightPow = 1.;
    Struct->sizePow = 0.5;
    Struct->effPow = 1.;
    Struct->growthBase = 100.;
    Struct->growthStorage = 1.;
}

void MAIN_InitSettings(MAIN_Settings *Struct)
{
    MAIN_InitGeneConstraints(&Struct->geneConstraints);
    MAIN_InitMapSettings(&Struct->map);
    MAIN_InitInitialSettings(&Struct->init);
    MAIN_InitEnergySettings(&Struct->energy);
    Struct->killIlligal = true;
}

void MAIN_InitMap(MAIN_Map *Struct)
{
    Struct->settings = NULL;
    MAIN_InitSize(&Struct->size);
    Struct->tiles = NULL;
    Struct->plantList = NULL;
    Struct->plantCount = 0;
    Struct->time = 0;
    Struct->random = 0;
}

void MAIN_InitTile(MAIN_Tile *Struct)
{
    Struct->energy = 0;
    Struct->plantCount = 0;
    Struct->plantList = NULL;
}

void MAIN_InitPlant(MAIN_Plant *Struct)
{
    Struct->map = NULL;
    Struct->tileList = NULL;
    MAIN_InitPlantStats(&Struct->stats);
    MAIN_InitGene(&Struct->gene);
}

void MAIN_InitPlantStats(MAIN_PlantStats *Struct)
{
    Struct->age = 0;
    Struct->energy = 0;
    Struct->energyUsage = 0;
    Struct->height = 0;
    Struct->maxEnergy = 0;
    Struct->size = 0;
}

void MAIN_InitGene(MAIN_Gene *Struct)
{
    Struct->maxHeight = 0;
    Struct->maxSize = 0;
    Struct->efficiency = 0.;
    Struct->growthRateHeight = 0.;
    Struct->growthRateSize = 0.;
    Struct->maxTileEnergy = 0;
    Struct->minGrowthEnergyHeight = 0;
    Struct->minGrowthEnergySize = 0;
    Struct->minSpawnEnergy = 0;
    Struct->mutationAttempts = 0;
    Struct->mutationRate = 0.;
    Struct->spawnEnergy = 0;
    Struct->spawnRate = 0.;
    Struct->spawnSize = 0;
    Struct->spawnSpread = 0;
}

void MAIN_InitSize(MAIN_Size *Struct)
{
    Struct->w = 0;
    Struct->h = 0;
}


void MAIN_CleanUintConstraint(MAIN_UintConstraint *Struct)
{

}

void MAIN_CleanIntConstraint(MAIN_IntConstraint *Struct)
{

}

void MAIN_CleanFloatConstraint(MAIN_FloatConstraint *Struct)
{

}

void MAIN_CleanGeneConstraints(MAIN_GeneConstraints *Struct)
{
    // Clean all constraints
    MAIN_CleanUintConstraint(&Struct->maxHeight);
    MAIN_CleanUintConstraint(&Struct->maxSize);
    MAIN_CleanFloatConstraint(&Struct->efficiency);
    MAIN_CleanFloatConstraint(&Struct->growthRateHeight);
    MAIN_CleanFloatConstraint(&Struct->growthRateSize);
    MAIN_CleanUintConstraint(&Struct->minGrowthEnergyHeight);
    MAIN_CleanUintConstraint(&Struct->minGrowthEnergySize);
    MAIN_CleanFloatConstraint(&Struct->spawnRate);
    MAIN_CleanUintConstraint(&Struct->minSpawnEnergy);
    MAIN_CleanUintConstraint(&Struct->maxTileEnergy);
    MAIN_CleanUintConstraint(&Struct->spawnEnergy);
    MAIN_CleanUintConstraint(&Struct->spawnSize);
    MAIN_CleanUintConstraint(&Struct->spawnSpread);
    MAIN_CleanFloatConstraint(&Struct->mutationRate);
    MAIN_CleanUintConstraint(&Struct->mutationAttempts);
}

void MAIN_CleanMapSettings(MAIN_MapSettings *Struct)
{
    if (Struct->energyMethod != NULL)
        free(Struct->energyMethod);
}

void MAIN_CleanInitialSettings(MAIN_InitialSettings *Struct)
{

}

void MAIN_CleanEnergySettings(MAIN_EnergySettings *Struct)
{

}

void MAIN_CleanSettings(MAIN_Settings *Struct)
{
    // Clean gene constraints
    MAIN_CleanGeneConstraints(&Struct->geneConstraints);

    // Clean map settings
    MAIN_CleanMapSettings(&Struct->map);

    // Clean init
    MAIN_CleanInitialSettings(&Struct->init);

    // Clean energy
    MAIN_CleanEnergySettings(&Struct->energy);
}

void MAIN_CleanMap(MAIN_Map *Struct)
{
    // Destroy remaining plants
    if (Struct->plantList != NULL)
        for (MAIN_Plant **PlantList = Struct->plantList + Struct->plantCount - 1, **StartPlantList = Struct->plantList; PlantList >= StartPlantList; --PlantList)
            if (*PlantList != NULL)
                MAIN_DestroyPlant(*PlantList);

    if (Struct->plantList != NULL)
        free(Struct->plantList);

    // Clean tiles
    if (Struct->tiles != NULL)
    {
        for (MAIN_Tile *TileList = Struct->tiles, *EndTileList = Struct->tiles + Struct->size.w * Struct->size.h; TileList < EndTileList; ++TileList)
            MAIN_CleanTile(TileList);

        free(Struct->tiles);
    }

    // Clean the size
    MAIN_CleanSize(&Struct->size);
}

void MAIN_CleanTile(MAIN_Tile *Struct)
{
    // Free the plants
    if (Struct->plantList != NULL)
        free(Struct->plantList);
}

void MAIN_CleanPlant(MAIN_Plant *Struct)
{
    // Remove from tiles
    if (Struct->tileList != NULL)
    {
        for (MAIN_Tile **TileList = Struct->tileList + Struct->stats.size, **StartTileList = Struct->tileList; TileList >= StartTileList; --TileList)
            if (*TileList != NULL)
                if (!MAIN_RemoveFromTile(*TileList, Struct))
                {
                    _MAIN_AddError(MAIN_ERRORID_CLEANPLANT_INTILE, MAIN_ERRORMES_REMOVEPLANTFROMTILE);
                    continue;
                }

        free(Struct->tileList);
    }

    // Remove it from the map list
    if (Struct->map != NULL)
        if (!MAIN_RemoveFromMap(Struct->map, Struct))
            _MAIN_AddError(MAIN_ERRORID_CLEANPLANT_INMAP, MAIN_ERRORMES_REMOVEPLANTFROMMAP);

    // Clean stats
    MAIN_CleanPlantStats(&Struct->stats);

    // Clean genes
    MAIN_CleanGene(&Struct->gene);
}

void MAIN_CleanPlantStats(MAIN_PlantStats *Struct)
{

}

void MAIN_CleanGene(MAIN_Gene *Struct)
{

}

void MAIN_CleanSize(MAIN_Size *Struct)
{

}


void MAIN_DestroyUintConstraint(MAIN_UintConstraint *Struct)
{
    MAIN_CleanUintConstraint(Struct);
    free(Struct);
}

void MAIN_DestroyIntConstraint(MAIN_IntConstraint *Struct)
{
    MAIN_CleanIntConstraint(Struct);
    free(Struct);
}

void MAIN_DestroyFloatConstraint(MAIN_FloatConstraint *Struct)
{
    MAIN_CleanFloatConstraint(Struct);
    free(Struct);
}

void MAIN_DestroyGeneConstraints(MAIN_GeneConstraints *Struct)
{
    MAIN_CleanGeneConstraints(Struct);
    free(Struct);
}

void MAIN_DestroyMapSettings(MAIN_MapSettings *Struct)
{
    MAIN_CleanMapSettings(Struct);
    free(Struct);
}

void MAIN_DestroyInitialSettings(MAIN_InitialSettings *Struct)
{
    MAIN_CleanInitialSettings(Struct);
    free(Struct);
}

void MAIN_DestroyEnergySettings(MAIN_EnergySettings *Struct)
{
    MAIN_CleanEnergySettings(Struct);
    free(Struct);
}

void MAIN_DestroySettings(MAIN_Settings *Struct)
{
    MAIN_CleanSettings(Struct);
    free(Struct);
}

void MAIN_DestroyMap(MAIN_Map *Struct)
{
    MAIN_CleanMap(Struct);
    free(Struct);
}

void MAIN_DestroyTile(MAIN_Tile *Struct)
{
    MAIN_CleanTile(Struct);
    free(Struct);
}

void MAIN_DestroyPlant(MAIN_Plant *Struct)
{
    MAIN_CleanPlant(Struct);
    free(Struct);
}

void MAIN_DestroyPlantStats(MAIN_PlantStats *Struct)
{
    MAIN_CleanPlantStats(Struct);
    free(Struct);
}

void MAIN_DestroyGene(MAIN_Gene *Struct)
{
    MAIN_CleanGene(Struct);
    free(Struct);
}

void MAIN_DestroySize(MAIN_Size *Struct)
{
    MAIN_CleanSize(Struct);
    free(Struct);
}


// Main program
int main(int argc, char **argv)
{
    // Test reading a setting
    MAIN_Settings *Settings = MAIN_LoadSettings("Settings.txt");

    if (Settings == NULL)
    {
        printf("Unable to load settings: %s\n", MAIN_GetError());
        return -1;
    }

    // Test creating a map
    MAIN_Map *Map = MAIN_CreateMap(Settings);

    if (Map == NULL)
    {
        printf("Unable to create map: %s\n", MAIN_GetError());
        return -1;
    }

    // Check the tile energy
    /*for (MAIN_Tile *TileList = Map->tiles, *EndTileList = Map->tiles + Map->size.w * Map->size.h; TileList < EndTileList; TileList += Map->size.w)
        printf("TileEnergy: %llu\n", TileList->energy);*/

    // Print the number of plants
    printf("InitialPlantCount: %llu\n", Map->plantCount);

    // Print energy usage
    /*for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
        printf("EnergyUsage: %llu\n", (*PlantList)->stats.energyUsage);*/
    printf("%llu, %llu, %llu, %llu\n", (*Map->plantList)->stats.energy, (*Map->plantList)->stats.maxEnergy, (*Map->plantList)->stats.energyUsage, (*Map->plantList)->stats.biomass);
    // Clean up
    MAIN_DestroyMap(Map);
    MAIN_DestroySettings(Settings);

    // Print errors
    char *ErrorMes;
    while ((ErrorMes = MAIN_GetArchivedError()) != NULL)
        printf("Error: %s\n", ErrorMes);

    printf("Finished without errors\n");

    return 0;
}