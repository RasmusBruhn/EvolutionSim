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
    MAIN_ERRORID_ADDTOTILE_REALLOC2 = 0x00070201,
    MAIN_ERRORID_ADDTOMAP_REALLOC = 0x00080200,
    MAIN_ERRORID_STEP_MALLOC = 0x00090200,
    MAIN_ERRORID_STEP_SPAWN = 0x00090201,
    MAIN_ERRORID_STEP_GROWHEIGHT = 0x00090202,
    MAIN_ERRORID_STEP_GROWSIZE = 0x00090203,
    MAIN_ERRORID_STEP_HISTLOG = 0x00090204,
    MAIN_ERRORID_GROWHEIGHT_INTILE = 0x000A0200,
    MAIN_ERRORID_GROWSIZE_ADDTOTILE = 0x000B0200,
    MAIN_ERRORID_SPAWN_CREATEPLANT = 0x000C0200,
    MAIN_ERRORID_LOGUINT8_MAXMIN = 0x000D0200,
    MAIN_ERRORID_LOGUINT8_MALLOC = 0x000D0201,
    MAIN_ERRORID_LOGUINT8_MALLOC2 = 0x000D0202,
    MAIN_ERRORID_LOGUINT8_MALLOC3 = 0x000D0203,
    MAIN_ERRORID_LOGUINT8_ITERATION = 0x000D0204,
    MAIN_ERRORID_LOGUINT8_MALLOC4 = 0x000D0205,
    MAIN_ERRORID_LOGUINT8_SAVE = 0x000D0206,
    MAIN_ERRORID_LOGUINT16_MAXMIN = 0x000E0200,
    MAIN_ERRORID_LOGUINT16_MALLOC = 0x000E0201,
    MAIN_ERRORID_LOGUINT16_MALLOC2 = 0x000E0202,
    MAIN_ERRORID_LOGUINT16_MALLOC3 = 0x000E0203,
    MAIN_ERRORID_LOGUINT16_ITERATION = 0x000E0204,
    MAIN_ERRORID_LOGUINT16_MALLOC4 = 0x000E0205,
    MAIN_ERRORID_LOGUINT16_SAVE = 0x000E0206,
    MAIN_ERRORID_LOGUINT32_MAXMIN = 0x000F0200,
    MAIN_ERRORID_LOGUINT32_MALLOC = 0x000F0201,
    MAIN_ERRORID_LOGUINT32_MALLOC2 = 0x000F0202,
    MAIN_ERRORID_LOGUINT32_MALLOC3 = 0x000F0203,
    MAIN_ERRORID_LOGUINT32_ITERATION = 0x000F0204,
    MAIN_ERRORID_LOGUINT32_MALLOC4 = 0x000F0205,
    MAIN_ERRORID_LOGUINT32_SAVE = 0x000F0206,
    MAIN_ERRORID_LOGUINT64_MAXMIN = 0x00100200,
    MAIN_ERRORID_LOGUINT64_MALLOC = 0x00100201,
    MAIN_ERRORID_LOGUINT64_MALLOC2 = 0x00100202,
    MAIN_ERRORID_LOGUINT64_MALLOC3 = 0x00100203,
    MAIN_ERRORID_LOGUINT64_ITERATION = 0x00100204,
    MAIN_ERRORID_LOGUINT64_MALLOC4 = 0x00100205,
    MAIN_ERRORID_LOGUINT64_SAVE = 0x00100206,
    MAIN_ERRORID_LOGINT8_MAXMIN = 0x00110200,
    MAIN_ERRORID_LOGINT8_MALLOC = 0x00110201,
    MAIN_ERRORID_LOGINT8_MALLOC2 = 0x00110202,
    MAIN_ERRORID_LOGINT8_MALLOC3 = 0x00110203,
    MAIN_ERRORID_LOGINT8_ITERATION = 0x00110204,
    MAIN_ERRORID_LOGINT8_MALLOC4 = 0x00110205,
    MAIN_ERRORID_LOGINT8_SAVE = 0x00110206,
    MAIN_ERRORID_LOGINT16_MAXMIN = 0x00120200,
    MAIN_ERRORID_LOGINT16_MALLOC = 0x00120201,
    MAIN_ERRORID_LOGINT16_MALLOC2 = 0x00120202,
    MAIN_ERRORID_LOGINT16_MALLOC3 = 0x00120203,
    MAIN_ERRORID_LOGINT16_ITERATION = 0x00120204,
    MAIN_ERRORID_LOGINT16_MALLOC4 = 0x00120205,
    MAIN_ERRORID_LOGINT16_SAVE = 0x00120206,
    MAIN_ERRORID_LOGINT32_MAXMIN = 0x00130200,
    MAIN_ERRORID_LOGINT32_MALLOC = 0x00130201,
    MAIN_ERRORID_LOGINT32_MALLOC2 = 0x00130202,
    MAIN_ERRORID_LOGINT32_MALLOC3 = 0x00130203,
    MAIN_ERRORID_LOGINT32_ITERATION = 0x00130204,
    MAIN_ERRORID_LOGINT32_MALLOC4 = 0x00130205,
    MAIN_ERRORID_LOGINT32_SAVE = 0x00130206,
    MAIN_ERRORID_LOGINT64_MAXMIN = 0x00140200,
    MAIN_ERRORID_LOGINT64_MALLOC = 0x00140201,
    MAIN_ERRORID_LOGINT64_MALLOC2 = 0x00140202,
    MAIN_ERRORID_LOGINT64_MALLOC3 = 0x00140203,
    MAIN_ERRORID_LOGINT64_ITERATION = 0x00140204,
    MAIN_ERRORID_LOGINT64_MALLOC4 = 0x00140205,
    MAIN_ERRORID_LOGINT64_SAVE = 0x00140206,
    MAIN_ERRORID_LOGFLOAT_MAXMIN = 0x00150200,
    MAIN_ERRORID_LOGFLOAT_MALLOC = 0x00150201,
    MAIN_ERRORID_LOGFLOAT_MALLOC2 = 0x00150202,
    MAIN_ERRORID_LOGFLOAT_MALLOC3 = 0x00150203,
    MAIN_ERRORID_LOGFLOAT_ITERATION = 0x00150204,
    MAIN_ERRORID_LOGFLOAT_MALLOC4 = 0x00150205,
    MAIN_ERRORID_LOGFLOAT_SAVE = 0x00150206,
    MAIN_ERRORID_HISTLOG_MAXHEIGTH = 0x00160200,
    MAIN_ERRORID_HISTLOG_MAXSIZE = 0x00160201,
    MAIN_ERRORID_HISTLOG_EFFICIENCY = 0x00160202,
    MAIN_ERRORID_HISTLOG_GROWTHRATEHEIGHT = 0x00160203,
    MAIN_ERRORID_HISTLOG_GROWTHRATESIZE = 0x00160204,
    MAIN_ERRORID_HISTLOG_MINGROWTHENERGYHEIGHT = 0x00160205,
    MAIN_ERRORID_HISTLOG_MINGROWTHENERGYSIZE = 0x00160206,
    MAIN_ERRORID_HISTLOG_SPAWNRATE = 0x00160207,
    MAIN_ERRORID_HISTLOG_MINSPAWNENERGY = 0x00160208,
    MAIN_ERRORID_HISTLOG_MAXTILEENERGY = 0x00160209,
    MAIN_ERRORID_HISTLOG_SPAWNENERGY = 0x0016020A,
    MAIN_ERRORID_HISTLOG_MAXSPAWNENERGY = 0x0016020B,
    MAIN_ERRORID_HISTLOG_MUTATIONRATE = 0x0016020C,
    MAIN_ERRORID_HISTLOG_MUTATIONATTEMPTS = 0x0016020D,
    MAIN_ERRORID_HISTLOG_SIZE = 0x0016020E,
    MAIN_ERRORID_HISTLOG_HEIGHT = 0x0016020F,
    MAIN_ERRORID_HISTLOG_ENERGY = 0x00160210,
    MAIN_ERRORID_HISTLOG_AGE = 0x00160211,
    MAIN_ERRORID_HISTLOG_ENERGYUSAGE = 0x00160212,
    MAIN_ERRORID_HISTLOG_MAXENERGY = 0x00160213,
    MAIN_ERRORID_HISTLOG_BIOMASS = 0x00160214,
    MAIN_ERRORID_FINDUNIQUESPECIES_MALLOC = 0x00170200,
    MAIN_ERRORID_FINDUNIQUESPECIES_MALLOC2 = 0x00170201,
    MAIN_ERRORID_FINDUNIQUESPECIES_REALLOC = 0x00170202,
    MAIN_ERRORID_FINDUNIQUESPECIES_REALLOC2 = 0x00170203
};

#define MAIN_ERRORMES_MALLOC "Unable to allocate memory (Size: %u)"
#define MAIN_ERRORMES_REALLOC "Unable to reallocate memory (Size: %u)"
#define MAIN_ERRORMES_LOADSETTINGS "Unable to load settings (FileName: %s)"
#define MAIN_ERRORMES_TRANSLATESETTINGS "Unable to translate settings (FileName: %s)"
#define MAIN_ERRORMES_PLANTINTILE "Unable to locate plant in tiles plant list"
#define MAIN_ERRORMES_PLANTINMAP "Unable to locate plant in maps plant list"
#define MAIN_ERRORMES_GENERATEPLANT "Unable to generate plant (ID: %u)"
#define MAIN_ERRORMES_ENERGYMETHOD "Uknown energy method (Method: %s)"
#define MAIN_ERRORMES_LOWWIDTH "The width of the map is too low"
#define MAIN_ERRORMES_LOWHEIGHT "The height of the map is too low"
#define MAIN_ERRORMES_ADDTOTILE "Unable to add plant to tile"
#define MAIN_ERRORMES_ADDTOMAP "Unable to add plant to map"
#define MAIN_ERRORMES_REMOVEPLANTFROMTILE "Unable to remove plant from tile"
#define MAIN_ERRORMES_REMOVEPLANTFROMMAP "Unable to remove plant from map"
#define MAIN_ERRORMES_NULLPLANTLIST "Plant list is NULL"
#define MAIN_ERRORMES_SPAWN "An error occured while spawning new plants"
#define MAIN_ERRORMES_GROWHEIGHT "An error occured while growing in height"
#define MAIN_ERRORMES_GROWSIZE "An error occured while growing in size"
#define MAIN_ERRORMES_CREATEPLANT "Unable to create plant"
#define MAIN_ERRORMES_LOGMAXMIN "Maximum log value cannot be smaller then min log value (Max: %.2g, Min: %.2g)"
#define MAIN_ERRORMES_LOGITERATION "Too many files has been written (Count: %u, max: %u)"
#define MAIN_ERRORMES_SAVECSV "Unable to save csv (FileName: %s)"
#define MAIN_ERRORMES_HISTLOG "Unable to write hist log (Field: %s)"
#define MAIN_ERRORMES_HISTLOGSTEP "Unable to create hist log (Time: %lu)"

// Settings
typedef struct __MAIN_Settings MAIN_Settings;
typedef struct __MAIN_GeneConstraints MAIN_GeneConstraints;
typedef struct __MAIN_Uint64Constraint MAIN_Uint64Constraint;
typedef struct __MAIN_Int64Constraint MAIN_Int64Constraint;
typedef struct __MAIN_Uint32Constraint MAIN_Uint32Constraint;
typedef struct __MAIN_Int32Constraint MAIN_Int32Constraint;
typedef struct __MAIN_Uint16Constraint MAIN_Uint16Constraint;
typedef struct __MAIN_Int16Constraint MAIN_Int16Constraint;
typedef struct __MAIN_Uint8Constraint MAIN_Uint8Constraint;
typedef struct __MAIN_Int8Constraint MAIN_Int8Constraint;
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
typedef struct __MAIN_Filter MAIN_Filter;
typedef struct __MAIN_LogUint8Settings MAIN_LogUint8Settings;
typedef struct __MAIN_LogUint16Settings MAIN_LogUint16Settings;
typedef struct __MAIN_LogUint32Settings MAIN_LogUint32Settings;
typedef struct __MAIN_LogUint64Settings MAIN_LogUint64Settings;
typedef struct __MAIN_LogInt8Settings MAIN_LogInt8Settings;
typedef struct __MAIN_LogInt16Settings MAIN_LogInt16Settings;
typedef struct __MAIN_LogInt32Settings MAIN_LogInt32Settings;
typedef struct __MAIN_LogInt64Settings MAIN_LogInt64Settings;
typedef struct __MAIN_LogFloatSettings MAIN_LogFloatSettings;
typedef struct __MAIN_HistLogSettings MAIN_HistLogSettings;
typedef enum __MAIN_Direction MAIN_Direction;

enum __MAIN_Direction {
    MAIN_DIR_POSX = 0,
    MAIN_DIR_POSY = 1,
    MAIN_DIR_NEGX = 2,
    MAIN_DIR_NEGY = 3
};

struct __MAIN_Uint64Constraint {
    uint64_t min;    // The minimum allowed value
    uint64_t max;    // The maximum allowed value
    uint64_t mean;   // The mean starting value
    uint64_t spread; // The spread of the starting value
};

struct __MAIN_Int64Constraint {
    int64_t min;    // The minimum allowed value
    int64_t max;    // The maximum allowed value
    int64_t mean;   // The mean starting value
    int64_t spread; // The spread of the starting value
};

struct __MAIN_Uint32Constraint {
    uint32_t min;    // The minimum allowed value
    uint32_t max;    // The maximum allowed value
    uint32_t mean;   // The mean starting value
    uint32_t spread; // The spread of the starting value
};

struct __MAIN_Int32Constraint {
    int32_t min;    // The minimum allowed value
    int32_t max;    // The maximum allowed value
    int32_t mean;   // The mean starting value
    int32_t spread; // The spread of the starting value
};

struct __MAIN_Uint16Constraint {
    uint16_t min;    // The minimum allowed value
    uint16_t max;    // The maximum allowed value
    uint16_t mean;   // The mean starting value
    uint16_t spread; // The spread of the starting value
};

