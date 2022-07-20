#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <Random.h>

// Size of the map
#define MAP_WIDTH 1000
#define MAP_HEIGHT 1000
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)

// Simulation details
#define SIM_STEPSPERFRAME 100000
#define SIM_FRAMES 100
#define SIM_ENERGY 1.0
#define SIM_COST_TOTAL 0.1
#define SIM_COST_EFF 1.0
#define SIM_COST_HEIGHT 1.0
#define SIM_GROWCOST_TILE 1.0
#define SIM_GROWCOST_HEIGHT 1.0
#define SIM_GROWCOST_EFF 1.0
#define SIM_SPAWN_DIST 5

// Initial population
#define INITIAL_COUNT 100
#define INITIAL_ENERGY 10.0

// Stats
#define STATS_MAXHEIGHT_MIN 0
#define STATS_MAXHEIGHT_MAX 1e3
#define STATS_MAXHEIGHT_MEAN 1
#define STATS_MAXHEIGHT_SPREAD 1

#define STATS_MAXSIZE_MIN 1
#define STATS_MAXSIZE_MAX 1000000
#define STATS_MAXSIZE_MEAN 25
#define STATS_MAXSIZE_SPREAD 50

#define STATS_EFFICIENCY_MIN 0
#define STATS_EFFICIENCY_MAX 0.99
#define STATS_EFFICIENCY_MEAN 0.5
#define STATS_EFFICIENCY_SPREAD 1.0

#define STATS_GROWTHRATE_MIN 0
#define STATS_GROWTHRATE_MAX 1
#define STATS_GROWTHRATE_MEAN 0.5
#define STATS_GROWTHRATE_SPREAD 1

#define STATS_MINGROWTHENERGY_MIN 0
#define STATS_MINGROWTHENERGY_MAX 1e12
#define STATS_MINGROWTHENERGY_MEAN 5e2
#define STATS_MINGROWTHENERGY_SPREAD 1e3

#define STATS_MINSPAWNENERGY_MIN 0
#define STATS_MINSPAWNENERGY_MAX 1e9
#define STATS_MINSPAWNENERGY_MEAN 10.0
#define STATS_MINSPAWNENERGY_SPREAD 20.0

#define STATS_MAXTILEENERGY_MIN 0
#define STATS_MAXTILEENERGY_MAX 1e3
#define STATS_MAXTILEENERGY_MEAN 10.0
#define STATS_MAXTILEENERGY_SPREAD 20.0

#define STATS_SPAWNENERGY_MIN 0
#define STATS_SPAWNENERGY_MAX 1e9
#define STATS_SPAWNENERGY_MEAN 10.0
#define STATS_SPAWNENERGY_SPREAD 20.0

#define STATS_SPAWNSIZE_MIN 1
#define STATS_SPAWNSIZE_MAX 1000
#define STATS_SPAWNSIZE_MEAN 25
#define STATS_SPAWNSIZE_SPREAD 50

#define STATS_MUTATIONRATE_MIN 0
#define STATS_MUTATIONRATE_MAX 1
#define STATS_MUTATIONRATE_MEAN 0.5
#define STATS_MUTATIONRATE_SPREAD 1

#define STATS_MUTATIONSIZE_MIN 0
#define STATS_MUTATIONSIZE_MAX 1e6
#define STATS_MUTATIONSIZE_MEAN 0.5
#define STATS_MUTATIONSIZE_SPREAD 1

typedef struct __Plant Plant;
typedef struct __Tile Tile;

// Define a plant
struct __Plant {
    double maxHeight; // Maximum possible height for plant
    uint64_t maxSize; // Maximum number of tiles the plant fills
    double efficiency; // How much of the light it absorbs
    double growthRate; // How fast it should grow
    double minGrowthEnergy; // Minimum required energy in storage to grow
    double minSpawnEnergy; // Minimum required energy in storage to spawn
    double maxTileEnergy; // Maximum energy storage per tile
    double spawnEnergy; // How much energy to use for spawning
    uint32_t spawnSize; // How many children to create when spawning
    double mutationRate; // The probability for a mutation on a gene
    double mutationSize; // How much to mutate

