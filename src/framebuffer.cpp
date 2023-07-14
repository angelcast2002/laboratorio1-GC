#include "framebuffer.h"
#include <fstream>
#include <algorithm>
#include <cmath>

Framebuffer::Framebuffer(size_t width, size_t height)
        : width_(width), height_(height), buffer_(width * height)
{
}

void Framebuffer::setPixel(int x, int y, const Color& color)
{
    if (x >= 0 && x < static_cast<int>(width_) && y >= 0 && y < static_cast<int>(height_))
    {
        size_t index = y * width_ + x;
        buffer_[index] = color;
    }
}

void Framebuffer::clear(const Color& clearColor)
{
    std::fill(buffer_.begin(), buffer_.end(), clearColor);
}

void Framebuffer::point(const Vertex& vertex, const Color& currentColor)
{
    int x = static_cast<int>(std::round(vertex.x));
    int y = static_cast<int>(std::round(vertex.y));
    setPixel(x, y, currentColor);
}

void Framebuffer::line(const Vertex& start, const Vertex& end, const Color& currentColor)
{
    int x0 = static_cast<int>(std::round(start.x));
    int y0 = static_cast<int>(std::round(start.y));
    int x1 = static_cast<int>(std::round(end.x));
    int y1 = static_cast<int>(std::round(end.y));

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        setPixel(x0, y0, currentColor);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}



void Framebuffer::drawPolygon(const std::vector<Vertex>& vertices, const Color& fillColor, const Color& borderColor)
{
    fillPolygon(vertices, fillColor);
    if (vertices.size() < 2)
    {
        return;
    }

    size_t last = vertices.size() - 1;
    for (size_t i = 0; i < last; ++i)
    {
        line(vertices[i], vertices[i + 1], borderColor);
    }

    // Dibujar el último borde que conecta el último y primer vértice
    line(vertices[last], vertices[0], borderColor);


}

void Framebuffer::fillPolygon(const std::vector<Vertex>& vertices, const Color& fillColor)
{
    if (vertices.size() < 3)
    {
        return;
    }

    int minY = height_;
    int maxY = 0;

    // Obtener los valores mínimo y máximo de la coordenada y
    for (const Vertex& vertex : vertices)
    {
        minY = std::min(minY, static_cast<int>(std::round(vertex.y)));
        maxY = std::max(maxY, static_cast<int>(std::round(vertex.y)));
    }

    // Rellenar el polígono línea por línea, omitiendo las orillas
    for (int y = minY + 1; y < maxY; ++y)
    {
        std::vector<float> intersections;

        size_t last = vertices.size() - 1;
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            const Vertex& v1 = vertices[i];
            const Vertex& v2 = vertices[(i + 1) % vertices.size()];

            float y1 = std::round(v1.y);
            float y2 = std::round(v2.y);

            if ((y1 < y && y2 >= y) || (y2 < y && y1 >= y))
            {
                float x = v1.x + ((y - y1) / (y2 - y1)) * (v2.x - v1.x);
                intersections.push_back(x);
            }
        }

        // Ordenar intersecciones de manera ascendente
        std::sort(intersections.begin(), intersections.end());

        // Rellenar los píxeles entre las intersecciones usando la función point
        for (size_t i = 0; i < intersections.size(); i += 2)
        {
            int startX = static_cast<int>(std::ceil(intersections[i]));
            int endX = static_cast<int>(std::floor(intersections[i + 1]));

            // Asegurarse de que startX esté dentro del rango
            startX = std::max(startX, 0);

            // Asegurarse de que endX esté dentro del rango
            endX = std::min(endX, static_cast<int>(width_ - 1));

            // Dibujar los píxeles en el rango de intersecciones, omitiendo los bordes
            for (int x = startX; x <= endX; ++x)
            {
                if (y > 0 && y < static_cast<int>(height_) && x >= 0 && x < static_cast<int>(width_))
                {
                    point({static_cast<float>(x), static_cast<float>(y)}, fillColor);
                }
            }
        }
    }
}



void Framebuffer::renderBuffer() const
{
    std::ofstream file("out.bmp", std::ios::binary);

    // Encabezado del archivo BMP
    int fileSize = 54 + 3 * width_ * height_;
    unsigned char fileHeader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char infoHeader[40] = {40, 0, 0, 0, static_cast<unsigned char>(width_), static_cast<unsigned char>(width_ >> 8),
                                    static_cast<unsigned char>(width_ >> 16), static_cast<unsigned char>(width_ >> 24),
                                    static_cast<unsigned char>(height_), static_cast<unsigned char>(height_ >> 8),
                                    static_cast<unsigned char>(height_ >> 16), static_cast<unsigned char>(height_ >> 24), 1, 0, 24, 0};

    // Escribir encabezados en el archivo
    file.write(reinterpret_cast<const char*>(fileHeader), 14);
    file.write(reinterpret_cast<const char*>(infoHeader), 40);

    for (size_t y = 0; y < height_; ++y)
    {
        for (size_t x = 0; x < width_; ++x)
        {
            size_t index = y * width_ + x;
            const Color& color = buffer_[index];
            unsigned char pixelData[3] = {color.getBlue(), color.getGreen(), color.getRed()};
            file.write(reinterpret_cast<const char*>(pixelData), 3);
        }
    }

    file.close();
}