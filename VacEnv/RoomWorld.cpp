//
// Created by Oliver on 12.02.2019.
//

#include "RoomWorld.h"


RoomWorld::RoomWorld(b2World* world, unsigned int mapSizeX, unsigned mapSizeY,
                     float wXmin, float wXmax, float wYmin, float wYmax) :
        map(mapSizeX, std::vector<bool>(mapSizeY, false)),
        worldBoundX({ wXmin,wXmax }),
        worldBoundY({ wYmin,wYmax })
{
    cnt = 0;

    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2ChainShape cShape;

    std::vector< b2Vec2>  edge = generateEdge({ worldBoundX.first, worldBoundY.first },   { worldBoundX.first, worldBoundY.second }, 1, 0.37f);
    std::vector< b2Vec2>  edge2 = generateEdge({ worldBoundX.first, worldBoundY.second }, { worldBoundX.second, worldBoundY.second }, 1, 0.37f);
    std::vector< b2Vec2>  edge3 = generateEdge({ worldBoundX.second, worldBoundY.second },  { worldBoundX.second, worldBoundY.first }, 1, 0.37f);
    std::vector< b2Vec2>  edge4 = generateEdge({ worldBoundX.second, worldBoundY.first },  { worldBoundX.first, worldBoundY.first }, 1, 0.37f);

    edge.pop_back();
    edge2.pop_back();
    edge3.pop_back();
    edge4.pop_back();

    edge.insert(edge.end(), edge2.begin(), edge2.end());
    edge.insert(edge.end(), edge3.begin(), edge3.end());
    edge.insert(edge.end(), edge4.begin(), edge4.end());

    lineCount = edge.size();
    cShape.CreateLoop(edge.data(), edge.size());

    fixtureDef.shape = &cShape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.25f;
    fixtureDef.friction = 0.1f;

    bodyDef.position = b2Vec2(0, 0);
    bodyDef.type = b2_staticBody;

    room = world->CreateBody(&bodyDef);
    room->CreateFixture(&fixtureDef);
}

RoomWorld::~RoomWorld()
{
    room->GetWorld()->DestroyBody(room);
    room = nullptr;
}

void RoomWorld::calculateDistance(VacBot* bot)
{
    //*********************************************************
    //------------------Sensor Ray Tracing---------------------
    //*********************************************************

    for (int i = 0; i < 3; i++) {
        b2RayCastInput input = bot->rays[i];
        //b2Vec2 intersectionNormal;
        dists[i] = 1;
        // g_debugDraw.DrawPoint(input.p1, 5, { 1,1,1 });
        // g_debugDraw.DrawPoint(input.p2, 5, { 1,1,1 });

        float closestFraction = input.maxFraction;
        for (b2Fixture* f = room->GetFixtureList(); f; f = f->GetNext()) {

            b2RayCastOutput output;

            for (int j = 0; j < lineCount; j++) {
                if (!f->RayCast(&output, input, j))
                    continue;

                b2Vec2 intersectionPoint = input.p1 + output.fraction * (input.p2 - input.p1);

                // g_debugDraw.DrawPoint(intersectionPoint, 5, { 1,1,1 });

                if (output.fraction < closestFraction) {
                    closestFraction = output.fraction;
                    //intersectionNormal = output.normal;
                    b2Vec2 dist = (input.p1 - input.p2);
                    dist *= output.fraction;
                    dists[i] = closestFraction;
                }
            }
        }
    }
}

void RoomWorld::calculateCoverage(VacBot* bot) {

    //*********************************************************
    //-------------Calculate Coverage of the map---------------
    //*********************************************************
    b2Vec2 pos = bot->m_body->GetPosition();

    int xmin = worldToMapX(pos.x - bot->bot_radius);
    int ymin = worldToMapY(pos.y - bot->bot_radius);

    int xmax = worldToMapX(pos.x + bot->bot_radius);
    int ymax = worldToMapY(pos.y + bot->bot_radius);
    float bot_rad_sq = bot->bot_radius * bot->bot_radius;
    for (int i = xmin; i <= xmax; i++) {
        for (int j = ymin; j <= ymax; j++) {

            if (i < 0 || i >= map.size()) { continue; }
            if (j < 0 || j >= map[0].size()) { continue; }

            float w_x = MapToWorldX(i);
            float w_y = MapToWorldY(j);
            float r_sq = (b2Vec2(w_x,w_y)  - pos).LengthSquared();

            if (r_sq <= bot_rad_sq) {
                if (!map[i][j]) {
                    map[i][j] = true;
                    cnt++;
                }
            }
        }
    }
}

std::vector<b2Vec2> RoomWorld::generateEdge(b2Vec2 from, b2Vec2 to , float pop, float height)
{
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    std::vector<b2Vec2> edge;


    if (r < pop) {
        float f = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8f - 0.2f)));
        float f2 = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8f - 0.2f)));
        float f4 = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.9f - 0.2f)));

        float f3 = 0.09f * f2;
        b2Vec2 p1 = from + ((f- f3)*(to - from));

        auto edge2 = generateEdge(from, p1, pop / 2, height *0.9f);

        edge.insert(edge.end(),edge2.begin(), edge2.end());

        float dx = to.x - from.x;
        float dy = to.y - from.y;

        b2Vec2 normal1(-dy, dx);
        b2Vec2 normal2(dy, -dx);

        edge.push_back(p1 + (height*f4*normal2));


        b2Vec2 p2 = from + ((f+ f3)*(to - from));
        edge.push_back(p2 + (height*f4*normal2));


        auto edge3 = generateEdge(p2, to, pop / 2, height *0.9f);

        edge.insert(edge.end(), edge3.begin(), edge3.end());

        return edge;
    }
    else {
        edge.push_back(from);
        edge.push_back(to);
        return edge;
    }



}