    double maxEnergy; // The maximum energy the plant can carry
    double energy; // The curretn energy of the plant
    double height; // Current height of the plant
    uint64_t size; // The number of tiles the plant inhabits

    Tile **tiles; // List of all the tiles it inhabits
};

struct __Tile {
    Plant **plants; // List of all the plants in the tile from highest to lowest plant
    uint32_t plantCount; // The number of plants

    uint64_t pos; // The position in the board array
    int64_t x; // The x position of the tile
    int64_t y; // The y position of the tile
};

// The list of plants
Plant **PlantList = NULL;
uint32_t PlantCount = 0;
uint32_t PlantMaxCount = 0;

// Board
Tile Map[MAP_SIZE] = {};

// Make mutations
#define MUTATE(Variable, Spread) if (RNG_Randf() < MutationRate) (Variable) += (RNG_Randf() - 0.5) * MutationSize * (Spread)
#define BOUNDSTATS(Variable, Min, Max) if ((Variable) < (Min)) (Variable) = (Min); else if ((Variable) > (Max)) (Variable) = (Max)

// Make sure they are within the correct range
void BoundPos(int64_t *x, int64_t *y)
{
    if (*x < 0)
        *x += (1 - *x / MAP_WIDTH) * MAP_WIDTH;

    if (*y < 0)
        *x += (1 - *y / MAP_HEIGHT) * MAP_HEIGHT;

    *x %= MAP_WIDTH;
    *y %= MAP_HEIGHT;
}

// Change the height of a plant
void UpdateHeight(Tile *Object, Plant *Update)
{
    if (Object->plants == NULL)
        return;

    // Find the object
    Plant **List = Object->plants + Object->plantCount - 1;

    for (; List >= Object->plants; --List)
        if (*List == Update)
            break;
    
    for (; List > Object->plants; --List)
    {
        if (Update->height > (*(List - 1))->height)
            *List = *(List - 1);

        else
            break;
    }

    *List = Update;
}

// Place a plant tile on the map
void PlacePlant(Plant *Object, int64_t x, int64_t y)
{
    extern Tile Map[];

    // Find tile
    uint64_t Pos = y * MAP_WIDTH + x;
    Tile *UseTile = Map + Pos;

    // Add plant
    UseTile->plants = (Plant **)realloc(UseTile->plants, ++UseTile->plantCount * sizeof(Plant *));
    UseTile->plants[UseTile->plantCount - 1] = Object;

    UpdateHeight(UseTile, Object);

    // Add information on plant
    Object->tiles = (Tile **)realloc(Object->tiles, sizeof(Tile *) * ++Object->size);
    Object->tiles[Object->size - 1] = UseTile;

    Object->maxEnergy = Object->size * Object->maxTileEnergy;
}

// Add a plant to the plant list
void AddPlant(Plant *Object)
{
    extern Plant **PlantList;
    extern uint32_t PlantCount;
    extern uint32_t PlantMaxCount;

    // Make room
    if (PlantMaxCount <= PlantCount)
    {
        PlantMaxCount *= 2;
        PlantList = (Plant **)realloc(PlantList, sizeof(Plant *) * PlantMaxCount);

        // Initialise
        for (Plant **List = PlantList + PlantCount, **EndList = PlantList + PlantMaxCount; List < EndList; ++List)
            *List = NULL;
    }

    // Add plant
    PlantList[PlantCount++] = Object;
}

// Remove a plant from a list
void RemovePlantFromList(Plant *Object, Plant ***List, uint32_t *Count)
{
    // Find the plant
    Plant **UseList = *List;

    for (Plant **EndList = *List + *Count; UseList < EndList; ++UseList)
        if (*UseList == Object)
            break;

    UseList += 1;

    // Overwrite
    for (Plant **EndList = *List + *Count; UseList < EndList; ++UseList)
        *(UseList - 1) = *UseList;

    *List = (Plant **)realloc(*List, sizeof(Plant *) * --(*Count));
}

// Remove a plant from the list and all of the tiles
void RemovePlant(Plant *Object)
{
    extern Plant **PlantList;
    extern uint32_t PlantCount;

    // Remove from plant list
    RemovePlantFromList(Object, &PlantList, &PlantCount);

    // Remove from all tiles
    for (Tile **List = Object->tiles, **EndList = Object->tiles + Object->size; List < EndList; ++List)
        RemovePlantFromList(Object, &((*List)->plants), &((*List)->plantCount));
}

