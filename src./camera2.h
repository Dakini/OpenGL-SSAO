#ifndef CAMERA2_H
#define CAMERA2_H

#include "opengl.h"
#include "iNodeIdentifier.h"

//camera class
class camera2: public iNodeModifier{
public:
	//create thecamera
static camera2 *create(
		float aspect = 4.0f/3.0f,
		float fov = glm::radians(45.0f),
		float nearClip = 0.1f,
		float farClip = 1000.0f,
		bool isOrtho = false,
		const string name ="camera"
		);
	//get world Matrix
	const glm::mat4 & getWorldMatrix() const;
	//get viewMatrix
	const glm::mat4 & getViewMatrix() const;
	//set viewMatrix
	void setViewMatrix(glm::mat4);
	//get and set projection matrix
	const glm::mat4 & getProjectionMatrix() const;
	void setProjectionMatrix(glm::mat4);
	const glm::mat4 & getViewProjectionMatrix() const;

	//get previous matrices
	const glm::mat4 & getPreviousViewProjectionMatrix() const;
	const glm::mat4 & getPreviousProjectionMatrix() const;
	const glm::mat4 & getPreviousViewMatrix() const;
	//get and set aspect ratio and FOV
		float getAspectRatio() const;
		void setAspectRatio(float ratio);
		float getFOV() const;
		void setFOV(float);

		//get half tan FOV and near and far plane
		float getTanHalfFOV()const ; 

		float getNearPlane() const;
		float getFarPlane() const;

		void setNearPlane(float);
		void setFarPlane(float);

		//is it orthographic
		bool isOrthographic() const;
		void setOrthograthic(bool);

	
		//update
		virtual void update(float dt);
		float getHalfTan() const;
protected:
	//structure to hold the matrices
	struct cameraMatrices{
		glm::mat4 view, projection, viewProjection;
	};


	cameraMatrices current_;

	cameraMatrices previous_;
	//members
	float aspect_, fov_, tanHalfFov_, nearPlane_, farPlane_;
	bool isOrtho_;
	const string name_;
	glm::vec3 position_;
	glm::mat4 orientation_;
	float hAngle, vAngle;
	//constructor
	camera2(Node2 * Parent, float aspect, float fov, float nearPlane, float farPlane, bool isOrtho, const string name);
	//build projection matrix
	void buildProjectionMatrix();
};

inline camera2::camera2(Node2 * parent, float aspect, float fov, float nearPlane, float farPlane, bool isOrtho, const string name):iNodeModifier(parent, 0,name), aspect_(aspect), fov_(fov), nearPlane_(nearPlane), farPlane_(farPlane), isOrtho_(isOrtho), name_(name){
	//make all matrices identity matrices
	current_.view = current_.projection = current_.viewProjection = glm::mat4();
	//get the half tan FOV
	tanHalfFov_ = tan(fov_/2.0f);
	//build the projection matrix
	buildProjectionMatrix();
}

inline const glm::mat4 & camera2:: getWorldMatrix() const{
	return getParent()->getWorldMatrix();
}

inline camera2* camera2::create(float aspect ,float fov ,float nearClip,float farClip,bool isOrth,const string name){
	camera2* result = new(std::nothrow) camera2(0,aspect, fov, nearClip, farClip, isOrth, name);
	assert(result);
	return result;
}
inline float camera2::getHalfTan() const{
 return tanHalfFov_;
}

inline const glm::mat4& camera2::getViewMatrix() const{
	return current_.view;
}

inline const glm::mat4& camera2::getViewProjectionMatrix() const{
	return current_.viewProjection;
}

inline const glm::mat4& camera2::getProjectionMatrix() const{
	return current_.projection;
}
inline void camera2::setViewMatrix( glm::mat4 mat){
	current_.view = mat;
	current_.viewProjection = current_.projection * current_.view;
}


inline const glm::mat4& camera2::getPreviousViewMatrix() const{
	return previous_.view;
}

inline const glm::mat4& camera2::getPreviousViewProjectionMatrix() const{
	return previous_.viewProjection;
}

inline const glm::mat4& camera2::getPreviousProjectionMatrix() const{
	return previous_.projection;
}

inline float camera2::getAspectRatio() const{
	return aspect_;

}
inline void camera2::setAspectRatio(float ratio){
	aspect_ = ratio;
}
inline float camera2::getFOV() const{
	return fov_;
}
inline void camera2::setFOV(float fov){
	fov_ = fov;
}
inline float camera2::getTanHalfFOV() const {
	return tanHalfFov_;
}

/*----------------------------------------------------------------------------*/
inline float camera2::getNearPlane() const {
	return nearPlane_;
}
/*----------------------------------------------------------------------------*/
inline void camera2::setNearPlane(float clipNear) {
	nearPlane_ = clipNear;
}

/*----------------------------------------------------------------------------*/
inline float camera2::getFarPlane() const {
	return farPlane_;
}
/*----------------------------------------------------------------------------*/
inline void camera2::setFarPlane(float clipFar) {
	farPlane_ = clipFar;
}

/*----------------------------------------------------------------------------*/
inline bool camera2::isOrthographic() const {
	return isOrtho_;
}
/*----------------------------------------------------------------------------*/
inline void camera2::setOrthograthic(bool isOrthographic) {
	isOrtho_ = isOrthographic;
}
#endif