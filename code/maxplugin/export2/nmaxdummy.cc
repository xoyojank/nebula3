//-----------------------------------------------------------------------------
//  nmaxdummy.cc
//
//  (c)2004 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxdummy.h"
#include "export2/nmaxbones.h"
#include "export2/nmaxutil.h"
#include "export2/nmaxcustattrib.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "scene/ntransformnode.h"
#include "scene/nlodnode.h"
#include "scene/nskynode.h"
#include "scene/nattachmentnode.h"


//-----------------------------------------------------------------------------
/**
*/
nMaxDummy::nMaxDummy() : 
    dummyType(Invalid),
    skyStateTime(0.0f)
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxDummy::~nMaxDummy()
{
}

//-----------------------------------------------------------------------------
/**
    @note 
        Do not create more than one nSceneNode derived node from the given node.

    -27-Mar-07  kims  Changed to export all kind of dummy node as transform node
                      except which has biped control. Thank Cho Jun Heung for the patch.
*/
nSceneNode* nMaxDummy::Export(INode* inode)
{

    nSceneNode* createdNode = 0;

    this->nodeName = inode->GetName();

    createdNode = this->CreateNodeFromCustAttrib(inode);

    if (createdNode)
    {
        n_maxlog(Medium, "%s node has custom attributes.", inode->GetName());
    }
    else
    {
        // Any kind of dummy nodes should can be exproted because artists might use it.
        // But we do not export a dummy node which has biped control.
        if( nMaxBoneManager::IsDummy(inode) )
        {
            // the node is group owner node.
            // If this node is a group owner node(this is a dummy node in max), add 
            // a transform node, so we can handle the group complete later)
            nTransformNode* transformNode;
            transformNode = static_cast<nTransformNode*>(CreateNebulaObject("ntransformnode", 
                                                                            inode->GetName()));
            createdNode = transformNode;

            // if it needs attachment node in its hierarchy, it creates attachmentnode - createnode hierarchy.
            nMaxBoneManager::Instance()->BuildAttachmentNode(inode, createdNode, 0);
        }
    }
    
    return createdNode;
}

//-----------------------------------------------------------------------------
/**
    Creates Nebula node from the given node which has corresponding custom
    attributes for the node.
*/
nSceneNode*  
nMaxDummy::CreateNodeFromCustAttrib(INode* inode)
{
    TiXmlDocument xmlDoc;

    Object* obj = nMaxUtil::GetBaseObject(inode, 0);
    if (!obj)
    {
        n_maxlog(Medium, "The node %s has no object.", inode->GetName());
        return NULL;
    }

    // convert node custom attributes to xml data.
    nMaxCustAttrib custAttrib;
    if (!custAttrib.Convert(obj, xmlDoc))
    {
        n_maxlog(Medium, "The node %s has no custom attribute.", inode->GetName());
        return NULL;
    }

    TiXmlHandle xmlHandle(&xmlDoc);

    TiXmlElement* e = 0;
    nSceneNode* createdNode = 0;

    if (e = xmlHandle.FirstChild("LodParam").Element())
    {
        createdNode = ExportLODNode(inode, xmlHandle, "LodParam");

        dummyType = Lod;
    }
    else
    if (e = xmlHandle.FirstChild("SkyNodeParam").Element())
    {
        createdNode = ExportSkyNode(inode, xmlHandle, "SkyNodeParam");

        dummyType = SkyNode;
    }
    else
    if (e = xmlHandle.FirstChild("SkyStateParam").Element())
    {
        createdNode = ExportSkyState(inode, xmlHandle, "SkyStateParam");
    }
    else
    {
        // the given node has unknown custom attribute.
        n_maxlog(Warning, "Warning: The dummy node '%s' has unknown custom attributes.", inode->GetName());
    }

    return createdNode;
}

