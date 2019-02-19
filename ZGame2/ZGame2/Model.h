#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>


/*
INFORMATION HERE







*/



class Full_Model
{
public:

	//Does this model have a skeleton
	bool HasSkeleton = false;
	bool HasTextures = false;

	//List of animations
	std::vector<Animation> Animations;

	//Skeleton Data
	Skeleton skeleton;
	
	//Constructor
	Full_Model(std::string path,bool hasskeleton,bool hastexture);			

	//Functions
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, Shader shader,std::vector<glm::mat4> &joint_transforms);
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, Shader shader);
private:
	std::vector<Texture> textures_loaded;		//stores all the textures loaded so far, so we don't load in the same texture twice
	//Model Data
	std::vector<Mesh> meshes;
	
	std::string directory;
	//Functions
	void ProcessSkeleton(const aiScene* scene,std::vector<aiBone*> &bones);
	void loadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene* scene,std::vector<aiBone*> &bones);		//retrieve mesh indices, meshes, process each mesh and its children
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene,std::vector<aiBone*> &bones);		//Translate iaMesh to a Mesh object that we defined
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};





/*
This is used to find the displacement from the node where a bone exists in the tree away from the root node
Because ASSIMP puts the bone nodes everywhere all mixed into it...


*/
unsigned int Displacment_From_Root(const aiScene* scene, aiString name);

/*
Iterate through the bones and organize the vector with the children of the bone
*/
std::vector<Joint> Construct_Bone_Hierarchy(aiBone* bone, const aiScene* scene,std::vector<aiBone*> &bones);


/*determines if the tested int is in the list of vertices this weight effects
* Takes in a list of vertex weights, and testes the vertex index to see if it is the same as the tested
* does this in a sequence of half partitions like in the proof Bolzono Wierstrass theorem
* the size_list is the size of the list
*/
std::pair<int,bool> Is_Int_In_List(aiVertexWeight* list, int size_list, unsigned int &tested);

#endif