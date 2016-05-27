#ifndef NODE_IDENTIFIER_H
#define NODE_IDENTIFIER_H

#include "opengl.h"
#include "iResource.h"
#include "node.h"

class Node2;
	//base class for node modifiers, these update a local marix during scene update. A modifier may only be associated with a single node
class iNodeModifier: public iResource<iNodeModifier>{
	public:
		typedef void(*callBackT)(iNodeModifier&);
		//destroy and remove node form parent
		static void destroy(iNodeModifier **ppNode);
		//get access to node on complete
		callBackT getOncomplete() const;
		void setOnComplete( callBackT);
		//get and set parent
		Node2* getParent() const;
		void setParent(Node2* m);
		//updates parents node local matrix.
		virtual void update(float dt) = 0;
		virtual void reset(); //reset node

		static  void reset(iNodeModifier&); //static to use as a callback
		virtual void relativeReset();  //reset node relative to its current state
		static void relativeReset(iNodeModifier &); //to be used as call back

		virtual void reverse(); //reverse node
		static void reverse(iNodeModifier&); //use as a call back

		static void remove(iNodeModifier&); //remove and destroy from parent
	protected:
		callBackT onComplete_;
		//constructor
		iNodeModifier(Node2* parent, callBackT onComplete, const string &name);
		// virtual destroctor
		virtual ~iNodeModifier();
	private:
		Node2 * parent_; //parent node

};


	inline void iNodeModifier::destroy(iNodeModifier **ppNode){
		assert(ppNode !=0); 
		assert(*ppNode !=0);
		
		if((*ppNode)->parent_)
			cout<<"woow"<<endl;
			//(*ppNode)->parent_->removeModifier(*ppNode);
		delete *ppNode;
		*ppNode =0;
	}
	inline iNodeModifier::callBackT iNodeModifier::getOncomplete() const{
		return onComplete_;
	}
	inline void iNodeModifier::setOnComplete(callBackT callback){
		onComplete_ = callback;
	}
	inline Node2* iNodeModifier::getParent() const{
		return parent_;
	}
	inline void iNodeModifier::setParent(Node2* p){
		parent_ = p;
	}
	inline void iNodeModifier::reset(){
	}

	inline void iNodeModifier::reset(iNodeModifier &mdf){
		mdf.reset();
	}
	inline void iNodeModifier::relativeReset(){
	}
	inline void iNodeModifier::relativeReset(iNodeModifier &mdf){
		mdf.relativeReset();
	}
	inline void iNodeModifier::reverse(){
	}
	inline void iNodeModifier::reverse(iNodeModifier &mdf){
		mdf.reverse();
	}
	inline void iNodeModifier::remove(iNodeModifier &mdf){
		iNodeModifier *pMDF = &mdf;
		iNodeModifier::destroy(&pMDF);
	}

	inline iNodeModifier::iNodeModifier(Node2 * parent, callBackT onComplete, const string &name): iResource(name), onComplete_(onComplete), parent_(parent){
	}
	inline iNodeModifier::~iNodeModifier(){}




#endif