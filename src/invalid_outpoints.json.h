// Copyright (c) 2018 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BCZ_INVALID_OUTPOINTS_JSON_H
#define BCZ_INVALID_OUTPOINTS_JSON_H
#include <string>

std::string LoadInvalidOutPoints()
{
    std::string str = "[\n"
                      "  {\n"
                      "    \"txid\": \"000\",\n"
                      "    \"n\": 1\n"
                      "  },\n"
                      "  {\n"
                      "    \"txid\": \"000\",\n"
                      "    \"n\": 0\n"
                      "  }\n"
                      "]";
    return str;
}

#endif //BCZ_INVALID_OUTPOINTS_JSON_H
