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
    MAIN_ERRORID_LOADSETTINGS_TRANSLATE = 0x00010202
};

#define MAIN_ERRORMES_MALLOC "Unable to allocate memory (Size: %lu)"
#define MAIN_ERRORMES_LOADSETTINGS "Unable to load settings (FileName: %s)"
#define MAIN_ERRORMES_TRANSLATESETTINGS "Unable to translate settings (FileName: %s)"

// Settings
typedef struct __MAIN_Settings MAIN_Settings;
typedef struct __MAIN_GeneConstraints MAIN_GeneConstraints;
typedef struct __MAIN_UintConstraint MAIN_UintConstraint;
typedef struct __MAIN_IntConstraint MAIN_IntConstraint;
typedef struct __MAIN_FloatConstraint MAIN_FloatConstraint;

struct __MAIN_UintConstraint
{
    uint64_t min;    // The minimum allowed value
    uint64_t max;    // The maximum allowed value
    uint64_t mean;   // The mean starting value
    uint64_t spread; // The spread of the starting value
};

struct __MAIN_IntConstraint
{
    int64_t min;    // The minimum allowed value
    int64_t max;    // The maximum allowed value
    int64_t mean;   // The mean starting value
    int64_t spread; // The spread of the starting value
};

struct __MAIN_FloatConstraint
{
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

struct __MAIN_Settings
{
    MAIN_GeneConstraints geneConstraints; // All the gene constraints
    bool killIlligal;                     // If true then it should kill any plant created with illigal genes, if false they should be trunctated
};

// Settings translation tables
#define MAIN_SETTINGSCONSTRAINTCOUNT 4
#define MAIN_SETTINGSGENECONSTRAINTCOUNT 15
#define MAIN_SETTINGSCOUNT 2

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

SET_TranslationTable MAIN_SettingsTableMain[MAIN_SETTINGSCOUNT] = {
    {.name = "geneConstraints", .type = SET_DATATYPE_STRUCT, .depth = 0, .offset = offsetof(MAIN_Settings, geneConstraints), .size = sizeof(MAIN_GeneConstraints), .sub = MAIN_SettingsTableGeneConstrains, .count = MAIN_SETTINGSGENECONSTRAINTCOUNT},
    {.name = "killIlligal", .type = SET_DATATYPE_BOOL, .depth = 0, .offset = offsetof(MAIN_Settings, killIlligal)}
};

// Functions
// Load a settings file
MAIN_Settings *MAIN_LoadSettings(const char *FileName);

// Init functions
void MAIN_InitSettings(MAIN_Settings *Struct);

// Destroy functions
void MAIN_DestroySettings(MAIN_Settings *Struct);

MAIN_Settings *MAIN_LoadSettings(const char *FileName)
{
    // Get memory
    MAIN_Settings *Settings = (MAIN_Settings *)malloc(sizeof(MAIN_Settings));

    if (Settings == NULL)
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOADSETTINGS_MALLOC, strerror(errno), MAIN_ERRORMES_MALLOC, sizeof(MAIN_Settings));
        return NULL;
    }

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
    if (!SET_Translate(Settings, SettingsDict, MAIN_SettingsTableMain, MAIN_SETTINGSCOUNT, SET_TRANSLATIONMODE_FILL))
    {
        _MAIN_AddErrorForeign(MAIN_ERRORID_LOADSETTINGS_TRANSLATE, SET_GetError(), MAIN_ERRORMES_TRANSLATESETTINGS, FileName);
        SET_DestroyDataStruct(SettingsDict);
        free(Settings);
        return NULL;
    }

    SET_DestroyDataStruct(SettingsDict);

    return Settings;
}

void MAIN_InitSettings(MAIN_Settings *Struct)
{
}

void MAIN_DestroySettings(MAIN_Settings *Struct)
{
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

    // Free settings
    MAIN_DestroySettings(Settings);

    printf("Finished without errors\n");

    return 0;
}