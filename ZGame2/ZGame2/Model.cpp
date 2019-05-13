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


//-----------------------------------------------
//Full_Model Functions

//-----------------------------------
//Constructors

//Copy Constructor
Full_Model::Full_Model(const Full_Model &other)
{
	this->Animations=(other.Animations);
	//Need to make sure the animations have their skeletons pointing to their new model skeleton.
	for (auto k = this->Animations.begin(); k != this->Animations.end(); ++k)
	{
		k->second.pskeleton = &this->skeleton;
	}
	this->directory = other.directory;
	this->file_type = other.file_type;
	this->HasSkeleton = other.HasSkeleton;
	this->HasTextures = other.HasTextures;
	this->meshes = other.meshes;
	this->skeleton = other.skeleton;
	this->textures_loaded = other.textures_loaded;
}

//--------------------------------
//Copy Assigment operator
Full_Model& Full_Model::operator=(const Full_Model& other)
{
	this->Animations = (other.Animations);
	//Need to make sure the animations have their skeletons pointing to their new model skeleton.
	for (auto k = this->Animations.begin(); k != this->Animations.end(); ++k)
	{
		k->second.pskeleton = &this->skeleton;
	}
	this->directory = other.directory;
	this->file_type = other.file_type;
	this->HasSkeleton = other.HasSkeleton;
	this->HasTextures = other.HasTextures;
	this->meshes = other.meshes;
	this->skeleton = other.skeleton;
	this->textures_loaded = other.textures_loaded;

	return *this;
}

//---------------------------------------
//Main Functions
Full_Model::Full_Model(std::string path,bool hasskeleton,bool hastexture)
{
	this->HasTextures = hastexture;
	this->HasSkeleton = hasskeleton;
	loadModel(path);
}



void Full_Model::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 pos, Shader &shader,SkeletonPose &pose)
{
	shader.use();
	if (HasSkeleton)
	{
		//Sets if the vertices have a skeleton associated with it
		shader.setBool("skeleton", true);
		//Set the joints in the vertex shader for this skeleton

		shader.setMat4("Joint_Transforms", JointPoses_To_JointTransforms(pose));	
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
		
		meshes[i].Draw( projection, view, pos, shader,this->file_type);
	}
	shader.Stop();
}

void Full_Model::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 pos,float &angle,glm::vec3 &rot_vec, Shader &shader, SkeletonPose &pose)
{
	shader.use();
	if (HasSkeleton)
	{
		//Sets if the vertices have a skeleton associated with it
		shader.setBool("skeleton", true);
		//Set the joints in the vertex shader for this skeleton

		shader.setMat4("Joint_Transforms", JointPoses_To_JointTransforms(pose));
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

		meshes[i].Draw(projection, view, pos,angle,rot_vec, shader,this->file_type);
	}
	shader.Stop();
}

void Full_Model::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 pos, Shader &shader)
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
		meshes[i].Draw(projection, view, pos, shader,this->file_type);
	}
	shader.Stop();
}


//Import Animation Function
void Full_Model::Import_Animation(std::string file, Type_of_Animation type,std::string name)
{
	
	if (HasSkeleton)
	{
		Assimp::Importer import;	//Import the file 
		const aiScene* scene = import.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights| aiProcess_ValidateDataStructure);


		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;				//Check if ASSIMP through a error
		}
		directory = file.substr(0, file.find_last_of('/'));
		

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
		}
		//Process animations
		if (scene->HasAnimations())
		{
			ProcessAnimation(scene, bones,type,name);
		}
		import.FreeScene();
	}
	//otherwise do nothing
}





