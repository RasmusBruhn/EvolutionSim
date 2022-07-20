#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
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
    MAIN_ERRORID_CLEANPLANT_INTILE = 0x00010100,
    MAIN_ERRORID_CLEANPLANT_INMAP = 0x00010101
};

#define MAIN_ERRORMES_MALLOC "Unable to allocate memory (Size: %lu)"
#define MAIN_ERRORMES_LOADSETTINGS "Unable to load settings (FileName: %s)"
#define MAIN_ERRORMES_TRANSLATESETTINGS "Unable to translate settings (FileName: %s)"
#define MAIN_ERRORMES_PLANTINTILE "Unable to locate plant in tiles plant list"
#define MAIN_ERRORMES_PLANTINMAP "Unable to locate plant in maps plant list"

// Settings
typedef struct __MAIN_Settings MAIN_Settings;
typedef struct __MAIN_GeneConstraints MAIN_GeneConstraints;
typedef struct __MAIN_UintConstraint MAIN_UintConstraint;
typedef struct __MAIN_IntConstraint MAIN_IntConstraint;
typedef struct __MAIN_FloatConstraint MAIN_FloatConstraint;
typedef struct __MAIN_MapSettings MAIN_MapSettings;
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
};

struct __MAIN_Settings {
    MAIN_GeneConstraints geneConstraints; // All the gene constraints
    MAIN_MapSettings map;                 // All the map settings
    bool killIlligal;                     // If true then it should kill any plant created with illigal genes, if false they should be trunctated
};

struct __MAIN_Size {
    uint64_t w; // The width
    uint64_t h; // The height
};

struct __MAIN_Map {
    MAIN_Settings *settings; // The settings of the map
    MAIN_Tile *tiles; // All of the tiles for the map
    MAIN_Size size; // The size of the map
    MAIN_Plant **plantList; // A list with all of the plants on the map with the oldest first
    size_t plantCount; // The number of plants in the plant list
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
#define MAIN_SETTINGSCOUNT 3
#define MAIN_SETTINGSMAPCOUNT 4

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
    {.name = "maxEnergy", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_MapSettings, maxEnergy)}
};

SET_TranslationTable MAIN_SettingsTableMain[MAIN_SETTINGSCOUNT] = {
    {.name = "geneConstraints", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, geneConstraints), .size = sizeof(MAIN_GeneConstraints), .sub = MAIN_SettingsTableGeneConstrains, .count = MAIN_SETTINGSGENECONSTRAINTCOUNT},
    {.name = "map", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, map), .size = sizeof(MAIN_MapSettings), .sub = MAIN_SettingsTableMap, .count = MAIN_SETTINGSMAPCOUNT},
    {.name = "killIlligal", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_Settings, killIlligal)}
};

// Functions
// Load a settings file
MAIN_Settings *MAIN_LoadSettings(const char *FileName);

// Create a new map
MAIN_Map *MAIN_CreateMap(MAIN_Settings *Settings);

// Init functions
void MAIN_InitUintConstraint(MAIN_UintConstraint *Struct);
void MAIN_InitIntConstraint(MAIN_IntConstraint *Struct);
void MAIN_InitFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_InitGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_InitMapSettings(MAIN_MapSettings *Struct);
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

MAIN_Map *MAIN_CreateMap(MAIN_Settings *Settings)
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

    // Populate with random plants

    return Map;
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
    Struct->mutationAttempts.max = 0xFFFFFFFFFFFFFFFF;
    Struct->mutationAttempts.mean = 10;
    Struct->mutationAttempts.spread = 10;
}

void MAIN_InitMapSettings(MAIN_MapSettings *Struct)
{
    Struct->height = 1024;
    Struct->width = 1024;
    Struct->minEnergy = 1000;
    Struct->maxEnergy = 10000;
}

void MAIN_InitSettings(MAIN_Settings *Struct)
{
    MAIN_InitGeneConstraints(&Struct->geneConstraints);
    MAIN_InitMapSettings(&Struct->map);
    Struct->killIlligal = true;
}

void MAIN_InitMap(MAIN_Map *Struct)
{
    Struct->settings = NULL;
    MAIN_InitSize(&Struct->size);
    Struct->tiles = NULL;
    Struct->plantList = NULL;
    Struct->plantCount = 0;
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

}

void MAIN_CleanSettings(MAIN_Settings *Struct)
{
    // Clean gene constraints
    MAIN_CleanGeneConstraints(&Struct->geneConstraints);

    // Clean map settings
    MAIN_CleanMapSettings(&Struct->map);
}

