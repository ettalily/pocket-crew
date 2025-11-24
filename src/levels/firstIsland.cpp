#include "../global.hpp"

Area firstIsland;
const char *firstIslandPath = "assets/models/firstIsland.glb";

void Logic() {
    for (Collectable& it : firstIsland.collectables) {
        it.PickupCheck();
    }
}

void InitCollectables() {
    firstIsland.collectables.push_back(Collectable{ (Vector3){ -5.0f, 2.0f, 0.0f } });
    firstIsland.collectables.push_back(Collectable{ (Vector3){ -7.0f, 5.0f, 0.0f } });
    firstIsland.collectables.push_back(Collectable{ (Vector3){ -9.0f, 5.0f, 0.0f } });
    firstIsland.collectables.push_back(Collectable{ (Vector3){ -12.0f, 8.0f, 0.0f } });
}

void firstIslandInit() {
    firstIsland.modelPath = firstIslandPath;
    firstIsland.position = Vector3Zero();
    firstIsland.scale = 1.0f;
    firstIsland.AreaLogic = Logic;

    InitCollectables();
}