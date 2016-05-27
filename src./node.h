#ifndef NODE_H
#define NODE_H

#include "opengl.h"
#include "iNodeIdentifier.h"
#include "iResource.h"
class iNodeModifier;
//Base class for node
class Node2: public iResource<Node2>{
public:
	//constructors
	static Node2* create(const string &name ="");
	static Node2* create(Node2* parent, const string &name = "");

	//destroy  ppnode, If destroy children is true, all children are recurseively destroyed. 
	//else they are reparented 
	static void destroy(Node2 **ppNode, bool destroyChildren = true);
	//get and set local matrix
	const glm::mat4& getLocalMatrix()const ;
	void setLocalMatrix(const glm::mat4&);

	// multiply input into local matriux
	void mulLocalMatrix(const glm::mat4 &);
	//get and set worldMatrix
	const glm::mat4& getWorldMatrix() const;
	void setWorldMatrix(const glm::mat4& );
	//get prev local and world matrix
	const glm::mat4 & getPreviousLocalMatrix() const;
	const glm::mat4 & getPreviousWorldMatrix() const;
	//get and set parent
	Node2 * getParent() const;
	void setParent(Node2*);

	Node2* getChild(int i ) const;
	int getChildren() const;

	iNodeModifier* getModifier(int i) const;
	int getModifiers() const;

	//Add child to list, remove child from parent if applicable
	void addChild(Node2* child);
	//remove child from the list
	void removeChild(Node2* child);
	//add or remove to modifier list
	void addModifier(iNodeModifier* modifier);
	void removeModifier(iNodeModifier* modifier);
	//update all modifiers in order and then builds world matrix
	void update(float dt);

	void print(int tabin =0);
private:
	struct nodeMatrices{
		glm::mat4 local, world;
	};
	nodeMatrices current_;
	nodeMatrices previous_;

	Node2* parent_;
	vector<Node2*> children_;
	vector<iNodeModifier*> modifiers_;
	Node2(Node2* parent, const string &name);
	~Node2();
};

inline const glm::mat4& Node2::getLocalMatrix()const{
	return current_.local;
}

inline void Node2::setLocalMatrix(const glm::mat4 &m){
	current_.local = m;
}
inline void Node2::mulLocalMatrix(const glm::mat4 &m){
	current_.local *=m;
}
inline const glm::mat4& Node2::getWorldMatrix() const{
	return current_.world;
}

inline void Node2::setWorldMatrix(const glm::mat4 &m){
	current_.world = m;
}

inline const glm::mat4& Node2::getPreviousWorldMatrix() const{
	return previous_.world;
}

inline Node2* Node2::getParent() const{
	return parent_;
}
inline void Node2::setParent( Node2* parent){
	parent_ = parent;
}
inline Node2* Node2::getChild( int i) const{
	assert(i<getChildren());
	return children_[i];
}
inline int Node2::getChildren()const{
	return children_.size();
}

inline iNodeModifier* Node2::getModifier(int i) const{
	assert(i<getModifiers());
	return modifiers_[i];
}
inline int Node2::getModifiers() const{
	return modifiers_.size();
}

inline Node2::Node2(Node2* parent, const string &name): iResource(name), parent_(parent){
	if(parent_)
		parent_->addChild(this);
	current_.world = current_.local = glm::mat4();
}
 inline Node2::~Node2(){

 }



#endif