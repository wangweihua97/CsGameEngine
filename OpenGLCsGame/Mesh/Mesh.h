#ifndef Mesh_H
#define Mesh_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>
#include <glad/glad.h>
#include "../Render/RenderStruct.h"
#define MAX_BONE_INFLUENCE 4
using namespace std;
struct Vertex {
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};
class Mesh
{
public:
    /*  网格数据  */
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    unsigned int VAO;
    unsigned int BaseVertex;
    unsigned int Topology = GL_TRIANGLES;

    /*  函数  */
    Mesh(vector<Vertex> vertices , vector<unsigned int> indices ,int offset);
    virtual void setupMesh();
    virtual void Draw(vector<Texture>* textures);
    void RenderShadow();
    ~Mesh();
private:
    unsigned int VBO, EBO;
};
#endif

