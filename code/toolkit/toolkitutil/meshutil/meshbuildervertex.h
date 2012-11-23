#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::MeshBuilderVertex
    
    Contains per-vertex data in a mesh builder object.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"
#include "math/float4.h"
#include "math/matrix44.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class MeshBuilderVertex
{
public:
    /// vertex component indices
    enum ComponentIndex
    {
        CoordIndex		= 0,
        NormalIndex		= 1,
        Uv0Index		= 3,
        Uv1Index		= 5,
        Uv2Index		= 7,
        Uv3Index		= 9,
        ColorIndex		= 11,
		TangentIndex	= 13,
		BinormalIndex	= 15,
        WeightsIndex	= 17,
        JIndicesIndex	= 19,

        NumComponents	= 21,
        InvalidComponentIndex,
    };

    /// vertex component bits
    enum ComponentBit
    {
        CoordBit = (1<<CoordIndex),
        NormalBit = (1<<NormalIndex),
        TangentBit = (1<<TangentIndex),
        BinormalBit = (1<<BinormalIndex),
        Uv0Bit = (1<<Uv0Index),
        Uv1Bit = (1<<Uv1Index),
        Uv2Bit = (1<<Uv2Index),
        Uv3Bit = (1<<Uv3Index),
        ColorBit = (1<<ColorIndex),
        WeightsBit = (1<<WeightsIndex),
        JIndicesBit = (1<<JIndicesIndex),
    };
    /// a mask of ComponentBits
    typedef uint ComponentMask;

    /// vertex flags
    enum Flag
    {
        Redundant = (1<<0),
    };
    /// a mask of Flags
    typedef uint FlagMask;

    /// constructor
    MeshBuilderVertex();

    /// equality operator
    bool operator==(const MeshBuilderVertex& rhs) const;
    /// inequality operator
    bool operator!=(const MeshBuilderVertex& rhs) const;
    /// less-then operator
    bool operator<(const MeshBuilderVertex& rhs) const;
    /// greather-then operator
    bool operator>(const MeshBuilderVertex& rhs) const;

    /// set a vertex component
    void SetComponent(ComponentIndex compIndex, const Math::float4& value);
    /// get a vertex component index
    const Math::float4& GetComponent(ComponentIndex compIndex) const;
    /// check if all components are valid
    bool HasComponents(ComponentMask compMask) const;
    /// set components to their default values
    void InitComponents(ComponentMask compMask);
    /// delete component
    void DeleteComponents(ComponentMask compMask);
    /// get component mask
    ComponentMask GetComponentMask() const;
	/// get vertex width
	int GetWidth() const;

    /// set vertex flag
    void SetFlag(Flag f);
    /// unset vertex flag
    void UnsetFlag(Flag f);
    /// check vertex flag
    bool CheckFlag(Flag f) const;

    /// compare vertex against other, return -1, 0 or +1
    int Compare(const MeshBuilderVertex& rhs) const;
    /// transform the vertex
    void Transform(const Math::matrix44& m);

	void SetOrigIndex(IndexT index){ this->origIndex = index;}

	IndexT GetOrigIndex() const { return this->origIndex;}

private:
    ComponentMask compMask;
    FlagMask flagMask;
    Math::float4 comps[NumComponents];
	IndexT origIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
MeshBuilderVertex::HasComponents(ComponentMask mask) const
{
    return (mask == (this->compMask & mask));
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBuilderVertex::DeleteComponents(ComponentMask mask)
{
    this->compMask &= ~mask;
}

//------------------------------------------------------------------------------
/**
*/
inline MeshBuilderVertex::ComponentMask
MeshBuilderVertex::GetComponentMask() const
{
    return this->compMask;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBuilderVertex::SetFlag(Flag f)
{
    this->flagMask |= f;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MeshBuilderVertex::UnsetFlag(Flag f)
{
    this->flagMask &= ~f;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
MeshBuilderVertex::CheckFlag(Flag f) const
{
    return (0 != (this->flagMask & f));
}

//------------------------------------------------------------------------------
/**
*/	 
inline int
MeshBuilderVertex::GetWidth() const
{
	int w = 0;
	if (this->HasComponents(CoordBit))      w += 3;
	if (this->HasComponents(NormalBit))     w += 3;		
	if (this->HasComponents(TangentBit))    w += 3;		
	if (this->HasComponents(BinormalBit))   w += 3;		
	if (this->HasComponents(ColorBit))      w += 4;		
	if (this->HasComponents(Uv0Bit))        w += 2;		
	if (this->HasComponents(Uv1Bit))        w += 2;		
	if (this->HasComponents(Uv2Bit))        w += 2;		
	if (this->HasComponents(Uv3Bit))        w += 2;		
	if (this->HasComponents(WeightsBit))    w += 4;		
	if (this->HasComponents(JIndicesBit))   w += 4;
	return w;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    