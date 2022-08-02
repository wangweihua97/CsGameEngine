#ifndef BoneMesh_H
#define BoneMesh_H
#include"../Component/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <map>
#include "../Res/Texture.h"
#include "../Render/Shader.h"
#include "../Render/RenderStruct.h"
#include "Mesh.h"
#define MAX_BONE_INFLUENCE 4
using namespace std;
struct BoneVertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;
    // texCoords
    
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    ////weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};
struct BoneInfo
{
    glm::mat4 FinalTransformation; // Final transformation to apply to vertices 
    glm::mat4 BoneOffset; // Initial offset from local to bone space. 

    BoneInfo()
    {
        BoneOffset = glm::mat4(1.0f);
        FinalTransformation = glm::mat4(1.0f);
    }

    void SetBoneOffset(glm::mat4 a)
    {
        if (BoneOffset == glm::mat4(1.0f))
            BoneOffset = a;
    }
};

struct VertexBoneData
{
    unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
    float Weights[4]; //!< An array of the weight influence per bone. 

    VertexBoneData()
    {
        // 0's out the arrays. 
        Reset();
    }

    void Reset()
    {
        memset(IDs, 0, 4 * sizeof(IDs[0]));
        memset(Weights, 0, 4 * sizeof(Weights[0]));
    }

    void AddBoneData(unsigned int BoneID, float Weight)
    {
        for (unsigned int i = 0; i < 4; i++) {

            // Check to see if there are any empty weight values. 
            if (Weights[i] == 0.0) {
                // Add ID of bone. 
                IDs[i] = BoneID;

                // Set the vertex weight influence for this bone ID. 
                Weights[i] = Weight;
                return;
            }

        }
        // should never get here - more bones than we have space for
        assert(0);
    }
};
class BoneMesh : public Mesh
{
public:
    vector<VertexBoneData> bones;

    /*  º¯Êý  */
    BoneMesh(vector<Vertex> vertices, vector<unsigned int> indices, int offset, std::vector<VertexBoneData> bones);
    void setupMesh()override;
    void RenderShadow();
private:
    unsigned int VBO, EBO, boneBo;
    //map<unsigned int, glm::mat4> m_boneOffsets;
    /*  º¯Êý  */

};
#endif

