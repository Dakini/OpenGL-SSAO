#include "sceneVarsmethods.h"


using namespace std;



float getRnd() {
    return (2.0 * (float)rand() / RAND_MAX) - 1.0;
}
float lerp(float start, float end, float weight) {
    return (1.0 - weight) * start + weight * end;
}
bool resizeSSAONoise(){
	int noiseDatas = ssaoNoiseSize * ssaoNoiseSize;
	vector<glm::vec3> noiseData;
	//random seed
	srand(time(NULL));
	//generate noise data
	for(int i =0; i<noiseDatas; i++){
		noiseData.push_back(glm::vec3( glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), 0.0));
		noiseData[i] = glm::normalize(noiseData[i]);
	}
	//resize nosie texure and upload data
	texSSAONoise->bind();
	texSSAONoise->image(0, texSSAONoise->getFormat(), ssaoNoiseSize, ssaoNoiseSize, GL_RGB, GL_FLOAT, &noiseData[0]);

	//delete and free memory allocation
	tex2D::unbind();
	vector<glm::vec3> tmp;
	noiseData.swap(tmp);

	return true;
}
frameBuffer2D *fboHDR , *fbogBuffer, *fboAux, *fboSSAOblur, *fboLuma, *fboAdaptedLuma[2], *fboDefault;

bool resizeAuxBuffer(){
	//bind the auxualliry texture and resize
	texAuxA->bind();
	texAuxA->image(0, texAuxA->getFormat(), 1280/(int)auxBufferSize, 720/(int)auxBufferSize);
	tex2D::unbind();
	//bind the appropriate frame buffer and attach auxa texture
	fboAux->bind();
	fboAux->attach(texAuxA, GL_COLOR_ATTACHMENT0);
	frameBuffer2D::unbind();
	return true;
}



void initScene(){
	//crerate scene graph
	sceneGraph_ = Node2::create("root");
	assert(sceneGraph_);
	//add projection and view matrices
	addCamera(camera2::create(1280.0f/720.0f, glm::radians(45.0f), 0.1f,100.0f, false),	addFirstPersonNode(firstPerson::create(glm::vec3(0.0f,0.0f,15.0f))));
	
	pCurrentCamera = cameras_[currentCamera_];
	pCurrentClipCamera_ = cameras_[currentClipCamera];
}
bool resizessaoKernel(){
	//generate Kernel
	vector<glm::vec3> kernel;
	
	srand(time(NULL));
	float *kern = new float[ssaoKernelSize*3];
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;
	for(int i =0; i<ssaoKernelSize; i++){
		glm::vec3 vec(randomFloats(generator) * 2.0 - 1.0, 
        randomFloats(generator) * 2.0 - 1.0, 
        randomFloats(generator));
		vec *= randomFloats(generator);
		float scale = (float(i))/(float) ssaoKernelSize;
		scale =lerp(0.1,1.0,scale*scale);
		vec *=scale;
		kernel.push_back(vec);
	}
	//upload kernel
	gSSAO->use();
	//gSSAO->setUniform("uKernelSize",ssaoKernelSize);

	glUniform1i(glGetUniformLocation(gSSAO->object(),"uKernelSize"),  ssaoKernelSize);
	
	glUniform3fv(glGetUniformLocation(gSSAO->object(), "uKernelOffsets"), ssaoKernelSize,(const GLfloat*) &kernel[0]);

	gSSAO->stopUsing();
	//deallocate memory
	vector<glm::vec3> tmp;
	kernel.swap(tmp);
	return true;
}

