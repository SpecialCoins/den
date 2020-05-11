// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORKID_H
#define SPORKID_H

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/

enum SporkId : int32_t {
    SPORK_2_SWIFTTX                             = 10002,
    SPORK_3_SWIFTTX_BLOCK_FILTERING             = 10003,
    SPORK_5_MAX_VALUE                           = 10005,
    SPORK_20_ZERO_FEES                          = 10020,
    SPORK_21_MASTERNODE_PAYMENT_ENFORCEMENT     = 10021,
    SPORK_22_MASTERNODE_PAYMENT                 = 10022,
    SPORK_23_F_PAYMENT                          = 10023,
    SPORK_24_F_PAYMENT_ENFORCEMENT              = 10024,
    SPORK_25_BLOCK_V5                           = 10025,
    SPORK_26_COLDSTAKING_ENFORCEMENT            = 10026,
    SPORK_27_NODE_V_NEW                         = 10027,
    SPORK_28_MIN_STAKING                        = 10028,
    SPORK_29_NODE_V_NEW2                        = 10029,
    SPORK_30_SAFETY_KILL                        = 10030,
    SPORK_31_MN_FIX                             = 10031,
    SPORK_INVALID                               = -1
};

// Default values
struct CSporkDef
{
    CSporkDef(): sporkId(SPORK_INVALID), defaultValue(0) {}
    CSporkDef(SporkId id, int64_t val, std::string n): sporkId(id), defaultValue(val), name(n) {}
    SporkId sporkId;
    int64_t defaultValue;
    std::string name;
};

#endif
