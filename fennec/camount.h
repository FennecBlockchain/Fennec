// Copyright (c) 2009-2010 Satoshi Nakamoto
// Fennec Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AMOUNT_H
#define BITCOIN_AMOUNT_H

#include <stdint.h>

typedef int64_t CAmount;

static const CAmount COIN = 100000000;

static const CAmount MAX_MONEY = 21000000 * COIN;
inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

#endif //  BITCOIN_AMOUNT_H