void initModels(){
	//create temp mesh
	mesh temp;
	//load the mysty and turin models 
	//mysty main
	load_mesh_ctm("models/mysty.ctm", temp);
	temp.mat = Material::create(tex2D::loadfromFile("textures/mysty/Mysty Texture2.jpg",2),tex2D::loadfromFile("textures/plastic1.specular.png",2),  tex2D::loadfromFile("textures/plastic1.normal.png",2),tex2D::loadfromFile("textures/11454-normal.jpg",2), "Texture2");
	mysty.addModel(temp);
	//mysty back
	load_mesh_ctm("models/mysty-back.ctm", temp);
	temp.mat = Material::create(tex2D::loadfromFile("textures/mysty/Mysty Texture2.jpg",2),tex2D::loadfromFile("textures/plastic1.specular.png",2),  tex2D::loadfromFile("textures/gibbs-normal.jpg",2),tex2D::loadfromFile("textures/11454-normal.jpg",2), "Texture3");
	mysty.addModel(temp);
	//mysty disk
	load_mesh_ctm("models/mysty-disk.ctm", temp);
	temp.mat = Material::create(tex2D::loadfromFile("textures/mysty/Disk Max Texture.jpg",2),tex2D::loadfromFile("textures/plastic1.specular.png",2),  tex2D::loadfromFile("textures/gibbs-normal.jpg",2),tex2D::loadfromFile("textures/11454-normal.jpg",2), "Texture4");
	mysty.addModel(temp);

	
	//set selection turin model
	selection = &mysty;
	
}
void initRender(){
	//create the default frame buffer
	fboDefault = frameBuffer2D::create(1280,720,"moo");
	//create a texture for screen space ambient occlusion
	texSSAONoise= tex2D::create(GL_RGB, (GLsizei)ssaoNoiseSize,(GLsizei)ssaoNoiseSize);

	//resize the noise
	if(resizeSSAONoise())
		cout<<"Done"<<endl;
	//create a texture for cubemap
	texHdr= tex2D::create(GL_RGBA16F, 1280,720);
	texHdr->setMagFilter(GL_NEAREST);
	texHdr->setWrap(GL_CLAMP_TO_EDGE);
	//create the frame buffer for the cubemap
	fboHDR =frameBuffer2D::create();
	//attach the texHDR to color attachment 0
	fboHDR->attach(texHdr, GL_COLOR_ATTACHMENT0);

	//create a depth texture for a frame buffer
	texDepth= tex2D::create(GL_DEPTH_COMPONENT, 1280,720);
	texDepth->setWrap(GL_CLAMP_TO_EDGE);
	texDepth->setName("DEPTH");
	
	//Gbuffer
	texDiffuse= tex2D::create(GL_RGBA8, 1280,720);
	texDiffuse->setMinFilter(GL_LINEAR);
	
	texSpec= tex2D::create(GL_RGBA8, 1280,720);
	texSpec->setMagFilter(GL_NEAREST);
	texSpec->setMinFilter(GL_LINEAR);

	texGeometric = tex2D::create(GL_RGBA8, 1280,720);
	texGeometric->setMinFilter(GL_LINEAR);
	//set up framebuffe for gBuffer
	fbogBuffer =frameBuffer2D::create();
	fbogBuffer->attach(texDiffuse, GL_COLOR_ATTACHMENT0);
	fbogBuffer->attach(texSpec, GL_COLOR_ATTACHMENT1);
	fbogBuffer->attach(texGeometric, GL_COLOR_ATTACHMENT2);
	//attach a depth buffer
	fbogBuffer->attach(texDepth, GL_DEPTH_ATTACHMENT);
	//create a frame buffer for the SSAO
	fboSSAOblur=frameBuffer2D::create();
	fboSSAOblur->attach(texGeometric, GL_COLOR_ATTACHMENT0);

	//aux render buffers

	texAuxA= tex2D::create(GL_RGBA8, 1280, 720);
	texAuxA->setMinFilter(GL_LINEAR);
	if(texAuxA->getMinFilter() ==GL_LINEAR){
		int moo = 2;
	}
	texAuxA->setMagFilter(GL_LINEAR);
	texAuxA->setWrap(GL_CLAMP_TO_EDGE);

	fboAux  =frameBuffer2D::create();
	fboAux->attach(texAuxA, GL_COLOR_ATTACHMENT0);


	//auto exposure buffers

	texLuma = tex2D::create(GL_RGB16F, 256,256);
	texLuma->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	texLuma->setWrap(GL_CLAMP_TO_EDGE);
	texLuma->generateMipMap();

	fboLuma = frameBuffer2D::create();
		
	fboLuma->attach(texLuma,GL_COLOR_ATTACHMENT0);
	//
	for(int i=0; i<2; i++){
		
		texAdaptLuma[i]= tex2D::create(GL_R16F, 1, 1);
		texAdaptLuma[i]->setMinFilter(GL_NEAREST); //dont want filtering between miplevels
		texAdaptLuma[i]->setMagFilter(GL_NEAREST);
		texAdaptLuma[i]->setWrap(GL_REPEAT);
		fboAdaptedLuma[i] = frameBuffer2D::create();
		if(!fboAdaptedLuma[i])
			exit(EXIT_FAILURE);
		fboAdaptedLuma[i]->attach(texAdaptLuma[i], GL_COLOR_ATTACHMENT0);
	}

	//resize aux buffer and ssaoKernel
	if(resizeAuxBuffer())
		cout<<"Done"<<endl;
	if(resizessaoKernel())
		cout<<"Done"<<endl;

	//load in 3D solid textures

	tex3d = tex3D::loadfromFile("textures/caustic.vol",2);
	jagnow= tex3D::loadfromFile("textures/jagnow.vol");
	cobble= tex3D::loadfromFile("textures/cobblestone.vol");
	rgranCool= tex3D::loadfromFile("textures/RGran_Cool_1.vol");
}
//material type
enum MATERIAL_TYPE {
	MATERIAL_SIMPLE		= 0x00000001,  
	MATERIAL_TEXTURE	= 0x00000002,  
	MATERIAL_SOLID		= 0x00000003, 
	
