// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"
#include "chain.h"
#include "chainparams.h"
#include "main.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include <math.h>

unsigned int static DarkGravityWave3(const CBlockIndex* pindexLast) {

   int64_t nPastBlocks = 24;
   const CBlockIndex *pindex = pindexLast;
   uint256 bnPastTargetAvg;
   for (unsigned int nCountBlocks = 1; nCountBlocks <= nPastBlocks; nCountBlocks++) {
       uint256 bnTarget = uint256().SetCompact(pindex->nBits);
       if (nCountBlocks == 1) {
           bnPastTargetAvg = bnTarget;
       } else {
           bnPastTargetAvg = (bnPastTargetAvg * nCountBlocks + bnTarget) / (nCountBlocks + 1);
       }

       if(nCountBlocks != nPastBlocks) {
           assert(pindex->pprev);
           pindex = pindex->pprev;
       }
   }

   uint256 bnNew(bnPastTargetAvg);
   int64_t nActualTimespan = pindexLast->GetBlockTime() - pindex->GetBlockTime();
   int64_t nTargetTimespan = nPastBlocks * 150;
   if (nActualTimespan < nTargetTimespan/1.5)
       nActualTimespan = nTargetTimespan/1.5;
   if (nActualTimespan > nTargetTimespan*1.5)
       nActualTimespan = nTargetTimespan*1.5;

   bnNew *= nActualTimespan;
   bnNew /= nTargetTimespan;

   if (bnNew > Params().ProofOfWorkLimit()) {
       bnNew = Params().ProofOfWorkLimit();
   }

   return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast)
{
   if (pindexLast->nHeight > UTXOF)
   {
       return DarkGravityWave3(pindexLast);
   }
   else
   {
       return Params().ProofOfWorkLimit().GetCompact();
   }
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
   bool fNegative;
   bool fOverflow;
   uint256 bnTarget;

   if (Params().SkipProofOfWorkCheck())
       return true;

   bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

   // Check range
   if (fNegative || bnTarget.IsNull() || fOverflow || bnTarget > Params().ProofOfWorkLimit())
       return error("CheckProofOfWork() : nBits below minimum work");

   // Check proof of work matches claimed amount
   if (hash > bnTarget)
       return error("CheckProofOfWork() : hash doesn't match nBits");

   return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
   uint256 bnTarget;
   bool fNegative;
   bool fOverflow;
   bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
   if (fNegative || fOverflow || bnTarget.IsNull())
       return UINT256_ZERO;
   // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
   // as it's too large for a uint256. However, as 2**256 is at least as large
   // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
   // or ~bnTarget / (nTarget+1) + 1.
   return (~bnTarget / (bnTarget + 1)) + 1;
}
