//
// Created by Oliver on 12.02.2019.
//

#include "VacBot.h"

VacTire::VacTire(b2World *world, float size) {
    m_maxForwardSpeed = 100 * size;
    m_maxBackwardSpeed = -20 * size;
    m_maxDriveForce = 150 * size;
    maxLateralImpulse = 1;
    last_pos = {0, 0};
    distance = 0;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.5f * size, 1.25f * size);
    m_body->CreateFixture(&polygonShape, 1);//shape, density

    m_body->SetUserData(this);
}

VacTire::~VacTire() {
    m_body->GetWorld()->DestroyBody(m_body);

}

b2Vec2 VacTire::getLateralVelocity() {
    b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1, 0));
    return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 VacTire::getForwardVelocity() {
    b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 VacTire::getForwardMovement(b2Vec2 lastpos) {
    b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(currentRightNormal, (m_body->GetPosition() - lastpos)) * currentRightNormal;
}

void VacTire::updateFriction() {
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    if (impulse.Length() > maxLateralImpulse)
        impulse *= maxLateralImpulse / impulse.Length();

    m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);
    m_body->ApplyAngularImpulse(0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -10 * currentForwardSpeed;
    m_body->ApplyForce(dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void VacTire::updateDrive(int controlState) {
    //find desired speed
    float desiredSpeed = 0;
    switch (controlState & (TDC_UP | TDC_DOWN)) {
        case TDC_UP:
            desiredSpeed = m_maxForwardSpeed;
            break;
        case TDC_DOWN:
            desiredSpeed = m_maxBackwardSpeed;
            break;
        default:
            return;//do nothing
    }

    //find current speed in forward direction
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
    float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = m_maxDriveForce;
    else if (desiredSpeed < currentSpeed)
        force = -m_maxDriveForce;
    else
        return;
    m_body->ApplyForce(force * currentForwardNormal, m_body->GetWorldCenter(), true);

    b2Vec2 movement = getForwardMovement(last_pos);
    b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(0, 1));
    distance += b2Dot(currentRightNormal, (movement));
    last_pos = m_body->GetPosition();
}

void VacTire::updateTurn(int controlState) {
    float desiredTorque = 0;
    switch (controlState & (TDC_LEFT | TDC_RIGHT)) {
        case TDC_LEFT:
            desiredTorque = 15;
            break;
        case TDC_RIGHT:
            desiredTorque = -15;
            break;
        default:;//nothing
    }
    m_body->ApplyTorque(desiredTorque, true);
}

VacBot::VacBot(b2World* world, float size)
{
    m_controlStateA = 0;
    m_controlStateB = 0;
    lastVel = { 0,0 };
    acc = { 0,0 };
    m_size = size;

    //***************************************
    //---------------Wheels------------------
    //***************************************
    tireA = new VacTire(world, size);
    tireB = new VacTire(world, size);

    //***************************************
    //-------------Bot-Center----------------
    //***************************************
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    b2CircleShape cShape;
    cShape.m_radius = 4.5f* size;
    bot_radius= 4.5f* size;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &cShape;
    fixtureDef.density = 0.01;
    m_body = world->CreateBody(&bodyDef);
    m_body->CreateFixture(&fixtureDef);

    //*****************************************
    //-------------Attach Wheels to Bot--------
    //*****************************************

    //Define Joint-Config
    b2WeldJointDef jDef;
    jDef.bodyB = tireA->m_body;
    jDef.bodyA = m_body;
    jDef.localAnchorA = { 4.5f * size,0.0f };
    jDef.localAnchorB = { -0.5f* size,0.0f };
    jDef.dampingRatio = 1;
    jDef.frequencyHz = 0;
    jDef.collideConnected = false;
    world->CreateJoint(&jDef);

    //Joint-Config
    jDef.bodyB = tireB->m_body;
    jDef.bodyA = m_body;
    jDef.localAnchorA = { -4.5f * size,0.0f };
    jDef.localAnchorB = { 0.5f * size,0.0f };
    world->CreateJoint(&jDef);
}

VacBot::~VacBot()
{
    m_body->GetWorld()->DestroyBody(m_body);
    delete tireA;
    delete tireB;
}

b2Vec2 VacBot::getAcceleration(float dt) {
    b2Vec2 dv = m_body->GetLinearVelocityFromLocalPoint(m_body->GetLocalCenter()) - lastVel;
    lastVel = m_body->GetLinearVelocityFromLocalPoint(m_body->GetLocalCenter());
    dv *= (1.0/dt);
    return dv;
}

void VacBot::updateDrive(int controlStateA, int controlStateB, float dt) {
    m_controlStateA = controlStateA;
    m_controlStateB = controlStateB;
    updateDrive(dt);
};

void VacBot::updateDrive(float dt) {
    tireA->updateFriction();
    tireB->updateFriction();
    tireA->updateDrive(m_controlStateA);
    tireB->updateDrive(m_controlStateB);

    //Calculate Accelerometer
    acc = getAcceleration(dt);

    //Calculate RayTracer
    float angle = m_body->GetAngle() + (DEGTORAD *90);
    rays[0].p1 = m_body->GetPosition();
    rays[0].p2 = m_body->GetPosition() + ((100 * m_size) * b2Vec2(cos(angle), sin(angle)));
    rays[0].maxFraction = 1;

    rays[1].p1 = m_body->GetPosition();
    rays[1].p2 = m_body->GetPosition() + ((100 * m_size) * b2Vec2(cos(angle+(45*DEGTORAD)), sin(angle+(45* DEGTORAD))));
    rays[1].maxFraction = 1;

    rays[2].p1 = m_body->GetPosition();
    rays[2].p2 = m_body->GetPosition() + ((100 * m_size) * b2Vec2(cos(angle-(45* DEGTORAD)), sin(angle-(45* DEGTORAD))));
    rays[2].maxFraction = 1;


}