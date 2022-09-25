//
// Created by Diego S. Seabra on 23/09/22.
//

#include "World.h"
#include "../rendering/Sprite.h"
#include <ldtk/inc/Project.hpp>

using json = nlohmann::json;

void World::Init()
{
    LoadLDTK();
//    LoadJSON();
}

void World::Update()
{
    // TODO: Implement
}

void World::Render()
{
    // NOTE: Rendering is done in the "RenderGameObjects" of the TestRenderSystem
}

void World::Shutdown()
{
    for (auto &obj: gameObjects)
    {
        obj.shape.Destroy();
    }

    std::destroy(gameObjects.begin(), gameObjects.end());
}

void World::Save()
{

}

void World::LoadLDTK()
{
    ldtk::Project ldtk_project;
    ldtk_project.loadFromFile("assets/world/world.ldtk");

    const auto& world = ldtk_project.getWorld();

    const auto& level0 = world.getLevel(0);

    const auto& entitiesLayer = level0.getLayer("Entities");
    const auto& tilesLayer = level0.getLayer("Tiles");

    // iterate on the tiles of the layer
    for (const auto& tile : tilesLayer.allTiles()) {
        auto verts = tile.getVertices();
        std::cout << "something" << std::endl;
        // do something with the tile (storing, rendering ...)
    }
}

void World::LoadJSON()
{
    std::ifstream f("assets/jsons/world.json");
    json data = json::parse(f);
    auto objs = data["world"]["objects"];

    for (auto &obj: objs)
    {
        GameObject gameObj = GameObject::Create();

        if (obj["type"] == "shape")
        {
            gameObj = GameObject::Create();
            gameObj.shape = Shape(Shape::GetEnumFromString(obj["shape"]));
            gameObj.pipeline = "shape";
        }

        if (obj["type"] == "sprite")
        {
            auto tile = obj["tile"];
            auto sprite = Sprite(tile[0], tile[1]);
            sprite.shape = Shape(ShapeType::Quad);
            sprite.pipeline = "sprite";
            sprite.SetTile();

            gameObj = sprite;
        }

        gameObj.id = currentId;
        gameObj.name = obj["name"];
        gameObj.color = Color::GetColorFromString(obj["color"]);
        gameObj.transform.position = {
                obj["transform"]["position"][0],
                obj["transform"]["position"][1],
                obj["transform"]["position"][2],
        };
        gameObj.transform.scale = {
                obj["transform"]["scale"][0],
                obj["transform"]["scale"][1],
                obj["transform"]["scale"][2],
        };
        gameObj.transform.rotation = obj["transform"]["rotation"];

        gameObjects.push_back(gameObj);
        currentId++;
    }
}

std::vector<GameObject> *World::GetGameObjects()
{
    return &gameObjects;
}