struct __MAIN_Int16Constraint {
    int16_t min;    // The minimum allowed value
    int16_t max;    // The maximum allowed value
    int16_t mean;   // The mean starting value
    int16_t spread; // The spread of the starting value
};

struct __MAIN_Uint8Constraint {
    uint8_t min;    // The minimum allowed value
    uint8_t max;    // The maximum allowed value
    uint8_t mean;   // The mean starting value
    uint8_t spread; // The spread of the starting value
};

struct __MAIN_Int8Constraint {
    int8_t min;    // The minimum allowed value
    int8_t max;    // The maximum allowed value
    int8_t mean;   // The mean starting value
    int8_t spread; // The spread of the starting value
};

struct __MAIN_FloatConstraint {
    double min;    // The minimum allowed value
    double max;    // The maximum allowed value
    double mean;   // The mean starting value
    double spread; // The spread of the starting value
};

struct __MAIN_GeneConstraints {
    MAIN_Uint8Constraint maxHeight; // The maximum height of the plant
    MAIN_Uint8Constraint maxSize; // The maximum size of the plant
    MAIN_FloatConstraint efficiency; // The effieciency at collecting energy
    MAIN_FloatConstraint growthRateHeight; // The probability the plant will grow in height if it can
    MAIN_FloatConstraint growthRateSize; // The probability the plant will grow in size if it can
    MAIN_Uint16Constraint minGrowthEnergyHeight; // The minimum energy required to grow in height
    MAIN_Uint16Constraint minGrowthEnergySize; // The minimum energy required to grow in size
    MAIN_FloatConstraint spawnRate; // The probability of spawning if it can
    MAIN_Uint16Constraint minSpawnEnergy; // The minimum energy required to spawn
    MAIN_Uint32Constraint maxTileEnergy; // The maximum amount of energy allowed to store per tile
    MAIN_Uint32Constraint spawnEnergy; // The amount of energy to give each seed
    MAIN_Uint32Constraint maxSpawnEnergy; // The maximum amount of energy to spent on spawning
    MAIN_FloatConstraint mutationRate; // The probability that a mutation occurs during spawning
    MAIN_Uint8Constraint mutationAttempts; // The number of attempts to do a mutation during spawning
};

struct __MAIN_MapSettings {
    uint32_t width; // The width of the map
    uint32_t height; // The height of the map
    uint32_t minEnergy; // The minimum energy production per tile
    uint32_t maxEnergy; // The maximum energy production per tile
    char *energyMethod; // The method to calculate the energy
    double energyNoise; // How much noise is in the energy
};

struct __MAIN_InitialSettings {
    uint16_t count; // The number of plant to create at the beginning
    uint64_t seed; // The seed for the simulation
    uint32_t energy; // The starting energy of all the initial plants
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
    uint32_t baseCost; // A base cost that is added to any plant
    double spawnEfficiency; // The efficiency of the energy given to the seeds
};

struct __MAIN_LogUint8Settings {
    bool active; // If true, this field will be logged
    uint8_t min; // The min value in the histogram
    uint8_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogUint16Settings {
    bool active; // If true, this field will be logged
    uint16_t min; // The min value in the histogram
    uint16_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogUint32Settings {
    bool active; // If true, this field will be logged
    uint32_t min; // The min value in the histogram
    uint32_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogUint64Settings {
    bool active; // If true, this field will be logged
    uint64_t min; // The min value in the histogram
    uint64_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogInt8Settings {
    bool active; // If true, this field will be logged
    int8_t min; // The min value in the histogram
    int8_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogInt16Settings {
    bool active; // If true, this field will be logged
    int16_t min; // The min value in the histogram
    int16_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogInt32Settings {
    bool active; // If true, this field will be logged
    int32_t min; // The min value in the histogram
    int32_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogInt64Settings {
    bool active; // If true, this field will be logged
    int64_t min; // The min value in the histogram
    int64_t max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_LogFloatSettings {
    bool active; // If true, this field will be logged
    double min; // The min value in the histogram
    double max; // The max value in the histogram
    uint16_t bins; // The number of bins to use
};

struct __MAIN_HistLogSettings {
    uint64_t period; // How often it should log, if 0 then never log
    char *name; // The base name of the file, full name will be [name]_[fieldName]_[Count].csv
    uint32_t maxFileCount; // The maximum number of iterations of the logs to write
    MAIN_LogUint8Settings maxHeight; // The maximum height of the plant
    MAIN_LogUint8Settings maxSize; // The maximum size of the plant
    MAIN_LogFloatSettings efficiency; // The effieciency at collecting energy
    MAIN_LogFloatSettings growthRateHeight; // The probability the plant will grow in height if it can
    MAIN_LogFloatSettings growthRateSize; // The probability the plant will grow in size if it can
    MAIN_LogUint16Settings minGrowthEnergyHeight; // The minimum energy required to grow in height
    MAIN_LogUint16Settings minGrowthEnergySize; // The minimum energy required to grow in size
    MAIN_LogFloatSettings spawnRate; // The probability of spawning if it can
    MAIN_LogUint16Settings minSpawnEnergy; // The minimum energy required to spawn
    MAIN_LogUint32Settings maxTileEnergy; // The maximum amount of energy allowed to store per tile
    MAIN_LogUint32Settings spawnEnergy; // The amount of energy to give each seed
    MAIN_LogUint32Settings maxSpawnEnergy; // The maximum amount of energy to spent on spawning
    MAIN_LogFloatSettings mutationRate; // The probability that a mutation occurs during spawning
    MAIN_LogUint8Settings mutationAttempts; // The number of attempts to do a mutation during spawning
    MAIN_LogUint8Settings size; // The size of the plant
    MAIN_LogUint8Settings height; // The height of the plant
    MAIN_LogUint32Settings energy; // The energy storage of the plant
    MAIN_LogUint64Settings age; // At what random tick it was born
    MAIN_LogUint32Settings energyUsage; // The amount of energy to use per turn
    MAIN_LogUint32Settings maxEnergy; // The maximum amount of energy the plant can store
    MAIN_LogUint32Settings biomass; // The biomass of the plant
};

struct __MAIN_Settings {
    MAIN_GeneConstraints geneConstraints; // All the gene constraints
    MAIN_MapSettings map;                 // All the map settings
    MAIN_InitialSettings init;            // The settings for the initial population
    MAIN_EnergySettings energy;           // The settings for the energy usage
    MAIN_HistLogSettings histLog;         // The settings for the hist log
    bool killIlligal;                     // If true then it should kill any plant created with illigal genes, if false they should be trunctated
    bool killLow;                         // If true then it will kill any plant with 0 energy
    uint8_t spawnCount;                   // The maximum number of seeds per spawning
    uint16_t spawnRange;                  // Determines how far seeds are spread
    uint64_t steps;                       // The number of steps to print
    uint64_t subSteps;                    // The number of sim steps per step
};

struct __MAIN_Size {
    uint32_t w; // The width
    uint32_t h; // The height
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
    uint32_t energy; // The energy production of the tile
};

struct __MAIN_Gene {
    uint8_t maxHeight;             // The maximum height of the plant
    uint8_t maxSize;               // The maximum size of the plant
    double efficiency;              // The effieciency at collecting energy
    double growthRateHeight;        // The probability the plant will grow in height if it can
    double growthRateSize;          // The probability the plant will grow in size if it can
    uint16_t minGrowthEnergyHeight; // The minimum energy required to grow in height
    uint16_t minGrowthEnergySize;   // The minimum energy required to grow in size
    double spawnRate;               // The probability of spawning if it can
    uint16_t minSpawnEnergy;        // The minimum energy required to spawn
    uint32_t maxTileEnergy;         // The maximum amount of energy allowed to store per tile
    uint32_t spawnEnergy;           // The amount of energy to give each seed
    uint32_t maxSpawnEnergy;        // The amount of energy to take from the plant during spawning
    uint8_t spawnSize;             // The average number of seeds per spawning
    uint8_t spawnSpread;           // The spread of the number of seeds per spawning
    double mutationRate;            // The probability that a mutation occurs during spawning
    uint8_t mutationAttempts;      // The number of attempts to do a mutation during spawning
};

struct __MAIN_PlantStats {
    uint8_t size; // The size of the plant
    uint8_t height; // The height of the plant
    uint32_t energy; // The energy storage of the plant
    uint64_t age; // At what random tick it was born
    uint32_t energyUsage; // The amount of energy to use per turn
    uint32_t maxEnergy; // The maximum amount of energy the plant can store
    uint32_t biomass; // The biomass of the plant
};

struct __MAIN_Plant {
    MAIN_Gene gene; // The genes for this plant
    MAIN_Tile **tileList; // A list of all the tiles this plant is within
    MAIN_PlantStats stats; // The stats of the plant
    MAIN_Map *map; // The map this plant belongs to
};

struct __MAIN_Filter {

};

// Settings translation tables
#define MAIN_SETTINGSCONSTRAINTCOUNT 4
#define MAIN_SETTINGSGENECONSTRAINTCOUNT 14
#define MAIN_SETTINGSCOUNT 11
#define MAIN_SETTINGSMAPCOUNT 6
#define MAIN_SETTINGSINITCOUNT 3
#define MAIN_SETTINGSENERGYCOUNT 10
#define MAIN_SETTINGSLOGTYPECOUNT 4
#define MAIN_SETTINGSHISTLOGCOUNT 24

SET_TranslationTable MAIN_SettingsTableUint64Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_Uint64Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_Uint64Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_Uint64Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_Uint64Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableInt64Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_Int64Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_Int64Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_Int64Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_Int64Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableUint32Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_Uint32Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_Uint32Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_Uint32Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_Uint32Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableInt32Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_INT32, .depth = 0, .offset = offsetof(MAIN_Int32Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_INT32, .depth = 0, .offset = offsetof(MAIN_Int32Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_INT32, .depth = 0, .offset = offsetof(MAIN_Int32Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_INT32, .depth = 0, .offset = offsetof(MAIN_Int32Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableUint16Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_Uint16Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_Uint16Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_Uint16Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_Uint16Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableInt16Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_INT16, .depth = 0, .offset = offsetof(MAIN_Int16Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_INT16, .depth = 0, .offset = offsetof(MAIN_Int16Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_INT16, .depth = 0, .offset = offsetof(MAIN_Int16Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_INT16, .depth = 0, .offset = offsetof(MAIN_Int16Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableUint8Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_Uint8Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_Uint8Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_Uint8Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_Uint8Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableInt8Constraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_INT8, .depth = 0, .offset = offsetof(MAIN_Int8Constraint, min)},
    {.name = "max", .type = SET_DATATYPE_INT8, .depth = 0, .offset = offsetof(MAIN_Int8Constraint, max)},
    {.name = "mean", .type = SET_DATATYPE_INT8, .depth = 0, .offset = offsetof(MAIN_Int8Constraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_INT8, .depth = 0, .offset = offsetof(MAIN_Int8Constraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableFloatConstraint[MAIN_SETTINGSCONSTRAINTCOUNT] = {
    {.name = "min", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, min)},
    {.name = "max", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, max)},
    {.name = "mean", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, mean)},
    {.name = "spread", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_FloatConstraint, spread)}
};

SET_TranslationTable MAIN_SettingsTableGeneConstrains[MAIN_SETTINGSGENECONSTRAINTCOUNT] = {
    {.name = "maxHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxHeight), .size = sizeof(MAIN_Uint8Constraint), .sub = MAIN_SettingsTableUint8Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "maxSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxSize), .size = sizeof(MAIN_Uint8Constraint), .sub = MAIN_SettingsTableUint8Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "efficiency", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, efficiency), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "growthRateHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, growthRateHeight), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "growthRateSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, growthRateSize), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "minGrowthEnergyHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, minGrowthEnergyHeight), .size = sizeof(MAIN_Uint16Constraint), .sub = MAIN_SettingsTableUint16Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "minGrowthEnergySize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, minGrowthEnergySize), .size = sizeof(MAIN_Uint16Constraint), .sub = MAIN_SettingsTableUint16Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "spawnRate", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, spawnRate), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "minSpawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, minSpawnEnergy), .size = sizeof(MAIN_Uint16Constraint), .sub = MAIN_SettingsTableUint16Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "maxTileEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxTileEnergy), .size = sizeof(MAIN_Uint32Constraint), .sub = MAIN_SettingsTableUint32Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "spawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, spawnEnergy), .size = sizeof(MAIN_Uint32Constraint), .sub = MAIN_SettingsTableUint32Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "maxSpawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, maxSpawnEnergy), .size = sizeof(MAIN_Uint32Constraint), .sub = MAIN_SettingsTableUint32Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "mutationRate", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, mutationRate), .size = sizeof(MAIN_FloatConstraint), .sub = MAIN_SettingsTableFloatConstraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT},
    {.name = "mutationAttempts", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_GeneConstraints, mutationAttempts), .size = sizeof(MAIN_Uint8Constraint), .sub = MAIN_SettingsTableUint8Constraint, .count = MAIN_SETTINGSCONSTRAINTCOUNT}
};

SET_TranslationTable MAIN_SettingsTableMap[MAIN_SETTINGSMAPCOUNT] = {
    {.name = "width", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_MapSettings, width)},
    {.name = "height", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_MapSettings, height)},
    {.name = "minEnergy", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_MapSettings, minEnergy)},
    {.name = "maxEnergy", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_MapSettings, maxEnergy)},
    {.name = "energyMethod", .type = SET_DATATYPE_STR, .depth = 0, .offset = offsetof(MAIN_MapSettings, energyMethod)},
    {.name = "energyNoise", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_MapSettings, energyNoise)}
};

SET_TranslationTable MAIN_SettingsTableInit[MAIN_SETTINGSINITCOUNT] = {
    {.name = "count", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_InitialSettings, count)},
    {.name = "seed", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_InitialSettings, seed)},
    {.name = "energy", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_InitialSettings, energy)}
};

