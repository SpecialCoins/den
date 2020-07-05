// Copyright (c) 2017-2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BCZ_STAKEINPUT_H
#define BCZ_STAKEINPUT_H

#include "chain.h"
#include "streams.h"
#include "uint256.h"

class CKeyStore;
class CWallet;
class CWalletTx;

class CStakeInput
{
protected:
    CBlockIndex* pindexFrom = nullptr;

public:
    virtual ~CStakeInput(){};
    virtual bool InitFromTxIn(const CTxIn& txin) = 0;
    virtual CBlockIndex* GetIndexFrom() = 0;
    virtual bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = UINT256_ZERO) = 0;
    virtual bool GetTxFrom(CTransaction& tx) const = 0;
    virtual bool GetTxOutFrom(CTxOut& out) const = 0;
    virtual CAmount GetValue() const = 0;
    virtual bool CreateTxOuts(CWallet* pwallet, std::vector<CTxOut>& vout, CAmount nTotal) = 0;
    virtual CDataStream GetUniqueness() const = 0;
    virtual bool ContextCheck(int nHeight, uint32_t nTime) = 0;
    virtual bool GetModifier(uint64_t& nStakeModifier) = 0;
    virtual uint64_t getStakeModifierHeight() const {
        return 0;
    }
};


class CBczStake : public CStakeInput
{
private:
    CTransaction txFrom{CTransaction()};
    unsigned int nPosition{0};

    // cached data
    uint64_t nStakeModifier = 0;
    int nStakeModifierHeight = 0;

public:
    CBczStake() {}

    bool InitFromTxIn(const CTxIn& txin) override;
    bool SetPrevout(CTransaction txPrev, unsigned int n);

    CBlockIndex* GetIndexFrom() override;
    bool GetTxFrom(CTransaction& tx) const override;
    bool GetTxOutFrom(CTxOut& out) const override;
    CAmount GetValue() const override;
    CDataStream GetUniqueness() const override;
    bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = UINT256_ZERO) override;
    bool CreateTxOuts(CWallet* pwallet, std::vector<CTxOut>& vout, CAmount nTotal) override;
    bool ContextCheck(int nHeight, uint32_t nTime) override;
    bool GetModifier(uint64_t& nStakeModifier) override;
    uint64_t getStakeModifierHeight() const override { return nStakeModifierHeight; }
};


#endif //BCZ_STAKEINPUT_H
