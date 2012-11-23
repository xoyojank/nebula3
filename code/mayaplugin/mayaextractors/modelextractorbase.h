#pragma once
//------------------------------------------------------------------------------
/**
@class Maya::MayaExctractorBase

A MayaExtractor object is responsible to extract a specific type
of data from a Maya scene. Specialized subclasses extract transform
nodes, static geometry, skinned geometry, particle systems and
special data from the scene. Scene extractor will populate
the MayaScene singleton with a simplified, abstract version of
the actual Maya scene.

(C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "mayacore/model.h"

//------------------------------------------------------------------------------
namespace Models
{
	class ModelExtractorBase : public Core::RefCounted
	{
		__DeclareClass(ModelExtractorBase);
	public:
		/// result codes
		enum Result
		{
			Error = 0,
			Success,
			NothingTodo,
		};

		/// constructor
		ModelExtractorBase();
		/// destructor
		virtual ~ModelExtractorBase();

		/// setup the object
		virtual void Setup();
		/// discard the object
		virtual void Discard();
		/// return true if the object has been setup
		bool IsValid() const;
		/// set export all
		void SetExportAll(bool exportall);

		/// called to extract scene data into MayaScene object
		virtual Result Extract(const Ptr<Model>& model);

		/// get error string
		const Util::String& GetError() const;

	protected:    
		/// set error string
		void SetError(const Util::String& err);

		bool isValid;
		Util::String error;
		bool isExportAll;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline bool
	ModelExtractorBase::IsValid() const
	{
		return this->isValid;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	ModelExtractorBase::SetError(const Util::String& err)
	{
		this->error = err;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::String&
	ModelExtractorBase::GetError() const
	{
		return this->error;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline	void 
	ModelExtractorBase::SetExportAll(bool exportall)
	{
		isExportAll = exportall;
	}

} // namespace Maya
//------------------------------------------------------------------------------