// Free a plant struct
void FreePlant(Plant *Object)
{
    // Remove it from plant list
    RemovePlant(Object);

    // Remove it from all the tiles
    if (Object->tiles != NULL)
        free(Object->tiles);

    free(Object);
}

// Generate a plant
Plant *GeneratePlant(Plant *Parent, double StartEnergy, int64_t x, int64_t y)
{
    RNG_Init();

    // Allocate memory
    Plant *NewPlant = (Plant *)malloc(sizeof(Plant));

    if (NewPlant == NULL)
        return NULL;

    double MutationRate = 1;
    double MutationSize = 1;

    // Set base stats
    if (Parent == NULL)
    {
        NewPlant->maxHeight = STATS_MAXHEIGHT_MEAN;
        NewPlant->maxSize = STATS_MAXSIZE_MEAN;
        NewPlant->efficiency = STATS_EFFICIENCY_MEAN;
        NewPlant->growthRate = STATS_GROWTHRATE_MEAN;
        NewPlant->minGrowthEnergy = STATS_MINGROWTHENERGY_MEAN;
        NewPlant->minSpawnEnergy = STATS_MINSPAWNENERGY_MEAN;
        NewPlant->maxTileEnergy = STATS_MAXTILEENERGY_MEAN;
        NewPlant->spawnEnergy = STATS_SPAWNENERGY_MEAN;
        NewPlant->spawnSize = STATS_SPAWNSIZE_MEAN;
        NewPlant->mutationRate = STATS_MUTATIONRATE_MEAN;
        NewPlant->mutationSize = STATS_MUTATIONSIZE_MEAN;
    }

    else
    {
        NewPlant->maxHeight = Parent->maxHeight;
        NewPlant->maxSize = Parent->maxSize;
        NewPlant->efficiency = Parent->efficiency;
        NewPlant->growthRate = Parent->growthRate;
        NewPlant->minGrowthEnergy = Parent->minGrowthEnergy;
        NewPlant->minSpawnEnergy = Parent->minSpawnEnergy;
        NewPlant->maxTileEnergy = Parent->maxTileEnergy;
        NewPlant->spawnEnergy = Parent->spawnEnergy;
        NewPlant->spawnSize = Parent->spawnSize;
        NewPlant->mutationRate = Parent->mutationRate;
        NewPlant->mutationSize = Parent->mutationSize;

        MutationRate = Parent->mutationRate;
        MutationSize = Parent->mutationSize;
    }

    // Mutate
    MUTATE(NewPlant->maxHeight, STATS_MAXHEIGHT_SPREAD);
    MUTATE(NewPlant->maxSize, STATS_MAXSIZE_SPREAD);
    MUTATE(NewPlant->efficiency, STATS_EFFICIENCY_SPREAD);
    MUTATE(NewPlant->growthRate, STATS_GROWTHRATE_SPREAD);
    MUTATE(NewPlant->minGrowthEnergy, STATS_MINGROWTHENERGY_SPREAD);
    MUTATE(NewPlant->minSpawnEnergy, STATS_MINSPAWNENERGY_SPREAD);
    MUTATE(NewPlant->maxTileEnergy, STATS_MAXTILEENERGY_SPREAD);
    MUTATE(NewPlant->spawnEnergy, STATS_SPAWNENERGY_SPREAD);
    MUTATE(NewPlant->spawnSize, STATS_SPAWNSIZE_SPREAD);
    MUTATE(NewPlant->mutationRate, STATS_MUTATIONRATE_SPREAD);
    MUTATE(NewPlant->mutationSize, STATS_MUTATIONSIZE_SPREAD);

    // Cut off the values
    BOUNDSTATS(NewPlant->maxHeight, STATS_MAXHEIGHT_MIN, STATS_MAXHEIGHT_MAX);
    BOUNDSTATS(NewPlant->maxSize, STATS_MAXSIZE_MIN, STATS_MAXSIZE_MAX);
    BOUNDSTATS(NewPlant->efficiency, STATS_EFFICIENCY_MIN, STATS_EFFICIENCY_MAX);
    BOUNDSTATS(NewPlant->growthRate, STATS_GROWTHRATE_MIN, STATS_GROWTHRATE_MAX);
    BOUNDSTATS(NewPlant->minGrowthEnergy, STATS_MINGROWTHENERGY_MIN, STATS_MINGROWTHENERGY_MAX);
    BOUNDSTATS(NewPlant->minSpawnEnergy, STATS_MINSPAWNENERGY_MIN, STATS_MINSPAWNENERGY_MAX);
    BOUNDSTATS(NewPlant->maxTileEnergy, STATS_MAXTILEENERGY_MIN, STATS_MAXTILEENERGY_MAX);
    BOUNDSTATS(NewPlant->spawnEnergy, STATS_SPAWNENERGY_MIN, STATS_SPAWNENERGY_MAX);
    BOUNDSTATS(NewPlant->spawnSize, STATS_SPAWNSIZE_MIN, STATS_SPAWNSIZE_MAX);
    BOUNDSTATS(NewPlant->mutationRate, STATS_MUTATIONRATE_MIN, STATS_MUTATIONRATE_MAX);
    BOUNDSTATS(NewPlant->mutationSize, STATS_MUTATIONSIZE_MIN, STATS_MUTATIONSIZE_MAX);

    // Set start values
    NewPlant->maxEnergy = 0;
    NewPlant->energy = StartEnergy;
    NewPlant->height = 0;
    NewPlant->size = 0;
    NewPlant->tiles = NULL;

    // Place it
    PlacePlant(NewPlant, x, y);

    // Add plant to plant list
    AddPlant(NewPlant);

    if (NewPlant->energy > NewPlant->maxEnergy)
        NewPlant->energy = NewPlant->maxEnergy;

    return NewPlant;
}

