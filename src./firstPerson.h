#ifndef FIRSTPERSON_H
#define FIRSTPERSON_H

#include "opengl.h"
#include "iNodeIdentifier.h"
//First person controller for camera
class firstPerson: public iNodeModifier{
public:
	//constructor
	static firstPerson* create(glm::vec3 &position = glm::vec3(), const glm::quat & rotation = glm::quat(), const string & name = "firstPerson");
	//get and set position
	const glm::vec3 &getPosition() const;
	void setPosition(const glm::vec3&);
	void offsetPosition(const glm::vec3 & offset);
	//get rotation and setrotation
	const glm::quat& getRotation() const;
	void setRotation(const glm::quat&);
	//apply directionsto the camera
	void pitch(float x);
	void yaw(float y);
	void roll(float z);
	//get the vector for the camera rotation transformation
	const glm::vec3& getPitchYawRoll() const;
	//set the vecotor for camera rotation
	void setPitchYawRoll(const glm::vec3&);

	//velocity and motion blur 
	void forward(float s);
	void backward(float s);
	void left(float s);
	void right(float s);

	void up(float s);
	//set zoom
	void setZoom(float s,float dt);
	//look at 
	void offsetOrientation(float upAngle, float rightAngle);
	void normaliseAngles();
	virtual void update(float dt); //update position, rotation and tranformation
protected:
	glm::vec3 position_, velocity_; //members
	float maxVelocitySpeed_;//max speed
	float velocityDamp_; //dampener for speed per frame

	glm::quat rotation_;
	glm::vec3 pitchYawRoll; // pitch/yaw/ roll in radians
	float maxRotationSpeed_;
	float rotationDamp_;

	glm::mat4 localMatrix_; //represents the position/rotation for getting local axes
	//constructor
	firstPerson(Node2* parent, const glm::vec3& position, const glm::quat &rotation, const string &name);
	//set velocity 
	void setVelocity(const glm::vec3 &v);

	//mat4 orientation matrix
	glm::mat4 getOrientation() const;

	//float horizontal and vertical angles;
	float _hAngle, _vAngle;

};
static inline float raidenstoDegrees(float radians){
return radians*180.0f / (float)3.14159265359;
}
inline glm::mat4 firstPerson::getOrientation()const{
	return glm::rotate(glm::mat4(), _vAngle, glm::vec3(1,0,0)) * glm::rotate(glm::mat4(), _hAngle, glm::vec3(0,1,0)) ; 
}
inline const glm::vec3& firstPerson::getPosition() const{
	return position_;
}
inline void firstPerson::setPosition(const glm::vec3 &pos){
	position_ = pos;
}
inline 	void firstPerson::normaliseAngles(){
	_hAngle = fmodf(_hAngle, 360.0f);
	//fmodf can return neg values but with this all are positive
	if(_hAngle<0.0f){
		_hAngle+=360.0f;
	}

	if(_vAngle>85.0f)
		_vAngle = 85.0f;
	else if(_vAngle<-85.0f)
		_vAngle = -85.0f;
}
inline void firstPerson::offsetPosition(const glm::vec3 &pos){
	position_ -= pos;
}
inline void firstPerson::offsetOrientation(float upAngle, float rightAngle){
	_hAngle = rightAngle;
	_vAngle = upAngle;
	normaliseAngles();
}
inline void firstPerson::setZoom(float pos,float dt){
	position_.z = pos;
}
inline const glm::quat& firstPerson::getRotation() const {
	return rotation_;
}
inline void firstPerson::setRotation(const glm::quat& rot){
	rotation_ = rot;
}

inline void firstPerson::pitch(float x) {
	pitchYawRoll.x +=x;
}

inline void firstPerson::yaw(float y) {
	pitchYawRoll.y +=y;
}

inline void firstPerson::roll(float z) {
	pitchYawRoll.z +=z;
}
inline const glm::vec3 & firstPerson::getPitchYawRoll() const{
	return pitchYawRoll;
}
inline void firstPerson::setPitchYawRoll(const glm::vec3 &pyr){
	pitchYawRoll = pyr;
}

inline void firstPerson::forward(float s){
	velocity_ += glm::vec3(glm::column(localMatrix_,2)) * s;
}
inline void firstPerson::backward(float s){
	forward(-s);
}
inline void firstPerson::left(float s){
	right(-s);
}
inline void firstPerson::right(float s){

	velocity_ += glm::vec3(glm::column(localMatrix_,0)) * s;
}
inline void firstPerson::up(float s){
	glm::vec3 _up = glm::vec3(glm::column(localMatrix_,1));
	if(_up.y<0){
		velocity_.y -=(_up.y)*s;
	}
	else{	
	velocity_.y +=_up.y*s;
	}
}

inline firstPerson::firstPerson(Node2 *parent, const glm::vec3 &position, const glm::quat &rotation, const string &name): iNodeModifier(parent, (iNodeModifier::callBackT)0 , name), 
	position_(position), rotation_(rotation), velocity_(glm::vec3(0.0f)), maxVelocitySpeed_(100.0f), velocityDamp_(0.001f), pitchYawRoll(glm::vec3(0.0f)), maxRotationSpeed_(10.0f), rotationDamp_(0.000005f), localMatrix_(glm::mat4()), _hAngle(0.0f), _vAngle(0.0){

}
#endif