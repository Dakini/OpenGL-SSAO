#ifndef SCENE_H
#define SCENE_H

// relevent header files
#include "opengl.h"
#include "program.h"
#include "shader.h"
#include "ctmModel.h"
#include "tex3d.h"
#include "framebuffer.h"
#include "tex2d.h"
#include "camera2.h"
#include "firstPerson.h"
#include "clock.h"
#include "textCube.h"

int sceneIndex;
int objectIndex;
int materialIndex;

float dt; //timer
vector<camera2*>cameras_; 
vector<firstPerson*> fpsCtrl;
Node2 * sceneGraph_ =0;
int currentfpCtrl =0;
int currentCamera_ =0, currentClipCamera =0;
camera2* pCurrentCamera =0, *pCurrentClipCamera_=0; 
Clock timer_;
int currentAdapLuma;// =0;
#define NUM_SAMPLES 8
GLFWwindow *output;
double mouseX, mouseY; 
GLuint Texture;
void init(int argc, char** argv);
void initWindow(int argc, char** argv);
void cleanup();
//bool loadModel(const char* filename, Model& model);
void processUI();
void resizeMultiSampledFramebuffer();

mesh select2,sel;

mesh* selected2;
//cmtmesh** dragon2;
//camera options for pitch and yaw

float camYaw = 0.0f, camPitch = 0.0f;
float ssaoRadius = 3.0f, ssaoPower = 3.0f;
void renderScene();
GLuint g_ss_quad_vao =0;
int g_ss_quad_point_count;
GLuint skyBoxVBO;
glm::mat4 prevProjection;
glm::mat4 Projection;
glm::mat4 View;
GLuint skyVBO, skyIPO;

tex3D *tex3d =NULL;
tex3D *cobble = NULL;
tex3D *jagnow = NULL;
tex3D *rgranCool = NULL;


tdogl::Program *gTest2 = NULL, *gTest3= NULL;
camera2 *cam;

tex2D *specles, *diffusely, *detaily, *normally;
float setAuxBufferSize = 1.0f;
float auxBufferSize = 1.0f;
//static void mouse_scroll(GLFWwindow* window, double something, double pos)
static void mouseButtonHandler(GLFWwindow *window,int button, int action ,int mods);
static void mousePosHandler(GLFWwindow * window,double x, double y);
static void error_callback(int erro, const char* desc);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z);
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z);
meshModel  mysty;

meshModel *selection;
float setSSaoKernelSize = 16.0f;
int ssaoKernelSize = 16;
float setSSaoNoiseSize = 4.0f;
int ssaoNoiseSize = 4;



//Shaders 
tdogl::Program *gSkybox = NULL;
tdogl::Program *gShader = NULL;
tdogl::Program *gShader2 = NULL;
tdogl::Program *gShader3 = NULL;
tdogl::Program *gLighting =NULL;
tdogl::Program *gSSAO=NULL;
tdogl::Program *gblurSSAO=NULL;
tdogl::Program *shaderPostProcess=NULL;
tdogl::Program *shaderLuminance=NULL;
tdogl::Program *shaderAvgminMax=NULL;
tdogl::Program *shaderLuminanceAdpat=NULL;

tex2D * texSSAONoise, * texDepth, * texHdr , *texDiffuse, *texSpec, * texGeometric, *texAuxA, *texLuma, *texAdaptLuma[2];
vector<tdogl::Program*> blurShaders;
vector<tdogl::Program*> materialShaders;


GLuint depthTex, fb;
//camera details for model matrix
float cameraYaw =0.0f, cameraPitch = 0.0f, cameraZoom =15.0f;

bool leftMouseButtonPressed = false;
bool rightMouseButtonPressed = false;

int windowWidth = 640;
int windowHeight = 480;
void initFb();
void initQuad();

const char* hdrTex[4] = {"textures/grove.env.hdr",
	"textures/pisa.env.hdr",
	"textures/overcast.env.hdr",
	"textures/stpeters.env.hdr"};

texCu * hdrs[4];

void setCurrentCamera(camera2* cam){
	pCurrentCamera = cam;
}
camera2 * getCurrentCamera(){
	return pCurrentCamera;
}
void setCurrentClipCamera(camera2* cam){
	pCurrentClipCamera_ = cam;
}
camera2 * getCurrentClipCamera(){
	return pCurrentClipCamera_;
}

Node2* addNode(Node2* node){
	//add node to scenegraph
	assert(sceneGraph_);
	assert(node);
	sceneGraph_->addChild(node);
	return node;
}
camera2* addCamera(camera2 *camera, Node2* parent){
	//if parent is 0 create a new parent
	if(parent==0){
		parent = addNode(Node2::create());
	}
	//add the modifier
	parent->addModifier(camera);
	cameras_.push_back(camera);
	return camera;
}
Node2* addFirstPersonNode(firstPerson *fpCtrl, Node2 *parent = 0);

Node2 *addFirstPersonNode(firstPerson *fpCtrl, Node2 *parent){
	assert(fpCtrl);
	fpsCtrl.push_back(fpCtrl);
	Node2 * result = addNode(Node2::create(parent, fpCtrl->getName().c_str()));
	result->addModifier(fpCtrl);
	return result;

}

#endif