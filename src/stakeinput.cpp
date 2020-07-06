// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chain.h"
#include "main.h"
#include "stakeinput.h"
#include "wallet/wallet.h"

//Normal Stake

bool CBczStake::InitFromTxIn(const CTxIn& txin)
{

    // Find the previous transaction in database
    uint256 hashBlock;
    CTransaction txPrev;
    if (!GetTransaction(txin.prevout.hash, txPrev, hashBlock, true))
        return error("%s : INFO: read txPrev failed, tx id prev: %s", __func__, txin.prevout.hash.GetHex());
    SetPrevout(txPrev, txin.prevout.n);

    // Find the index of the block of the previous transaction
    if (mapBlockIndex.count(hashBlock)) {
        CBlockIndex* pindex = mapBlockIndex.at(hashBlock);
        if (chainActive.Contains(pindex)) pindexFrom = pindex;
    }
    // Check that the input is in the active chain
    if (!pindexFrom)
        return error("%s : Failed to find the block index for stake origin", __func__);

    // All good
    return true;
}

bool CBczStake::SetPrevout(CTransaction txPrev, unsigned int n)
{
    this->txFrom = txPrev;
    this->nPosition = n;
    return true;
}

bool CBczStake::GetTxFrom(CTransaction& tx) const
{
    if (txFrom.IsNull())
        return false;

    tx = txFrom;
    return true;
}

bool CBczStake::GetTxOutFrom(CTxOut& out) const
{
    if (txFrom.IsNull() || nPosition >= txFrom.vout.size())
        return false;
    out = txFrom.vout[nPosition];
    return true;
}

bool CBczStake::CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut)
{
    txIn = CTxIn(txFrom.GetHash(), nPosition);
    return true;
}

CAmount CBczStake::GetValue() const
{
    return txFrom.vout[nPosition].nValue;
}

bool CBczStake::CreateTxOuts(CWallet* pwallet, std::vector<CTxOut>& vout, CAmount nTotal)
{
    std::vector<valtype> vSolutions;
    txnouttype whichType;
    CScript scriptPubKeyKernel = txFrom.vout[nPosition].scriptPubKey;
    if (!Solver(scriptPubKeyKernel, whichType, vSolutions))
        return error("%s: failed to parse kernel", __func__);

    if (whichType != TX_PUBKEY && whichType != TX_PUBKEYHASH && whichType != TX_COLDSTAKE)
        return error("%s: type=%d (%s) not supported for scriptPubKeyKernel", __func__, whichType, GetTxnOutputType(whichType));

    CScript scriptPubKey;
    CKey key;
    if (whichType == TX_PUBKEYHASH) {
        // if P2PKH check that we have the input private key
        if (!pwallet->GetKey(CKeyID(uint160(vSolutions[0])), key))
            return error("%s: Unable to get staking private key", __func__);

        // convert to P2PK inputs
        scriptPubKey << key.GetPubKey() << OP_CHECKSIG;

    } else {
        // if P2CS, check that we have the coldstaking private key
        if ( whichType == TX_COLDSTAKE && !pwallet->GetKey(CKeyID(uint160(vSolutions[0])), key) )
            return error("%s: Unable to get cold staking private key", __func__);

        // keep the same script
        scriptPubKey = scriptPubKeyKernel;
    }
    vout.emplace_back(CTxOut(0, scriptPubKey));

    // Calculate if we need to split the output
    if (pwallet->nStakeSplitThreshold > 0) {
        int nSplit = static_cast<int>(nTotal / pwallet->nStakeSplitThreshold);
        if (nSplit > 1) {
            // if nTotal is twice or more of the threshold; create more outputs
            int txSizeMax = MAX_STANDARD_TX_SIZE >> 11; // limit splits to <10% of the max TX size (/2048)
            if (nSplit > txSizeMax)
                nSplit = txSizeMax;
            for (int i = nSplit; i > 1; i--) {
                LogPrintf("%s: StakeSplit: nTotal = %d; adding output %d of %d\n", __func__, nTotal, (nSplit-i)+2, nSplit);
                vout.emplace_back(CTxOut(0, scriptPubKey));
            }
        }
    }

    return true;
}


bool CBczStake::GetModifier(uint64_t& nStakeModifier)
{
    if (this->nStakeModifier == 0) {
        // look for the modifier
        GetIndexFrom();
        if (!pindexFrom)
            return error("%s: failed to get index from", __func__);
        // TODO: This method must be removed from here in the short terms.. it's a call to an static method in kernel.cpp when this class method is only called from kernel.cpp, no comments..
        if (!GetKernelStakeModifier(pindexFrom->GetBlockHash(), this->nStakeModifier, this->nStakeModifierHeight, this->nStakeModifierTime, false))
            return false;
    }
    nStakeModifier = this->nStakeModifier;
    return true;
}

CDataStream CBczStake::GetUniqueness() const
{
    //The unique identifier for a BCZ stake is the outpoint
    CDataStream ss(SER_NETWORK, 0);
    ss << nPosition << txFrom.GetHash();
    return ss;
}

//The block that the UTXO was added to the chain
CBlockIndex* CBczStake::GetIndexFrom()
{
    if (pindexFrom)
        return pindexFrom;
    uint256 hashBlock = UINT256_ZERO;
    CTransaction tx;
    if (GetTransaction(txFrom.GetHash(), tx, hashBlock, true)) {
        // If the index is in the chain, then set it as the "index from"
        if (mapBlockIndex.count(hashBlock)) {
            CBlockIndex* pindex = mapBlockIndex.at(hashBlock);
            if (chainActive.Contains(pindex))
                pindexFrom = pindex;
        }
    } else {
        LogPrintf("%s : failed to find tx %s\n", __func__, txFrom.GetHash().GetHex());
    }

    return pindexFrom;
}

// Verify stake contextual checks
bool CBczStake::ContextCheck(int nHeight, uint32_t nTime)
{
    const Consensus::Params& consensus = Params().GetConsensus();
    // Get Stake input block time/height
    CBlockIndex* pindexFrom = GetIndexFrom();
    if (!pindexFrom)
        return error("%s: unable to get previous index for stake input", __func__);
    const int nHeightBlockFrom = pindexFrom->nHeight;
    const uint32_t nTimeBlockFrom = pindexFrom->nTime;

    // Check that the stake has the required depth/age
    if (!consensus.HasStakeMinAgeOrDepth(nHeight, nTime, nHeightBlockFrom, nTimeBlockFrom))
        return error("%s : min age violation - height=%d - time=%d, nHeightBlockFrom=%d, nTimeBlockFrom=%d",
                         __func__, nHeight, nTime, nHeightBlockFrom, nTimeBlockFrom);
    // All good
    return true;
}
