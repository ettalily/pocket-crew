#include "../global.hpp"

Area firstIsland;
const char *firstIslandPath = "assets/models/firstIsland.glb";

void Logic() {
    bugCollectables[0].PickupCheck();
    bugCollectables[1].PickupCheck();
    bugCollectables[2].PickupCheck();
}

void FirstIslandInit() {
    firstIsland.modelPath = firstIslandPath;
    firstIsland.position = Vector3Zero();
    firstIsland.scale = 1.0f;
    firstIsland.AreaLogic = Logic;
}