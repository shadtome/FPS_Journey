#include "Model.h"
#include "Textures2D.h"
#include "AssimpExchange.h"


unsigned int Displacment_From_Root(const aiScene* scene, aiString name)
{
	aiNode* node;
	node = scene->mRootNode->FindNode(name.C_Str());
	
	//Now we will travel backwards up through its parents till we get to the root node
	unsigned int counter = 0;
	aiNode* iterator = node->mParent;
	while (iterator != scene->mRootNode)
	{
		counter += 1;
		iterator = iterator->mParent;
	}
	
	return counter;
}





Full_Model::Full_Model(std::string path,bool hasskeleton,bool hastexture)
{
	this->HasTextures = hastexture;
	this->HasSkeleton = hasskeleton;
	loadModel(path);
}



void Full_Model::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, Shader shader,std::vector<glm::mat4> &joint_transforms)
{
	shader.use();
	if (HasSkeleton)
	{
		shader.setBool("skeleton", true);
		//NOTE the joint transforms has to have the same size as the number of bones in the skeleton
		std::string index;
		//Set the joints in the vertex shader for this skeleton
		for (unsigned int k = 0; k < this->skeleton.JointCount; ++k)
		{
			index = k;
			shader.setInt(("JointIDs[" +index+ "]" ).c_str(), k);								//set joint IDs
			shader.setMat4(("Joint_Transforms[" + index + "]"), joint_transforms[k]);		//set Joint transforms

		}
	}
	else
	{
		shader.setBool("skeleton", false);
	}

	if (this->HasTextures)
	{
		shader.setBool("textures", true);
	}
	else
	{
		shader.setBool("textures", false);
		shader.setVec3("material.ambient", 0.8, 0.7, 0.31);
		shader.setVec3("material.diffuse", 0.8, 0.7, 0.31);
		shader.setVec3("material.specular", 0.2, 0.2, 0.2);
		shader.setFloat("material.shininess", 32.0);
	}
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw( projection, view, pos, shader);
	}
	shader.Stop();
}

void Full_Model::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos, Shader shader)
{
	shader.use();
	shader.setBool("skeleton", false);

	if (this->HasTextures)
	{
		shader.setBool("textures", true);
	}
	else
	{
		shader.setBool("textures", false);
		shader.setVec3("material.ambient", 0.8, 0.7, 0.31);
		shader.setVec3("material.diffuse", 0.8, 0.7, 0.31);
		shader.setVec3("material.specular", 0.2, 0.2, 0.2);
		shader.setFloat("material.shininess", 32.0);
	}
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(projection, view, pos, shader);
	}
	shader.Stop();
}


void Full_Model::loadModel(std::string path)
{
	Assimp::Importer import;	//importer object from and we will use its ReadFile function
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |aiProcess_LimitBoneWeights);			//Using bitmasks here (later we can use this to import animation)
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;				//Check if ASSIMP through a error
	}
	directory = path.substr(0, path.find_last_of('/'));

	//First, lets detect if this has textures
	if (scene->HasTextures())
	{
		this->HasTextures = true;
	}
	



	//Process Skeleton before we start to process all the nodes, so we can have the skeleton structure ready to go for the processing the weights of the bones
	//To do this, we will order all the bones in a list so we can have easy access to them
	std::vector<aiBone*> bones;
	if (HasSkeleton)
	{
		
		for (unsigned int k = 0; k < scene->mNumMeshes; ++k)			//Search through all the meshes and collect all the bones in one place
		{
			
			if (scene->mMeshes[k]->HasBones())
			{

				for (unsigned int j = 0; j < scene->mMeshes[k]->mNumBones; ++j)
				{
					
					bones.push_back(scene->mMeshes[k]->mBones[j]);
				}
			}
		}
		ProcessSkeleton(scene,bones);
	}

	
	
	ProcessNode(scene->mRootNode, scene,bones);

	
	import.FreeScene();
	
}


void Full_Model::ProcessNode(aiNode* node, const aiScene* scene,std::vector<aiBone*> &bones)
{
	//process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene,bones));
		
	}
	//Then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene,bones);
	}
}


Mesh Full_Model::ProcessMesh(aiMesh* mesh, const aiScene* scene,std::vector<aiBone*> &bones)
{

	
	std::vector<Vertex> vertices;					//define the usual components of Mesh object
	vertices.reserve(mesh->mNumVertices);
	std::vector<unsigned int> indices;

	std::vector<Texture> textures;			
	
	//Vertex attributes
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		
		Vertex vertex;
		glm::vec3 vector;

		//Position of verticesd
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		//Normals of vertices
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//Texture coordinates (note that Assimp allows each vertex to have up to 8 different texture coordinates per vertex
		if (mesh->mTextureCoords[0]) //does the mesh have texture coordinates? 
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords=glm::vec2(0.0, 0.0);
		}
		
		
		//Joint IDs and weights
		
		if (HasSkeleton)
		{
			std::vector<int> jointIDs;
			std::vector<float> weights;
			int counter = 0;
			for (unsigned int k = 0; k < bones.size(); ++k)
			{
				std::pair<int,bool> test = Is_Int_In_List(bones[k]->mWeights, bones[k]->mNumWeights, i);
				if (test.second)
				{
					jointIDs.push_back(this->skeleton.Search(bones[k]->mName.C_Str()));
					weights.push_back(bones[k]->mWeights[test.first].mWeight);
					counter += 1;
				}
				
			}

			
			if (counter > 4)
			{
				counter = 4;
			}
			
			for (unsigned int t = 0; t < counter; ++t)
			{
				
				vertex.JointID[t] = jointIDs[t];
				vertex.weights[t] = weights[t];
				
			}
			
			
		}
		
		vertices.push_back(vertex);
		
		
		
	}
	
	//Indices attribute
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	//Mesh's Material Data
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	
	

	
	
	return Mesh(vertices, indices, textures);
}