//-----------------------------------------------------------------------------
/**
    The given node has custom attributes for LOD node so, create nLodNode from
    its custom attributes.

    @param inode - 3dsmax node.
    @param xmlHandle - XML handle constains parameters.
    @param paramName - parameter name of the custom attribute in the gvien 3dsmax node.

    @return created nLodNode, or NULL if it fails to create.
*/
nSceneNode* 
nMaxDummy::ExportLODNode(INode *inode, TiXmlHandle &xmlHandle, const char* paramName)
{
    // retrieve node name.
    nString objectname = inode->GetName();
    objectname += nMaxUtil::CorrectName(objectname);

    nLodNode* lodNode = static_cast<nLodNode*>(CreateNebulaObject("nlodnode", objectname.Get()));
    if (lodNode)
    {
        TiXmlElement* child;

        double value;

        // get the mesh type
        child = xmlHandle.FirstChild(paramName).FirstChild("threshold").Child("", 0).Element();
        if (child)
        {
            child->Attribute("value", &value);

            // specifies LOD treshold.
            lodNode->AppendThreshold((float)value);
        }

        child = xmlHandle.FirstChild(paramName).FirstChild("mindistance").Child("", 0).Element();
        if (child)
        {
            child->Attribute("value", &value);

            // specifies LOD min distance.
            lodNode->SetMinDistance((float)value);
        }

        child = xmlHandle.FirstChild(paramName).FirstChild("maxdistance").Child("", 0).Element();
        if (child)
        {
            child->Attribute("value", &value);
            lodNode->SetMaxDistance((float)value);
        }
    }
    else
    {
        n_maxlog(Error, "Error: Failed to create LOD node for the dummy node '%s'.", inode->GetName());
    }

    return lodNode;
}

//-----------------------------------------------------------------------------
/**
    The given node has custom attributes for LOD node so, create nLodNode from
    its custom attributes.

    @param inode - 3dsmax node.
    @param xmlHandle - XML handle constains parameters.
    @param paramName - parameter name of the custom attribute in the gvien 3dsmax node.

    @return created nLodNode, or NULL if it fails to create.
*/
nSceneNode* 
nMaxDummy::ExportSkyNode(INode *inode, TiXmlHandle &xmlHandle, const char* paramName)
{
    // retrieve node name.
    nString objectname = inode->GetName();
    objectname += nMaxUtil::CorrectName(objectname);

    nSkyNode* skyNode = static_cast<nSkyNode*>(CreateNebulaObject("nskynode", objectname.Get()));
    if (skyNode)
    {
        TiXmlElement* child;

        // lock viewer
        child = xmlHandle.FirstChild(paramName).FirstChild("lockviewer").Child("", 0).Element();
        if (child)
        {
            int boolean;
            child->Attribute("value", &boolean);

            skyNode->SetLockViewer( ((boolean) ? true : false) );
        }

        double value;

        // time factor
        child = xmlHandle.FirstChild(paramName).FirstChild("timefactor").Child("", 0).Element();
        if (child)
        {
            child->Attribute("value", &value);
            skyNode->SetTimeFactor((float)value);
        }

        // time period
        child = xmlHandle.FirstChild(paramName).FirstChild("timeperiod").Child("", 0).Element();
        if (child)
        {
            child->Attribute("value", &value);
            skyNode->SetTimePeriod((float)value);
        }

        // start time
        child = xmlHandle.FirstChild(paramName).FirstChild("starttime").Child("", 0).Element();
        if (child)
        {
            child->Attribute("value", &value);
            skyNode->SetStartTime((float)value);
        }
    }
    else
    {
        n_maxlog(Error, "Error: Failed to create Sky node for the dummy node '%s'.", inode->GetName());
    }

    return skyNode;
}

//-----------------------------------------------------------------------------
/**
    The given node has custom attributes for LOD node so, create nLodNode from
    its custom attributes.

    @param inode - 3dsmax node.
    @param xmlHandle - XML handle constains parameters.
    @param paramName - parameter name of the custom attribute in the gvien 3dsmax node.

    @return created nLodNode, or NULL if it fails to create.
*/
nSceneNode* 
nMaxDummy::ExportSkyState(INode *inode, TiXmlHandle &xmlHandle, const char* paramName)
{
    // retrieve node name.
    nString objectname = inode->GetName();
    objectname += nMaxUtil::CorrectName(objectname);

    nSkyState* skyState = static_cast<nSkyState*>(CreateNebulaObject("nskystate", objectname.Get()));
    if (skyState)
    {
        //TODO: should retrieve picked node name from parameter block.
        TiXmlElement* child;

        child = xmlHandle.FirstChild(paramName).FirstChild("skyElement").Child("", 0).Element();
        if (child)
        {
            
            const char* elementName = child->Attribute("value");
            this->elemOfSkyState = elementName;
        }

        child = xmlHandle.FirstChild(paramName).FirstChild("stateTime").Child("", 0).Element();
        if (child)
        {
            double value;
            child->Attribute("value", &value);

            this->skyStateTime = (float)value;
        }
    }

    return skyState;
}