	MATERIAL_SILVER		= 0x00000021,  
	MATERIAL_DIAMOND	= 0x00000012,
	MATERIAL_EMERALD	= 0x00000022,
	MATERIAL_COBBLE		= 0x00000013, 
	MATERIAL_JAGNOW		= 0x00000023, 
	MATERIAL_RGRAN		= 0x00000033, 
	
};struct MTLData {
	MTLData(int _type, float _r, float _g, float _b, float _a){
		type = _type; r = _r; g = _g; b = _b; a = _a;
	}
	int type;
	float r;
	float g;
	float b;
	float a;
};

MTLData material[9]={
	MTLData(MATERIAL_SIMPLE, 1.0, 1.0, 1.0, 0.0),
	MTLData(MATERIAL_TEXTURE, 1.0, 1.0, 1.0, 0.0),
	MTLData(MATERIAL_SOLID, 1.0, 1.0, 1.0, 0.0),

	MTLData(MATERIAL_SILVER, 1.0, 1.0, 1.0, 0.9),
	MTLData(MATERIAL_DIAMOND, 0.8, 0.8, 0.8, 1.0),
	MTLData(MATERIAL_EMERALD, 0.2, 0.8, 0.2, 1.0),

	MTLData(MATERIAL_COBBLE, 1.0, 1.0, 1.0, 0.05),
	MTLData(MATERIAL_JAGNOW, 1.0, 1.0, 1.0, 0.05),
	MTLData(MATERIAL_RGRAN, 1.0, 1.0, 1.0, 0.05),

};


