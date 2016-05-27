#ifndef MESH_HPP
#define MESH_HPP
#include "opengl.h"
#include "tex2d.h"
#include "Material.h"
//#include <OpenGL/gl.h>
#include "openctm\openctm.h"

struct mesh
{

	typedef int IndexT;
	//vertex struct with pos, normals, tangents adn uvs
	struct vertexT{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
	    glm::vec2 uvCoords;
	}; 
	//struct for each triangle
	struct TriangleT {
		IndexT a, b, c;

		IndexT& operator[](IndexT i) {
			assert(i >= 0 && i < 3);
			return (&a)[i];
		}
	};

    GLuint vert_vbo; //vert vbo
	GLuint vba; //vertex buffer array
    GLuint norm_vbo; //norm vbo
    GLuint ibo; ///index buffer object
	GLuint uv_vbo; //uv vbo
	GLuint tan_vbo; //tan vbo

	Material *mat; //material to e applied to object
	glm::mat4 model; //model matrix
	
    int num_tris; //num triangles
    bool normals; //has normals
	std::vector<vertexT> vertices_; //vector to store vertices
	std::vector<TriangleT> indices_; //vector to store indices
	int numIndices; // num indices
	vector<glm::vec3> vertex, norms, tans;
	glm::vec3  sceneMax, sceneMin;
	vector<glm::ivec3>indice;
	vector<glm::vec2> uvCoords;
	//sort and reorder vector and triangles
	void sortStuff(vector<glm::vec3> normals, CTMuint const *indices, vector<glm::vec3>  vertices, vector<glm::vec2> uvs, int numIndices, int numVertices);
	void genTangentsAndBitangents(); //gen tan and bitangents for normal mapping

};
//struct to store each multiple meshs within one model
struct meshModel{
	vector<mesh> meshes; //vector of meshs
	glm::mat4 model; //model matrix
	void calcModelMatrix(); //calc model matrix for all
	void addModel(mesh Mesh); //add mesh to struct
};
inline void meshModel::addModel(mesh Mesh){
	meshes.push_back(Mesh); //push the mesh onto the vector
	calcModelMatrix(); //calc model matrix
}
inline void meshModel::calcModelMatrix(){
	//get min and max vectors within mesh
	glm::vec3 meshMax, meshMin; 
	meshMax = meshMin = glm::vec3(0.0,0.0,0.0);
	for(int i =0; i<meshes.size(); i++){
		meshMin = glm::min(meshMin, meshes[i].sceneMin);
		meshMax = glm::max(meshMax, meshes[i].sceneMax);
	}
	
	float _scaleFactor;
	float diff = 0.0f;
	glm::vec3 _sceneCenter;
	//get centre of bounding box
	_sceneCenter.x = ((meshMin.x+meshMax.x)/2.0f);
	_sceneCenter.y = ((meshMin.y+meshMax.y)/2.0f);
	_sceneCenter.z = ((meshMin.z+meshMax.z)/2.0f);
	//get diff
	diff = meshMax.x - meshMin.x;
	if(meshMax.y -meshMin.y > diff){
		diff = meshMax.y -meshMin.y ;
	}
		if(meshMax.z -meshMin.z > diff){
		diff = meshMax.z -meshMin.z ;
	}
		//create scale factor
	_scaleFactor = 10.5f/diff;
	//create the model matrix by tanslating the model to centre of screen and scaling down to fit on screen
	model =  glm::translate(glm::mat4(), glm::vec3(-_sceneCenter.x*_scaleFactor, -_sceneCenter.y*_scaleFactor, -_sceneCenter.z*_scaleFactor)) * glm::scale(glm::mat4(), glm::vec3(_scaleFactor,_scaleFactor,_scaleFactor));
  
}

void load_mesh_ctm(const char* filepath, mesh &Mesh); //load OpenCTM file

void draw_meshModel(const meshModel *mesh, GLuint pos_attrib, bool isTextured, GLint normal_attrib = -1); //drawMeshModel
void boundingBox(vector <glm::vec3> mVertices, glm::vec3 &aMin, glm::vec3 &aMax); //calcBounding box

#endif
