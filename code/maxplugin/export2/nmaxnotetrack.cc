//---------------------------------------------------------------------------
//  nmaxnotetrack.cc
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmax.h"

#include "export2/nmaxnotetrack.h"
#include "export2/nmaxutil.h"
#include "export2/nmaxbones.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

//#include "kernel/ntypes.h"
//#include "tools/nanimbuilder.h"
#include "coreanimation/infinitytype.h"
#include "toolkitutil/animutil/animbuilder.h"

using namespace ToolkitUtil;
using namespace Util;
using namespace CoreAnimation;
//---------------------------------------------------------------------------
/**
*/
nMaxAnimState::nMaxAnimState() :
    firstFrame(0),
    duration(0),
    fadeInTime(0.0f),
    loopType(InfinityType::Constant/*nAnimBuilder::Group::REPEAT*/)
{
    // default clip.
    this->AddClip("one");
}

//---------------------------------------------------------------------------
/**
*/
void nMaxAnimState::AddClip(const char* weightChannelName)
{
    n_assert(weightChannelName);

    this->clipArray.Append(String(weightChannelName));
}

//---------------------------------------------------------------------------
/**
*/
const String& 
nMaxAnimState::GetClip(int index) const
{
    n_assert(index >= 0);

    return this->clipArray[index];
}

//---------------------------------------------------------------------------
/**
*/
nMaxNoteTrack::nMaxNoteTrack()
{
}

//---------------------------------------------------------------------------
/**
*/
nMaxNoteTrack::~nMaxNoteTrack()
{
}

//---------------------------------------------------------------------------
/**
    Collect note key from the given node and creates animation states.
    A note key is used for representing animation state.

    The following presents a note key format:
    @verbatim
    loopName: loop-type 
    duration: frame-count 
    fadeintime: seconds-as-float 
    @endverbatim
    Note that ':' colon is used to separate a name and its value.

    -30-Aug-06  kims  Changed the current format to support fade-in-time.
                      Thank you bruce for the patch.

    -20-Feb-06  kims  Fixed to get correct index of ExtractRange().
                      Thanks Ivan Tivonenko for the patch.

    @param inode A node which we collect animation states from.
                 The node should be bone.
*/
void nMaxNoteTrack::GetAnimState(INode* inode)
{
    n_assert(inode);

    for (int i=0; i<inode->NumNoteTracks(); i++)
    {
        DefNoteTrack* noteTrack = static_cast<DefNoteTrack*>(inode->GetNoteTrack(i));

        for (int j=0; j<noteTrack->keys.Count(); j++)
        {
            NoteKey* noteKey = noteTrack->keys[j];

            int frame = noteKey->time / GetTicksPerFrame();
            int nextFrame = GetNextFrame(j, noteTrack);

            String note = noteKey->note.data();
            if (!note.IsEmpty())
            {
                int duration = -1;
                float fadeInTime = 0.0f;
                String strNote, strNoteName, strDuration, strLoopType, strFadeInTime;

                Array<String> noteLines;
                note.Tokenize("\n", noteLines);
                for (int i = 0; i < noteLines.Size(); i++)
                {
//                    String line = noteLines[i].Trim(" \r");
					noteLines[i].Trim(" \r");
					String line = noteLines[i];
                    Array<String> tokens;
                    line.Tokenize(":", tokens);
                    if (tokens.Size() > 1)
                    {
                        String key = tokens[0];						
                        String value = tokens[1];
						key.Trim(" ");
						value.Trim(" ");
                        key.ToLower();
                        value.ToLower();
                        if (i == 0)
                        {
                            // First line, will be the state name and loop type.
                            strNoteName = tokens[0]; // Get the original token to maintain case.
                            strLoopType = value;
                        }
                        else if (key == "duration")
                        {
                            strDuration = value;
                            duration = value.AsInt();
                        }
                        else if (key == "fadeintime")
                        {
                            strFadeInTime = value;
                            fadeInTime = value.AsFloat();
                        }
                    }
                    else if (tokens.Size() == 1)
                    {
                        // No value, just a key
                        if (i == 0)
                        {
                            // First line, it'll be the name.
                            strNoteName = line;
                        }
                        else
                        {
                            // Is it an int? Maybe it is the duration, but warn
                            // about deprecated usage and suggest that they update
                            // their data format.
                            if (line.IsValidInt())
                            {
                                strDuration = line;
                                duration = line.AsInt();
                                n_maxlog(Warning, "Integer found alone on line %d of note on frame %d, assuming it is a duration.", i + 1, frame);
                            }
                        }
                    }
                }

                if (duration == -1)
                {
                    duration = nextFrame - frame;
                }

                // add animation state.
                nMaxAnimState animState;

                animState.name       = strNoteName;
                animState.firstFrame = frame;
                animState.duration   = duration;
                animState.fadeInTime = fadeInTime;

                strLoopType.ToLower();
                if (strLoopType == "repeat")
                {
					animState.loopType = InfinityType::Cycle;//nAnimBuilder::Group::REPEAT;
                }
                else
                if (strLoopType == "clamp")
                {
					animState.loopType = InfinityType::Constant;//nAnimBuilder::Group::CLAMP;
                }
                else
                {
                    // unknown loop type. just specify it to repeat type.
					animState.loopType = InfinityType::Cycle;//::Group::REPEAT;
                }

                this->stateArray.Append(animState);
            }
        }// end of for each key in note track.
    }// end of for each note track.
}


