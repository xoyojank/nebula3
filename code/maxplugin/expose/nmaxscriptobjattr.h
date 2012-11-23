//------------------------------------------------------------------------------
//  nmaxscriptobjattr.h
//
//  (C)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#ifndef N_MAXSCRIPTOBJATTR_H
#define N_MAXSCRIPTOBJATTR_H
//------------------------------------------------------------------------------
/**
    @class nObjectAttribute
    @ingroup NMaxExposeContrib

    @brief 
*/
#include "util/nhashlist.h"

//------------------------------------------------------------------------------
class nObjectAttribute
{
public:
    class Command;

    enum ArgType {
        Float,
        Bool,
        String,
        Int,
        Vector2,
        Vector3,
        Vector4,
        Matrix3,
        Matrix4,
        RGB,
        RGBA,
        Various,
    };

    /// Constructor.
    nObjectAttribute();
    /// Destructor.
    virtual ~nObjectAttribute();
    /// Specify xml file which to open.
    void SetFilename(const nString& name);
    /// Open specified xml file.(and parse it)
    bool Open();
    /// Close opened xml file.
    void Close();

    bool IsOpen() const;

    const nString& GetName() const;

    /// Find the command in the hash table with given command name.
    Command* Find(const char* command);

    class Command : public nHashNode
    {
    public:
        Command();
        virtual ~Command();
        
        void StringToType(const char* str);
        ArgType GetType() const;
        
        void SetMin(float val);
        float GetMin() const;
        void SetMax(float val);
        float GetMax() const;

        ArgType type;

        float fMin;
        float fMax;
    };

private:
    bool ParseXmlFile();
    void ClearCommandList();

private:
    nHashList commandList; ///hash list for commands.

    bool isOpen;

    nString filename; /// xml filename
    nString name;     /// nebula object name
    
};
//------------------------------------------------------------------------------
inline
bool nObjectAttribute::IsOpen() const
{
    return this->isOpen;
}
//------------------------------------------------------------------------------
/**
    @return one of the nebula object's name.
*/
inline
const nString& nObjectAttribute::GetName() const
{
    return this->name;
}
//------------------------------------------------------------------------------
inline
nObjectAttribute::Command* 
nObjectAttribute::Find(const char* command)
{
    Command* cmd = (Command*)this->commandList.Find(command);
    return cmd;
}
//------------------------------------------------------------------------------
inline
nObjectAttribute::ArgType nObjectAttribute::Command::GetType() const
{
    return this->type;
}
//------------------------------------------------------------------------------
inline
void nObjectAttribute::Command::SetMin(float val)
{
     this->fMin = val;
}
//------------------------------------------------------------------------------
inline
float nObjectAttribute::Command::GetMin() const
{
     return this->fMin;
}
//------------------------------------------------------------------------------
inline
void nObjectAttribute::Command::SetMax(float val)
{
    this->fMax = val;
}
//------------------------------------------------------------------------------
inline
float nObjectAttribute::Command::GetMax() const
{
    return this->fMax;
}

//------------------------------------------------------------------------------
#endif