SET_TranslationTable MAIN_SettingsTableEnergy[MAIN_SETTINGSENERGYCOUNT] = {
    {.name = "storageRate", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, storageRate)},
    {.name = "storagePow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, storagePow)},
    {.name = "baseRate", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, baseRate)},
    {.name = "heightPow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, heightPow)},
    {.name = "sizePow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, sizePow)},
    {.name = "effPow", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, effPow)},
    {.name = "growthBase", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, growthBase)},
    {.name = "growthStorage", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, growthStorage)},
    {.name = "baseCost", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_EnergySettings, baseCost)},
    {.name = "spawnEfficiency", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_EnergySettings, spawnEfficiency)}
};

SET_TranslationTable MAIN_SettingsTableLogUint8[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogUint8Settings, active)},
    {.name = "min", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_LogUint8Settings, min)},
    {.name = "max", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_LogUint8Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogUint8Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogUint16[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogUint16Settings, active)},
    {.name = "min", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogUint16Settings, min)},
    {.name = "max", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogUint16Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogUint16Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogUint32[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogUint32Settings, active)},
    {.name = "min", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_LogUint32Settings, min)},
    {.name = "max", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_LogUint32Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogUint32Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogUint64[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogUint64Settings, active)},
    {.name = "min", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_LogUint64Settings, min)},
    {.name = "max", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_LogUint64Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogUint64Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogInt8[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogInt8Settings, active)},
    {.name = "min", .type = SET_DATATYPE_INT8, .depth = 0, .offset = offsetof(MAIN_LogInt8Settings, min)},
    {.name = "max", .type = SET_DATATYPE_INT8, .depth = 0, .offset = offsetof(MAIN_LogInt8Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogInt8Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogInt16[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogInt16Settings, active)},
    {.name = "min", .type = SET_DATATYPE_INT16, .depth = 0, .offset = offsetof(MAIN_LogInt16Settings, min)},
    {.name = "max", .type = SET_DATATYPE_INT16, .depth = 0, .offset = offsetof(MAIN_LogInt16Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogInt16Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogInt32[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogInt32Settings, active)},
    {.name = "min", .type = SET_DATATYPE_INT32, .depth = 0, .offset = offsetof(MAIN_LogInt32Settings, min)},
    {.name = "max", .type = SET_DATATYPE_INT32, .depth = 0, .offset = offsetof(MAIN_LogInt32Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogInt32Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogInt64[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogInt64Settings, active)},
    {.name = "min", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_LogInt64Settings, min)},
    {.name = "max", .type = SET_DATATYPE_INT64, .depth = 0, .offset = offsetof(MAIN_LogInt64Settings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogInt64Settings, bins)}
};

SET_TranslationTable MAIN_SettingsTableLogFloat[MAIN_SETTINGSLOGTYPECOUNT] = {
    {.name = "active", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_LogFloatSettings, active)},
    {.name = "min", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_LogFloatSettings, min)},
    {.name = "max", .type = SET_DATATYPE_DOUBLE, .depth = 0, .offset = offsetof(MAIN_LogFloatSettings, max)},
    {.name = "bins", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_LogFloatSettings, bins)}
};

SET_TranslationTable MAIN_SettingsTableHistLog[MAIN_SETTINGSHISTLOGCOUNT] = {
    {.name = "period", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, period)},
    {.name = "name", .type = SET_DATATYPE_STR, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, name)},
    {.name = "maxFileCount", .type = SET_DATATYPE_UINT32, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, maxFileCount)},
    {.name = "maxHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, maxHeight), .size = sizeof(MAIN_LogUint8Settings), .sub = MAIN_SettingsTableLogUint8, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "maxSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, maxSize), .size = sizeof(MAIN_LogUint8Settings), .sub = MAIN_SettingsTableLogUint8, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "efficiency", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, efficiency), .size = sizeof(MAIN_LogFloatSettings), .sub = MAIN_SettingsTableLogFloat, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "growthRateHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, growthRateHeight), .size = sizeof(MAIN_LogFloatSettings), .sub = MAIN_SettingsTableLogFloat, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "growthRateSize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, growthRateSize), .size = sizeof(MAIN_LogFloatSettings), .sub = MAIN_SettingsTableLogFloat, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "minGrowthEnergyHeight", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, minGrowthEnergyHeight), .size = sizeof(MAIN_LogUint16Settings), .sub = MAIN_SettingsTableLogUint16, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "minGrowthEnergySize", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, minGrowthEnergySize), .size = sizeof(MAIN_LogUint16Settings), .sub = MAIN_SettingsTableLogUint16, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "spawnRate", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, spawnRate), .size = sizeof(MAIN_LogFloatSettings), .sub = MAIN_SettingsTableLogFloat, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "minSpawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, minSpawnEnergy), .size = sizeof(MAIN_LogUint16Settings), .sub = MAIN_SettingsTableLogUint16, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "maxTileEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, maxTileEnergy), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "spawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, spawnEnergy), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "maxSpawnEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, maxSpawnEnergy), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "mutationRate", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, mutationRate), .size = sizeof(MAIN_LogFloatSettings), .sub = MAIN_SettingsTableLogFloat, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "mutationAttempts", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, mutationAttempts), .size = sizeof(MAIN_LogUint8Settings), .sub = MAIN_SettingsTableLogUint8, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "size", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, size), .size = sizeof(MAIN_LogUint8Settings), .sub = MAIN_SettingsTableLogUint8, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "height", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, height), .size = sizeof(MAIN_LogUint8Settings), .sub = MAIN_SettingsTableLogUint8, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "energy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, energy), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "age", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, age), .size = sizeof(MAIN_LogUint64Settings), .sub = MAIN_SettingsTableLogUint64, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "energyUsage", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, energyUsage), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "maxEnergy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, maxEnergy), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT},
    {.name = "biomass", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_HistLogSettings, biomass), .size = sizeof(MAIN_LogUint32Settings), .sub = MAIN_SettingsTableLogUint32, .count = MAIN_SETTINGSLOGTYPECOUNT}
};

SET_TranslationTable MAIN_SettingsTableMain[MAIN_SETTINGSCOUNT] = {
    {.name = "geneConstraints", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, geneConstraints), .size = sizeof(MAIN_GeneConstraints), .sub = MAIN_SettingsTableGeneConstrains, .count = MAIN_SETTINGSGENECONSTRAINTCOUNT},
    {.name = "map", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, map), .size = sizeof(MAIN_MapSettings), .sub = MAIN_SettingsTableMap, .count = MAIN_SETTINGSMAPCOUNT},
    {.name = "init", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, init), .size = sizeof(MAIN_InitialSettings), .sub = MAIN_SettingsTableInit, .count = MAIN_SETTINGSINITCOUNT},
    {.name = "energy", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, energy), .size = sizeof(MAIN_EnergySettings), .sub = MAIN_SettingsTableEnergy, .count = MAIN_SETTINGSENERGYCOUNT},
    {.name = "histLog", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, histLog), .size = sizeof(MAIN_HistLogSettings), .sub = MAIN_SettingsTableHistLog, .count = MAIN_SETTINGSHISTLOGCOUNT},
    {.name = "killIlligal", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_Settings, killIlligal)},
    {.name = "killLow", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_Settings, killLow)},
    {.name = "spawnCount", .type = SET_DATATYPE_UINT8, .depth = 0, .offset = offsetof(MAIN_Settings, spawnCount)},
    {.name = "spawnRange", .type = SET_DATATYPE_UINT16, .depth = 0, .offset = offsetof(MAIN_Settings, spawnRange)},
    {.name = "steps", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_Settings, steps)},
    {.name = "subSteps", .type = SET_DATATYPE_UINT64, .depth = 0, .offset = offsetof(MAIN_Settings, subSteps)}
};

// Energy method names
#define MAIN_ENERGYMETHOD_CONST "const"
#define MAIN_ENERGYMETHOD_LINEAR "linear"
#define MAIN_ENERGYMETHOD_COS "cos"
#define MAIN_ENERGYMETHOD_COS2 "cos2"

