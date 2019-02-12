//
// Created by Oliver on 12.02.2019.
//

#ifndef VACBOTSIM_VACENV_H
#define VACBOTSIM_VACENV_H

#include "Box2D/Box2D.h"
#include "VacBot.h"
#include "RoomWorld.h"
#include <mutex>



class VacEnv {
    friend class VacEnvWindow;

    VacBot * bot;
    RoomWorld * room;
    //std::mutex acc;
public:
    b2World * world;
    //Motor control Status
    int m_controlStateA;
    int m_controlStateB;

    std::vector<float> step(int input, float dt);

    explicit VacEnv();
    ~VacEnv();

};


#endif //VACBOTSIM_VACENV_H
