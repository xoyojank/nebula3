//-----------------------------------------------------------------------------
//  nmaxscene.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXSCENE_H
#define N_MAXSCENE_H
//-----------------------------------------------------------------------------
/**
    @class nMaxScene
    @ingroup NebulaMaxExport2Contrib

    @brief A class for 3dsmax scene which to be exported.

    -16-Feb-06  kims  made some improvements: <br>
                      made methods of maxscene virtual and added methods like
                      GetAnimFileNameToSave - this needed for subclassing.
                      Thanks Ivan Tivonenko for the patch.

*/

class nMaxDummy;
class nMaxMesh;
class nVariableServer;
class nSceneNode;
class nMaxBoneManager;

//-----------------------------------------------------------------------------
class nMaxScene
{
public:
    ///
    nMaxScene();
    ///
    virtual ~nMaxScene();

    ///
    bool Export(INode* inode);

    ///
    const nString& GetSceneDir() const;
protected:
    // @name Helper functions for begin and end stage of export.
    // @{
    ///
    virtual bool Begin(INode* root);
    ///
    virtual bool End();

    ///
    virtual bool OpenNebula();
    ///
    virtual bool CloseNebula();

    ///
    virtual bool Preprocess(INode* root);
    ///
    virtual bool Postprocess();

    ///
    void InitializeNodes(INode* inode);
    ///
    void UnInitializeNodes(INode* inode);
    // @}

    /// To be overrided if needed.
    virtual nString GetMeshFileNameToSave(nString& meshPath, nMaxMesh::Type type, bool isSkinned) const;
    /// To be overrided if needed.
    virtual nString GetAnimFileNameToSaveBase();
    ///
    virtual nString GetAnimFileNameToSave(int skelIndex);

    // @name Functions for export max's nodes
    // @{
    ///
    bool ExportNodes(INode* inode);
	///
	bool ExportSkinAnimator(bool animOnly = false);
    ///
    virtual nSceneNode* ExportGeomObject(INode* inode, Object* obj);
    ///
    virtual nSceneNode* ExportGeomObject2(INode* inode);
    ///
    virtual nSceneNode* ExportMorph();
    ///
    virtual nSceneNode* ExportParticle();
    ///
    virtual nSceneNode* ExportCamera(INode* inode, Object* obj);
    ///
    virtual nSceneNode* ExportLight(INode* inode, Object* obj);
    ///
    virtual nSceneNode* ExportDummy(INode* inode, Object* obj);
    ///
    virtual nSceneNode* ExportNodesHook(SClass_ID sID, INode* inode, Object* obj);

    ///
    void ExportBackgroudColor();
    ///
    Point3 GetBackgroundColor() const;

    ///
    void ExportXForm(INode* inode, nSceneNode* sceneNode, TimeValue &animStart);
    // @}

    ///
    void CollectTopLevelNodes(INode* inode);
    ///
    bool IsExportedNode(INode* inode);

    ///
    virtual void ProcessOnMeshBuilder(nMaxMesh* mesh, nMeshBuilder& meshBuilder, bool isShadowMesh, nString meshName);
    ///
    void AddMeshByType(nMaxMesh* mesh);

    ///
    void ProcessOnSkyNode();
    ///
    void AddDummyByType(nMaxDummy* dummy);

    void CollectChildNode(nRoot* node, nArray<nRoot*>& array);

    /// get world transform matrix
    matrix44 GetWorldTM(nTransformNode* node);

protected:
    /// variable server
    nRef<nVariableServer> varServer;
    
    /// bone manager for all bones in the scene.
    /// we keep the pointer to delete the singleton at the end of the export stage.
    nMaxBoneManager* boneManager;

    /// scene root node.
    INode* sceneRoot; 

    /// Nebula object name of the scene base object.
    nSceneNode* exportRoot;

    /// mesh array where meshes that exported from nMaxMesh instance are stored.
    nArray<nMaxMesh*> meshArray;
    nArray<nMaxMesh*> skinnedMeshArray;
    nArray<nMaxMesh*> shadowMeshArray;
    nArray<nMaxMesh*> skinnedShadowMeshArray;
    nArray<nMaxMesh*> collisionMeshArray;
    nArray<nMaxMesh*> particle2MeshArray;
    nArray<nMaxMesh*> skyMeshArray;

    /// Mesh builder for meshes in the mesh array.
    nMeshBuilder globalMeshBuilder;
    /// Mesh builder for meshes in the skinned mesh array.
    nMeshBuilder globalSkinnedMeshBuilder;
    /// Mesh builder for meshes in the shadow mesh array.
    nMeshBuilder globalShadowMeshBuilder;
    /// Mesh builder for meshes in the skinned shadow mesh array.
    nMeshBuilder globalSkinnedShadowMeshBuilder;
    /// Mesh builder for meshes in the particle2 mesh array
    nMeshBuilder globalParticle2MeshBuilder;

    ///hashmap for skystates.
    //HashMap2<nMaxDummy*> skyStateMap;
    nKeyArray<nMaxDummy*> skyStateArray;

    /// array for top level nodes.
    nArray<INode*> topLevelNodes;

    /// viewport background color.
    Point3 backgroudCol;

    ///
    nString sceneDir;

};
//-----------------------------------------------------------------------------
inline
const nString&
nMaxScene::GetSceneDir() const
{
    return this->sceneDir;
}

//-----------------------------------------------------------------------------
inline
Point3 
nMaxScene::GetBackgroundColor() const
{
    return this->backgroudCol;
}
//-----------------------------------------------------------------------------
#endif
