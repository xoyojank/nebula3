//-----------------------------------------------------------------------------
//  nmaxskinanimator.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxnotetrack.h"
#include "export2/nmaxbones.h"
#include "export2/nmaxskinanimator.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "scene/ncharacter3skinanimator.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxSkinAnimator::nMaxSkinAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxSkinAnimator::~nMaxSkinAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
nAnimator* nMaxSkinAnimator::Export(int skelIndex, const char* animatorName, const char* animFilename) 
{
    n_assert(skelIndex >= 0);
    n_assert(animatorName);
    n_assert(animFilename);

	nString skinAnimatorName;
	nSkinAnimator* animator;

	if (nMaxOptions::Instance()->GetUseCharacter3SkinAnimator())
	{
		skinAnimatorName = "ncharacter3skinanimator";
		//FIXME: ugly casting!
		animator = (nCharacter3SkinAnimator*)this->CreateNebulaObject(skinAnimatorName.Get(), animatorName);
	}
	else
	{
		skinAnimatorName = "nskinanimator";
		//FIXME: ugly casting!
		animator = (nSkinAnimator*)this->CreateNebulaObject(skinAnimatorName.Get(), animatorName);
	}

    if (animator)
    {
        // build joints from bone array of the scene.
        nMaxBoneManager* boneManager = nMaxBoneManager::Instance();
        nArray<nMaxBoneManager::Bone>& boneArray = boneManager->GetBoneArray(skelIndex);

        this->BuildJoints(animator, boneArray);

        animator->SetChannel("time");
        animator->SetLoopType(nAnimLoopType::Loop);

        animator->SetAnim(animFilename);

        // build animation states.
        nMaxNoteTrack& noteTrack = boneManager->GetNoteTrack(skelIndex);
        this->BuildAnimClips(animator, noteTrack);
    }
    else
    {
        n_maxlog(Error, "Failed to create a nskinanimator.");
    }

    return animator;
}
//-----------------------------------------------------------------------------
/**
    Specifies joints of skin animator.

    - 01-Mar-05 kims added joint name export option.
*/
void nMaxSkinAnimator::BuildJoints(nSkinAnimator* animator, 
                                   const nArray<nMaxBoneManager::Bone> &boneArray)
{
    Matrix3 localTM;
    AffineParts ap;

    //int numJoints = nMaxBoneManager::Instance()->GetNumBones();
    int numJoints = boneArray.Size();

    // build joints.
    animator->BeginJoints(numJoints);

    float scale = nMaxOptions::Instance()->GetGeomScaleValue();
    for (int i=0; i<numJoints; i++)
    {
        const nMaxBoneManager::Bone &bone = boneArray[i];

        localTM = bone.localTransform;

        // transform scale.
        if (scale != 0.0f)
        {
            Point3 scaleVal(scale, scale, scale);

            Matrix3 scaleTM;
            scaleTM.IdentityMatrix();
            scaleTM.Scale(scaleVal);

            localTM = localTM * scaleTM;
        }

        localTM.NoScale();

        decomp_affine(localTM, &ap);

        vector3 poseTranlator (-ap.t.x, ap.t.z, ap.t.y);
        quaternion poseRotate (-ap.q.x, ap.q.z, ap.q.y, -ap.q.w);
        vector3 poseScale (ap.k.x, ap.k.z, ap.k.y);

        animator->SetJoint(bone.id, 
                           bone.parentID,
                           poseTranlator,
                           poseRotate,
                           poseScale,
                           bone.name.Get());
    }

    animator->EndJoints();
}

//-----------------------------------------------------------------------------
/**
    Build animation clips.

    - 22-Apr-09 kims Changed to add "default" clip name if it does not have any.
*/
void nMaxSkinAnimator::BuildAnimClips(nSkinAnimator* animator, nMaxNoteTrack& noteTrack)
{
    int numClips = noteTrack.GetNumStates();
    animator->BeginClips(numClips);

    nString clipName;

    for (int i=0; i<numClips; i++)
    {
        const nMaxAnimState& clip = noteTrack.GetState(i);

        // force to get clip name if it does not have any.
        if(clip.name.IsEmpty())
        {
            clipName = "default";
            clipName.AppendInt(i);
        }
        else
        {
            clipName = clip.name;
        }

        animator->SetClip(i, i, clip.name);
    }

    animator->EndClips();
}

