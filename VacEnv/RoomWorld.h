//
// Created by Oliver on 12.02.2019.
//

#ifndef VACBOTSIM_ROOMWORLD_H
#define VACBOTSIM_ROOMWORLD_H

#include <vector>
#include "Box2D/Box2D.h"
#include "VacBot.h"

class RoomWorld
{
public:
    explicit RoomWorld(b2World* world, unsigned int mapSizeX = 1024, unsigned mapSizeY = 1024,
              float wXmin = -50, float wXmax = 50, float wYmin = -50, float wYmax = 50);
    ~RoomWorld();

    b2Body* room;
    int lineCount;

    //Coverage Map  --
    std::vector< std::vector<bool> > map;
    //Coverage Count
    int cnt;
    //World Boundary
    const std::pair<float, float> worldBoundX;
    const std::pair<float, float> worldBoundY;

    //Corrdinate Mapping
    inline int worldToMapX(float x) { return static_cast<int>(map_value<float>(worldBoundX, { 0.0f, (float)map.size() }, x)); };
    inline int worldToMapY(float y) { return static_cast<int>(map_value<float>(worldBoundY, { 0.0f, (float)map[0].size() }, y)); };

    inline float MapToWorldX(int x) { return map_value<float>({ 0.0f, (float)map.size() }, worldBoundX, (float)x); };
    inline float MapToWorldY(int y) { return map_value<float>({ 0.0f, (float)map[0].size() }, worldBoundY, (float)y); };

    float dists[3];

    void calculateDistance(VacBot* bot);
    void calculateCoverage(VacBot* bot);

    std::vector<b2Vec2> generateEdge(b2Vec2 from, b2Vec2 to,float pop, float height);
private:

};




#endif //VACBOTSIM_ROOMWORLD_H
