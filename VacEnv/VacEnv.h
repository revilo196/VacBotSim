//
// Created by Oliver on 12.02.2019.
//

#ifndef VACBOTSIM_VACENV_H
#define VACBOTSIM_VACENV_H

#include <mutex>
#include <vector>

class VacBot;
class RoomWorld;
class b2World;

class VacEnv {
    friend class VacEnvWindow;
    VacBot * bot;
    RoomWorld * room;


public:
    //std::mutex acc;
    b2World * world;
    float score;
    int collision;
    bool running;
    //Motor control Status
    //int m_controlStateA;
    //int m_controlStateB;
    void reset();
    std::vector<double> step(int input, float dt);

    VacEnv();
    ~VacEnv();

};





#endif //VACBOTSIM_VACENV_H