// Log data
#define MAIN_HISTLOGHEADER "Histogram log for %s, iteration: %u, time: %lu\nBin Centres, Counts"
#define MAIN_HISTLOGHEADER_SIZE (strlen(MAIN_HISTLOGHEADER) + 100)
#define MAIN_HISTLOGNAME "%s_%s_%u.csv"
#define MAIN_HISTLOGNAME_SIZE 100
#define MAIN_HISTLOGBASENAME "PlantEvolutionHistLog"
#define MAIN_HISTLOGDELIM ","

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
uint64_t MAIN_GenerateUint64(const MAIN_Uint64Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
int64_t MAIN_GenerateInt64(const MAIN_Int64Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
uint32_t MAIN_GenerateUint32(const MAIN_Uint32Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
int32_t MAIN_GenerateInt32(const MAIN_Int32Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
uint16_t MAIN_GenerateUint16(const MAIN_Uint16Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
int16_t MAIN_GenerateInt16(const MAIN_Int16Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
uint8_t MAIN_GenerateUint8(const MAIN_Uint8Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
int8_t MAIN_GenerateInt8(const MAIN_Int8Constraint *Constraint, uint64_t *Random);

// Generates a uint from constraints
double MAIN_GenerateFloat(const MAIN_FloatConstraint *Constraint, uint64_t *Random);

// Generate a random gene
void MAIN_GenerateGene(MAIN_Map *Map, MAIN_Gene *Gene);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateUint64(const MAIN_Uint64Constraint *Constraint, uint64_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateInt64(const MAIN_Int64Constraint *Constraint, int64_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateUint32(const MAIN_Uint32Constraint *Constraint, uint32_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateInt32(const MAIN_Int32Constraint *Constraint, int32_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateUint16(const MAIN_Uint16Constraint *Constraint, uint16_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateInt16(const MAIN_Int16Constraint *Constraint, int16_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateUint8(const MAIN_Uint8Constraint *Constraint, uint8_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateInt8(const MAIN_Int8Constraint *Constraint, int8_t *Value);

// Truncate an unsigned int, returns true if it did
bool MAIN_TruncateFloat(const MAIN_FloatConstraint *Constraint, double *Value);

// Truncate all values of a gene so that they are legal, returns true if it trancated anything
bool MAIN_TruncateGene(const MAIN_Settings *Settings, MAIN_Gene *Gene);

// Create a plant and mutates the parent genes for it
bool MAIN_CreatePlant(MAIN_Map *Map, MAIN_Tile *Tile, uint32_t Energy, const MAIN_Gene *ParentGene);

// Adds a plant to a tile
bool MAIN_AddToTile(MAIN_Tile *Tile, MAIN_Plant *Plant);

// Removes a plant from a tile
bool MAIN_RemoveFromTile(MAIN_Tile *Tile, const MAIN_Plant *Plant);

// Adds a plant to the map
bool MAIN_AddToMap(MAIN_Map *Map, MAIN_Plant *Plant);

// Removes a plant from the map
bool MAIN_RemoveFromMap(MAIN_Map *Map, const MAIN_Plant *Plant);

// Calculates the energy usage of a plant
uint32_t MAIN_EnergyUsage(const MAIN_Plant *Plant);

// Calculates the biomass for a plant
uint32_t MAIN_Biomass(const MAIN_Plant *Plant);

// Does one step of the evolution
bool MAIN_Step(MAIN_Map *Map);

// Create spawn from a plant
bool MAIN_Spawn(MAIN_Tile *Tile, MAIN_Plant *Parent);

// Grow in height
bool MAIN_GrowHeight(MAIN_Plant *Plant);

// Grow in size
bool MAIN_GrowSize(MAIN_Tile *Tile, MAIN_Plant *Plant);

// Gives the tile in the specified direction
MAIN_Tile *MAIN_GetRelativeTile(MAIN_Map *Map, MAIN_Tile *Tile, int32_t x, int32_t y);

// Checks if a plant is consistent with a filter
bool MAIN_CheckFilter(const MAIN_Filter *Filter, const MAIN_Plant *Plant);

// Logs a uint8 into a histogram
bool MAIN_LogUint8(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint8Settings *LogSettings, const char *Name);

// Logs a uint16 into a histogram
bool MAIN_LogUint16(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint16Settings *LogSettings, const char *Name);

// Logs a uint32 into a histogram
bool MAIN_LogUint32(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint32Settings *LogSettings, const char *Name);

// Logs a uint64 into a histogram
bool MAIN_LogUint64(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint64Settings *LogSettings, const char *Name);

// Logs a int8 into a histogram
bool MAIN_LogInt8(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt8Settings *LogSettings, const char *Name);

// Logs a int16 into a histogram
bool MAIN_LogInt16(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt16Settings *LogSettings, const char *Name);

// Logs a int32 into a histogram
bool MAIN_LogInt32(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt32Settings *LogSettings, const char *Name);

// Logs a int64 into a histogram
bool MAIN_LogInt64(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt64Settings *LogSettings, const char *Name);

// Logs a float into a histogram
bool MAIN_LogFloat(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogFloatSettings *LogSettings, const char *Name);

// Do all the hist logs
bool MAIN_HistLog(MAIN_Map *Map, const MAIN_Filter *Filter);

// Find the number of bits different in the 2 genes
uint32_t MAIN_GeneDiff(const MAIN_Gene *Gene1, const MAIN_Gene *Gene2);

// Find the number of unique species
MAIN_Plant **MAIN_FindUniqueSpecies(MAIN_Map *Map, uint32_t Tolerance, uint32_t **Count, uint32_t *SpeciesCount);

// Init functions
void MAIN_InitUint64Constraint(MAIN_Uint64Constraint *Struct);
void MAIN_InitInt64Constraint(MAIN_Int64Constraint *Struct);
void MAIN_InitUint32Constraint(MAIN_Uint32Constraint *Struct);
void MAIN_InitInt32Constraint(MAIN_Int32Constraint *Struct);
void MAIN_InitUint16Constraint(MAIN_Uint16Constraint *Struct);
void MAIN_InitInt16Constraint(MAIN_Int16Constraint *Struct);
void MAIN_InitUint8Constraint(MAIN_Uint8Constraint *Struct);
void MAIN_InitInt8Constraint(MAIN_Int8Constraint *Struct);
void MAIN_InitFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_InitGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_InitMapSettings(MAIN_MapSettings *Struct);
void MAIN_InitInitialSettings(MAIN_InitialSettings *Struct);
void MAIN_InitEnergySettings(MAIN_EnergySettings *Struct);
void MAIN_InitLogUint8Settings(MAIN_LogUint8Settings *Struct);
void MAIN_InitLogUint16Settings(MAIN_LogUint16Settings *Struct);
void MAIN_InitLogUint32Settings(MAIN_LogUint32Settings *Struct);
void MAIN_InitLogUint64Settings(MAIN_LogUint64Settings *Struct);
void MAIN_InitLogInt8Settings(MAIN_LogInt8Settings *Struct);
void MAIN_InitLogInt16Settings(MAIN_LogInt16Settings *Struct);
void MAIN_InitLogInt32Settings(MAIN_LogInt32Settings *Struct);
void MAIN_InitLogInt64Settings(MAIN_LogInt64Settings *Struct);
void MAIN_InitLogFloatSettings(MAIN_LogFloatSettings *Struct);
void MAIN_InitHistLogSettings(MAIN_HistLogSettings *Struct);
void MAIN_InitSettings(MAIN_Settings *Struct);
void MAIN_InitMap(MAIN_Map *Struct);
void MAIN_InitTile(MAIN_Tile *Struct);
void MAIN_InitPlant(MAIN_Plant *Struct);
void MAIN_InitPlantStats(MAIN_PlantStats *Struct);
void MAIN_InitGene(MAIN_Gene *Struct);
void MAIN_InitSize(MAIN_Size *Struct);

// Clean functions
void MAIN_CleanUint64Constraint(MAIN_Uint64Constraint *Struct);
void MAIN_CleanInt64Constraint(MAIN_Int64Constraint *Struct);
void MAIN_CleanUint32Constraint(MAIN_Uint32Constraint *Struct);
void MAIN_CleanInt32Constraint(MAIN_Int32Constraint *Struct);
void MAIN_CleanUint16Constraint(MAIN_Uint16Constraint *Struct);
void MAIN_CleanInt16Constraint(MAIN_Int16Constraint *Struct);
void MAIN_CleanUint8Constraint(MAIN_Uint8Constraint *Struct);
void MAIN_CleanInt8Constraint(MAIN_Int8Constraint *Struct);
void MAIN_CleanFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_CleanGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_CleanMapSettings(MAIN_MapSettings *Struct);
void MAIN_CleanInitialSettings(MAIN_InitialSettings *Struct);
void MAIN_CleanEnergySettings(MAIN_EnergySettings *Struct);
void MAIN_CleanLogUint8Settings(MAIN_LogUint8Settings *Struct);
void MAIN_CleanLogUint16Settings(MAIN_LogUint16Settings *Struct);
void MAIN_CleanLogUint32Settings(MAIN_LogUint32Settings *Struct);
void MAIN_CleanLogUint64Settings(MAIN_LogUint64Settings *Struct);
void MAIN_CleanLogInt8Settings(MAIN_LogInt8Settings *Struct);
void MAIN_CleanLogInt16Settings(MAIN_LogInt16Settings *Struct);
void MAIN_CleanLogInt32Settings(MAIN_LogInt32Settings *Struct);
void MAIN_CleanLogInt64Settings(MAIN_LogInt64Settings *Struct);
void MAIN_CleanLogFloatSettings(MAIN_LogFloatSettings *Struct);
void MAIN_CleanHistLogSettings(MAIN_HistLogSettings *Struct);
void MAIN_CleanSettings(MAIN_Settings *Struct);
void MAIN_CleanMap(MAIN_Map *Struct);
void MAIN_CleanTile(MAIN_Tile *Struct);
void MAIN_CleanPlant(MAIN_Plant *Struct);
void MAIN_CleanPlantStats(MAIN_PlantStats *Struct);
void MAIN_CleanGene(MAIN_Gene *Struct);
void MAIN_CleanSize(MAIN_Size *Struct);

// Destroy functions
void MAIN_DestroyUint64Constraint(MAIN_Uint64Constraint *Struct);
void MAIN_DestroyInt64Constraint(MAIN_Int64Constraint *Struct);
void MAIN_DestroyUint32Constraint(MAIN_Uint32Constraint *Struct);
void MAIN_DestroyInt32Constraint(MAIN_Int32Constraint *Struct);
void MAIN_DestroyUint16Constraint(MAIN_Uint16Constraint *Struct);
void MAIN_DestroyInt16Constraint(MAIN_Int16Constraint *Struct);
void MAIN_DestroyUint8Constraint(MAIN_Uint8Constraint *Struct);
void MAIN_DestroyInt8Constraint(MAIN_Int8Constraint *Struct);
void MAIN_DestroyFloatConstraint(MAIN_FloatConstraint *Struct);
void MAIN_DestroyGeneConstraints(MAIN_GeneConstraints *Struct);
void MAIN_DestroyMapSettings(MAIN_MapSettings *Struct);
void MAIN_DestroyInitialSettings(MAIN_InitialSettings *Struct);
void MAIN_DestroyEnergySettings(MAIN_EnergySettings *Struct);
void MAIN_DestroyLogUint8Settings(MAIN_LogUint8Settings *Struct);
void MAIN_DestroyLogUint16Settings(MAIN_LogUint16Settings *Struct);
void MAIN_DestroyLogUint32Settings(MAIN_LogUint32Settings *Struct);
void MAIN_DestroyLogUint64Settings(MAIN_LogUint64Settings *Struct);
void MAIN_DestroyLogInt8Settings(MAIN_LogInt8Settings *Struct);
void MAIN_DestroyLogInt16Settings(MAIN_LogInt16Settings *Struct);
void MAIN_DestroyLogInt32Settings(MAIN_LogInt32Settings *Struct);
void MAIN_DestroyLogInt64Settings(MAIN_LogInt64Settings *Struct);
void MAIN_DestroyLogFloatSettings(MAIN_LogFloatSettings *Struct);
void MAIN_DestroyHistLogSettings(MAIN_HistLogSettings *Struct);
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
    if (Settings->map.energyMethod == NULL || strcmp(Settings->map.energyMethod, MAIN_ENERGYMETHOD_CONST) == 0)
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
        TileList->energy = Settings->map.minEnergy + (uint32_t)((double)(Settings->map.maxEnergy - Settings->map.minEnergy) * pow(cos((double)((TileList - Tiles) / Size->w) / (double)Size->h * MAIN_PI), 2.));
}

void MAIN_TileEnergyCos2(const MAIN_Settings *Settings, MAIN_Tile *Tiles, const MAIN_Size *Size)
{
    for (MAIN_Tile *TileList = Tiles, *EndTileList = Tiles + Size->w * Size->h; TileList < EndTileList; ++TileList)
        TileList->energy = Settings->map.minEnergy + (uint32_t)((double)(Settings->map.maxEnergy - Settings->map.minEnergy) * pow(cos((double)((TileList - Tiles) / Size->w) / (double)Size->h * 2 * MAIN_PI), 2.));
}

uint64_t MAIN_GenerateUint64(const MAIN_Uint64Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

int64_t MAIN_GenerateInt64(const MAIN_Int64Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

uint32_t MAIN_GenerateUint32(const MAIN_Uint32Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

int32_t MAIN_GenerateInt32(const MAIN_Int32Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

uint16_t MAIN_GenerateUint16(const MAIN_Uint16Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

int16_t MAIN_GenerateInt16(const MAIN_Int16Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

uint8_t MAIN_GenerateUint8(const MAIN_Uint8Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

int8_t MAIN_GenerateInt8(const MAIN_Int8Constraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + (RNG_RandS(*Random) % (2 * Constraint->spread + 1));
}

double MAIN_GenerateFloat(const MAIN_FloatConstraint *Constraint, uint64_t *Random)
{
    return Constraint->mean - Constraint->spread + RNG_RandSf(*Random) * (double)(2 * Constraint->spread);
}

void MAIN_GenerateGene(MAIN_Map *Map, MAIN_Gene *Gene)
{
    Gene->maxHeight = MAIN_GenerateUint8(&Map->settings->geneConstraints.maxHeight, &Map->random);
    Gene->maxSize = MAIN_GenerateUint8(&Map->settings->geneConstraints.maxSize, &Map->random);
    Gene->efficiency = MAIN_GenerateFloat(&Map->settings->geneConstraints.efficiency, &Map->random);
    Gene->growthRateHeight = MAIN_GenerateFloat(&Map->settings->geneConstraints.growthRateHeight, &Map->random);
    Gene->growthRateSize = MAIN_GenerateFloat(&Map->settings->geneConstraints.growthRateSize, &Map->random);
    Gene->minGrowthEnergyHeight = MAIN_GenerateUint16(&Map->settings->geneConstraints.minGrowthEnergyHeight, &Map->random);
    Gene->minGrowthEnergySize = MAIN_GenerateUint16(&Map->settings->geneConstraints.minGrowthEnergySize, &Map->random);
    Gene->spawnRate = MAIN_GenerateFloat(&Map->settings->geneConstraints.spawnRate, &Map->random);
    Gene->minSpawnEnergy = MAIN_GenerateUint16(&Map->settings->geneConstraints.minSpawnEnergy, &Map->random);
    Gene->maxTileEnergy = MAIN_GenerateUint32(&Map->settings->geneConstraints.maxTileEnergy, &Map->random);
    Gene->spawnEnergy = MAIN_GenerateUint32(&Map->settings->geneConstraints.spawnEnergy, &Map->random);
    Gene->maxSpawnEnergy = MAIN_GenerateUint32(&Map->settings->geneConstraints.maxSpawnEnergy, &Map->random);
    Gene->mutationRate = MAIN_GenerateFloat(&Map->settings->geneConstraints.mutationRate, &Map->random);
    Gene->mutationAttempts = MAIN_GenerateUint8(&Map->settings->geneConstraints.mutationAttempts, &Map->random);
}

bool MAIN_TruncateUint64(const MAIN_Uint64Constraint *Constraint, uint64_t *Value)
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

bool MAIN_TruncateInt64(const MAIN_Int64Constraint *Constraint, int64_t *Value)
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

bool MAIN_TruncateUint32(const MAIN_Uint32Constraint *Constraint, uint32_t *Value)
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

bool MAIN_TruncateInt32(const MAIN_Int32Constraint *Constraint, int32_t *Value)
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

bool MAIN_TruncateUint16(const MAIN_Uint16Constraint *Constraint, uint16_t *Value)
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

bool MAIN_TruncateInt16(const MAIN_Int16Constraint *Constraint, int16_t *Value)
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

bool MAIN_TruncateUint8(const MAIN_Uint8Constraint *Constraint, uint8_t *Value)
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

bool MAIN_TruncateInt8(const MAIN_Int8Constraint *Constraint, int8_t *Value)
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

    Return |= MAIN_TruncateUint8(&Settings->geneConstraints.maxHeight, &Gene->maxHeight);
    Return |= MAIN_TruncateUint8(&Settings->geneConstraints.maxSize, &Gene->maxSize);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.efficiency, &Gene->efficiency);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.growthRateHeight, &Gene->growthRateHeight);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.growthRateSize, &Gene->growthRateSize);
    Return |= MAIN_TruncateUint16(&Settings->geneConstraints.minGrowthEnergyHeight, &Gene->minGrowthEnergyHeight);
    Return |= MAIN_TruncateUint16(&Settings->geneConstraints.minGrowthEnergySize, &Gene->minGrowthEnergySize);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.spawnRate, &Gene->spawnRate);
    Return |= MAIN_TruncateUint16(&Settings->geneConstraints.minSpawnEnergy, &Gene->minSpawnEnergy);
    Return |= MAIN_TruncateUint32(&Settings->geneConstraints.maxTileEnergy, &Gene->maxTileEnergy);
    Return |= MAIN_TruncateUint32(&Settings->geneConstraints.spawnEnergy, &Gene->spawnEnergy);
    Return |= MAIN_TruncateUint32(&Settings->geneConstraints.maxSpawnEnergy, &Gene->maxSpawnEnergy);
    Return |= MAIN_TruncateFloat(&Settings->geneConstraints.mutationRate, &Gene->mutationRate);
    Return |= MAIN_TruncateUint8(&Settings->geneConstraints.mutationAttempts, &Gene->mutationAttempts);

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

    MAIN_Tile **NewTileList = (MAIN_Tile **)realloc(Plant->tileList, sizeof(MAIN_Tile *) * (Plant->stats.size + 1));

    if (NewTileList == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_ADDTOTILE_REALLOC2, strerror(errno), MAIN_ERRORMES_REALLOC, sizeof(MAIN_Tile *) * (Plant->stats.size + 1));
        return false;
    }

    Plant->tileList = NewTileList;

    // Move other plants to find position
    MAIN_Plant **PlantList = NewPlantList + Tile->plantCount++;

    for (; PlantList > NewPlantList && (*(PlantList - 1))->stats.height < Plant->stats.height; --PlantList)
        *PlantList = *(PlantList - 1);

    *PlantList = Plant;

    // Add tile
    Plant->tileList[Plant->stats.size++] = Tile;

    // Update energy stats
    Plant->stats.maxEnergy = Plant->stats.size * Plant->gene.maxTileEnergy;
    Plant->stats.energyUsage = MAIN_EnergyUsage(Plant);
    Plant->stats.biomass = MAIN_Biomass(Plant);

    return true;
}

bool MAIN_RemoveFromTile(MAIN_Tile *Tile, const MAIN_Plant *Plant)
{
    // Find it in the plant list
    if (Tile->plantList == NULL)
    {
        _MAIN_SetError(MAIN_ERRORID_REMOVEFROMTILE_PLANTLIST, MAIN_ERRORMES_NULLPLANTLIST);
        return false;
    }

    MAIN_Plant **PlantList = Tile->plantList + Tile->plantCount / 2;

    for (MAIN_Plant **StartPlantList = Tile->plantList - 1, **EndPlantList = Tile->plantList + Tile->plantCount; StartPlantList < EndPlantList - 1; PlantList = StartPlantList + (EndPlantList - StartPlantList) / 2)
    {
        if ((*PlantList)->stats.height > Plant->stats.height)
            StartPlantList = PlantList;

        else if ((*PlantList)->stats.height < Plant->stats.height)
            EndPlantList = PlantList;

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

    for (; PlantList > NewPlantList && (*(PlantList - 1))->stats.age > Plant->stats.age; --PlantList)
        *PlantList = *(PlantList - 1);

    *PlantList = Plant;

    return true;
}

bool MAIN_RemoveFromMap(MAIN_Map *Map, const MAIN_Plant *Plant)
{
    if (Map->plantList == NULL)
    {
        _MAIN_SetError(MAIN_ERRORID_REMOVEFROMMAP_PLANTLIST, MAIN_ERRORMES_NULLPLANTLIST);
        return false;
    }

    MAIN_Plant **PlantList = Map->plantList + Map->plantCount / 2;

    for (MAIN_Plant **StartPlantList = Map->plantList - 1, **EndPlantList = Map->plantList + Map->plantCount; StartPlantList < EndPlantList - 1; PlantList = StartPlantList + (EndPlantList - StartPlantList) / 2)
    {
        if ((*PlantList)->stats.age < Plant->stats.age)
            StartPlantList = PlantList;

        else if ((*PlantList)->stats.age > Plant->stats.age)
            EndPlantList = PlantList;

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

uint32_t MAIN_EnergyUsage(const MAIN_Plant *Plant)
{
    uint32_t StorageEnergy = (uint32_t)(Plant->map->settings->energy.storageRate * pow((double)Plant->stats.maxEnergy, Plant->map->settings->energy.storagePow));
    uint32_t BaseEnergy = (uint32_t)(Plant->map->settings->energy.baseRate * pow((double)Plant->stats.height, Plant->map->settings->energy.heightPow) * pow((double)Plant->stats.size, Plant->map->settings->energy.sizePow) * exp(1. / (1. - (double)Plant->gene.efficiency) * Plant->map->settings->energy.effPow));
    return StorageEnergy + BaseEnergy + Plant->map->settings->energy.baseCost;
}

bool MAIN_CreatePlant(MAIN_Map *Map, MAIN_Tile *Tile, uint32_t Energy, const MAIN_Gene *ParentGene)
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

    for (uint16_t Attempt = 0; Attempt < ParentGene->mutationAttempts; ++Attempt)
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
    if (Energy > ParentGene->spawnEnergy)
        Energy = ParentGene->spawnEnergy;

    // Remove biomass from it
    if (Plant->stats.biomass > Energy)
    {
        MAIN_DestroyPlant(Plant);
        return true;
    }

    Plant->stats.energy = Energy - Plant->stats.biomass;

    if (Plant->stats.energy > Plant->stats.maxEnergy)
        Plant->stats.energy = Plant->stats.maxEnergy;

    // Make sure it can survive
    if (Plant->stats.energy < Plant->gene.minGrowthEnergyHeight + Plant->stats.energyUsage)
    {
        MAIN_DestroyPlant(Plant);
        return true;
    }

    return true;
}

uint32_t MAIN_Biomass(const MAIN_Plant *Plant)
{
    uint32_t StorageSize = (uint32_t)((double)Plant->stats.maxEnergy * Plant->map->settings->energy.growthStorage);
    uint32_t BaseSize = (uint32_t)((double)(Plant->stats.height * Plant->stats.size) * exp(1. / (1. - (double)Plant->gene.efficiency) * Plant->map->settings->energy.effPow) * Plant->map->settings->energy.growthBase);
    return StorageSize + BaseSize;
}

bool MAIN_Step(MAIN_Map *Map)
{
    // Increase timer
    ++Map->time;

    // Get random tile
    MAIN_Tile *Tile = Map->tiles + (RNG_RandS(Map->random) % (Map->size.w * Map->size.h));

    if (Tile->plantCount != 0)
    {
        // Get energy
        uint32_t Energy = (uint32_t)((double)Tile->energy * (1 - RNG_RandSf(Map->random) * Map->settings->map.energyNoise));

        // Copy plant list
        MAIN_Plant **TempPlantList = (MAIN_Plant **)malloc(sizeof(MAIN_Plant *) * Tile->plantCount);

        if (TempPlantList == NULL)
        {
            _MAIN_AddErrorForeign(MAIN_ERRORID_STEP_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Plant *) * Tile->plantCount);
            return false;
        }

        memcpy(TempPlantList, Tile->plantList, sizeof(MAIN_Plant *) * Tile->plantCount);

        // Run through plant list
        for (MAIN_Plant **PlantList = TempPlantList, **EndPlantList = TempPlantList + Tile->plantCount; PlantList < EndPlantList; ++PlantList)
        {
            // Give energy
            if ((*PlantList)->stats.height > 0)
            {
                uint64_t GetEnergy = (uint32_t)((double)Energy * (*PlantList)->gene.efficiency);
                Energy -= GetEnergy;
                (*PlantList)->stats.energy += GetEnergy;
                if ((*PlantList)->stats.energy > (*PlantList)->stats.maxEnergy)
                    (*PlantList)->stats.energy = (*PlantList)->stats.maxEnergy;
            }

            // Take energy
            if ((*PlantList)->stats.energyUsage > (*PlantList)->stats.energy)
            {
                MAIN_DestroyPlant(*PlantList);
                continue;
            }

            (*PlantList)->stats.energy -= (*PlantList)->stats.energyUsage;

            // Grow in height
            if ((*PlantList)->stats.energy >= (*PlantList)->gene.minGrowthEnergyHeight && (*PlantList)->stats.height < (*PlantList)->gene.maxHeight && RNG_RandSf(Map->random) < (*PlantList)->gene.growthRateHeight)
                if (!MAIN_GrowHeight(*PlantList))
                {
                    _MAIN_AddError(MAIN_ERRORID_STEP_GROWHEIGHT, MAIN_ERRORMES_GROWHEIGHT);
                    free(TempPlantList);
                    return false;
                }

            // Kill if it did not grow
            if ((*PlantList)->stats.height == 0)
            {
                MAIN_DestroyPlant(*PlantList);
                continue;
            }

            // Grow in size
            if ((*PlantList)->stats.energy >= (*PlantList)->gene.minGrowthEnergySize && (*PlantList)->stats.size < (*PlantList)->gene.maxSize && (*PlantList)->stats.height > 0 && RNG_RandSf(Map->random) < (*PlantList)->gene.growthRateSize)
                if (!MAIN_GrowSize(Tile, *PlantList))
                {
                    _MAIN_AddError(MAIN_ERRORID_STEP_GROWSIZE, MAIN_ERRORMES_GROWSIZE);
                    free(TempPlantList);
                    return false;
                }
        
            // Spawn
            if ((*PlantList)->stats.energy >= (*PlantList)->gene.minSpawnEnergy && (*PlantList)->stats.height > 0 && RNG_RandSf(Map->random) < (*PlantList)->gene.spawnRate)
                if (!MAIN_Spawn(Tile, *PlantList))
                {
                    _MAIN_AddError(MAIN_ERRORID_STEP_SPAWN, MAIN_ERRORMES_SPAWN);
                    free(TempPlantList);
                    return false;
                }

            // Kill if it has no energy
            if (Map->settings->killLow && (*PlantList)->stats.energy == 0)
            {
                MAIN_DestroyPlant(*PlantList);
                continue;
            }
        }

        // Free copy of plant list
        free(TempPlantList);
    }

    // Log
    if (Map->settings->histLog.period != 0 && Map->time % Map->settings->histLog.period == 0)
        if (!MAIN_HistLog(Map, NULL))
        {
            _MAIN_AddError(MAIN_ERRORID_STEP_HISTLOG, MAIN_ERRORMES_HISTLOGSTEP, Map->time);
            return false;
        }

    return true;
}

bool MAIN_Spawn(MAIN_Tile *Tile, MAIN_Plant *Parent)
{
    // Take the energy
    uint32_t Energy = Parent->gene.maxSpawnEnergy;

    if (Energy > Parent->gene.spawnEnergy * Parent->map->settings->spawnCount)
        Energy = Parent->gene.spawnEnergy * Parent->map->settings->spawnCount;

    if (Energy > Parent->stats.energy)
        Energy = Parent->stats.energy;

    Parent->stats.energy -= Energy;

    while (Energy > 0)
    {
        // Get how much energy to use
        uint32_t UseEnergy = Parent->gene.spawnEnergy;

        if (UseEnergy > Energy)
            UseEnergy = Energy;

        Energy -= UseEnergy;

        // Get the tile
        int32_t Range = Parent->map->settings->spawnRange / UseEnergy;
        MAIN_Tile *NewTile = MAIN_GetRelativeTile(Parent->map, Tile, (RNG_RandS(Parent->map->random) % (2 * Range + 1)) - Range, (RNG_RandS(Parent->map->random) % (2 * Range + 1)) - Range);

        if (!MAIN_CreatePlant(Parent->map, NewTile, (uint32_t)((double)UseEnergy * Parent->map->settings->energy.spawnEfficiency), &Parent->gene))
        {
            _MAIN_AddError(MAIN_ERRORID_SPAWN_CREATEPLANT, MAIN_ERRORMES_CREATEPLANT);
            return false;
        }
    }
    
    return true;
}

bool MAIN_GrowHeight(MAIN_Plant *Plant)
{
    // Calculate the energy needed
    ++Plant->stats.height;
    uint32_t NewBiomass = MAIN_Biomass(Plant);
    uint32_t NeededEnergy = NewBiomass - Plant->stats.biomass;
    --Plant->stats.height;

    // Test if it has enough energy
    if (NeededEnergy > Plant->stats.energy)
        return true;

    // Update the height in all tiles
    for (MAIN_Tile **TileList = Plant->tileList, **EndTileList = Plant->tileList + Plant->stats.size; TileList < EndTileList; ++TileList)
    {
        // Find plant in plant list
        MAIN_Plant **PlantList = (*TileList)->plantList + (*TileList)->plantCount / 2;

        for (MAIN_Plant **StartPlantList = (*TileList)->plantList - 1, **EndPlantList = (*TileList)->plantList + (*TileList)->plantCount; StartPlantList < EndPlantList - 1; PlantList = StartPlantList + (EndPlantList - StartPlantList) / 2)
        {
            if ((*PlantList)->stats.height > Plant->stats.height)
                StartPlantList = PlantList;

            else if ((*PlantList)->stats.height < Plant->stats.height)
                EndPlantList = PlantList;

            else
                break;
        }

        // Make sure it found it
        MAIN_Plant **FoundPlant = NULL;

        for (MAIN_Plant **TempPlantList = PlantList, **StartTempPlantList = (*TileList)->plantList; TempPlantList >= StartTempPlantList && (*TempPlantList)->stats.height == Plant->stats.height; --TempPlantList)
            if (*TempPlantList == Plant)
            {
                FoundPlant = TempPlantList;
                break;
            }

        if (FoundPlant == NULL)
            for (MAIN_Plant **TempPlantList = PlantList + 1, **EndTempPlantList = (*TileList)->plantList + (*TileList)->plantCount; TempPlantList < EndTempPlantList && (*TempPlantList)->stats.height == Plant->stats.height; ++TempPlantList)
                if (*TempPlantList == Plant)
                {
                    FoundPlant = TempPlantList;
                    break;
                }

        if (FoundPlant == NULL)
        {
            _MAIN_SetError(MAIN_ERRORID_GROWHEIGHT_INTILE, MAIN_ERRORMES_PLANTINTILE);
            return false;
        }

        // Move it
        for (MAIN_Plant **StartPlantList = (*TileList)->plantList; FoundPlant > StartPlantList && (*(FoundPlant - 1))->stats.height < Plant->stats.height + 1; --FoundPlant)
            *FoundPlant = *(FoundPlant - 1);

        *FoundPlant = Plant;
    }

    // Subtract energy, set new biomass and energy usage
    ++Plant->stats.height;
    Plant->stats.energy -= NeededEnergy;
    Plant->stats.biomass = NewBiomass;
    Plant->stats.energyUsage = MAIN_EnergyUsage(Plant);

    return true;
}

bool MAIN_GrowSize(MAIN_Tile *Tile, MAIN_Plant *Plant)
{
    if (Plant->stats.height == 0)
        return true;

    // Calculate the energy needed
    ++Plant->stats.size;
    Plant->stats.maxEnergy += Plant->gene.maxTileEnergy;
    uint32_t NewBiomass = MAIN_Biomass(Plant);
    uint32_t NeededEnergy = NewBiomass - Plant->stats.biomass;
    Plant->stats.maxEnergy -= Plant->gene.maxTileEnergy;
    --Plant->stats.size;

    // Test if it has enough energy
    if (NeededEnergy > Plant->stats.energy)
        return true;

    // Find the next tile
    MAIN_Tile *NewTile = NULL;

    // Go through all directions and test if this plant is already there
    for (uint8_t Direction = RNG_RandS(Plant->map->random) % 4, EndDirection = Direction + 4; Direction < EndDirection; ++Direction)
    {
        int32_t x = 0, y = 0;

        switch (Direction % 4)
        {
            case (MAIN_DIR_POSX):
                x = 1;
                break;

            case (MAIN_DIR_POSY):
                y = 1;
                break;

            case (MAIN_DIR_NEGX):
                x = -1;
                break;

            case (MAIN_DIR_NEGY):
                y = -1;
                break;
            
            default:
                break;
        }

        MAIN_Tile *TestTile = MAIN_GetRelativeTile(Plant->map, Tile, x, y);

        // Attempt to locate the plant
        if (TestTile->plantCount == 0)
        {
            NewTile = TestTile;
            break;
        }

        MAIN_Plant **PlantList = TestTile->plantList + TestTile->plantCount / 2;

        for (MAIN_Plant **StartPlantList = TestTile->plantList - 1, **EndPlantList = TestTile->plantList + TestTile->plantCount; StartPlantList < EndPlantList - 1; PlantList = StartPlantList + (EndPlantList - StartPlantList) / 2)
        {
            if ((*PlantList)->stats.height > Plant->stats.height)
                StartPlantList = PlantList;

            else if ((*PlantList)->stats.height < Plant->stats.height)
                EndPlantList = PlantList;

            else
                break;
        }

        // Make sure it found it
        MAIN_Plant **FoundPlant = NULL;

        for (MAIN_Plant **TempPlantList = PlantList, **StartTempPlantList = TestTile->plantList; TempPlantList >= StartTempPlantList && (*TempPlantList)->stats.height == Plant->stats.height; --TempPlantList)
            if (*TempPlantList == Plant)
            {
                FoundPlant = TempPlantList;
                break;
            }

        if (FoundPlant == NULL)
            for (MAIN_Plant **TempPlantList = PlantList + 1, **EndTempPlantList = TestTile->plantList + TestTile->plantCount; TempPlantList < EndTempPlantList && (*TempPlantList)->stats.height == Plant->stats.height; ++TempPlantList)
                if (*TempPlantList == Plant)
                {
                    FoundPlant = TempPlantList;
                    break;
                }

        if (FoundPlant == NULL)
        {
            NewTile = TestTile;
            break;
        }
    }

    // Stop if there is no possible place for it
    if (NewTile == NULL)
        return true;

    // Add it to the tile
    if (!MAIN_AddToTile(NewTile, Plant))
    {
        _MAIN_AddError(MAIN_ERRORID_GROWSIZE_ADDTOTILE, MAIN_ERRORMES_ADDTOTILE);
        return false;
    }

    // Remove energy
    Plant->stats.energy -= NeededEnergy;

    return true;
}

MAIN_Tile *MAIN_GetRelativeTile(MAIN_Map *Map, MAIN_Tile *Tile, int32_t x, int32_t y)
{
    int32_t CurrentX = (Tile - Map->tiles) % Map->size.w;
    int32_t CurrentY = (Tile - Map->tiles) / Map->size.w;

    int32_t NewX = (CurrentX + x) % Map->size.w;
    int32_t NewY = (CurrentY + y) % Map->size.h;

    return Map->tiles + NewX + NewY * Map->size.w;
}

bool MAIN_CheckFilter(const MAIN_Filter *Filter, const MAIN_Plant *Plant)
{
    return true;
}

bool MAIN_LogUint8(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint8Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT8_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT8_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT8_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT8_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(uint8_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT8_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT8_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT8_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogUint16(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint16Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT16_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT16_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT16_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT16_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(uint16_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT16_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT16_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT16_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogUint32(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint32Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT32_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT32_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT32_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT32_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(uint32_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT32_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT32_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT32_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogUint64(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogUint64Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT64_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGUINT64_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT64_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT64_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(uint64_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT64_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT64_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGUINT64_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogInt8(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt8Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT8_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT8_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT8_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT8_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(int8_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT8_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT8_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT8_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogInt16(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt16Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT16_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT16_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT16_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT16_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(int16_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT16_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT16_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT16_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogInt32(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt32Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT32_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT32_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT32_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT32_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(int32_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT32_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT32_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT32_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogInt64(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogInt64Settings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT64_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGINT64_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, (double)LogSettings->max, (double)LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT64_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double DoubleMin = (double)LogSettings->min;
    double BinDist = ((double)LogSettings->max - (double)LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT64_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)(((double)(*(int64_t *)((void *)(*PlantList) + Offset)) - DoubleMin) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = DoubleMin + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT64_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT64_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGINT64_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_LogFloat(MAIN_Map *Map, const MAIN_Filter *Filter, size_t Offset, const MAIN_LogFloatSettings *LogSettings, const char *Name)
{
    // Make sure the iteration is not too high
    uint32_t Iteration = 0;

    if (Map->settings->histLog.period != 0)
        Iteration = Map->time / Map->settings->histLog.period;

    if (Iteration > Map->settings->histLog.maxFileCount)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGFLOAT_ITERATION, MAIN_ERRORMES_LOGITERATION, Iteration + 2, Map->settings->histLog.maxFileCount);
        return false;
    }

    // Set base name
    char *BaseName = Map->settings->histLog.name;

    if (BaseName == NULL)
        BaseName = MAIN_HISTLOGBASENAME;

    // Make sure max is larger than min
    if (LogSettings->max < LogSettings->min)
    {
        _MAIN_SetError(MAIN_ERRORID_LOGFLOAT_MAXMIN, MAIN_ERRORMES_LOGMAXMIN, LogSettings->max, LogSettings->min);
        return false;
    }

    uint64_t Bins = LogSettings->bins;

    if (Bins == 0)
        Bins = LogSettings->max - LogSettings->min + 1;

    // Get enough memory
    uint64_t *Array = (uint64_t *)malloc(sizeof(uint64_t) * Bins);

    if (Array == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGFLOAT_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint64_t) * Bins);
        return false;
    }

    // Initialize
    memset(Array, 0, sizeof(uint64_t) * Bins);

    // Get the distance between bins
    double BinDist = (LogSettings->max - LogSettings->min) / (double)Bins;

    // Allocate memory for the bin centres
    double *BinCentres = (double *)malloc(sizeof(double) * Bins);

    if (BinCentres == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGFLOAT_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(double) * Bins);
        free(Array);
        return false;
    }

    // Go through all the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Check if it should use it
        if (!MAIN_CheckFilter(Filter, *PlantList))
            continue;

        // Get the bin
        int64_t Bin = (int64_t)((*(double *)((void *)(*PlantList) + Offset) - LogSettings->min) / BinDist);

        if (Bin < 0)
            Bin = 0;

        else if (Bin >= Bins)
            Bin = Bins - 1;

        // Add to hist
        ++Array[Bin];
    }

    // Find the bin centres
    double *TempList = BinCentres;

    for (int64_t Bin = 0; Bin < Bins; ++Bin, ++TempList)
        *TempList = LogSettings->min + ((double)Bin + 0.5) * BinDist;

    // Get header for the file
    size_t HeaderLength = MAIN_HISTLOGHEADER_SIZE + strlen(Name);
    char *Header = (char *)malloc(sizeof(char) * HeaderLength);

    if (Header == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGFLOAT_MALLOC3, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * HeaderLength);
        free(Array);
        free(BinCentres);
        return false;
    }

    snprintf(Header, HeaderLength, MAIN_HISTLOGHEADER, Name, Iteration, Map->time);

    // Create the file name
    size_t FileNameLength = MAIN_HISTLOGNAME_SIZE + strlen(BaseName) + strlen(Name);
    char *FileName = (char *)malloc(sizeof(char) * FileNameLength);

    if (FileName == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGFLOAT_MALLOC4, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(char) * FileNameLength);
        free(Array);
        free(BinCentres);
        free(Header);
        return false;
    }

    snprintf(FileName, FileNameLength, MAIN_HISTLOGNAME, BaseName, Name, Iteration);

    // Save the csv
    if (!FIL_SaveCSV(FileName, Header, MAIN_HISTLOGDELIM, Bins, 2, BinCentres, FIL_DATATYPE_DOUBLE, Array, FIL_DATATYPE_UINT64))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOGFLOAT_SAVE, FIL_GetError(), MAIN_ERRORMES_SAVECSV, FileName);
        free(Array);
        free(BinCentres);
        free(Header);
        free(FileName);
        return false;
    }

    free(Array);
    free(BinCentres);
    free(Header);
    free(FileName);

    return true;
}

bool MAIN_HistLog(MAIN_Map *Map, const MAIN_Filter *Filter)
{
    // Do all of the logs
    if (Map->settings->histLog.maxHeight.active)
        if (!MAIN_LogUint8(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, maxHeight), &Map->settings->histLog.maxHeight, "maxHeight"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MAXHEIGTH, MAIN_ERRORMES_HISTLOG, "maxHeight");
            return false;
        }

    if (Map->settings->histLog.maxSize.active)
        if (!MAIN_LogUint8(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, maxSize), &Map->settings->histLog.maxSize, "maxSize"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MAXSIZE, MAIN_ERRORMES_HISTLOG, "maxSize");
            return false;
        }

    if (Map->settings->histLog.efficiency.active)
        if (!MAIN_LogFloat(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, efficiency), &Map->settings->histLog.efficiency, "efficiency"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_EFFICIENCY, MAIN_ERRORMES_HISTLOG, "efficiency");
            return false;
        }

    if (Map->settings->histLog.growthRateHeight.active)
        if (!MAIN_LogFloat(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, growthRateHeight), &Map->settings->histLog.growthRateHeight, "growthRateHeight"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_GROWTHRATEHEIGHT, MAIN_ERRORMES_HISTLOG, "growthRateHeight");
            return false;
        }

    if (Map->settings->histLog.growthRateSize.active)
        if (!MAIN_LogFloat(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, growthRateSize), &Map->settings->histLog.growthRateSize, "growthRateSize"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_GROWTHRATESIZE, MAIN_ERRORMES_HISTLOG, "growthRateSize");
            return false;
        }

    if (Map->settings->histLog.minGrowthEnergyHeight.active)
        if (!MAIN_LogUint16(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, minGrowthEnergyHeight), &Map->settings->histLog.minGrowthEnergyHeight, "minGrowthEnergyHeight"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MINGROWTHENERGYHEIGHT, MAIN_ERRORMES_HISTLOG, "minGrowthEnergyHeight");
            return false;
        }

    if (Map->settings->histLog.minGrowthEnergySize.active)
        if (!MAIN_LogUint16(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, minGrowthEnergySize), &Map->settings->histLog.minGrowthEnergySize, "minGrowthEnergySize"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MINGROWTHENERGYSIZE, MAIN_ERRORMES_HISTLOG, "minGrowthEnergySize");
            return false;
        }

    if (Map->settings->histLog.spawnRate.active)
        if (!MAIN_LogFloat(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, spawnRate), &Map->settings->histLog.spawnRate, "spawnRate"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_SPAWNRATE, MAIN_ERRORMES_HISTLOG, "spawnRate");
            return false;
        }

    if (Map->settings->histLog.minSpawnEnergy.active)
        if (!MAIN_LogUint16(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, minSpawnEnergy), &Map->settings->histLog.minSpawnEnergy, "minSpawnEnergy"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MINSPAWNENERGY, MAIN_ERRORMES_HISTLOG, "minSpawnEnergy");
            return false;
        }

    if (Map->settings->histLog.maxTileEnergy.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, maxTileEnergy), &Map->settings->histLog.maxTileEnergy, "maxTileEnergy"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MAXTILEENERGY, MAIN_ERRORMES_HISTLOG, "maxTileEnergy");
            return false;
        }

    if (Map->settings->histLog.spawnEnergy.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, spawnEnergy), &Map->settings->histLog.spawnEnergy, "spawnEnergy"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_SPAWNENERGY, MAIN_ERRORMES_HISTLOG, "spawnEnergy");
            return false;
        }

    if (Map->settings->histLog.maxSpawnEnergy.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, maxSpawnEnergy), &Map->settings->histLog.maxSpawnEnergy, "maxSpawnEnergy"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MAXSPAWNENERGY, MAIN_ERRORMES_HISTLOG, "maxSpawnEnergy");
            return false;
        }

    if (Map->settings->histLog.mutationRate.active)
        if (!MAIN_LogFloat(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, mutationRate), &Map->settings->histLog.mutationRate, "mutationRate"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MUTATIONRATE, MAIN_ERRORMES_HISTLOG, "mutationRate");
            return false;
        }

    if (Map->settings->histLog.mutationAttempts.active)
        if (!MAIN_LogUint8(Map, Filter, offsetof(MAIN_Plant, gene) + offsetof(MAIN_Gene, mutationAttempts), &Map->settings->histLog.mutationAttempts, "mutationAttempts"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MUTATIONATTEMPTS, MAIN_ERRORMES_HISTLOG, "mutationAttempts");
            return false;
        }

    if (Map->settings->histLog.size.active)
        if (!MAIN_LogUint8(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, size), &Map->settings->histLog.size, "size"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_SIZE, MAIN_ERRORMES_HISTLOG, "size");
            return false;
        }

    if (Map->settings->histLog.height.active)
        if (!MAIN_LogUint8(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, height), &Map->settings->histLog.height, "height"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_HEIGHT, MAIN_ERRORMES_HISTLOG, "height");
            return false;
        }

    if (Map->settings->histLog.energy.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, energy), &Map->settings->histLog.energy, "energy"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_ENERGY, MAIN_ERRORMES_HISTLOG, "energy");
            return false;
        }

    if (Map->settings->histLog.age.active)
        if (!MAIN_LogUint64(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, age), &Map->settings->histLog.age, "age"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_AGE, MAIN_ERRORMES_HISTLOG, "age");
            return false;
        }

    if (Map->settings->histLog.energyUsage.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, energyUsage), &Map->settings->histLog.energyUsage, "energyUsage"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_ENERGYUSAGE, MAIN_ERRORMES_HISTLOG, "energyUsage");
            return false;
        }

    if (Map->settings->histLog.maxEnergy.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, maxEnergy), &Map->settings->histLog.maxEnergy, "maxEnergy"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_MAXENERGY, MAIN_ERRORMES_HISTLOG, "maxEnergy");
            return false;
        }

    if (Map->settings->histLog.biomass.active)
        if (!MAIN_LogUint32(Map, Filter, offsetof(MAIN_Plant, stats) + offsetof(MAIN_PlantStats, biomass), &Map->settings->histLog.biomass, "biomass"))
        {
            _MAIN_AddError(MAIN_ERRORID_HISTLOG_BIOMASS, MAIN_ERRORMES_HISTLOG, "biomass");
            return false;
        }

    return true;
}