void renderObject(){
	//is the object textured?
	bool isTextured = false;
	//enable depth testing and depth etc. 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	//get material class
	int mtlClass = material[materialIndex].type & 0xf;
	//bind the appropriate shader for texture, solid or plain texture
	tdogl::Program * shader = materialShaders[mtlClass-1];
	
	shader->use();

	//set projection and view matrix
	shader->setUniform("uModelViewMatrix",getCurrentCamera()->getViewMatrix() *selection->model);
	shader->setUniform("uModelViewProjectionMatrix",getCurrentCamera()->getProjectionMatrix()  * getCurrentCamera()->getViewMatrix() *selection->model );
	switch (mtlClass)
	{
	case MATERIAL_SIMPLE:
		//bind the color to the shader if no texture
		shader->setUniform("color", glm::vec4(material[materialIndex].r,material[materialIndex].g,material[materialIndex].b,material[materialIndex].a));
		break;
	case MATERIAL_TEXTURE:
		//bind a detail scale for normal mapping if textured
		shader->setUniform("uDetailTexScale", glm::vec2(25.5,25.5));
		isTextured = true;
			break;
	case MATERIAL_SOLID:
		//bind the model matrix for solid texture
		shader->setUniform("uModel",selection->model);
		glActiveTexture(GL_TEXTURE0);
		switch (materialIndex) //bind the various solid texture depending on material index
		{
		case 2:
			glBindTexture(GL_TEXTURE_3D,tex3d->getHandle());
			break;
		case 6:
		glBindTexture(GL_TEXTURE_3D,cobble->getHandle());
		break;
		case 7:
			glBindTexture(GL_TEXTURE_3D, jagnow->getHandle());
			break;
		case 8:
			glBindTexture(GL_TEXTURE_3D, rgranCool->getHandle());
			break;
		}

	}

	//render mesh
	draw_meshModel(selection, 0, 1, isTextured);
	
	
	shader->stopUsing();
	
}


void drawQuad(){
	//draw quad for frame buffer
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray (g_ss_quad_vao);
	glDrawArrays (GL_TRIANGLE_STRIP, 0, g_ss_quad_point_count);
}

void drawQuad2(){
	//draw quad for frame buffer
	glBindVertexArray (g_ss_quad_vao);
	glDrawArrays (GL_TRIANGLE_STRIP, 0, g_ss_quad_point_count);
}



void loadCubeMaps(){
	//load 4 hdr cubemaps
	for(int i =0; i<4; i++){
		hdrs[i] = texCu::loadfromFile(hdrTex[i], iTexture::TEX_MIPMAP);
	}
	
}


void initssQuad () {
	//get quad vertices
	GLfloat ss_quad_pos[] = {
		
		-1.0, -1.0,
		 1.0, -1.0,
		 -1.0,  1.0,
		 1.0,  1.0,
	};

	// create VBOs, VAO, and set attribute locations
	g_ss_quad_point_count = sizeof (ss_quad_pos) / sizeof (GLfloat) / 2;

	/* create VBO and VAO here */
	glGenVertexArrays (1, &g_ss_quad_vao);
	glBindVertexArray (g_ss_quad_vao);
	GLuint points_vbo = 0;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (
		GL_ARRAY_BUFFER, sizeof (ss_quad_pos), ss_quad_pos, GL_STATIC_DRAW
	);
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
}



static tdogl::Program* loadShaders(char* frag, char* vert){
	//load shaders from file
	vector<tdogl::shader>shaders;
	shaders.push_back(tdogl::shader::shaderFromFile(vert,GL_VERTEX_SHADER));
	shaders.push_back(tdogl::shader::shaderFromFile(frag,GL_FRAGMENT_SHADER));
	//return a new program thats bound the shaders
	return new tdogl::Program(shaders);

}


void fpsCounter(GLFWwindow* window){

	static double preSeconds = glfwGetTime();
	static int framecount;
	//get elapsed time 
	double curSeconds = glfwGetTime();
	double elapsed = curSeconds-preSeconds;
	//change frames every 0.25 secs
	if(elapsed>0.25){
		preSeconds = curSeconds;
		double fps = (double)framecount /elapsed; //get frame count
	//	cout<<"fps"<<elapsed<<endl;
		char tmp[128];
		sprintf(tmp, "opengl @ fps: %0.2f", fps); //change string and add to title
		glfwSetWindowTitle (window, tmp);
		framecount = 0;
	}
	framecount++;

}

