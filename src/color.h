#pragma once

#include <cstdint>

class Color {
public:
    Color() : r(0), g(0), b(0) {}


    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}


    void setRed(uint8_t red) {
        r = red;
    }

    void setGreen(uint8_t green) {
        g = green;
    }

    void setBlue(uint8_t blue) {
        b = blue;
    }

    uint8_t getRed() const {
        return r;
    }

    uint8_t getGreen() const {
        return g;
    }

    uint8_t getBlue() const {
        return b;
    }

private:
    uint8_t r;  // Componente de color rojo
    uint8_t g;  // Componente de color verde
    uint8_t b;  // Componente de color azul
};
