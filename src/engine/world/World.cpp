//
// Created by Diego S. Seabra on 23/09/22.
//

#include "World.h"
#include "../rendering/Sprite.h"

using json = nlohmann::json;

void World::Init()
{
//    CreateGrid();
    Load();
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

void World::Load()
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

        if (obj["type"] == "grid")
        {
            gameObj.shape = Shape(ShapeType::Quad);
            gameObj.pipeline = "grid";
        }

        if (obj["type"] == "grid_plane")
        {
            gameObj.shape = Shape(ShapeType::Quad);
            gameObj.pipeline = "grid_plane";
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

void World::CreateGrid()
{
    // create the world grid object
    Grid::Create(20,20);

    // draw tiles
    auto tiles = Grid::GetTiles();
    for (auto &tile : tiles)
    {
        GameObject gridObj = GameObject::Create();
        gridObj.id = currentId;
        gridObj.name = tile.name;
        gridObj.shape = Shape(ShapeType::Quad);
        gridObj.pipeline = "grid";
        gridObj.color = Color::Red;
        gridObj.transform.position = tile.worldPosition;
        gridObj.transform.scale = {
                Grid::TileSize(),
                Grid::TileSize(),
                Grid::TileSize()
        };
        gridObj.transform.rotation = Grid::Rotation();

        gameObjects.push_back(gridObj);
        currentId++;
    }
}

void World::DestroyGrid()
{
    // TODO: Destroy grid
}
