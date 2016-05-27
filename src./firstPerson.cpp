#include "firstPerson.h"

//constructor
firstPerson * firstPerson::create(glm::vec3 &position, const glm::quat & rotation , const string & name ){
	
	firstPerson *result = new(std::nothrow) firstPerson(0, position, rotation, name);
	assert(result);
	return result;
}


void firstPerson::update(float dt){
	//set the view matrix using rotation and a translate mat4
	getParent()->setLocalMatrix( getOrientation() * glm::translate(glm::mat4(), -position_));


}