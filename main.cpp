#include "src/framebuffer.h"

void render() {
    Framebuffer framebuffer(800, 600);
    Color clearColor(0x00, 0x00, 0x00);

    framebuffer.clear(clearColor);

    // Poligono 1
    Color fillColor(0xFF, 0xFF, 0x00);       // Color de relleno (amarillo)
    Color borderColor(0xFF, 0xFF, 0xFF);     // Color de contorno (verde)
    std::vector<Vertex> poligono1{
            {165.0, 380.0},
            {185.0, 360.0},
            {180.0, 330.0},
            {207.0, 345.0},
            {233.0, 330.0},
            {230.0, 360.0},
            {250.0, 380.0},
            {220.0, 385.0},
            {205.0, 410.0},
            {193.0, 383.0}
    };

    framebuffer.drawPolygon(poligono1, fillColor, borderColor);

    framebuffer.renderBuffer();
}

int main() {

    render();

    return 0;
}
