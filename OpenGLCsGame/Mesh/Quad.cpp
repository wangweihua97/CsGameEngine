#include "Quad.h"

Quad::Quad():Mesh(vector<Vertex>(), vector<unsigned int>(), 0)
{
    std::vector<glm::vec3> Positions =
    {
        { -1.0f,  1.0f, 0.0f, },
        { -1.0f, -1.0f, 0.0f, },
        {  1.0f,  1.0f, 0.0f, },
        {  1.0f, -1.0f, 0.0f, },
    };
    std::vector<glm::vec2> UV = {
        { 0.0f, 1.0f, },
        { 0.0f, 0.0f, },
        { 1.0f, 1.0f, },
        { 1.0f, 0.0f, },
    };
    Topology = GL_TRIANGLE_STRIP;
    for (int i = 0; i < 4; i++)
    {
        Vertex v;
        v.Position = Positions[i];
        v.TexCoords = UV[i];
        vertices.push_back(v);
    }
}
