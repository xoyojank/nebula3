//-----------------------------------------------------------------------------
//  nmaxinterface.h
//
//  (c)2004 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#ifndef N_MAXINTERFACE_H
#define N_MAXINTERFACE_H
//-----------------------------------------------------------------------------
/**
    @class nMaxInterface
    @ingroup NebulaMaxExport2Contrib

    @brief A wrapper class of 3dsmax Interface class.

*/

//-----------------------------------------------------------------------------
class nMaxInterface
{
public:
    /// Destructor.
    virtual ~nMaxInterface();

    /// Retrieve singleton instance.
    static nMaxInterface* Instance();

    bool Create(Interface *intf);

    int GetCurrentFrame();

    /// Retrieve start time of the animation.
    TimeValue GetAnimStartTime() const;
    /// Retrieve end time of the animation.
    TimeValue GetAnimEndTime() const;
    /// Retrieve number of frames.
    int GetNumFrames() const;

    /// Retrieve windows handle.
    HWND GetHwnd();

    /// Retrieves scene root node.
    INode* GetRootNode();

    /// @name Functions for prgessive bar
    /// @{
    bool ProgressStart(TCHAR* title);
    void ProgressUpdate(int percent);
    bool CheckUserCancel();
    void ProgressEnd();
    /// @}

    /// Retrieve pointer to instance of 3dsmax Interface class.
    Interface* GetInterface() const;

protected:
    ///
    Interface* maxInterface;

private:
    /// Constructor.
    nMaxInterface();
    /// Copy Constructor.
    nMaxInterface(const nMaxInterface&) {};

    /// Singleton instance.
    static nMaxInterface* Singleton;

};
//-----------------------------------------------------------------------------
inline
TimeValue 
nMaxInterface::GetAnimStartTime() const
{
    TimeValue animStart = this->maxInterface->GetAnimRange().Start();

    return animStart;
}
//-----------------------------------------------------------------------------
inline
TimeValue 
nMaxInterface::GetAnimEndTime() const
{
    return this->maxInterface->GetAnimRange().End();
}
//-----------------------------------------------------------------------------
inline
int nMaxInterface::GetNumFrames() const
{
    TimeValue animOffset;
    animOffset = (this->GetAnimEndTime() - this->GetAnimStartTime());
    return ((animOffset / GetTicksPerFrame()) + 1);
}
//-----------------------------------------------------------------------------
inline
HWND nMaxInterface::GetHwnd()
{
    return (this->maxInterface->GetMAXHWnd());
}
//-----------------------------------------------------------------------------
/**
    The root node does not physically exist in the scene, rather it is a special 
    node that is the parent node of all nodes.
    This root node has "Scene Root" for its node name.
*/
inline
INode* nMaxInterface::GetRootNode()
{
    return this->maxInterface->GetRootNode();
}
//-----------------------------------------------------------------------------
inline
Interface* nMaxInterface::GetInterface() const
{
    return this->maxInterface;
}
//-----------------------------------------------------------------------------
#endif
