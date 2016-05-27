#include "camera2.h"

void camera2::update(float dt){
	//pervious matrices = current matrices
	previous_ = current_;

	//ifthe node has a parent
	//get the inverse view matrix
	if(getParent()){
		current_.view = glm::inverse(getParent()->getLocalMatrix());

	}
	//build the projection and view matrix
	buildProjectionMatrix();
	current_.viewProjection = current_.projection * current_.view;
}
//want to rebuild the camera class to use GLM perspective function without errors and use quaternions for rotation, pitch and roll
void camera2::buildProjectionMatrix(){
	//Perspective Matrix!
	//Wished to use the  glm::perspective(fov, aspect, near, far) but
	//getting to many artefacts that will hopefully be changed as I go along! 
	float d = 1.0/ tanHalfFov_;
		float npf = nearPlane_ + farPlane_;
		float nmf = nearPlane_ - farPlane_; 
		float nf = nearPlane_ * farPlane_;

		current_.projection  =glm::mat4(
			d / aspect_,		0.0,			0.0,							0.0,
			0.0,					d,				0.0,							0.0,
			0.0,					0.0,			-npf / nmf, 				1.0f,
			0.0,					0.0,			2.0f * nf / nmf,							0.0);

}

