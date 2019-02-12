//
// Created by Oliver on 12.02.2019.
//

#ifndef VACBOTSIM_VACENVWINDOW_H
#define VACBOTSIM_VACENVWINDOW_H

#include <thread>
#include <utility>
#include <vector>


#include "Box2D/Box2D.h"
#include <simple2d.h>


class VacEnv;


class VacEnvWindow {

public:
    static void render();
    static void render_world(b2World* w);
    static void render_body(const b2Body* b);
    static void render_shape(const b2Shape* s,b2Vec2 p, b2Vec2 rot);
    static void render_map(std::vector< std::vector<bool> > map);
    static S2D_Window *window;
    static std::vector<VacEnv*> * worlds;
    static unsigned int m_size;
    static float scale_x;
    static float scale_y;
    static float offset_x;
    static float offset_y;

    static void init(unsigned int m_size, float wxmin,float wxmax,float wymin,float wymax);
    static void close();
    static int run();
};



#endif //VACBOTSIM_VACENVWINDOW_H