// Initialise the map
void InitMap(void)
{
    // Initialise map
    extern Tile Map[];

    for (Tile *CurrentTile = Map, *MaxTile = Map + MAP_SIZE; CurrentTile < MaxTile; ++CurrentTile)
    {
        CurrentTile->plants = NULL;
        CurrentTile->plantCount = 0;
        CurrentTile->pos = (uint64_t)(CurrentTile - Map);
        CurrentTile->x = CurrentTile->pos % MAP_WIDTH;
        CurrentTile->y = CurrentTile->pos / MAP_WIDTH;
    }
}

// Initialise the plants
void InitPlants(void)
{
    // Initialise randomness
    RNG_Init();
    RNG_RandSeed();

    // Initialise plant list
    extern Plant **PlantList;
    extern uint32_t PlantCount;
    extern uint32_t PlantMaxCount;

    PlantList = (Plant **)malloc(sizeof(Plant *));
    PlantMaxCount = 1;
    PlantList[0] = NULL;

    // Fill initial population
    for (uint32_t Var = 0; Var < INITIAL_COUNT; ++Var)
        GeneratePlant(NULL, INITIAL_ENERGY, RNG_Rand() % MAP_WIDTH, RNG_Rand() % MAP_HEIGHT);
}

// Clean up everything
void Quit(void)
{
    extern Plant **PlantList;
    extern uint32_t PlantCount;

    for (Plant **List = PlantList, **EndList = PlantList + PlantCount; List < EndList; ++List)
        if (*List != NULL)
            FreePlant(*List);
        
    free(PlantList);
}

// Remove energy from plant dependent on how much energy it requires
void RemoveEnergy(Plant *Object)
{
    // Calculate the energy cost
    double EnergyCost = SIM_COST_TOTAL * exp(SIM_COST_EFF / (1.0 - Object->efficiency)) * pow(Object->height, SIM_COST_HEIGHT);

    // Remove energy
    Object->energy -= EnergyCost;

    // Check if it dies
    if (Object->energy < 0)
        FreePlant(Object);
}

