#include "iResource.h"
#include "iTexture.h"
#include "material.h"
#include "iNodeIdentifier.h"
#include "node.h"


template <class C>
//find an instance within the instance list
C* iResource<C>::find(const string &name){
	for(size_t i =0; i<instances_.size(); i++)
		if(instances_[i]->name_.compare(name)==0)
			return instances_[i];
	return 0;
}

template <class C>
void iResource<C>::destroyAll(){
	//call destroy on front instances
	///keep doing this while its not empty
	while(!instances_.empty()){
		//copy ptr to prevent destory from nullifying the 0 entry in the instances list
		//which would be the wrong entry
		C * pInstance = instances_[0];
		C::destroy(&pInstance);
	}
}
//add to instance list
template <class C>
iResource<C>::iResource(const string &name): name_(name){
	instances_.push_back(static_cast<C*>(this));
}
//loop through instances list and erase the instance
template <class C>
iResource<C>::~iResource(){
	typename vector<C*>::iterator it;
	it = std::find(instances_.begin(), instances_.end(), static_cast<C*>(this));
	instances_.erase(it);
	assert(it!=instances_.end());
}
//return instance at[i]
template <class C>
C& iResource<C>::instance(size_t i){
	assert(!instances_.empty());
	assert(i< instances_.size());
	return *instances_[i];
}
template <class C>
size_t iResource<C>::nInstances(){
	return instances_.size();
}
template <class C>
vector<C*> iResource<C>::instances_;

//explicit template instantiation

template class iResource<iTexture>;
template class iResource<iNodeModifier>;
template class iResource<Node2>;
template class iResource<Material>;