void Full_Model::loadModel(std::string path)
{
	Assimp::Importer import;	//importer object from and we will use its ReadFile function
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |aiProcess_LimitBoneWeights|aiProcess_ValidateDataStructure);			//Using bitmasks here (later we can use this to import animation)
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;				//Check if ASSIMP through a error
	}
	directory = path.substr(0, path.find_last_of('/'));

	//Get what kind of file it is, since different files have different coordinate systems
	this->file_type=path.substr(path.find_last_of('.'), path.max_size() - 1);

	//First, lets detect if this has textures
	if (scene->HasTextures())
	{
		this->HasTextures = true;
	}
	
	//Need the root transformation 
	


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
	

	if (scene->HasAnimations())
	{
		//ProcessAnimation(scene, bones,ONE_OFF,"Base");
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
		meshes[i].mesh_transform = Assimp_MatrixConv(node->mTransformation,file_type);

		
		
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
		
		//Position of vertices
		vertex.Position = Assimp_Vec3Conv(mesh->mVertices[i], this->file_type);

		//Normals of vertices
		vertex.Normal = Assimp_Vec3Conv(mesh->mNormals[i], this->file_type);
		
		
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
				vertex.Number_Joints = counter;
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
	std::vector<Joint> joints = Construct_Bone_Hierarchy(winner, scene,bones,file_type);
	


	Skeleton skelly(joints);
	skelly.Root_Transform =Assimp_MatrixConv( scene->mRootNode->mTransformation,file_type);
	
	this->skeleton = skelly;
	
}


