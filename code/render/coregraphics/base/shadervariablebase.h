#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::ShaderVariableBase
    
    Provides direct access to a shader's global variable.
    The fastest way to change the value of a shader variable is to
    obtain a pointer to a shader variable once, and use it repeatedly
    to set new values.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"

namespace CoreGraphics
{
    class Texture;
    class ShaderInstance;
    class ShaderVariableInstance;
}

//------------------------------------------------------------------------------
namespace Base
{
class ShaderVariableBase : public Core::RefCounted
{
    __DeclareClass(ShaderVariableBase);
public:
    /// shader variable types
    enum Type
    {
        UnknownType,
        IntType,
        FloatType,
        VectorType,
        MatrixType,
        BoolType,
        TextureType,
    };

    /// shader variable name typedef
    typedef Util::StringAtom Name;
    /// shader variable semantic typedef
    typedef Util::StringAtom Semantic;

    /// constructor
    ShaderVariableBase();
    /// destructor
    virtual ~ShaderVariableBase();

    /// create a shader variable instance
    Ptr<CoreGraphics::ShaderVariableInstance> CreateInstance();
    /// get the data type of the variable
    Type GetType() const;
    /// get the name of the variable
    const Name& GetName() const;
    /// get the semantics of the variable
    const Semantic& GetSemantic() const;

    /// convert type to string
    static Util::String TypeToString(Type t);
    /// convert string to type
    static Type StringToType(const Util::String& str);

    /// set int value
    void SetInt(int value);
    /// set int array values
    void SetIntArray(const int* values, SizeT count);

    /// set float value
    void SetFloat(float value);
    /// set float array values
    void SetFloatArray(const float* values, SizeT count);

    /// set vector value
    void SetFloat4(const Math::float4& value);
    /// set vector array values
    void SetFloat4Array(const Math::float4* values, SizeT count);
    
    /// set matrix value
    void SetMatrix(const Math::matrix44& value);
    /// set matrix array values
    void SetMatrixArray(const Math::matrix44* values, SizeT count);
    
    /// set bool value
    void SetBool(bool value);
    /// set bool array values
    void SetBoolArray(const bool* values, SizeT count);

    /// set texture value
    void SetTexture(const Ptr<CoreGraphics::Texture>& value);

protected:
    /// set variable type
    void SetType(Type t);
    /// set variable name
    void SetName(const Name& n);
    /// set variable semantic
    void SetSemantic(const Semantic& s);

    Type type;
    Name name;
    Semantic semantic;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableBase::SetType(Type t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline ShaderVariableBase::Type
ShaderVariableBase::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableBase::SetName(const Name& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const ShaderVariableBase::Name&
ShaderVariableBase::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableBase::SetSemantic(const Semantic& s)
{
    this->semantic = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const ShaderVariableBase::Semantic&
ShaderVariableBase::GetSemantic() const
{
    return this->semantic;
}

} // Base
//------------------------------------------------------------------------------
 