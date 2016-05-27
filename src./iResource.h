#ifndef IRESOURCE_H
#define IRESOURCE_H

#include "opengl.h"

template <class C>
//Base class for all resource types, manages of all resources
class iResource{
public:
	//get or set name 
	const string& getName() const; 
	void setName(const string &name);
	//search instances for the first matching name
	static C* find(const string&);
	//destoy all instances and erleasing resources
	static void destroyAll();

protected:
	string name_; //pre instance name
	//constructor and destructor
	iResource(const string &name="");
	~iResource();
	static C& instance(size_t i); //return nth instance from instance list
	static size_t nInstances(); //num of instances
private:
	static vector<C*>instances_; //instances list
	//explicit  contructors/destuctors
	explicit iResource(const iResource&); //
	iResource& operator= (const iResource&);
};

template <class C>
const string& iResource<C>::getName() const{
	return name_;
}
template <class C>
 void iResource<C>::setName(const string &name){
	name_ = name;
}


#endif