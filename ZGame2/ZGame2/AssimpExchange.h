#ifndef ASSIMP_H
#define ASSIMP_H

#include <glm/glm.hpp>
#include "Quaternion.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/*This header is use to transfer the assimp data to my data structure
* Change the Matrix in Assimp to my matrix set up with glm
* also, change Quaternions in assimp to my quaternions
* 
*/


/* Transfer the Assimp matrix (which is row-major) to glm matrix (which is column-major)
* (this also depends on the file type that is being imported in)
*/
 glm::mat4 Assimp_MatrixConv(aiMatrix4x4 &matrix,std::string &file_type);


/* Transfer the Assimp Quaternion to the my Quaternion class
*/
 Quaternion Assimp_QuatConv(aiQuaternion &quat);


/* Transform the Assimp vectors to glm vec3
*/
 glm::vec3 Assimp_Vec3Conv(aiVector3D &vec, std::string &file_type);




#endif
