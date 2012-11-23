//------------------------------------------------------------------------------
//  shadervariablebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shadervariableinstance.h"
#include "coregraphics/shader.h"

namespace Base
{
__ImplementClass(Base::ShaderVariableBase, 'SVRB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Math;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ShaderVariableBase::ShaderVariableBase() :
    type(UnknownType)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderVariableBase::~ShaderVariableBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderVariableInstance>
ShaderVariableBase::CreateInstance() 
{
    Ptr<ShaderVariableInstance> newInst = ShaderVariableInstance::Create();
    Ptr<ShaderVariableBase> thisPtr(this);
    newInst->Setup(thisPtr.downcast<ShaderVariable>());
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
String
ShaderVariableBase::TypeToString(Type t)
{
    switch (t)
    {
        case UnknownType:   return "unknown";
        case IntType:       return "int";
        case FloatType:     return "float";
        case VectorType:    return "vector";
        case MatrixType:    return "matrix";
        case BoolType:      return "bool";
        case TextureType:   return "texture";
        default:
            n_error("ShaderVariableBase::TypeToString(): invalid type code!");
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
ShaderVariableBase::Type
ShaderVariableBase::StringToType(const String& str)
{
    if (str == "int") return IntType;
    else if (str == "float") return FloatType;
    else if (str == "vector") return VectorType;
    else if (str == "matrix") return MatrixType;
    else if (str == "bool") return BoolType;
    else if (str == "texture") return TextureType;
    else
    {
        n_error("ShaderVariable::StringToType(): invalid string '%s'!\n", str.AsCharPtr());
        return UnknownType;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetInt(int value)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetIntArray(const int* values, SizeT count)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetFloat(float value)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetFloatArray(const float* values, SizeT count)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetFloat4(const float4& value)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetFloat4Array(const float4* values, SizeT count)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetMatrix(const matrix44& value)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetMatrixArray(const matrix44* values, SizeT count)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetBool(bool value)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetBoolArray(const bool* values, SizeT count)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableBase::SetTexture(const Ptr<Texture>& value)
{
    // empty, override in subclass
}

} // namespace Base
