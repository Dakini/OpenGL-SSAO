#include "ctmModel.h"

struct vec2
{
    float x, y;
};
struct vec3
{
    float x, y, z;
};

struct vec4
{
    float x, y, z,w;

};



void load_mesh_ctm(const char* filepath, mesh &new_mesh) {
	//create new mesh, set material to 0
	new_mesh = mesh();
	new_mesh.mat = 0;
	//Use openCTM api to load model
	//create new context and load file
    CTMcontext context = ctmNewContext(CTM_IMPORT);
    ctmLoad(context, (filepath));
	//assert that a file was loaded
    assert(ctmGetError(context) == CTM_NONE);
    //get triangle count
    CTMuint tri_count = ctmGetInteger(context, CTM_TRIANGLE_COUNT);
	CTMuint  num_vertices = ctmGetInteger(context, CTM_VERTEX_COUNT); //get vertex count
    CTMuint num_indices = tri_count * 3; //num of indices
    CTMuint num_normals = 0;//
    CTMfloat const* vertices = ctmGetFloatArray(context, CTM_VERTICES); //get vertices
    CTMuint const* indices = ctmGetIntegerArray(context, CTM_INDICES); //get indices
	vector<glm::vec3> tempVertices, tempNormals; 
	vector<glm::vec2> tempUvs;
	CTMfloat const* uvs = ctmGetFloatArray(context, CTM_UV_MAP_1);
	CTMint numMpas = ctmGetInteger(context,CTM_UV_MAP_COUNT);
    CTMfloat const* normals = ctmGetFloatArray(context, CTM_NORMALS);
	
    new_mesh.num_tris = tri_count;
    new_mesh.normals = num_normals > 0;


	
	vector<glm::vec3> mNormals, mVertices;
	vector<glm::vec2> mUvs, indexedUvs;
	vector<glm::vec4> mTangent;
	
	
	//push the vertices, uvs, and normals into a 
	for(unsigned int i =0; i<num_vertices; i++){
		mNormals.push_back(glm::vec3(normals[i*3], normals[i*3+1], -normals[i*3+2]));
		indexedUvs.push_back(glm::vec2(uvs[i*2], uvs[i*2+1]));
		mVertices.push_back(glm::vec3(vertices[i*3], vertices[i*3+1],-vertices[i*3+2]));
	}

	//sort the mesh vertices etc, 
	new_mesh.sortStuff(mNormals, indices, mVertices, indexedUvs, num_indices, num_vertices);
	//gen tangents for bump/normal mapping
	new_mesh.genTangentsAndBitangents();
	//get bounding box of mesh
	boundingBox(mVertices, new_mesh.sceneMin, new_mesh.sceneMax);
	
	//create new vertex buffer array
	glGenVertexArrays(1,&(new_mesh.vba));
	glBindVertexArray((new_mesh.vba));
	//bind the vertex buffer object , allocate memory and send vertex data over
    glGenBuffers(1, &(new_mesh.vert_vbo));
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * num_vertices, &new_mesh.vertex[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(vec3),(const GLvoid*) 0);
	//bind the normal vbo, allocate memory and send normal data over
	glGenBuffers(1, &(new_mesh.norm_vbo));
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh.norm_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * num_vertices, &new_mesh.norms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(1);
	//bind the tan vbo , allocate memory and send tangent data over
	glGenBuffers(1, &(new_mesh.tan_vbo));
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh.tan_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * num_vertices,&new_mesh.tans[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
    glEnableVertexAttribArray(2);
	//bind the uv vbo , allocate memory and send uv data over
	glGenBuffers(1, &(new_mesh.uv_vbo));
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh.uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * num_vertices,&new_mesh.uvCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    glEnableVertexAttribArray(3);
	
	new_mesh.numIndices = new_mesh.indices_.size() ;
	//assert that there are indices
    assert(indices);
	//generate an index buffer object, and allocate memory and send over the indices
    glGenBuffers(1, &(new_mesh.ibo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, new_mesh.numIndices * sizeof(mesh::TriangleT), &new_mesh.indices_[0], GL_STATIC_DRAW);
	//free the open ctm context
	ctmFreeContext(context);
	//deallocate the memory from the vectors for the new mesh
	vector<glm::vec3> temp;
	vector<glm::ivec3> tmp2;
	vector<glm::vec2> tmp1;
	vector<mesh::vertexT> tmpVer;
	vector<mesh::TriangleT> tmpTri;
	new_mesh.norms.swap(temp);
	new_mesh.vertex.swap(temp);
	new_mesh.vertices_.swap(tmpVer);
	new_mesh.indices_.swap(tmpTri);
	new_mesh.vertex.swap(temp);
	new_mesh.tans.swap(temp);
	new_mesh.indice.swap(tmp2);
	new_mesh.uvCoords.swap(tmp1);

}



void boundingBox(vector <glm::vec3> mVertices, glm::vec3 &aMin, glm::vec3 &aMax){
	//if there are no vertices set bounding box to 0,0,0
	if(mVertices.size()<1)
		aMin = aMax = glm::vec3(0.0f,0.0f,0.0f);
	else //else set them to the first vertice
		aMin =aMax = mVertices[0];
	//loop through to get the max and min for x,y,z positon
	for(unsigned int i=1; i<mVertices.size();i++){
	if(mVertices[i].x <aMin.x)
		aMin.x = mVertices[i].x;
	else if(mVertices[i].x >aMax.x)
		aMax.x = mVertices[i].x;
	if(mVertices[i].y <aMin.y)
		aMin.y = mVertices[i].y;
	else if(mVertices[i].y >aMax.y)
		aMax.y = mVertices[i].y;
	if(mVertices[i].z <aMin.x)
		aMin.z = mVertices[i].z;
	else if(mVertices[i].z >aMax.z)
		aMax.z = mVertices[i].z;

	}
}
void mesh::genTangentsAndBitangents(){
	//set all the vertices to 0
	for(int i =0; i<vertices_.size(); i++){
		vertices_[i].tangent =glm::vec3(0.0);
		tans.push_back(glm::vec3(0.0));
	}
	//get sum of tans per triangle
	for(int i = 0; i<indices_.size(); i++){
		
		vertexT &a = vertices_[indices_[i].a];
		vertexT &b = vertices_[indices_[i].b];
		vertexT &c = vertices_[indices_[i].c];

		glm::vec3 ba = b.position - a.position;
		glm::vec3 ca = c.position - a.position;

		glm::vec2 tba = b.uvCoords - a.uvCoords;
		glm::vec2 tca = c.uvCoords - a.uvCoords;
		float ff = (tba.x* tca.y - tba.y * tca.x);
		if((ff==0)){
			ff=1.0f;
		}
		float r = 1.0f / ff;
		glm::vec3 bitan =  glm::vec3(
			(tca.x * ba.x -tba.x * ca.x) * r, 
			(tca.x * ba.y -tba.x * ca.y) * r, 
			(tca.x * ba.z -tba.x * ca.z) * r);
		glm::vec3 tang = glm::vec3(
			(tca.y * ba.x -tba.y * ca.x) * r, 
			(tca.y * ba.y -tba.y * ca.y) * r, 
			(tca.y * ba.z -tba.y * ca.z) * r);

		a.tangent += tang;
		b.tangent += tang;
		c.tangent += tang;

	}
	

	//normalize and orthagonalise the tangents
	for(int i=0; i<vertices_.size();i++){
		glm::vec3 a = vertices_[i].tangent;
		glm::vec3 n = vertices_[i].normal;
		
		a = glm::normalize(a-n * glm::dot(n,a));
		tans[i] = vertices_[i].tangent;
	}

}

void draw_meshModel(meshModel const* mesh, GLuint pos_attrib,bool isTex, GLint normal_attrib ) {

	if(!isTex){
		//if it doesnt have a texture
		//draw meshes
	for(int i = 0; i <mesh->meshes.size(); i++){
		glBindVertexArray(mesh->meshes[i].vba);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	mesh->meshes[i].ibo);
		glDrawElements(GL_TRIANGLES, mesh->meshes[i].numIndices* sizeof(mesh::TriangleT), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	}
	}else{
		//if it does have a texture, bind them and render the mesh
		for(int i = 0; i <mesh->meshes.size(); i++){
		mesh->meshes[i].mat->enable();
		glBindVertexArray(mesh->meshes[i].vba);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	mesh->meshes[i].ibo);
		glDrawElements(GL_TRIANGLES, mesh->meshes[i].numIndices* sizeof(mesh::TriangleT), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	}
	}

}
void mesh::sortStuff(vector<glm::vec3> normals, CTMuint const *indices, vector<glm::vec3>  vertices, vector<glm::vec2> uvs, int numIndices, int numVertices){
	//tmp list for expanding smoothing groups, used to prevent duplicate vertices
	//within a smoothing group. real index is used to track the actual vertex

	struct tmpVertexT{ size_t smoothGroup; IndexT vt; IndexT realIndex;};
	tmpVertexT * tempVertexList = new (std::nothrow) tmpVertexT[numVertices];

	assert(tempVertexList);

	for(int i =0; i<numVertices; i++){
		tempVertexList[i].smoothGroup = 0;
		tempVertexList[i].vt = 0;
		tempVertexList[i].realIndex = i;
	
	}
	struct objVertexT{size_t v, vt, vn;};
	int k =0;
	size_t thisSmoothGroup = 1;
	for(int i =0; i<numIndices/3; i++){
		//read in indices
		objVertexT tmpTri[3];
		objVertexT &a = tmpTri[0];
		objVertexT &b = tmpTri[1];
		objVertexT &c = tmpTri[2];

		a.v = indices[i * 3 +0];
		a.vn = indices[i * 3 +0];
		a.vt = indices[i * 3 +0];

		b.v = indices[i * 3 +1];
		b.vn = indices[i * 3 +1];
		b.vt = indices[i * 3 +1];

		c.v = indices[i * 3 +2];
		c.vn = indices[i * 3 +2];
		c.vt = indices[i * 3 +2];
		//construct final triangle
	TriangleT tri;
		for(size_t j=0; j<3; j++){
			tmpTri[j].v;
			tmpTri[j].vt;
			tmpTri[j].vn;

			if(tempVertexList[tmpTri[j].v].vt == tmpTri[j].vt && tempVertexList[tmpTri[j].v].smoothGroup ==thisSmoothGroup ){
				tri[j] = tempVertexList[tmpTri[j].v].realIndex;
			}else{
				tempVertexList[tmpTri[j].v].smoothGroup = thisSmoothGroup;
				tempVertexList[tmpTri[j].v].vt = tmpTri[j].vt;
				tempVertexList[tmpTri[j].v].realIndex =vertices_.size();
				tri[j] = vertices_.size();

				vertexT ver;
				vertex.push_back(vertices[tmpTri[j].v]);
				norms.push_back(normals[tmpTri[j].vn]);
				uvCoords.push_back(uvs[tmpTri[j].vt]);
				ver.position = vertices[tmpTri[j].v];
				ver.normal = normals[tmpTri[j].vn];
				ver.uvCoords = uvs[tmpTri[j].vt];
				vertices_.push_back(ver);
				
			}
		}
	
		indice.push_back(glm::ivec3(tri.a, tri.b, tri.c));
		indices_.push_back(tri);
	

	}
	
}