void ssaoPrePass(){

	//bind aux frame buffer
	fboAux->bindDraw();

	glViewport(0,0,1280,720);
	//disable cull and depth test
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//gTester->use();
	gSSAO->use();
	// bind textures
	glActiveTexture(GL_TEXTURE0);
	texGeometric->bind();
	glActiveTexture(GL_TEXTURE1);
	texDepth->bind();
	glActiveTexture(GL_TEXTURE2);
	texSSAONoise->bind();
	
	
	gSSAO->setUniform("uRadius", ssaoRadius);
	gSSAO->setUniform("uPower", ssaoPower);
	gSSAO->setUniform("uProjectionMatrix",getCurrentCamera()->getProjectionMatrix());
	drawQuad2();
	gSSAO->stopUsing();

	//blur pass on ssao
	fboSSAOblur->bindDraw();
	glViewport(0,0,1280,720);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//gTester->use();
	gblurSSAO->use();

	glActiveTexture(GL_TEXTURE0);
	texAuxA->bind();
	gblurSSAO->setUniform("uBlurSize", ssaoNoiseSize);
	//multiplicative blend into alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ZERO); // multiplicative blend
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); // only write alpha
	drawQuad2();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_BLEND);
	gblurSSAO->stopUsing();
	
}



void renderLights(){

	fboHDR->bindDraw();
	
	glClear(GL_COLOR_BUFFER_BIT);

	///bind gbuffer textures
	glActiveTexture(GL_TEXTURE1);
	texDiffuse->bind();
	glActiveTexture(GL_TEXTURE2);
	texSpec->bind();
	glActiveTexture(GL_TEXTURE3);
	texGeometric->bind();
	glActiveTexture(GL_TEXTURE4);
	texDepth->bind();
	
	glUseProgram(gSkybox->object());
	
	glActiveTexture(GL_TEXTURE0);
	

	//bind the cube map depending on the scene index
	hdrs[sceneIndex]->bind();
	gSkybox->setUniform("uInverseViewMatrix",getCurrentCamera()->getWorldMatrix());

	drawQuad2();
	gSkybox->stopUsing();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	//enqable the blend mode
	gLighting->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	hdrs[sceneIndex]->bind();
	gLighting->setUniform("uInverseViewMatrix",getCurrentCamera()->getWorldMatrix());
	drawQuad2();

	gLighting->stopUsing();
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
void renderToMipMap(frameBuffer2D* fbo, tdogl::Program * shader, int bindLocation){
	assert(fbo);
	tdogl::texture2DEX * tex = (tdogl::texture2DEX*)fbo->getAttachment(GL_COLOR_ATTACHMENT0);
	glm::ivec2 texSize(tex->getWidth(), tex->getHeight());

	
	int nMipMaps = 2;
	assert(shader);
	shader->use();

	fbo->bindDraw();
	
	glActiveTexture(GL_TEXTURE0 + bindLocation);
	tex->bind();

	for(int i = 1; i<nMipMaps; i++){
		fbo->attach(tex, GL_COLOR_ATTACHMENT0,i);
		tex->setBaseLevel(i-1);
		tex->setMaxLevel(i-1);
		texSize /=2;
		glViewport(0,0,texSize.x, texSize.y);
		drawQuad2();
	}
	//restore mip map
	fbo->attach(tex, GL_COLOR_ATTACHMENT0,0);
	tex->setBaseLevel(0);
	tex->setMaxLevel(nMipMaps);
	frameBuffer2D::unbind();
}

void render(float dt){
	//previous lumination is current lumination
	int previousAdaptLuma = currentAdapLuma;
	currentAdapLuma = (currentAdapLuma+1)%2;
	//if the aux buffer isnt same size, then resize
	if((int)setAuxBufferSize !=auxBufferSize){
		auxBufferSize = setAuxBufferSize;
		if(!resizeAuxBuffer())
			exit(EXIT_FAILURE);
	}
	//if kernel changes size then resize kerenl
	if((int) setSSaoKernelSize != ssaoKernelSize){
		ssaoKernelSize = setSSaoKernelSize;
		if(!resizessaoKernel()){
			exit(EXIT_FAILURE);
		}
	}
	//if ssaoNoise changes size then resize noise
	if((int) setSSaoNoiseSize != ssaoNoiseSize){
		ssaoNoiseSize = setSSaoNoiseSize;
		if(!resizeSSAONoise()){
			exit(EXIT_FAILURE);
		}
	}
	//get current camera and fps node

	
	//bind default framebuffer
	fbogBuffer->bindDraw();
	//clear depth and colour buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//render mesh
	renderObject();
	//render ssao prepass and blur it
	ssaoPrePass();
	//render image based lighting from cube maps
	renderLights();
	//render intial log luminance
	fboLuma->bindDraw();
	
	glViewport(0,0,fboLuma->getWidth(), fboLuma->getHeight());

	shaderLuminance->use();
	glActiveTexture(GL_TEXTURE0);
	texHdr->bind();
	drawQuad2();
	
	shaderLuminance->stopUsing();
	
	//downsample into the mipmap levels
	shaderAvgminMax->use();
	renderToMipMap(fboLuma, shaderAvgminMax,0);
	shaderAvgminMax->stopUsing();
	//adapt luminance value based on previous frame
	fboAdaptedLuma[currentAdapLuma]->bind();
	glViewport(0,0,fboAdaptedLuma[currentAdapLuma]->getWidth(), fboAdaptedLuma[currentAdapLuma]->getHeight());
	shaderLuminanceAdpat->use();
	glUniform1f(glGetUniformLocation( shaderLuminanceAdpat->object(), "uAdaptionRate"), dt * 2.5f);
	glActiveTexture(GL_TEXTURE0);
	texLuma->bind();
	glActiveTexture(GL_TEXTURE1);
	texAdaptLuma[previousAdaptLuma]->bind();
	drawQuad2();
	
	shaderLuminanceAdpat->stopUsing();
	
	//Post Processing, auto exposure, tonemapping, motion blur
	int width, height;
	glfwGetWindowSize(output, &width, &height);
	glViewport(0,0,width, height);

	//get matrices for input blur
	static glm::mat4  prevViewMatrix =getCurrentCamera()->getViewProjectionMatrix();
	glm::mat4 inverseViewPromatrix = glm::inverse(getCurrentCamera()->getViewProjectionMatrix());

	fboDefault->bindDraw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glfwGetWindowSize(output, &width, &height);
	glViewport(0,0,width, height);
	//bind inputs to the shader
	shaderPostProcess->use();
	glUniform1f(glGetUniformLocation( shaderPostProcess->object(), "uExposure"),2.0f);
	glUniform1f(glGetUniformLocation( shaderPostProcess->object(), "uFrameTime"), dt);
	glUniform1f(glGetUniformLocation( shaderPostProcess->object(), "uMotionScale"), (1.0f/60.0f)/dt );
	shaderPostProcess->setUniform( "uInverseViewProjectionMatrix",inverseViewPromatrix);
	shaderPostProcess->setUniform( "uPrevViewProjectionMatrix",prevViewMatrix);
	glActiveTexture(GL_TEXTURE0);
	texHdr->bind();
	glActiveTexture(GL_TEXTURE1);
	texDepth->bind();
	glActiveTexture(GL_TEXTURE2);
	texAdaptLuma[currentAdapLuma]->bind();

	drawQuad2();

	shaderPostProcess->stopUsing();
	prevViewMatrix = getCurrentCamera()->getViewProjectionMatrix();
}


void update (float dt){
	//update scene graph
	sceneGraph_->update(dt);

}
//main function
int main(int argc, char**argv)
{
	//init window
	initWindow(argc, argv);
	//init shaders , tecutres, camera, and cube maps
	init(argc,argv);

	initScene();

	initRender();
	initModels();
	loadCubeMaps();

	//renderer
	while(!glfwWindowShouldClose(output) &&  glfwGetKey(output, GLFW_KEY_ESCAPE ) != GLFW_PRESS){
		//get fps
		fpsCounter(output);
		//get timer
		timer_.update();
		 dt = timer_.seconds();
		 //update scene graph
		update(dt);
		//render
		render(dt);
		
		glfwSwapBuffers(output);
		glfwPollEvents();
	}

	//close openGL window and any other GLFW resources
	glfwDestroyWindow(output);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void resizeHandler(GLFWwindow * window, int width, int height)
{
 
}




void init(int argc, char**argv){
	//init viewport 

	int width, height;
	glfwGetWindowSize(output, &width, &height);
	glViewport(0,0,width,height);
	//init shaders
	gShader2 = loadShaders("shaders/materials/GenericMaterialDeferredTextured-frag.txt" , "shaders/models/genericModel-vert.txt");
	gShader = loadShaders("shaders/materials/GenericMaterialDeferredsimple-frag.txt" , "shaders/models/genericModel-vert.txt");
	gShader3 = loadShaders("shaders/materials/GenericMaterialDeferredsolid-frag.txt" , "shaders/models/genericModel-vert.txt");
	gSSAO= loadShaders("shaders/ssao-frag.txt" , "shaders/models/SAQuadModel-vert.txt");
	gblurSSAO = loadShaders("shaders/ssaoblur-frag.txt" , "shaders/basic-vert.txt");

	gSkybox = loadShaders("shaders/materials/envmapMaterial-frag.txt", "shaders/models/SAQuadModel-vert.txt");
	gLighting= loadShaders("shaders/materials/LightDeferredIbl-frag.txt", "shaders/models/SAQuadModel-vert.txt");
	shaderPostProcess = loadShaders("shaders/postprocess-frag.txt", "shaders/basic-vert.txt"); 
	shaderLuminance = loadShaders("shaders/lum-frag.txt", "shaders/basic-vert.txt"); 
	shaderAvgminMax = loadShaders("shaders/avgminmax-frag.txt", "shaders/basic-vert.txt"); 
	shaderLuminanceAdpat = loadShaders("shaders/adaptlum-frag.txt", "shaders/basic-vert.txt"); 
	//push material shaders to a vector
	materialShaders.push_back(gShader);
	materialShaders.push_back(gShader2);
	materialShaders.push_back(gShader3);
	
	//init quad for render frames
	initssQuad();

	
}

static void mouse_scroll(GLFWwindow* window, double something, double pos){
	//get camera 
	firstPerson * fpCtrl = fpsCtrl[currentfpCtrl];
	
	//if pos is greater than 0 then zoom in
if(pos>0){
	cameraZoom*=0.9f;
}else {
	cameraZoom *=1.1f;
}
cout<<cameraZoom<<endl;
fpCtrl->setZoom(cameraZoom,dt);

}

static void mouseButtonHandler(GLFWwindow *window,int button, int action ,int mods){
		//get mouse position on the screen
		double x, y;
		glfwGetCursorPos(window, &x,&y);

		//set mouse positions and set which mouse button is pressed
		switch(action){
			case GLFW_PRESS:
				mouseX =x;
				mouseY =y;
				switch (button){
					case GLFW_MOUSE_BUTTON_LEFT:
				//glfwGetCursorPos(window ,&mouseX, &mouseY);
				leftMouseButtonPressed = true;
				break;
					case GLFW_MOUSE_BUTTON_RIGHT:
					//	glfwGetCursorPos(window ,&mouseX, &mouseY);
				rightMouseButtonPressed = true;
				break;
					case GLFW_MOUSE_BUTTON_MIDDLE:
						break;
				}
			break;
			case GLFW_RELEASE:
				//on release set mouse x y to position on screen
				switch (button){
					case GLFW_MOUSE_BUTTON_LEFT:
				glfwGetCursorPos(window ,&mouseX, &mouseY);
				leftMouseButtonPressed = false;
				break;
					case GLFW_MOUSE_BUTTON_RIGHT:
						glfwGetCursorPos(window ,&mouseX, &mouseY);
				rightMouseButtonPressed = false;
				break;
					case GLFW_MOUSE_BUTTON_MIDDLE:

						break;
				}


			}
}

static void error_callback(int erro, const char* desc){
	fputs(desc ,stderr);
}
void initWindow(int argc, char** argv){
	//init GLFW and window variables
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	//opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS,8);
	
	//create window 
	output = glfwCreateWindow(1280,720, "OpenGL viewer", NULL, NULL);
	if(!output){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//set error and current context
	glfwSetErrorCallback(error_callback);
	glfwMakeContextCurrent(output);
	//enable glew experiemntal or odd crashes
	glewExperimental = true;
	//init glew and devil image library
	glewInit();
	ilInit();
	iluInit();

	//set the methods to be used for windowsize, keyboard and mouse input
	glfwSetWindowSizeCallback(output, resizeHandler);
	glfwSetKeyCallback(output, key_callback);
	glfwSetMouseButtonCallback(output, mouseButtonHandler);
	glfwSetCursorPosCallback(output, mousePosHandler);
	glfwSetScrollCallback(output, mouse_scroll);
	
	//set size of screen space ambient occlusion
	ssaoNoiseSize =4;
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	//enable seamless cubemaps or weird visual artefacts
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	//change scene index
	if(key==GLFW_KEY_Q){
		
	sceneIndex = 0;
	}
		
		if(key==GLFW_KEY_W){
		
	sceneIndex = 1;
	}
		if(key==GLFW_KEY_E){
		
	sceneIndex = 2;
	}
		if(key==GLFW_KEY_R){
		
	sceneIndex = 3;
	}

	//change material index
		if(key==GLFW_KEY_1){
		
			materialIndex = 0;
	}
		if(key==GLFW_KEY_2){
		
			materialIndex = 1;
	}
		if(key==GLFW_KEY_3){
		
			materialIndex = 2;
	}
		if(key==GLFW_KEY_4){
		
			materialIndex = 6;
	}
		if(key==GLFW_KEY_5){
		
			materialIndex = 7;
	}
		if(key==GLFW_KEY_6){
		
			materialIndex = 8;
	}
		
}



static void mousePosHandler(GLFWwindow * window,double x, double y){
	//static for rotation, accel and moouse sensivtivity
	static const float ROTATION_ACCEL = 50.0f;
	static const float TRANSLATION_ACCEL = 100.0f;
	static const float MOUSE_SENSITIVITY= 1.0f;
	///get the delta
	float rotationDelta = ROTATION_ACCEL * dt;
	float translationDelta = TRANSLATION_ACCEL * dt;
	//get current camera
	firstPerson * fpCtrl = fpsCtrl[currentfpCtrl];

	int width, height;
	glfwGetWindowSize(output, &width, &height);

	//if left mouse button is pressed
	if(true ==leftMouseButtonPressed){
		//work out how it has traveled from last position
		int dx = x-mouseX;
		int dy = y-mouseY;

		mouseX = x;
		mouseY = y;
		camYaw +=dx*0.9;
		camPitch +=dy*1.1f;
		//cout<<dx<<" " <<dy<<endl;
		//get delta and get mouse sensitivity
		glm::vec2 deltOid((float)dx/(float)width, (float)dy/(float)height);
		deltOid *= MOUSE_SENSITIVITY;
		//change pitch and yaw rotation
		fpCtrl->offsetOrientation(camPitch, camYaw);

	}
	if(rightMouseButtonPressed){
		//if mouse button is pressed pan the camera
		int dx = -(x-mouseX);
		int dy = (y-mouseY);
		glm::vec2 deltoid((float)dx /(float)width, (float)dy/(float) height );
		deltoid *=MOUSE_SENSITIVITY;

		fpCtrl->offsetPosition(glm::vec3(deltoid.x * translationDelta /dt, deltoid.y * translationDelta /dt,0.0));
		mouseX = x;
		mouseY = y;


	}
	//update
	update(dt);
}