//Process Animation..... This was convoluted, because of the way assimp has its data structure.
void Full_Model::ProcessAnimation(const aiScene* &scene, std::vector<aiBone*> &bones,Type_of_Animation type,std::string name)
{
	
	



	//Go through each animation
	for (unsigned int k = 0; k < scene->mNumAnimations; ++k)
	{
		
		//Need times for the keyframes
		std::vector<float> keyTimes;

		//Number of keyframes (since .fbx does not know for some reason
		// .fbx only 
		int number_keyframes=scene->mAnimations[k]->mDuration+1;
		
		

		//Need keyframes
		std::vector<std::pair<float, SkeletonPose>> key_frames;

		//Collection of keyframes for each bone, with each bone has its keyframes ordered 
		//Each sub vector holds the Joint poses for keyframes for a specific bone
		std::vector<std::vector<JointPose>> Coll_joint_poses;

		//Paramter to make sure we get at least one bone that has positive bone_index
		// and to record the mTime
		bool time=true;

		//Go through each bone (channels)
		for (unsigned int j = 0; j < scene->mAnimations[k]->mNumChannels; ++j)
		{
			//Which bone it is
			int bone_index = this->skeleton.Search(scene->mAnimations[k]->mChannels[j]->mNodeName.C_Str());
			
			

			//Collection of key frames for this specific bone, for this specific animation
			std::vector<JointPose> bone_keys;
			
			//Check to make sure this bone is actually in the skeleton structure (it might be a biped system bone)
			if (bone_index != -1)
			{
				//fbx only, this keeps track of which times for the keyframes.
				//Since fbx and assimp do not fill in all the keyframes( for example, the hands only have 2 out of the 17 it should have)
				std::vector<int> fbx_times;


				//Go through each of the key frames for this bone
				for (unsigned int i = 0; i < scene->mAnimations[k]->mChannels[j]->mNumPositionKeys; ++i)
				{
					

					fbx_times.push_back(scene->mAnimations[k]->mChannels[j]->mPositionKeys[i].mTime);

					//Basic information for Joint pose
					Quaternion quat;
					glm::vec3 pos;
					glm::vec3 scale;

					//Put in the information for the quat,pos, scale
					quat = Assimp_QuatConv(scene->mAnimations[k]->mChannels[j]->mRotationKeys[i].mValue,this->file_type);
					
					//Pos of the joints in its parent for this keyframe
					pos = Assimp_Vec3Conv(scene->mAnimations[k]->mChannels[j]->mPositionKeys[i].mValue,this->file_type);
					scale = Assimp_ScaleConv(scene->mAnimations[k]->mChannels[j]->mScalingKeys[i].mValue,this->file_type);

					JointPose jpose(quat, pos, scale, skeleton.Vector_Joints[bone_index], this->skeleton);
					bone_keys.push_back(jpose);
				

					//This grabs the key frame times for later, we only need to do it once
					if (time)
					{
						
						
						//Depending on which file type it is, we need to change the time information around.

						if (this->file_type == ".dae")
						{
							//This is ordered with respect to the keys
							keyTimes.push_back(scene->mAnimations[k]->mChannels[j]->mPositionKeys[i].mTime);
							if (i == scene->mAnimations[k]->mChannels[j]->mNumPositionKeys - 1)
								time = false;
						}
						if (this->file_type == ".fbx")
						{
							//This is ordered with respect to the keys
							keyTimes.push_back(scene->mAnimations[k]->mChannels[j]->mPositionKeys[i].mTime/24);
							if (i == scene->mAnimations[k]->mChannels[j]->mNumPositionKeys - 1)
								time = false;
						}

						
						
					}
				}
				//Next, we need to account for the keyframes that don't exists 
				//Because .fbx just forgets about the ones in between
				if (this->file_type == ".fbx")
				{
					
					int e = 0;
					while (bone_keys.size() < number_keyframes)
					{
						
						
						auto it = bone_keys.begin();

						it += fbx_times[e];
						//std::cout << "fbx" << fbx_times[e] << std::endl;
						JointPose temp_copy = *it;
						//std::cout << "jointpose" << std::endl;
						//std::cout << "number of key frames" << number_keyframes << std::endl;
						
						if (e < fbx_times.size() - 1)
						{
							bone_keys.insert(it, fbx_times[e + 1]-fbx_times[e]-1, temp_copy);
						}
						else
						{
							bone_keys.insert(it, number_keyframes - fbx_times[e] - 1, temp_copy);
						}
						e += 1;
						

					}
					
				}


				//Put in the ordered key frames 
				Coll_joint_poses.push_back(bone_keys);
			}
			
			
		}
		
		if (this->file_type == ".dae")
		{
			// set up the skeleton poses for each keyframes
			for (unsigned int u = 0; u < scene->mAnimations[k]->mChannels[0]->mNumPositionKeys; ++u)		//# of keyframes for this animation
			{
				//collection of joint poses for a specific key frame
				std::vector<JointPose> poses;

				for (unsigned int t = 0; t < skeleton.JointCount; ++t)
				{
					poses.push_back(Coll_joint_poses[t][u]);			//Go through each bone (t parameter) and pick the keyframe (u parameter)
				}
				//skeleton keyframe
				SkeletonPose keyframe(skeleton, poses);
				key_frames.push_back({ keyTimes[u],keyframe });
				
			}
		}
		
		if (this->file_type == ".fbx")
		{
			// set up the skeleton poses for each keyframes
			for (unsigned int u = 0; u < number_keyframes; ++u)		//# of keyframes for this animation
			{
				//collection of joint poses for a specific key frame
				std::vector<JointPose> poses;

				for (unsigned int t = 0; t < skeleton.JointCount; ++t)
				{
					
					
					poses.push_back(Coll_joint_poses[t][u]);			//Go through each bone (t parameter) and pick the keyframe (u parameter)
					
				}
				
				//skeleton keyframe
				SkeletonPose keyframe(skeleton, poses);
				key_frames.push_back({ (float)u/24,keyframe });
				
			}
		}


		


		//Put the new collection of keyframes as a animation in to the data for model
		Animation temp(scene->mAnimations[k]->mName.C_Str(), this->skeleton, key_frames, type);

		std::string Name = name + std::to_string(k);
		
		this->Animations[Name] = temp;
		
		
	}
	
}


/*void Full_Model::ProcessAnimation(const aiScene* &scene, std::vector<aiBone*> &bones, Type_of_Animation type, std::string name)
{

}*/








std::vector<Joint> Construct_Bone_Hierarchy(aiBone* bone, const aiScene* scene, std::vector<aiBone*> &bones,std::string &file_type)
{
	aiNode* node = scene->mRootNode->FindNode(bone->mName.C_Str());			//The node where this bone is located
	Joint parent;
	parent.Name = bone->mName.C_Str();
	parent.Parent_Index = 0;
	parent.M_invBindPose = Assimp_MatrixConv(bone->mOffsetMatrix,file_type);	
	

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
					childJ.M_invBindPose = Assimp_MatrixConv(bones[j]->mOffsetMatrix,file_type);					//Need a function to transfer their matrix to mine
					childJ.Parent_Index = 0;


					//Search through this child's children
					std::vector<Joint> child_V = Construct_Bone_Hierarchy(bones[j], scene, bones,file_type);

					//We need to set the parent_index to these children, since in their world, the parent index is 0
					//so, all we need to do is add the size of the result to these to count for the fact we moved them back in the vector
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