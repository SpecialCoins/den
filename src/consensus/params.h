// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_PARAMS_H
#define BITCOIN_CONSENSUS_PARAMS_H

#include "amount.h"
#include "uint256.h"
#include <map>
#include <string>

namespace Consensus {

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    int nFutureTimeDrift;
    int nTimeSlotLength;
	int nStakeMinDepth;
	int height_start_StakeModifierV2;

    //bool IsTimeProtocolV2(const int nHeight) const { return nHeight >= height_start_TimeProtoV2; }

    int FutureBlockTimeDrift(const int nHeight) const
    {
        // PoS (TimeV2):
        if (false) return nTimeSlotLength - 1;
        // PoS (TimeV1:
        return (nHeight > nFutureTimeDrift);
    }

    bool IsValidBlockTimeStamp(const int64_t nTime, const int nHeight) const
    {
        // Before time protocol V2, blocks can have arbitrary timestamps
        if (!IsTimeProtocolV2(nHeight)) return true;
        // Time protocol v2 requires time in slots
        return (nTime % nTimeSlotLength) == 0;
    }

    bool HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime,
            const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const
    {
        // before stake modifier V2, we require the utxo to be nStakeMinAge old
        if (contextHeight < height_start_StakeModifierV2)
			return (utxoFromBlockTime + 3600 <= contextTime);
        // with stake modifier V2+, we require the utxo to be nStakeMinDepth deep in the chain
        return (contextHeight - utxoFromBlockHeight >= nStakeMinDepth);
    }
};
} // namespace Consensus

#endif // BITCOIN_CONSENSUS_PARAMS_H
