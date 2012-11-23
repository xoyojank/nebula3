//-----------------------------------------------------------------------------
//  nmaxdummy.h
//
//  (c)2004 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#ifndef N_MAXDUMMY_H
#define N_MAXDUMMY_H
//-----------------------------------------------------------------------------
/**
    @class nMaxDummy
    @ingroup NebulaMaxExport2Contrib

    @brief A class for a dummy max object.

*/
#include "export2/nmaxnode.h"
#include "util/nstring.h"
#include "tinyxml/tinyxml.h"

class nSceneNode;

//-----------------------------------------------------------------------------
class nMaxDummy : public nMaxNode
{
public:
    enum Type {
        Invalid  = 0,
        Lod      = 1,
        SkyNode  = 2,
        SkyState = 3,
    };

    /// Constructor.
    nMaxDummy();
    /// Destructor.
    virtual ~nMaxDummy();

    /// Export
    nSceneNode* Export(INode* inode);

    /// Retrieve 3dsmax node name.
    nString GetNodeName() const;

    /// Retrieve type.
    Type GetType() const;

    /// Retrieve element node name of the skystate.
    /// This is only valid the dummy node is skystate type.
    nString GetElemOfState() const;
    ///
    float GetSkyStateTime() const;

protected:
    /// Create scene node from the node's custom attributes.
    nSceneNode* CreateNodeFromCustAttrib(INode* inode);

    /// Export nLodNode.
    nSceneNode* ExportLODNode(INode *inode, TiXmlHandle &xmlHandle, const char* paramName);
    /// Export nSkyNode.
    nSceneNode* ExportSkyNode(INode *inode, TiXmlHandle &xmlHandle, const char* paramName);
    /// Export skystate
    nSceneNode* ExportSkyState(INode *inode, TiXmlHandle &xmlHandle, const char* paramName);

protected:
    /// 3dsmax node name.
    nString nodeName;

    /// Type of this dummy helper node.
    Type dummyType;

    /// This member is only valid when the dummy is skystate type.
    nString elemOfSkyState;
    /// sky state time retrived from the skyStateParam parameter block on MaxScript side.
    float skyStateTime;

};
//-----------------------------------------------------------------------------
inline
nString 
nMaxDummy::GetNodeName() const
{
    return this->nodeName;
}
//-----------------------------------------------------------------------------
inline
nMaxDummy::Type 
nMaxDummy::GetType() const
{
    return this->dummyType;
}
//-----------------------------------------------------------------------------
inline
nString 
nMaxDummy::GetElemOfState() const
{
    return this->elemOfSkyState;
}
//-----------------------------------------------------------------------------
inline
float 
nMaxDummy::GetSkyStateTime() const
{
    return this->skyStateTime;
}
//-----------------------------------------------------------------------------
#endif
