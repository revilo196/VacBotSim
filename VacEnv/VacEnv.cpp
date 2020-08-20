//
// Created by Oliver on 12.02.2019.
//

#include "VacEnv.h"
#include "Box2D/Box2D.h"
#include "VacBot.h"
#include "RoomWorld.h"



VacEnv::VacEnv() {
    world = new b2World({0.0f,0.0f});
    bot = new VacBot(world,0.5);
    room = new RoomWorld(world,1024,1024);
    running = true;
    score = 0.0f;
    collision = 0;
}



VacEnv::~VacEnv() {
//    acc.lock();
    delete bot;
    bot = nullptr;
    delete room;
    room = nullptr;
    delete world;
    world = nullptr;
  //  acc.unlock();
}

std::vector<double> VacEnv::step(int input, float dt) {

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

//    acc.lock();
    world->Step(dt, 3, 3);
    room->calculateCoverage(bot);
  //  acc.unlock();

    room->calculateDistance(bot);

    int count = 0;
    auto list = bot->m_body->GetContactList();

    while (list)
    {
        list = list->next;
        count++;
        running = false;
    }

    collision = count;
            
    score = (static_cast<float>(room->cnt) / (1024.0f*1024.0f));
    float distA = bot->tireA->distance / 255.0f;
    float distB = bot->tireB->distance / 255.0f;
    float rot = fmod( bot->m_body->GetAngle()* RADTODEG, 360.0f) / 360.0f;
    float accX =  bot->acc.x / 25.0f;
    if (accX > 1.0f) {accX = 1.0f;}
    if (accX < -1.0f) {accX = -1.0f;}
    float accY = bot->acc.y / 75.0f;
    if (accY > 1.0f) {accY = 1.0f;}
    if (accY < -1.0f) {accY = -1.0f;}
    float dist1 = 1-room->dists[0];
    float dist2 = 1-room->dists[1];
    float dist3 = 1-room->dists[2];
    return {score,distA,distB,rot,accX,accY,dist1,dist2,dist3};
}

void VacEnv::reset() {
    delete bot;
    delete room;
    bot = new VacBot(world,0.5);
    room = new RoomWorld(world,1024,1024);
    running = true;
    score = 0.0f;
    collision = 0;
}


extern "C" {

__declspec(dllexport) VacEnv* VacEnv_new(){ return new VacEnv(); }
__declspec(dllexport)    void VacEnv_del(VacEnv * foo){  delete foo; }
__declspec(dllexport) void VacEnv_reset(VacEnv * foo) { foo->reset();   }
__declspec(dllexport) void VacEnv_step(VacEnv* foo, int input, float dt, double* out){ auto vec = foo->step(input,dt);
        std::copy(vec.begin(), vec.end(), out);}
__declspec(dllexport) float VacEnv_score(VacEnv* foo) { return foo->score;}
__declspec(dllexport) int VacEnv_colli(VacEnv* foo) { return foo->collision;}
__declspec(dllexport) bool VacEnv_running(VacEnv* foo) { return foo->running;}
__declspec(dllexport) b2World* VacEnv_world(VacEnv* foo) { return foo->world;}

}