uint32_t MAIN_GeneDiff(const MAIN_Gene *Gene1, const MAIN_Gene *Gene2)
{
    uint32_t Diff = 0;

    // Go through all bits
    for (const uint8_t *UseGene1 = (uint8_t *)Gene1, *UseGene2 = (uint8_t)Gene2, *EndGene = (uint8_t *)Gene1 + sizeof(MAIN_Gene); UseGene1 < EndGene; ++UseGene1, ++UseGene2)
    {
        uint8_t Byte = *UseGene1 ^ *UseGene2;

        for (uint8_t Shift = 0; Shift < 8; ++Shift)
            Diff += (Byte >> Shift) & 1;
    }

    return Diff;
}

MAIN_Plant **MAIN_FindUniqueSpecies(MAIN_Map *Map, uint32_t Tolerance, uint32_t **Count, uint32_t *SpeciesCount)
{
    // Initialise the array
    uint32_t UniqueCount = 0;
    uint32_t MaxCount = 1;
    MAIN_Plant **Unique = (MAIN_Plant **)malloc(sizeof(MAIN_Plant *));

    if (Unique == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_FINDUNIQUESPECIES_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Plant *));
        return NULL;
    }

    uint32_t *Population = (uint32_t *)malloc(sizeof(uint32_t));

    if (Population == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_FINDUNIQUESPECIES_MALLOC2, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(uint32_t));
        free(Unique);
        return NULL;
    }

    // Go through all of the plants
    for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
    {
        // Go through and check if it matches any previous species
        MAIN_Plant **SpeciesList = Unique;
        MAIN_Plant **EndSpeciesList = Unique + UniqueCount;

        for (; SpeciesList < EndSpeciesList; ++SpeciesList)
            if (MAIN_GeneDiff(&(*PlantList)->gene, &(*SpeciesList)->gene) <= Tolerance)
                break;

        // Add a population
        if (SpeciesList < EndSpeciesList)
            ++Population[SpeciesList - EndSpeciesList];

        // Add new species
        else
        {
            // Make lists larger
            if (++UniqueCount > MaxCount)
            {
                MaxCount *= 2;
                MAIN_Plant **NewUnique = (MAIN_Plant **)realloc((void *)Unique, sizeof(MAIN_Plant *) * MaxCount);

                if (NewUnique == NULL)
                {
                    _MAIN_AddErrorForeign(MAIN_ERRORID_FINDUNIQUESPECIES_REALLOC, strerror(errno), MAIN_ERRORMES_REALLOC, sizeof(MAIN_Plant *) * MaxCount);
                    free(Unique);
                    free(Population);
                    return NULL;
                }

                uint32_t *NewPopulation = (uint32_t *)realloc((void *)Population, sizeof(uint32_t) * MaxCount);

                if (NewPopulation == NULL)
                {
                    _MAIN_AddErrorForeign(MAIN_ERRORID_FINDUNIQUESPECIES_REALLOC2, strerror(errno), MAIN_ERRORMES_REALLOC, sizeof(uint32_t) * MaxCount);
                    free(NewUnique);
                    free(Population);
                    return NULL;
                }

                Unique = NewUnique;
                Population = NewPopulation;
            }

            Population[UniqueCount - 1] = 1;
            Unique[UniqueCount - 1] = *PlantList;
        }
    }

    // Shrink the arrays
    Unique = (MAIN_Plant **)realloc((void *)Unique, sizeof(MAIN_Plant *) * UniqueCount);
    Population = (uint32_t *)realloc((void *)Population, sizeof(uint32_t) * UniqueCount);

    // Set the values
    if (Count != NULL)
        *Count = Population;

    else
        free(Population);

    *SpeciesCount = UniqueCount;

    return Unique;
}


