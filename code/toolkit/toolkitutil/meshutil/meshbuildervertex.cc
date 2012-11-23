//------------------------------------------------------------------------------
//  meshbuildervertex.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/meshutil/meshbuildervertex.h"

namespace ToolkitUtil
{
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
MeshBuilderVertex::MeshBuilderVertex() :
    compMask(0),
    flagMask(0)
{
    IndexT i;
    for (i = 0; i < NumComponents; i++)
    {
        this->comps[i].set(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBuilderVertex::SetComponent(ComponentIndex compIndex, const float4& value)
{
    n_assert((compIndex >= 0) && (compIndex < NumComponents));
    this->comps[compIndex] = value;
    this->compMask |= (1<<compIndex);
}

//------------------------------------------------------------------------------
/**
*/
const float4&
MeshBuilderVertex::GetComponent(ComponentIndex compIndex) const
{
    n_assert(0 != (this->compMask & (1<<compIndex)));
    return this->comps[compIndex];
}

//------------------------------------------------------------------------------
/**
    Returns +1 if this vertex is greater then other vertex, -1 if 
    this vertex is less then other vertex, 0 if vertices
    are equal.
*/
int
MeshBuilderVertex::Compare(const MeshBuilderVertex& rhs) const
{
    IndexT i;
    for (i = 0; i < NumComponents; i++)
    {
        ComponentMask mask = (1<<i);
        if ((this->compMask & mask) && (rhs.compMask & mask))
        {
            const float4& f0 = this->comps[i];
            const float4& f1 = rhs.comps[i];
            if (float4::greater4_any(f0, f1))
            {
                return 1;
            }
            else if (float4::less4_any(f0, f1))
            {
                return -1;
            }
        }
    }
    // fallthrough: all components equal
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
MeshBuilderVertex::operator==(const MeshBuilderVertex& rhs) const
{
    return (0 == this->Compare(rhs));
}

//------------------------------------------------------------------------------
/**
*/
bool
MeshBuilderVertex::operator!=(const MeshBuilderVertex& rhs) const
{
    return (0 != this->Compare(rhs));
}

//------------------------------------------------------------------------------
/**
*/
bool
MeshBuilderVertex::operator<(const MeshBuilderVertex& rhs) const
{
    return (-1 == this->Compare(rhs));
}

//------------------------------------------------------------------------------
/**
*/
bool
MeshBuilderVertex::operator>(const MeshBuilderVertex& rhs) const
{
    return (+1 == this->Compare(rhs));
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBuilderVertex::Transform(const matrix44& m)
{
    if (this->compMask & CoordBit)
    {
        this->comps[CoordIndex] = float4::transform(this->comps[CoordIndex], m);
    }
    if (this->compMask & NormalBit)
    {
        this->comps[NormalIndex] = float4::transform(this->comps[NormalIndex], m);
    }
    if (this->compMask & TangentBit)
    {
        this->comps[TangentIndex] = float4::transform(this->comps[TangentIndex], m);
    }
    if (this->compMask & BinormalBit)
    {
        this->comps[BinormalIndex] = float4::transform(this->comps[BinormalIndex], m);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBuilderVertex::InitComponents(ComponentMask mask)
{
    this->compMask |= mask;
    IndexT i;
    for (i = 0; i < NumComponents; i++)
    {
        if (mask & (1<<i))
        {
            switch (i)
            {
                case CoordIndex:    
                    this->comps[i].set(0.0f, 0.0f, 0.0f, 1.0f); 
                    break;

                case NormalIndex:
                case TangentIndex:
                case BinormalIndex:
                    this->comps[i].set(0.0f, 1.0f, 0.0f, 0.0f);
                    break;

                case Uv0Index:
                case Uv1Index:
                case Uv2Index:
                case Uv3Index:
                case WeightsIndex:
                case JIndicesIndex:
                    this->comps[i].set(0.0f, 0.0f, 0.0f, 0.0f);
                    break;

                case ColorIndex:
                    this->comps[i].set(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
            }
        }
    }
}

} // namespace ToolkitUtil