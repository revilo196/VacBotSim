//
// Created by Oliver on 12.02.2019.
//

#include "VacEnvWindow.h"
#include "VacEnv.h"

S2D_Window *VacEnvWindow::window;
std::vector<VacEnv*> * VacEnvWindow::worlds;
float VacEnvWindow::scale_x;
float VacEnvWindow::scale_y;
float VacEnvWindow::offset_x;
float VacEnvWindow::offset_y;
unsigned int VacEnvWindow::m_size;

void  DrawLineGreen(float x1, float y1, float x2, float y2) {
    S2D_DrawLine(x1,y1,x2,y2,2.0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1);
}

void VacEnvWindow::render(){

    for (VacEnv* e : (*worlds)) {
        //e->acc.lock();
        render_world(e->world);
        render_map(e->room->map);
        //e->acc.unlock();
    }
}

void VacEnvWindow::render_world(b2World* w){
    for(const b2Body* b = w->GetBodyList(); b; b = b->GetNext()) {
        render_body(b);
    }
}

void VacEnvWindow::render_body(const b2Body* b){
    for (const b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
        render_shape(f->GetShape(),b->GetPosition(), b->GetWorldPoint({0,5}));
    }
}

void VacEnvWindow::render_shape(const b2Shape* s,b2Vec2 p, b2Vec2 rot){
    if(s->GetType() == b2Shape::e_chain) {
        auto cs = dynamic_cast<const b2ChainShape *>(s);
        b2Vec2 *array = cs->m_vertices;
        for (int i = 0; i < cs->m_count - 1; i++) {
            DrawLineGreen((p.x+array[i + 0].x+offset_x)*(scale_x), (p.y+array[i + 0].y+offset_y)*(scale_y),
                          (p.x+array[i + 1].x+offset_x)*(scale_x), (p.y+array[i + 1].y+offset_y)*(scale_y));
        }
    } else if (s->GetType() == b2Shape::e_circle) {
        auto cs = dynamic_cast<const b2CircleShape *>(s);

        S2D_DrawCircle((p.x+cs->m_p.x+offset_x)*(scale_x),(p.y+cs->m_p.y+offset_y)*(scale_y),(cs->m_radius)*(scale_x),64,1,0,0,1 );
        DrawLineGreen((p.x + offset_x)*(scale_x),(p.y + offset_y)*(scale_y),(rot.x + offset_x)*(scale_x),(rot.y + offset_x)*(scale_y) );
    } else {

    }
}

void VacEnvWindow::render_map(std::vector<std::vector<bool> > map) {
    for(int i = 0; i < map.size(); i+= 5) {
        for(int j = 0; j < map[i].size(); j+= 5) {
            if(map[i][j]) {
                float x = i * ((float) m_size / (float) map.size());
                float y = j * ((float) m_size / (float) map[i].size());
                S2D_DrawCircle((x), (y), (2), 3, 0.5, 0, 0, 0.5);
            }
        }
    }
};

void VacEnvWindow::init(unsigned int size, float wxmin,float wxmax,float wymin,float wymax){

    VacEnvWindow::scale_x = (float)size / (wxmax-wxmin);
    VacEnvWindow::scale_y = (float)size / (wymax-wymin);

    VacEnvWindow::offset_x = -wxmin;
    VacEnvWindow::offset_y = -wymin;
    VacEnvWindow::m_size = size;
    VacEnvWindow::worlds = new std::vector<VacEnv*>();
    VacEnvWindow::window = S2D_CreateWindow(
            "VacEnvWindow",  // title of the window
            size, size,        // width and height
            nullptr , VacEnvWindow::render,  // callback function pointers (these can be NULL)
            0                // flags
    );
};

void VacEnvWindow::close() {
    S2D_Close(window);
    delete window;
    delete worlds;
}

int VacEnvWindow::run(){
    return S2D_Show(window);
}