void MAIN_InitUint64Constraint(MAIN_Uint64Constraint *Struct)
{
    Struct->min = 0;
    Struct->max = 0xFFFFFFFFFFFFFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitInt64Constraint(MAIN_Int64Constraint *Struct)
{
    Struct->min = -0x8000000000000000;
    Struct->max = -0x7FFFFFFFFFFFFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitUint32Constraint(MAIN_Uint32Constraint *Struct)
{
    Struct->min = 0;
    Struct->max = 0xFFFFFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitInt32Constraint(MAIN_Int32Constraint *Struct)
{
    Struct->min = -0x80000000;
    Struct->max = -0x7FFFFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitUint16Constraint(MAIN_Uint16Constraint *Struct)
{
    Struct->min = 0;
    Struct->max = 0xFFFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitInt16Constraint(MAIN_Int16Constraint *Struct)
{
    Struct->min = -0x8000;
    Struct->max = -0x7FFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitUint8Constraint(MAIN_Uint8Constraint *Struct)
{
    Struct->min = 0;
    Struct->max = 0xFF;
    Struct->mean = 0;
    Struct->spread = 0;
}

void MAIN_InitInt8Constraint(MAIN_Int8Constraint *Struct)
{
    Struct->min = -0x80;
    Struct->max = -0x7F;
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
    Struct->maxHeight.max = 0xFF;
    Struct->maxHeight.mean = 100;
    Struct->maxHeight.spread = 100;

    Struct->maxSize.min = 1;
    Struct->maxSize.max = 0xFF;
    Struct->maxSize.mean = 10;
    Struct->maxSize.spread = 10;

    Struct->efficiency.min = 0.;
    Struct->efficiency.max = 0.99;
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
    Struct->minGrowthEnergyHeight.max = 0xFFFF;
    Struct->minGrowthEnergyHeight.mean = 1000;
    Struct->minGrowthEnergyHeight.spread = 1000;

    Struct->minGrowthEnergySize.min = 0;
    Struct->minGrowthEnergySize.max = 0xFFFF;
    Struct->minGrowthEnergySize.mean = 1000;
    Struct->minGrowthEnergySize.spread = 1000;

    Struct->spawnRate.min = 0.;
    Struct->spawnRate.max = 1.;
    Struct->spawnRate.mean = 0.5;
    Struct->spawnRate.spread = 0.5;

    Struct->minSpawnEnergy.min = 0;
    Struct->minSpawnEnergy.max = 0xFFFF;
    Struct->minSpawnEnergy.mean = 1000;
    Struct->minSpawnEnergy.spread = 1000;

    Struct->maxTileEnergy.min = 1;
    Struct->maxTileEnergy.max = 0xFFFFFFFF;
    Struct->maxTileEnergy.mean = 1000;
    Struct->maxTileEnergy.spread = 1000;

    Struct->spawnEnergy.min = 1;
    Struct->spawnEnergy.max = 0xFFFFFFFF;
    Struct->spawnEnergy.mean = 100;
    Struct->spawnEnergy.spread = 100;

    Struct->maxSpawnEnergy.min = 1;
    Struct->maxSpawnEnergy.max = 0xFFFFFFFF;
    Struct->maxSpawnEnergy.mean = 100;
    Struct->maxSpawnEnergy.spread = 100;

    Struct->mutationRate.min = 0.;
    Struct->mutationRate.max = 1.;
    Struct->mutationRate.mean = 0.5;
    Struct->mutationRate.spread = 0.5;

    Struct->mutationAttempts.min = 0;
    Struct->mutationAttempts.max = 0xFF;
    Struct->mutationAttempts.mean = 10;
    Struct->mutationAttempts.spread = 10;
}

void MAIN_InitMapSettings(MAIN_MapSettings *Struct)
{
    Struct->height = 128;
    Struct->width = 128;
    Struct->minEnergy = 1000;
    Struct->maxEnergy = 10000;
    Struct->energyNoise = 0.2;
    Struct->energyMethod = NULL;
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
    Struct->baseCost = 1;
    Struct->spawnEfficiency = 0.5;
}

void MAIN_InitLogUint8Settings(MAIN_LogUint8Settings *Struct)
{
    Struct->active = false;
    Struct->min = 0;
    Struct->max = 0xFF;
    Struct->bins = 100;
}

void MAIN_InitLogUint16Settings(MAIN_LogUint16Settings *Struct)
{
    Struct->active = false;
    Struct->min = 0;
    Struct->max = 0xFFFF;
    Struct->bins = 100;
}

void MAIN_InitLogUint32Settings(MAIN_LogUint32Settings *Struct)
{
    Struct->active = false;
    Struct->min = 0;
    Struct->max = 0xFFFFFFFF;
    Struct->bins = 100;
}

void MAIN_InitLogUint64Settings(MAIN_LogUint64Settings *Struct)
{
    Struct->active = false;
    Struct->min = 0;
    Struct->max = 0xFFFFFFFFFFFFFFFF;
    Struct->bins = 100;
}

void MAIN_InitLogInt8Settings(MAIN_LogInt8Settings *Struct)
{
    Struct->active = false;
    Struct->min = -0x80;
    Struct->max = 0x7F;
    Struct->bins = 100;
}

void MAIN_InitLogInt16Settings(MAIN_LogInt16Settings *Struct)
{
    Struct->active = false;
    Struct->min = -0x8000;
    Struct->max = 0x7FFF;
    Struct->bins = 100;
}

void MAIN_InitLogInt32Settings(MAIN_LogInt32Settings *Struct)
{
    Struct->active = false;
    Struct->min = -0x80000000;
    Struct->max = 0x7FFFFFFF;
    Struct->bins = 100;
}

void MAIN_InitLogInt64Settings(MAIN_LogInt64Settings *Struct)
{
    Struct->active = false;
    Struct->min = -0x8000000000000000;
    Struct->max = 0x7FFFFFFFFFFFFFFF;
    Struct->bins = 100;
}

void MAIN_InitLogFloatSettings(MAIN_LogFloatSettings *Struct)
{
    Struct->active = false;
    Struct->min = 0.;
    Struct->max = 1.;
    Struct->bins = 100;
}

void MAIN_InitHistLogSettings(MAIN_HistLogSettings *Struct)
{
    Struct->period = 0;
    Struct->name = NULL;
    Struct->maxFileCount = 100;
    MAIN_InitLogUint8Settings(&Struct->maxHeight);    
    MAIN_InitLogUint8Settings(&Struct->maxSize);
    MAIN_InitLogFloatSettings(&Struct->efficiency);
    MAIN_InitLogFloatSettings(&Struct->growthRateHeight);
    MAIN_InitLogFloatSettings(&Struct->growthRateSize);
    MAIN_InitLogUint16Settings(&Struct->minGrowthEnergyHeight);
    MAIN_InitLogUint16Settings(&Struct->minGrowthEnergySize);
    MAIN_InitLogFloatSettings(&Struct->spawnRate);
    MAIN_InitLogUint16Settings(&Struct->minSpawnEnergy);
    MAIN_InitLogUint32Settings(&Struct->maxTileEnergy);
    MAIN_InitLogUint32Settings(&Struct->spawnEnergy);
    MAIN_InitLogUint32Settings(&Struct->maxSpawnEnergy);
    MAIN_InitLogFloatSettings(&Struct->mutationRate);
    MAIN_InitLogUint8Settings(&Struct->mutationAttempts);
    MAIN_InitLogUint8Settings(&Struct->size);
    MAIN_InitLogUint8Settings(&Struct->height);
    MAIN_InitLogUint32Settings(&Struct->energy);
    MAIN_InitLogUint64Settings(&Struct->age);
    MAIN_InitLogUint32Settings(&Struct->energyUsage);
    MAIN_InitLogUint32Settings(&Struct->maxEnergy);
    MAIN_InitLogUint32Settings(&Struct->biomass);    
}

void MAIN_InitSettings(MAIN_Settings *Struct)
{
    MAIN_InitGeneConstraints(&Struct->geneConstraints);
    MAIN_InitMapSettings(&Struct->map);
    MAIN_InitInitialSettings(&Struct->init);
    MAIN_InitEnergySettings(&Struct->energy);
    MAIN_InitHistLogSettings(&Struct->histLog);
    Struct->killIlligal = true;
    Struct->killLow = true;
    Struct->spawnRange = 250;
    Struct->spawnCount = 16;
    Struct->steps = 10;
    Struct->subSteps = 100000;
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
    Struct->maxSpawnEnergy = 0;
    Struct->spawnRate = 0.;
    Struct->spawnSize = 0;
    Struct->spawnSpread = 0;
}

void MAIN_InitSize(MAIN_Size *Struct)
{
    Struct->w = 0;
    Struct->h = 0;
}


void MAIN_CleanUint64Constraint(MAIN_Uint64Constraint *Struct)
{

}

void MAIN_CleanInt64Constraint(MAIN_Int64Constraint *Struct)
{

}

void MAIN_CleanUint32Constraint(MAIN_Uint32Constraint *Struct)
{

}

void MAIN_CleanInt32Constraint(MAIN_Int32Constraint *Struct)
{

}

void MAIN_CleanUint16Constraint(MAIN_Uint16Constraint *Struct)
{

}

void MAIN_CleanInt16Constraint(MAIN_Int16Constraint *Struct)
{

}

void MAIN_CleanUint8Constraint(MAIN_Uint8Constraint *Struct)
{

}

void MAIN_CleanInt8Constraint(MAIN_Int8Constraint *Struct)
{

}

void MAIN_CleanFloatConstraint(MAIN_FloatConstraint *Struct)
{

}

void MAIN_CleanGeneConstraints(MAIN_GeneConstraints *Struct)
{
    // Clean all constraints
    MAIN_CleanUint8Constraint(&Struct->maxHeight);
    MAIN_CleanUint8Constraint(&Struct->maxSize);
    MAIN_CleanFloatConstraint(&Struct->efficiency);
    MAIN_CleanFloatConstraint(&Struct->growthRateHeight);
    MAIN_CleanFloatConstraint(&Struct->growthRateSize);
    MAIN_CleanUint16Constraint(&Struct->minGrowthEnergyHeight);
    MAIN_CleanUint16Constraint(&Struct->minGrowthEnergySize);
    MAIN_CleanFloatConstraint(&Struct->spawnRate);
    MAIN_CleanUint16Constraint(&Struct->minSpawnEnergy);
    MAIN_CleanUint32Constraint(&Struct->maxTileEnergy);
    MAIN_CleanUint32Constraint(&Struct->spawnEnergy);
    MAIN_CleanUint32Constraint(&Struct->maxSpawnEnergy);
    MAIN_CleanFloatConstraint(&Struct->mutationRate);
    MAIN_CleanUint8Constraint(&Struct->mutationAttempts);
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

void MAIN_CleanLogUint8Settings(MAIN_LogUint8Settings *Struct)
{

}

void MAIN_CleanLogUint16Settings(MAIN_LogUint16Settings *Struct)
{

}

void MAIN_CleanLogUint32Settings(MAIN_LogUint32Settings *Struct)
{

}

void MAIN_CleanLogUint64Settings(MAIN_LogUint64Settings *Struct)
{

}

void MAIN_CleanLogInt8Settings(MAIN_LogInt8Settings *Struct)
{

}

void MAIN_CleanLogInt16Settings(MAIN_LogInt16Settings *Struct)
{

}

void MAIN_CleanLogInt32Settings(MAIN_LogInt32Settings *Struct)
{

}

void MAIN_CleanLogInt64Settings(MAIN_LogInt64Settings *Struct)
{

}

void MAIN_CleanLogFloatSettings(MAIN_LogFloatSettings *Struct)
{

}

void MAIN_CleanHistLogSettings(MAIN_HistLogSettings *Struct)
{
    MAIN_InitLogUint8Settings(&Struct->maxHeight);
    MAIN_InitLogUint8Settings(&Struct->maxSize);
    MAIN_InitLogFloatSettings(&Struct->efficiency);
    MAIN_InitLogFloatSettings(&Struct->growthRateHeight);
    MAIN_InitLogFloatSettings(&Struct->growthRateSize);
    MAIN_InitLogUint16Settings(&Struct->minGrowthEnergyHeight);
    MAIN_InitLogUint16Settings(&Struct->minGrowthEnergySize);
    MAIN_InitLogFloatSettings(&Struct->spawnRate);
    MAIN_InitLogUint16Settings(&Struct->minSpawnEnergy);
    MAIN_InitLogUint32Settings(&Struct->maxTileEnergy);
    MAIN_InitLogUint32Settings(&Struct->spawnEnergy);
    MAIN_InitLogUint32Settings(&Struct->maxSpawnEnergy);
    MAIN_InitLogFloatSettings(&Struct->mutationRate);
    MAIN_InitLogUint8Settings(&Struct->mutationAttempts);
    MAIN_InitLogUint8Settings(&Struct->size);
    MAIN_InitLogUint8Settings(&Struct->height);
    MAIN_InitLogUint32Settings(&Struct->energy);
    MAIN_InitLogUint64Settings(&Struct->age);
    MAIN_InitLogUint32Settings(&Struct->energyUsage);
    MAIN_InitLogUint32Settings(&Struct->maxEnergy);
    MAIN_InitLogUint32Settings(&Struct->biomass);
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

    // Clean Hist log
    MAIN_CleanHistLogSettings(&Struct->histLog);
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
        for (MAIN_Tile **TileList = Struct->tileList + Struct->stats.size - 1, **StartTileList = Struct->tileList; TileList >= StartTileList; --TileList)
            if (*TileList != NULL)
                if (!MAIN_RemoveFromTile(*TileList, Struct))
                {
                    _MAIN_AddError(MAIN_ERRORID_CLEANPLANT_INTILE, MAIN_ERRORMES_REMOVEPLANTFROMTILE);
                    continue;
                }

    if (Struct->tileList != NULL)
        free(Struct->tileList);

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


void MAIN_DestroyUint64Constraint(MAIN_Uint64Constraint *Struct)
{
    MAIN_CleanUint64Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyInt64Constraint(MAIN_Int64Constraint *Struct)
{
    MAIN_CleanInt64Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyUint32Constraint(MAIN_Uint32Constraint *Struct)
{
    MAIN_CleanUint32Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyInt32Constraint(MAIN_Int32Constraint *Struct)
{
    MAIN_CleanInt32Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyUint16Constraint(MAIN_Uint16Constraint *Struct)
{
    MAIN_CleanUint16Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyInt16Constraint(MAIN_Int16Constraint *Struct)
{
    MAIN_CleanInt16Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyUint8Constraint(MAIN_Uint8Constraint *Struct)
{
    MAIN_CleanUint8Constraint(Struct);
    free(Struct);
}

void MAIN_DestroyInt8Constraint(MAIN_Int8Constraint *Struct)
{
    MAIN_CleanInt8Constraint(Struct);
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

void MAIN_DestroyLogUint8Settings(MAIN_LogUint8Settings *Struct)
{
    MAIN_CleanLogUint8Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogUint16Settings(MAIN_LogUint16Settings *Struct)
{
    MAIN_CleanLogUint16Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogUint32Settings(MAIN_LogUint32Settings *Struct)
{
    MAIN_CleanLogUint32Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogUint64Settings(MAIN_LogUint64Settings *Struct)
{
    MAIN_CleanLogUint64Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogInt8Settings(MAIN_LogInt8Settings *Struct)
{
    MAIN_CleanLogInt8Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogInt16Settings(MAIN_LogInt16Settings *Struct)
{
    MAIN_CleanLogInt16Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogInt32Settings(MAIN_LogInt32Settings *Struct)
{
    MAIN_CleanLogInt32Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogInt64Settings(MAIN_LogInt64Settings *Struct)
{
    MAIN_CleanLogInt64Settings(Struct);
    free(Struct);
}

void MAIN_DestroyLogFloatSettings(MAIN_LogFloatSettings *Struct)
{
    MAIN_CleanLogFloatSettings(Struct);
    free(Struct);
}

void MAIN_DestroyHistLogSettings(MAIN_HistLogSettings *Struct)
{
    MAIN_CleanHistLogSettings(Struct);
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

    // Do first log
    //MAIN_HistLog(Map, NULL);

    // Check the tile energy
    /*for (MAIN_Tile *TileList = Map->tiles, *EndTileList = Map->tiles + Map->size.w * Map->size.h; TileList < EndTileList; TileList += Map->size.w)
        printf("TileEnergy: %u\n", TileList->energy);*/

    // Print the number of plants
    printf("InitialPlantCount: %u\n", Map->plantCount);

    // Print energy usage
    /*for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
        printf("EnergyUsage: %u\n", (*PlantList)->stats.energyUsage);*/
    //printf("%u, %u, %u, %u\n", (*Map->plantList)->stats.energy, (*Map->plantList)->stats.maxEnergy, (*Map->plantList)->stats.energyUsage, (*Map->plantList)->stats.biomass);

    // Do the simulation
    uint64_t StartTime = clock();

    for (uint64_t Step = 0; Step < Settings->steps; ++Step)
    {
        for (uint64_t SubStep = 0; SubStep < Settings->subSteps; ++SubStep)
            if (!MAIN_Step(Map))
            {
                printf("Error while doing a simulation step: %s\n", MAIN_GetError());
                return -1;
            }

        double Eff = 0;
        uint32_t Size = 0;
        uint32_t MaxSize = 0;
        uint32_t MinEnergySize = 0;
        double SizeRate = 0.;
        double SizeLevel = 0.;
        uint32_t Height = 0;
        uint32_t MaxHeight = 0;
        uint32_t MinEnergyHeight = 0;
        double HeightRate = 0.;
        double HeightLevel = 0.;
        uint32_t Volume = 0;
        uint32_t Storage = 0;
        uint32_t Energy = 0;
        uint32_t Count = 0;
        uint32_t Biomass = 0;
        uint32_t Biomass1 = 0;
        uint32_t Biomass2 = 0;
        uint32_t Age = 0;
        uint32_t Usage = 0;
        uint32_t Usage1 = 0;
        uint32_t Usage2 = 0;
        double FillLevel = 0.;
        uint32_t SizeCount = 0;
        uint32_t SizeCount2 = 0;
        double MutationRate = 0.;
        uint32_t MutationSize = 0;
        uint32_t MinEnergySpawn = 0;
        uint32_t SpawnEnergy = 0;
        uint32_t MaxSpawnEnergy = 0;
        double SpawnRate = 0.;

        for (MAIN_Plant **PlantList = Map->plantList, **EndPlantList = Map->plantList + Map->plantCount; PlantList < EndPlantList; ++PlantList)
        {
            if ((*PlantList)->stats.height > 0)
            {
                Eff += (*PlantList)->gene.efficiency;
                Size += (*PlantList)->stats.size;
                MaxSize += (*PlantList)->gene.maxSize;
                MinEnergySize += (*PlantList)->gene.minGrowthEnergySize;
                SizeRate += (*PlantList)->gene.growthRateSize;
                SizeLevel += (double)(*PlantList)->stats.size / (double)(*PlantList)->gene.maxSize;
                Height += (*PlantList)->stats.height;
                MaxHeight += (*PlantList)->gene.maxHeight;
                MinEnergyHeight += (*PlantList)->gene.minGrowthEnergyHeight;
                HeightRate += (*PlantList)->gene.growthRateHeight;
                HeightLevel += (double)(*PlantList)->stats.height / (double)(*PlantList)->gene.maxHeight;
                Volume += (*PlantList)->stats.size * (*PlantList)->stats.height;
                Storage += (*PlantList)->stats.maxEnergy;
                Energy += (*PlantList)->stats.energy;
                Biomass += (*PlantList)->stats.biomass;
                Biomass1 += (uint32_t)((double)(*PlantList)->stats.maxEnergy * Settings->energy.growthStorage);
                Biomass2 += (uint32_t)((double)((*PlantList)->stats.height * (*PlantList)->stats.size) * exp(1. / (1. - (double)(*PlantList)->gene.efficiency) * Settings->energy.effPow) * Settings->energy.growthBase);
                Age += Map->time - (*PlantList)->stats.age;
                Usage += (*PlantList)->stats.energyUsage;
                Usage1 += (uint32_t)(Settings->energy.storageRate * pow((double)(*PlantList)->stats.maxEnergy, Settings->energy.storagePow));
                Usage2 += (uint32_t)(Settings->energy.baseRate * pow((double)(*PlantList)->stats.height, Settings->energy.heightPow) * pow((double)(*PlantList)->stats.size, Settings->energy.sizePow) * exp(1. / (1. - (double)(*PlantList)->gene.efficiency) * Settings->energy.effPow));
                FillLevel += (double)(*PlantList)->stats.energy / (double)(*PlantList)->stats.maxEnergy;
                if ((*PlantList)->stats.energy > (*PlantList)->stats.maxEnergy) printf("Test: %u, %u\n", (*PlantList)->stats.energy, (*PlantList)->stats.maxEnergy);
                MutationRate += (*PlantList)->gene.mutationRate;
                MutationSize += (*PlantList)->gene.mutationAttempts;
                MinEnergySpawn += (*PlantList)->gene.minSpawnEnergy;
                SpawnEnergy += ((*PlantList)->gene.spawnEnergy > 10000) ? (10000) : ((*PlantList)->gene.spawnEnergy);
                SpawnRate += (*PlantList)->gene.spawnRate;
                MaxSpawnEnergy += ((*PlantList)->gene.maxSpawnEnergy > 10000) ? (10000) : ((*PlantList)->gene.maxSpawnEnergy);
                ++Count;
            }

            SizeCount += (*PlantList)->stats.size;
        }

        for (MAIN_Tile *TileList = Map->tiles, *EndTileList = Map->tiles + Map->size.w * Map->size.h; TileList < EndTileList; ++TileList)
            SizeCount2 += TileList->plantCount;

        if (Count > 0)
        {
            Age /= Count;
            Eff /= Count;
            Size /= Count;
            MaxSize /= Count;
            MinEnergySize /= Count;
            SizeRate /= (double)Count;
            SizeLevel /= (double)Count;
            Height /= Count;
            MaxHeight /= Count;
            MinEnergyHeight /= Count;
            HeightRate /= (double)Count;
            HeightLevel /= (double)Count;
            Volume /= Count;
            Storage /= Count;
            Usage /= Count;
            Usage1 /= Count;
            Usage2 /= Count;
            Energy /= Count;
            Biomass /= Count;
            Biomass1 /= Count;
            Biomass2 /= Count;
            FillLevel /= (double)Count;
            MutationRate /= (double)Count;
            MutationSize /= Count;
            SpawnEnergy /= Count;
            MinEnergySpawn /= Count;
            SpawnRate /= (double)Count;
            MaxSpawnEnergy /= Count;
        }

        printf("PlantCount: %u/%u - ", Map->plantCount, Count);
        printf("SizeCount: %u/%u - ", SizeCount, SizeCount2);
        printf("Age: %u\n", Age);

        printf("Efficiency: %.2f - ", Eff);
        printf("Size: %u/%u(%.2f)[%u/%.2f] - ", Size, MaxSize, SizeLevel, MinEnergySize, SizeRate);
        printf("Height: %u/%u(%.2f)[%u/%.2f] - ", Height, MaxHeight, HeightLevel, MinEnergyHeight, HeightRate);
        printf("Volume: %u\n", Volume);
        printf("Energy: %u - ", Energy);
        printf("Usage: %u[%u/%u] - ", Usage, Usage1, Usage2);
        printf("Storage: %u - ", Storage);
        printf("Level: %.2g - ", FillLevel);
        printf("Biomass: %u[%u/%u]\n", Biomass, Biomass1, Biomass2);
        printf("Spawn: %u/%u[%u/%.2f] - ", SpawnEnergy, MaxSpawnEnergy, MinEnergySpawn, SpawnRate);
        printf("Mutation: %u/%.2f\n\n", MutationSize, MutationRate);
    }
    
    uint64_t EndTime = clock();

    printf("FinalPlantCount: %u\n", Map->plantCount);
    printf("SimulationTime: %.2g s (%.2g s/step)\n", (double)(EndTime - StartTime) / (double)CLOCKS_PER_SEC, (double)(EndTime - StartTime) / (double)CLOCKS_PER_SEC / (double)(Settings->steps * Settings->subSteps));

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
