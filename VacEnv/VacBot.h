//
// Created by Oliver on 12.02.2019.
//

#ifndef VACBOTSIM_VACBOT_H
#define VACBOTSIM_VACBOT_H

#include "vacutil.h"
#include "Box2D/Box2D.h"


class VacTire {
public:
    b2Body* m_body;

    float m_maxForwardSpeed;  // 100;
    float m_maxBackwardSpeed; // -20;
    float m_maxDriveForce;    // 150;
    float maxLateralImpulse; //1
    b2Vec2 last_pos;
    float distance;

    VacTire(b2World* world, float size);

    ~VacTire();

    inline b2Vec2 getLateralVelocity();
    inline b2Vec2 getForwardVelocity();
    inline b2Vec2 getForwardMovement(b2Vec2 lastpos);

    void updateFriction();
    void updateDrive(int controlState);
    void updateTurn(int controlState);
};

class VacBot
{
public:
    b2Body* m_body;
    VacTire* tireA;
    VacTire* tireB;


    VacBot(b2World* world, float size);
    ~VacBot();

    int m_controlStateA;
    int m_controlStateB;

    float bot_radius;
    float m_size;

    b2Vec2 lastVel;
    b2Vec2 acc;
    b2RayCastInput rays[3];

    void updateDrive(float dt);

    inline void updateDrive(int controlStateA, int controlStateB, float dt) ;
    inline b2Vec2 getAcceleration(float dt);

};


#endif //VACBOTSIM_VACBOT_H