void Grow(Plant *Object, int64_t x, int64_t y)
{
    RNG_Init();
    extern Tile Map[];

    // Test if it should grow
    if (Object->energy > Object->minGrowthEnergy)
    {
        // Check if it should grow a new tile
        if (RNG_Randf() < Object->growthRate)
        {
            // Get position
            uint64_t RNG = RNG_Rand() % 4;

            if (RNG % 2 == 0)
                x += 1;

            else
                x -= 1;

            if (RNG / 2 == 0)
                y += 1;

            else
                y -= 1;

            BoundPos(&x, &y);

            // Make sure the position is vacant
            Tile *NewTile = Map + y * MAP_WIDTH + x;

            Tile **List = Object->tiles;

            for (Tile **EndList = Object->tiles + Object->size; List < EndList; ++List)
                if (*List == NewTile)
                    break;

            if (List == Object->tiles + Object->size)
            {
                PlacePlant(Object, x, y);

                // Remove energy
                Object->energy -= SIM_GROWCOST_TILE * exp(SIM_GROWCOST_EFF / (1.0 - Object->efficiency));
            }
        }

        // Grow in height
        double AddHeight = (Object->maxHeight - Object->height) * Object->growthRate;
        Object->height += AddHeight;
        Object->energy -= Object->size * SIM_GROWCOST_HEIGHT;
    }

    // Kill it if needed
    if (Object->energy < 0)
        FreePlant(Object);
}

void Spawn(Plant *Object, int64_t x, int64_t y)
{
    RNG_Init();
    extern Tile Map[];

    // Test if it should grow
    if (Object->energy > Object->minSpawnEnergy)
    {
        // Calculate energy per seed
        double EnergyPerSeed = Object->spawnEnergy;

        if (EnergyPerSeed > Object->spawnEnergy)
            EnergyPerSeed = Object->spawnEnergy;

        EnergyPerSeed  /= Object->spawnSize;

        // Subtract energy
        Object->energy -= Object->spawnEnergy;

        // Add new plants
        for (uint32_t Var = 0; Var < Object->spawnSize; ++Var)
        {
            int64_t NewX = x + RNG_Rand() % (2 * SIM_SPAWN_DIST + 1) - SIM_SPAWN_DIST;
            int64_t NewY = y + RNG_Rand() % (2 * SIM_SPAWN_DIST + 1) - SIM_SPAWN_DIST;

            BoundPos(&NewX, &NewY);

            GeneratePlant(Object, EnergyPerSeed, NewX, NewY);
        }
    }

    // Kill it if needed
    if (Object->energy < 0)
        FreePlant(Object);
}

// Do one step of the simulation by update a certain position
void DoStep(int64_t x, int64_t y)
{
    extern Tile Map[];

    // Find tile
    Tile *UseTile = Map + y * MAP_WIDTH + x;

    // Stop if there are no plants
    if (UseTile->plantCount == 0)
        return;

    // Give energy to plants
    double EnergyLeft = SIM_ENERGY;

    for (Plant **List = UseTile->plants + UseTile->plantCount - 1; List >= UseTile->plants; --List)
    {
        (*List)->energy += EnergyLeft * (*List)->efficiency;
        EnergyLeft *= 1 - (*List)->efficiency;

        if ((*List)->energy > (*List)->maxEnergy)
            (*List)->energy = (*List)->maxEnergy;
    }

    // Remove energy cost
    for (Plant **List = UseTile->plants; List < UseTile->plants + UseTile->plantCount; ++List)
        RemoveEnergy(*List);

    // Growth
    for (Plant **List = UseTile->plants; List < UseTile->plants + UseTile->plantCount; ++List)
        Grow(*List, x, y);

    // Spawn
    for (Plant **List = UseTile->plants; List < UseTile->plants + UseTile->plantCount; ++List)
        Spawn(*List, x, y);
}

// Main loop
int main(int argc, char **argv)
{
    // Initialise
    InitMap();
    InitPlants();
    
    // Go through the game loop
    RNG_Init();
    extern uint32_t PlantCount;

    for (uint64_t Frame = 0; Frame < SIM_FRAMES; ++Frame)
    {
        for (uint64_t Step = 0; Step < SIM_STEPSPERFRAME; ++Step)
            ;//DoStep(RNG_Rand() % MAP_WIDTH, RNG_Rand() % MAP_HEIGHT);

        printf("Finished frame %u/%u - PlantCount = %u\n", Frame + 1, SIM_FRAMES, PlantCount);
    }
printf("Test\n");
    // Clean up
    Quit();

    printf("Done\n");

    return 0;
}