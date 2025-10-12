#include "../global.hpp"

Area firstIsland;
const char *firstIslandPath = "assets/models/firstIsland.glb";

void firstIslandInit() {
    firstIsland.modelPath = firstIslandPath;
    firstIsland.position = Vector3Zero();
    firstIsland.scale = 1.0f;
}