std::vector<Texture> Full_Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)		//Check the amount of textures stored in the material
	{
		aiString str;
		mat->GetTexture(type, i, &str);			//Retreive the texture file locations via the Get texture functions
		bool skip = false;						// we are going to check if this texture has already been loaded in
		for (unsigned int j = 0; j < textures_loaded.size(); ++j)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)		//if the texture has not been loaded in, then load it in
		{
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);		//Load the texture from the directory
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
		}
	}
	return textures;
}



void Full_Model::ProcessSkeleton(const aiScene* scene,std::vector<aiBone*> &bones)
{
	
	//Next, we will go through each bone, to test which one is the shortest away from the root node
	aiBone* winner = bones[0];

	for (unsigned int k = 0; k < bones.size(); ++k)
	{
		
		if (Displacment_From_Root(scene, winner->mName) > Displacment_From_Root(scene, bones[k]->mName))
		{
			
			winner = bones[k];
		}
	}
	
	//Now that we have winner, the root bone, we can finally construct the skeleton
	std::vector < std::pair<aiBone*, aiNode*>> bone_Hierarachy;

	aiBone* iterator = winner;
	std::vector<Joint> joints = Construct_Bone_Hierarchy(winner, scene,bones);
	
	/*for (unsigned int k = 0; k < joints.size(); ++k)
	{
		std::cout << joints[k].Parent_Index << "::" << k << std::endl;
	}*/


	Skeleton skelly(joints);
	
	this->skeleton = skelly;
}





std::vector<Joint> Construct_Bone_Hierarchy(aiBone* bone, const aiScene* scene, std::vector<aiBone*> &bones)
{
	aiNode* node = scene->mRootNode->FindNode(bone->mName.C_Str());			//The node where this bone is located
	Joint parent;
	parent.Name = bone->mName.C_Str();
	parent.Parent_Index = 0;
	parent.M_invBindPose = Assimp_MatrixConv(bone->mOffsetMatrix);								//Need a function to transfer their matrix to mine

	std::vector<Joint> result;

	result.push_back(parent);
	if (node->mNumChildren != 0)
	{
		for (unsigned int k = 0; k < node->mNumChildren; ++k)
		{
			for (unsigned int j = 0; j < bones.size(); ++j)
			{
				if (node->mChildren[k]->mName == bones[j]->mName)
				{
					Joint childJ;

					//Define the child joint with this bone
					childJ.Name = bones[j]->mName.C_Str();
					childJ.M_invBindPose = Assimp_MatrixConv(bones[j]->mOffsetMatrix);					//Need a function to transfer their matrix to mine
					childJ.Parent_Index = 0;


					//Search through this child's children
					std::vector<Joint> child_V = Construct_Bone_Hierarchy(bones[j], scene, bones);

					//We need to set the parent_index to these children, since in their world, the parent index is 0
					//so, all we need to do is add the size of the result to these to count for the fact we moved them back
					for (unsigned int j = 0; j < child_V.size(); ++j)
					{
						child_V[j].Parent_Index += result.size();
					}
					//Insert the child in to the result
					result.push_back(childJ);

					//Now insert this child's children in to the result vector
					auto it = result.end();
					//if the chilv_V is only one vector, dont add anymore, otherwise, add the resulting children that is not the parent in this vector
					//We already handeled the parent ChildJ
					if(child_V.size()!=1)		
					{
						result.insert(it, child_V.begin()+1, child_V.end());
					}
					
				}
			}
		}
	}
	
	
	return result;
	
}



std::pair<int,bool> Is_Int_In_List(aiVertexWeight* list, int size_list,unsigned int &tested)
{
	if (list[0].mVertexId < tested && tested < list[size_list - 1].mVertexId)			//Checks to see if the tested number is between the max and min values of list
	{
		int midpoint = size_list / 2;

		if (list[0].mVertexId < tested && tested <= list[midpoint-1].mVertexId)			//Now checks which half it is on and repeat the process
		{
			return Is_Int_In_List(list, midpoint, tested);
		}
		else
		{
			//We need to add midpoint to account the fact that we moved the beginning of the list for this case
			return std::pair<int,bool>(midpoint + Is_Int_In_List(list + midpoint, size_list - midpoint, tested).first, Is_Int_In_List(list + midpoint, size_list - midpoint, tested).second);
		}
	}
	else if (list[0].mVertexId == tested)
	{
		return std::pair<int,bool>(0,true);
	}
	else if (list[size_list - 1].mVertexId== tested)
	{
		return std::pair<int,bool>(size_list-1,true);
	}
	else
		return std::pair<int,bool>(-1,false);
}