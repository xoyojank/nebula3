#include "RakNetDefines.h"
#ifndef _USE_RAKNET_FLOW_CONTROL

/*****************************************************************************
Copyright (c) 2001 - 2007, The Board of Trustees of the University of Illinois.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the
  above copyright notice, this list of conditions
  and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the University of Illinois
  nor the names of its contributors may be used to
  endorse or promote products derived from this
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/*****************************************************************************
written by
   Yunhong Gu, last updated 07/22/2007
*****************************************************************************/

#ifndef __UDT_CO_OP_H__
#define __UDT_CO_OP_H__

#include "udt.h"
#include "common.h"
#include <set>
#include <map>


//class CUDT;

struct CHistoryBlock
{
   uint32_t m_IP[4];
   int m_iIPversion;
   uint64_t m_ullTimeStamp;
   int m_iRTT;
   int m_iBandwidth;
   int m_iLossRate;
   int m_iReorderDistance;
};

struct CIPComp
{
   bool operator()(const CHistoryBlock* hb1, const CHistoryBlock* hb2) const;
};

struct CTSComp
{
   bool operator()(const CHistoryBlock* hb1, const CHistoryBlock* hb2) const;
};

class CHistory
{
public:
   CHistory();
   CHistory(const unsigned int& size);
   ~CHistory();

public:
   void update(const sockaddr* addr, const int& ver, const int& rtt, const int& bw);
   void update(const CHistoryBlock* hb);
   int lookup(const sockaddr* addr, const int& ver, CHistoryBlock* hb);

private:
   void convert(const sockaddr* addr, const int& ver, uint32_t* ip);

private:
   unsigned int m_uiSize;
   std::set<CHistoryBlock*, CIPComp> m_sIPIndex;
   std::set<CHistoryBlock*, CTSComp> m_sTSIndex;

   pthread_mutex_t m_Lock;
};

//struct CUDTComp
//{
//   bool operator()(const CUDT* u1, const CUDT* u2) const;
//};

// KevinJ: It appears that the first parameter to join and leave is fixed, meaning the 
// map lookup isn't necessary. The dependency of this class on CUDT is therefore removed
class CControl
{
public:
   CControl();
   ~CControl();

public:
//   int join(CUDT* udt, const sockaddr* addr, const int& ver, int& rtt, int& bw);
//   void leave(CUDT* udt, const int& rtt, const int& bw);
   int join(const sockaddr* addr, const int& ver, int& rtt, int& bw);
   void leave(const int& rtt, const int& bw);

private:
   CHistory* m_pHistoryRecord;
//   std::map<CHistoryBlock*, std::set<CUDT*, CUDTComp>, CIPComp> m_mControlBlock;
//   std::map<CUDT*, CHistoryBlock*, CUDTComp> m_mUDTIndex;

   pthread_mutex_t m_Lock;
   CHistoryBlock hb;
};

#endif

#endif // #ifndef _USE_RAKNET_FLOW_CONTROL
