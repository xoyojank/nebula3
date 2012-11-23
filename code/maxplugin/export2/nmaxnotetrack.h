//---------------------------------------------------------------------------
//  nmaxnotetrack.h
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#ifndef N_MAXNOTETRACK_H
#define N_MAXNOTETRACK_H 
//---------------------------------------------------------------------------
#include "export2/nmaxinterface.h"

//#include "util/nstring.h"
#include "util/string.h"
#include "util/array.h"

//---------------------------------------------------------------------------
/**
    @class nMaxAnimState 
    @ingroup NebulaMaxExport2Contrib

    @brief A class which represents animation states.

*/
class nMaxAnimState 
{
public:
    nMaxAnimState();

    /// Adds clip.
    void AddClip(const char* weightChannelName);

    /// Get clip name from the given index.
    const Util::String& GetClip(int index) const;

    /// state first frame.
    int firstFrame;
    /// frame duration, number of frames of the state.
    int duration;
    /// fade time.
    float fadeInTime;
    /// loop type. (it has nAnimBuilder::Group::REPEAT or 
    /// nAnimBuilder::Group::CLAMP for its value. default is 'REPEAT')
    int loopType;
    /// state name.
    Util::String name;
    /// array for clips.
	Util::Array<Util::String> clipArray;
};

//---------------------------------------------------------------------------
/**
    @class nMaxNoteTrack
    @ingroup NebulaMaxExport2Contrib

    @brief Collect animation states and clips in note track of a max node.
           The collected animation states and clips are used when we build
           skin animator.

*/
class nMaxNoteTrack
{
public:
    nMaxNoteTrack();
    virtual ~nMaxNoteTrack();

    void GetAnimState(INode* inode);

    /// Get the number of animation states which collected.
    int GetNumStates() const;

    nMaxAnimState& AddAnimState(int firstframe, int duration, float fadeintime, bool loop = true);

    /// Get animation state from the given index.
    const nMaxAnimState& GetState(int index);

protected:
    ///
    int GetNextFrame(int index, DefNoteTrack* n);

protected:
    /// array for animation states which collected from note track.
	Util::Array<nMaxAnimState> stateArray;

};
//---------------------------------------------------------------------------
inline
int nMaxNoteTrack::GetNumStates() const
{
    return this->stateArray.Size();
}
//---------------------------------------------------------------------------
inline
int nMaxNoteTrack::GetNextFrame(int index, DefNoteTrack* n)
{
    return (index < n->keys.Count() - 1 ? 
        n->keys[index+1]->time/GetTicksPerFrame() : 
    nMaxInterface::Instance()->GetNumFrames());
}
//---------------------------------------------------------------------------
inline
const nMaxAnimState& 
nMaxNoteTrack::GetState(int index)
{
    n_assert(index >= 0);
    return this->stateArray[index];
}
//---------------------------------------------------------------------------
inline
nMaxAnimState&
nMaxNoteTrack::AddAnimState(int firstframe, int duration, float fadeintime, bool loop)
{
    nMaxAnimState animState;
    animState.firstFrame = firstframe;
    animState.duration   = duration;
    animState.fadeInTime = fadeintime;

    //return this->stateArray.PushBack(animState);
	this->stateArray.Append(animState);
	return this->stateArray.Back();
}
//---------------------------------------------------------------------------
#endif
