// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "bignum/bignum.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <limits>
#include "chainparamsseeds.h"

std::string CDNSSeedData::getHost(uint64_t requiredServiceBits) const {
    //use default host for non-filter-capable seeds or if we use the default service bits (NODE_NETWORK)
    if (!supportsServiceBitsFiltering || requiredServiceBits == NODE_NETWORK)
        return host;

    return strprintf("x%x.%s", requiredServiceBits, host);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
        (       0, uint256("0x00000fca55bd94508769f48d6d17c4aa3f9ae40b072036a3691752f1b70ef2dd"))
        (       1, uint256("0x000006bde18e8be09c47b5a76120be3cdc4661d935586a3ef730dbe67b3a70f0"))
        (      76, uint256("0x00000b5033e45adc8699cbc6b8240307f43104e04eb035115eba25fb047a9b4d"))
        (   51335, uint256("0x1a1c94116767503dbe26b657ec56ebc7948510c99de69ad73ac2355f649ce411"))
        (  107065, uint256("0xe08e12cf3f1309cf111a753668817db12ef5c0cd045dd060e957f54c575ac5cf"))
        (  107066, uint256("0xb2d9a91cb919a720168a523df3b67edffc94b5cf6fe6de1c4087af0eb88099b0"))
        (  107067, uint256("0xf0e4d0f0d6acbf991820715e92da3fa61998752e0e02fcfc22709af0cd4331e1"))
        (  107068, uint256("0x5fccff13c4cfdeb81f9e98abb46d35771c8e27c2e28d4f9a9df00478e00eb1a8"))
        (  107147, uint256("0x07c6580691382ff7087cfa4dbaf539b940098af6c2c78046d7c1f18df8e25988"))
        (  107198, uint256("0x4c9590a77382e857146bd0cac355589ad1decf5a7d54327c0347bd3acf52a88f"))
        (  108374, uint256("0x55957713ec6e6951e6f451c8cf3fb07c73cddaeb74dfefdb77acabba5d2c9f51"))
        (  126318, uint256("0xffcdbdf74be6db9497258de737b16cd596ce7bedf175a6796f54be1e3805dc98"))
        (  144624, uint256("0x1bd9410c714db4051a34b2d60b641ad9c63f4aed21b95a6f21018f6e221ce081"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1582137628, // * UNIX timestamp of last checkpoint block
    10000,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    500        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1560843157,
    2501682,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

bool CChainParams::HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime,
        const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const
{
    // before stake modifier V2, the age required was 60 * 60 (1 hour) / not required on regtest
    if (!IsStakeModifierV2(contextHeight))
        return (NetworkID() == CBaseChainParams::REGTEST || (utxoFromBlockTime + 3600 <= contextTime));

    // after stake modifier V2, we require the utxo to be nStakeMinDepth deep in the chain
    return (contextHeight - utxoFromBlockHeight >= nStakeMinDepth);
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = { 'b' };
        pchMessageStart[1] = { 'c' };
        pchMessageStart[2] = { 'z' };
        pchMessageStart[3] = { '0' };
        vAlertPubKey = ParseHex("04964af71decbf046031d1bf6a13b747a433bc14dc97c6f7f0b5b33d26eea81dc2a8df57d50b07251975857592989f730d0e7153ca3bc65ebc29e0b21cb57683b5");
        nDefaultPort = 29500;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetSpacing = 150;
        nTargetTimespan = 150;
        nTimeSlotLength = 150;
        nMaturity = 100;
        nStakeMinDepth = 120;
        nFutureTimeDrift = 180;
        nMinColdStakingAmount = 1000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 50000;
        nBlockStakeModifierlV2 = 66555;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = 162000;
        nColdStart = 165000;  //cold rescan

        const char* pszTimestamp = "BCZ BORN";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0x1e0fffff << CBigNum(4).getvch() << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0;
        txNew.vout[0].scriptPubKey = CScript();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
        genesis.nVersion = 2;
        genesis.nTime = 1554888888;
        genesis.nBits = 0x1e0fffff;
        genesis.nNonce = 249257;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000fca55bd94508769f48d6d17c4aa3f9ae40b072036a3691752f1b70ef2dd"));
        assert(genesis.hashMerkleRoot == uint256("0x0adff40a2ade8baf5c2c311748928ff4d4d727d2042b1f685fe8ca1f70a1021b"));
        vSeeds.push_back(CDNSSeedData("51.77.145.35", "51.77.145.35"));
        vSeeds.push_back(CDNSSeedData("51.91.156.249", "51.91.156.249"));
        vSeeds.push_back(CDNSSeedData("51.91.156.251", "51.91.156.251"));
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 20);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 210);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkPubKey = "04d7437801b20f7d3e585e829088d0773846b80fd06788d6f74b65d258b03fe9cbc2d2fb355bd878f81c491db08fbe5556d6aa220a80b43be927f4bdb41d2d00e8";
        strObfuscationPoolDummyAddress = "B52EagiPxecjS9zwyebbCZz3x3QuYBNezo";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
