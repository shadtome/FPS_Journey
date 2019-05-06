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
#include <map>


/*
This class is used to import all model information using ASSIMP.  This importing process depends on what kind of file system,
since different file systems have different coordinate systems.

If the model has a skeleton, it imports that as well, includeing all the inverse bind poses, which are included in the skeleton system
in the process of find the total transforms of the joints.

If the model has animations as well, this imports these as well, and holds the data in this class
with a name associated with it.

MAKE SURE TO CHANGE THE IMPORT ANIMATION FOR FBX, since FBX actually saves all the animations for it, while Collada does not.







*/



class Full_Model
{
	typedef std::string Name;
public:

	//Does this model have a skeleton
	bool HasSkeleton = false;
	bool HasTextures = false;

	//List of animations
	std::map<Name,Animation> Animations;

	//Skeleton Data
	Skeleton skeleton;


	//Functions
	void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 pos, Shader &shader,SkeletonPose &pose);
	void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 pos, Shader &shader);
	void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 pos,float &angle,glm::vec3 &rot_vec, Shader &shader, SkeletonPose &pose);

	//Import More Animations for this model
	//Should we save the bones so we dont have to keep making them, but after the animations are loaded in, there is no point in saving this bone orders
	void Import_Animation(std::string file, Type_of_Animation type,std::string name);

	//Model Data
	std::vector<Mesh> meshes;

	//Constructors---------
	
	Full_Model(std::string path, bool hasskeleton, bool hastexture);
	//Default constructor
	Full_Model() {};
		

	//Copy constructor
	Full_Model(const Full_Model &other);

	//-------------------
	//Copy Assignment operator
	Full_Model& operator=(const Full_Model &other);
private:
	std::vector<Texture> textures_loaded;		//stores all the textures loaded so far, so we don't load in the same texture twice
	
	
	
	std::string directory;
	std::string file_type;
	//Functions

	/* process through the nodes of the scene to find the bones
	* and find the root bone, and all the children.  Takes the bones and
	* puts them in to a skeleton structure.
	*/
	void ProcessSkeleton(const aiScene* scene,std::vector<aiBone*> &bones);

	/* Starts the process of loading all the necessary information for the model.
	* Processes the skeleton, the nodes, the meshes, and the textures/Materials
	*/
	void loadModel(std::string path);

	/* Process this nodes corresponding meshes, and recursivly process the children nodes 
	*/
	void ProcessNode(aiNode *node, const aiScene* scene,std::vector<aiBone*> &bones);		//retrieve mesh indices, meshes, process each mesh and its children

	/* This process the coordinates of the vertices, normals, texture coordinates,
	* weights of the bones on the vertices, bone ids that affect the vertices, and the importing of the textures
	*/
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene,std::vector<aiBone*> &bones);		//Translate iaMesh to a Mesh object that we defined

	void ProcessAnimation(const aiScene* &scene,std::vector<aiBone*> &bones,Type_of_Animation type,std::string name);

	/*Process the material textures
	*/
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
std::vector<Joint> Construct_Bone_Hierarchy(aiBone* bone, const aiScene* scene,std::vector<aiBone*> &bones,std::string &file_type);


/*determines if the tested int is in the list of vertices this weight effects
* Takes in a list of vertex weights, and testes the vertex index to see if it is the same as the tested
* does this in a sequence of half partitions like in the proof Bolzono Wierstrass theorem
* the size_list is the size of the list
*/
std::pair<int,bool> Is_Int_In_List(aiVertexWeight* list, int size_list, unsigned int &tested);

#endif