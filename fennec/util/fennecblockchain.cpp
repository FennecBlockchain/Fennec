// Copyright (c) 2009-2010 Satoshi Nakamoto
// 
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <util/fennecblockchain.h>

#include <consensus/fennecblockchain.h>
#include <tinyformat.h>

/** Convert CValidationState to a human-readable message for logging */
std::string FormatStateMessage(const CValidationState &state)
{
    return strprintf("%s%s (code %i)",
        state.GetRejectReason(),
        state.GetDebugMessage().empty() ? "" : ", "+state.GetDebugMessage(),
        state.GetRejectCode());
}

const std::string strMessageMagic = "Fennec Signed Message:\n";
