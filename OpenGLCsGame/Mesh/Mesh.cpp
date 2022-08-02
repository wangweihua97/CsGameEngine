#include "Mesh.h"
#include "../Render/Shader.h"
#include "../Res/ResourceManager.h"
#include "../Scene/Scene.h"
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices , int offset)
{
    this->vertices = vertices;
    this->indices = indices;
    BaseVertex = offset;
    setupMesh();
}

void Mesh::setupMesh()
{
    if (vertices.size() <= 0)
        return;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set the vertex attribute pointers
    // vertex Positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (indices.size() > 0)
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
    }
    
    glBindVertexArray(0);
}
void Mesh::Draw(vector<Texture>* textures)
{
    Shader* m_pShaderProg = ResourceManager::GetShaderP("autoModelShader");
    // bind appropriate textures
    // 
    float diffuseNr = 0.0f;
    float specularNr = 0.0f;
    float normalNr = 0.0f;
    float heightNr = 0.0f;
    float aoNr = 0.0f;
    float metallicNr = 0.0f;
    float roughnessNr = 0.0f;
    for (unsigned int i = 0; i < textures->size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = (*textures)[i].type;
        if (name == "albedoMap")
            diffuseNr = 1.0f;
        else if (name == "specularMap")
            specularNr = 1.0f; // transfer unsigned int to stream
        else if (name == "normalMap")
            normalNr = 1.0f; // transfer unsigned int to stream
        else if (name == "heightMap")
            heightNr = 1.0f; // transfer unsigned int to stream
        else if (name == "aoMap")
            aoNr = 1.0f; // transfer unsigned int to stream
        else if (name == "metallicMap")
            metallicNr = 1.0f; // transfer unsigned int to stream
        else if (name == "roughnessMap")
            roughnessNr = 1.0f;
        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(m_pShaderProg->ID, name.c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, (*textures)[i].texture2D->ID);
    }
    m_pShaderProg->SetFloat("use_albedoMap", diffuseNr, false);
    m_pShaderProg->SetFloat("use_specularMap", specularNr, false);
    m_pShaderProg->SetFloat("use_normalMap", normalNr, false);
    m_pShaderProg->SetFloat("use_heightMap", heightNr, false);
    m_pShaderProg->SetFloat("use_aoMap", aoNr, false);
    m_pShaderProg->SetFloat("use_metallicMap", metallicNr, false);
    m_pShaderProg->SetFloat("use_roughnessMap", roughnessNr, false);

    m_pShaderProg->SetInteger("irradianceMap", 15);
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::Instace->ibl.irradianceMap);
    m_pShaderProg->SetInteger("prefilterMap", 16);
    glActiveTexture(GL_TEXTURE16);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::Instace->ibl.prefilterMap);
    m_pShaderProg->SetInteger("brdfLUT", 17);
    glActiveTexture(GL_TEXTURE17);
    glBindTexture(GL_TEXTURE_2D, Scene::Instace->ibl.brdfLUTTexture);

    // draw mesh
    glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES_ADJACENCY, indices.size(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //// always good practice to set everything back to defaults once configured.

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::RenderShadow()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES_ADJACENCY, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}