void MAIN_CleanMap(MAIN_Map *Struct)
{
    // Destroy remaining plants
    if (Struct->plantList != NULL)
        for (MAIN_Plant **PlantList = Struct->plantList + Struct->plantCount - 1, **StartPlantList = Struct->plantList; PlantList >= StartPlantList; ++PlantList)
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
        for (MAIN_Tile **TileList = Struct->tileList, **EndTileList = Struct->tileList + Struct->stats.size; TileList < EndTileList; ++TileList)
        {
            // Find it in the plant list
            MAIN_Plant **PlantList = (*TileList)->plantList;

            for (MAIN_Plant **EndPlantList = (*TileList)->plantList + (*TileList)->plantCount, **MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2; PlantList < EndPlantList - 1; MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2)
            {
                if ((*MiddlePlantList)->stats.height > Struct->stats.height)
                    PlantList = MiddlePlantList;

                else if ((*MiddlePlantList)->stats.height < Struct->stats.height)
                    EndPlantList = MiddlePlantList;

                else
                    break;
            }

            // Make sure it found it
            MAIN_Plant **FoundPlant = NULL;

            for (MAIN_Plant **TempPlantList = PlantList, **StartTempPlantList = (*TileList)->plantList; TempPlantList >= StartTempPlantList && (*TempPlantList)->stats.height == Struct->stats.height; --TempPlantList)
                if (*TempPlantList == Struct)
                {
                    FoundPlant = TempPlantList;
                    break;
                }

            if (FoundPlant == NULL)
                for (MAIN_Plant **TempPlantList = PlantList + 1, **EndTempPlantList = (*TileList)->plantList + (*TileList)->plantCount; TempPlantList < EndTempPlantList && (*TempPlantList)->stats.height == Struct->stats.height; ++TempPlantList)
                    if (*TempPlantList == Struct)
                    {
                        FoundPlant = TempPlantList;
                        break;
                    }

            if (FoundPlant == NULL)
            {
                _MAIN_AddError(MAIN_ERRORID_CLEANPLANT_INTILE, MAIN_ERRORMES_PLANTINTILE);
                continue;
            }

            // Remove it from the list
            ++FoundPlant;

            for (MAIN_Plant **EndPlantList = (*TileList)->plantList + (*TileList)->plantCount; FoundPlant < EndPlantList; ++FoundPlant)
                *(FoundPlant - 1) = *FoundPlant;

            // Free the list if needed
            if (--((*TileList)->plantCount) == 0)
            {
                free((*TileList)->plantList);
                (*TileList)->plantList = NULL;
            }

            // Realloc
            else
                (*TileList)->plantList = realloc((*TileList)->plantList, sizeof(MAIN_Plant *) * (*TileList)->plantCount);
        }

        free(Struct->tileList);
    }

    // Remove it from the map list
    MAIN_Plant **PlantList = Struct->map->plantList;

    for (MAIN_Plant **EndPlantList = Struct->map->plantList + Struct->map->plantCount, **MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2; PlantList < EndPlantList - 1; MiddlePlantList = PlantList + (EndPlantList - PlantList) / 2)
    {
        if ((*MiddlePlantList)->stats.age > Struct->stats.age)
            EndPlantList = MiddlePlantList;

        else if ((*MiddlePlantList)->stats.age < Struct->stats.age)
            PlantList = MiddlePlantList;

        else
            break;
    }

    // Make sure it found it
    MAIN_Plant **FoundPlant = NULL;

    for (MAIN_Plant **TempPlantList = PlantList, **StartTempPlantList = Struct->map->plantList; TempPlantList >= StartTempPlantList && (*TempPlantList)->stats.age == Struct->stats.age; --TempPlantList)
        if (*TempPlantList == Struct)
        {
            FoundPlant = TempPlantList;
            break;
        }

    if (FoundPlant == NULL)
        for (MAIN_Plant **TempPlantList = PlantList + 1, **EndTempPlantList = Struct->map->plantList + Struct->map->plantCount; TempPlantList < EndTempPlantList && (*TempPlantList)->stats.age == Struct->stats.age; ++TempPlantList)
            if (*TempPlantList == Struct)
            {
                FoundPlant = TempPlantList;
                break;
            }

    if (FoundPlant == NULL)
        _MAIN_AddError(MAIN_ERRORID_CLEANPLANT_INMAP, MAIN_ERRORMES_PLANTINMAP);

    else
    {
        // Remove it from the list
        ++FoundPlant;

        for (MAIN_Plant **EndPlantList = Struct->map->plantList + Struct->map->plantCount; FoundPlant < EndPlantList; ++FoundPlant)
            *(FoundPlant - 1) = *FoundPlant;

        // Free the list if needed
        if (--(Struct->map->plantCount) == 0)
        {
            free(Struct->map->plantList);
            Struct->map->plantList = NULL;
        }

        // Realloc
        else
            Struct->map->plantList = realloc(Struct->map->plantList, sizeof(MAIN_Plant *) * Struct->map->plantCount);
    }

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