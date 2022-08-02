#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include"Component.h"
#include "../Mesh/BoneMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Render/Material.h"
#include<boost/bimap.hpp> 
using namespace std;
class ModelComponent : public Component
{
public:
    vector<Mesh*>  meshes;
    vector<string> meshsName;
    vector<Material*> meshsMat;
    vector<vector<Texture>*> texturess;
    string directory;
    std::vector<VertexBoneData> Bones;
    unsigned int totalVertices = 0;
    vector<glm::mat4> boneTransforms;
    bool gammaCorrection;
    const aiScene* pScene; //!< The Assimp aiScene object. 
	ModelComponent(GameObject* gameObject);
	void Update()override;
	void LateUpdate()override;
	void Render()override;
    void RenderShadow()override;
    void OnDestory()override;
	void InitAnimation();
	void PlayAnimation(string animaName);
	void LoadModel(string const& path, bool gamma = false , bool autoMat = false);
    void Draw();
    void DrawShadow();
    void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms); //!< Traverses the scene hierarchy and fetches the matrix transformation for each bone given the time. 
    void SetBoneTransform(unsigned int Index, const glm::mat4& Transform); //!< Inserts a bone transformation in the uniform array at the given index. 
    void SetShadowBoneTransform(unsigned int Index, const glm::mat4& Transform);
    int animationIndex;
    float animationTime;
    boost::bimap<string,int > animations;

private:
    bool _isPlay = false;
    bool _autoMat = false;
    static const unsigned int ui_BoneArraySize = 100;
    GLint** m_boneLocation;
    GLint* m_shadowBoneLocation;
    static bool _isBoneInit;
    Assimp::Importer importer;
    string texturePath;
    unsigned int m_NumBones;
    //Shader* m_pShaderProg;
    aiNode* m_RootNode;
    vector<unsigned int> baseVertex;
    map<unsigned int, glm::mat4>* boneOffsets;
    void loadModel(string path);
    void processNode(aiNode* node, glm::mat4 parentTransfrom);
    Mesh* processMesh(unsigned int meshId, aiMesh* mesh);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
    Texture2D* TextureFromFile(const string& filename);

    std::map<std::string, unsigned int> m_BoneMapping; //!< Map of bone names to ids

    std::vector<BoneInfo> m_BoneInfo; //!< Array containing bone information such as offset matrix and final transformation. 

    glm::mat4 GlobalTransformation; //!< Root node transformation. 
    glm::mat4 m_GlobalInverseTransform;
    void InitBone();
    void BeforeRenderAnim(int index);
    void BeforeShadowRenderAnim(int index);
    void DetermineAdjacency(vector<unsigned int>& el);
    void FindAdjacencies(const aiMesh* paiMesh, vector<unsigned int>& Indices);
    void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones, std::vector<unsigned int> baseVertex, glm::mat4 nodeTransform); //!< Loads the bone data from a given mesh. 
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim); //!< Calculates the interpolated quaternion between two keyframes. 
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim); //!< Calculates the interpolated scaling vector between two keyframes. 
    void CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim); //!< Calculates the interpolated translation vector between two keyframes. 

    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim); //!< Finds a rotation key given the current animation time. 
    unsigned int FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim); // Finds a scaling key given the current animation time. 
    unsigned int FindTranslation(float AnimationTime, const aiNodeAnim* pNodeAnim); // Finds a translation key given the current animation time. 

    void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
    int _meshIndex;
};

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif
DLLFUNC(void) ModelComponent_LoadModel(ModelComponent* modelComponent,char* path);
DLLFUNC(void) ModelComponent_LoadAutoMatModel(ModelComponent* modelComponent, char* path);
DLLFUNC(int) ModelComponent_GetMeshsCount(ModelComponent* modelComponent);
DLLFUNC(const char*) ModelComponent_GetMeshName(ModelComponent* modelComponent ,int index);
DLLFUNC(void) ModelComponent_SetMeshMat(ModelComponent* modelComponent, int index, Material* mat);
DLLFUNC(void) ModelComponent_InitAnimation(ModelComponent* modelComponent);
#endif

