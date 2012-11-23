//-----------------------------------------------------------------------------
//  nmaxskinanimator.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXSKINANIMATOR_H
#define N_MAXSKINANIMATOR_H
//-----------------------------------------------------------------------------
/**
    @class nMaxSkinAnimator
    @ingroup NebulaMaxExport2Contrib

    @brief A class for exporting nSkinAnimator.
*/
#include "export2/nmaxnode.h"

class nMaxBoneManager;
class nMaxBoneManager::Bone;
class nAnimator;
class nSkinAnimator;

//-----------------------------------------------------------------------------
class nMaxSkinAnimator : public nMaxNode
{
public:
    nMaxSkinAnimator();
    virtual ~nMaxSkinAnimator();

    nAnimator* Export(int skelIndex, const char* animatorName, const char* animFilename);

protected:
    void BuildJoints(nSkinAnimator* animator, const nArray<nMaxBoneManager::Bone> &boneArray);
    void BuildAnimClips(nSkinAnimator* animator, nMaxNoteTrack& noteTrack);

};
#endif
