//
// Created by Oliver on 12.02.2019.
//

#include "VacEnv.h"

#include "simple2d.h"

VacEnv::VacEnv() {
    world = new b2World({0.0f,0.0f});
    bot = new VacBot(world,0.5);
    room = new RoomWorld(world,1024,1024);

    S2D_Window *window = S2D_CreateWindow(
            "Hello World!",  // title of the window
            800, 600,        // width and height
            0, 0,  // callback function pointers (these can be NULL)
            0                // flags
    );
}



VacEnv::~VacEnv() {
    delete world;
    delete bot;
    delete room;
}

std::vector<float> VacEnv::step(int input, float dt) {

    int controlA;
    int controlB;

    switch (input) {
        case 1:
            controlA = TDC_UP;
            controlB = TDC_UP;
            break;
        case 2:
            controlA = TDC_DOWN;
            controlB = TDC_DOWN;
            break;
        case 3:
            controlA = TDC_UP;
            controlB = TDC_DOWN;
            break;
        case 4:
            controlA = TDC_DOWN;
            controlB = TDC_UP;
            break;
        default:
            controlA = 0;
            controlB = 0;
            break;
    }

    bot->m_controlStateA = controlA;
    bot->m_controlStateB = controlB;
    bot->updateDrive(dt);

    //acc.lock();
    world->Step(dt, 16, 16);
    room->calculateCoverage(bot);
    //acc.unlock();

    room->calculateDistance(bot);


    float cov = (static_cast<float>(room->cnt) / (1024.0f*1024.0f));
    float distA = bot->tireA->distance / 255.0f;
    float distB = bot->tireB->distance / 255.0f;
    float rot = fmod( bot->m_body->GetAngle()* RADTODEG, 360.0f) / 360.0f;
    float accX = bot->acc.x / 20.0f;
    float accY = bot->acc.y / 100.0f;
    float dist1 = 1-room->dists[0];
    float dist2 = 1-room->dists[1];
    float dist3 = 1-room->dists[2];
    return {cov,distA,distB,rot,accX,accY,dist1,dist2,dist3};
}
