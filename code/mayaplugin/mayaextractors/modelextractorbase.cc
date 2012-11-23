//------------------------------------------------------------------------------
//  mayaextractorbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayaextractors/modelextractor.h"

namespace Models
{
	__ImplementClass(Models::ModelExtractorBase, 'MEXB', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	ModelExtractorBase::ModelExtractorBase() :
	isValid(false),isExportAll(false)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ModelExtractorBase::~ModelExtractorBase()
	{
		n_assert(!this->IsValid());
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelExtractorBase::Setup()
	{
		n_assert(!this->IsValid());
		this->isValid = true;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelExtractorBase::Discard()
	{
		n_assert(this->IsValid());
		this->isValid = false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ModelExtractorBase::Result
	ModelExtractorBase::Extract(const Ptr<Model>& model)
	{
		return NothingTodo;
	}

} // namespace Maya