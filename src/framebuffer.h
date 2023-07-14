#pragma once

#include <vector>
#include "color.h"

struct Vertex {
    float x;
    float y;
};

class Framebuffer {
public:
    Framebuffer(size_t width, size_t height);
    void setPixel(int x, int y, const Color& color);
    void clear(const Color& clearColor);
    void point(const Vertex& vertex, const Color& currentColor);
    void line(const Vertex& start, const Vertex& end, const Color& currentColor);
    void drawPolygon(const std::vector<Vertex>& vertices, const Color& fillColor, const Color& borderColor);
    void fillPolygon(const std::vector<Vertex>& vertices, const Color& fillColor);

    void renderBuffer() const;

private:
    size_t width_;
    size_t height_;
    std::vector<Color> buffer_;
};
