#ifndef ASSIMP_H
#define ASSIMP_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*This header is use to transfer the assimp data to my data structure
* Change the Matrix in Assimp to my matrix set up with glm
* also, change Quaternions in assimp to my quaternions
* 
*/


glm::mat4 Assimp_MatrixConv(aiMatrix4x4 &matrix);






#endif
