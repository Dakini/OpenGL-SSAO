#include "node.h"

//constructors
Node2 * Node2::create(const string&name){
	Node2 * result = new(std::nothrow)Node2(0,name);
	assert(result);
	return result;
}
Node2* Node2::create(Node2* parent, const string&name){
	Node2* result= new(std::nothrow) Node2(parent, name);
	assert(result);
	return result;
}

void Node2::update(float dt){
	previous_ = current_;

	current_.local = glm::mat4();
	for(int i =0; i<getModifiers(); i++){
		getModifier(i)->update(dt);
	}
	//build world matrix
	if(parent_)
		current_.world = parent_->getWorldMatrix() * current_.local;
	else 
		current_.world = current_.local;

	//update children
	for(int i =0; i <getChildren(); i++){
		getChild(i)->update(dt);
	}
}
void Node2::destroy(Node2** ppNode, bool destoyChildren){
	assert(ppNode !=0);
	assert(*ppNode!=0); 
	//destory/reparent children
	if(destoyChildren){
		for(int i =0; i<(*ppNode)->getChildren(); i++){
			Node2* pChild = (*ppNode)->getChild(i);
			destroy(&pChild,true);
		}
	}else{
		for(int i =0; i<(*ppNode)->getChildren(); i++){
			(*ppNode)->getChild(i)->setParent((*ppNode)->getParent());

		}
	}

	//destory modifiers
	for(int i =0; i<(*ppNode)->getModifiers(); i++){
		iNodeModifier* pModifier = (*ppNode)->getModifier(i);
		pModifier->setParent(0); 
		iNodeModifier::destroy(&pModifier);
	}
	//delete node
	delete *ppNode;
	*ppNode = 0;
}

void Node2::addChild(Node2* child){
	assert(child!=0);
	if(child->getParent())
		child->getParent()->removeChild(child);
	child->setParent(this);
	children_.push_back(child);
}

void Node2::removeChild(Node2* child){
	assert(child!=0);
	for(size_t i = 0; i <children_.size(); i++){
		if(children_[i]==child){
			children_.erase(children_.begin() +i);
			break;
		}
	}
	child->setParent(0);
}

void Node2::addModifier(iNodeModifier* mod){
	assert(mod!=0);
	mod->setParent(this);
	modifiers_.push_back(mod);
}
void Node2::removeModifier(iNodeModifier* mod){
	assert(mod!=0);
	for(size_t i = 0; i <modifiers_.size(); i++){
		if(modifiers_[i]==mod){
			modifiers_.erase(modifiers_.begin() +i);
			mod->setParent(0);
			break;
		}
	}
}
