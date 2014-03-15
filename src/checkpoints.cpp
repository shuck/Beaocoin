// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of // Yo dawg, this is the secret. Checkpoint 0 hash == Genesis block hash.
        (         0, uint256("0xca6228129efc24e9ddc80de984e46d1eb01bdff08bc5d2da15d8392da826ed2c"))
        (         9, uint256("0xdb91faa7d06b2ef2c95f8d79a1c841d25ec875b0a29a1d7b8e85e21b954d41d0"))
        (        99, uint256("0xa30750e70999a6fd776c97625006625b04ffd37b9727bde60724537a3a57c3f7"))
        (       999, uint256("0x058f08095d567f63dd7f60d0d1ecf6a399756f74323cddbe918e109a96f6d094"))
        (      7999, uint256("0xc0d9bca06fc7c5c0739c7f5f8cf84336987264a22d25cf3a54fe07fb70af71ac"))
        (      30000, uint256("0x772d0d3e979da89d859f72000c77c607919761c7e962b6fe2ce9f6489a37f689"))
        (      59500, uint256("0x67b8cd3ea0a1984b10c959423fcda66d315471b263b185f1d2544345b18594e6"))
        ;

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
        return mapCheckpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
