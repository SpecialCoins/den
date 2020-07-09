// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"
#include "bignum/bignum.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
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
        (  144624, uint256("0x1bd9410c714db4051a34b2d60b641ad9c63f4aed21b95a6f21018f6e221ce081"))
        (  162435, uint256("0xe33a7cbfdf66da89d490d0acf072e10fed16b8e6f7740061e64279534db0d36f"))
        (  175281, uint256("0xfdb5fbd40c3a4c1288ccabf8c9b74a7590bbeb9bf900f3c520caafb8df74a773"))
        (  190345, uint256("0xba5b39a88308941ea56e24daebac57e5e166a1593fda42279cff5f8509452d67"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1589340234, // * UNIX timestamp of last checkpoint block
    10000,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    500        // * estimated number of transactions per day after checkpoint
};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

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
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000fca55bd94508769f48d6d17c4aa3f9ae40b072036a3691752f1b70ef2dd"));
        assert(genesis.hashMerkleRoot == uint256("0x0adff40a2ade8baf5c2c311748928ff4d4d727d2042b1f685fe8ca1f70a1021b"));
        consensus.powLimit = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDrift = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 21000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinDepth = 120;

        // spork keys
        consensus.strSporkPubKey = "04d7437801b20f7d3e585e829088d0773846b80fd06788d6f74b65d258b03fe9cbc2d2fb355bd878f81c491db08fbe5556d6aa220a80b43be927f4bdb41d2d00e8";
        consensus.strSporkPubKeyOld = "04d7437801b20f7d3e585e829088d0773846b80fd06788d6f74b65d258b03fe9cbc2d2fb355bd878f81c491db08fbe5556d6aa220a80b43be927f4bdb41d2d00e8";
        consensus.nTime_EnforceNewSporkKey = 1566860400;    //!> August 26, 2019 11:00:00 PM GMT
        consensus.nTime_RejectOldSporkKey = 1569538800;     //!> September 26, 2019 11:00:00 PM GMT

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 259201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 615800;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 1967000;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 2153200;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = { 'b' };
        pchMessageStart[1] = { 'c' };
        pchMessageStart[2] = { 'z' };
        pchMessageStart[3] = { '0' };
        nDefaultPort = 29500;

        vSeeds.push_back(CDNSSeedData("51.77.145.35", "51.77.145.35"));
        vSeeds.push_back(CDNSSeedData("51.91.156.249", "51.91.156.249"));
        vSeeds.push_back(CDNSSeedData("51.91.156.251", "51.91.156.251"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 63);     // starting with 'S'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 20);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 210);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
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


    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {

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

    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {

    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
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

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
