/**
'########:'########:'##::: ##:'##::: ##:'########::'######:::::'########::'##::::::::'#######:::'######::'##:::'##::'######::'##::::'##::::'###::::'####:'##::: ##:
 ##.....:: ##.....:: ###:: ##: ###:: ##: ##.....::'##... ##:::: ##.... ##: ##:::::::'##.... ##:'##... ##: ##::'##::'##... ##: ##:::: ##:::'## ##:::. ##:: ###:: ##:
 ##::::::: ##::::::: ####: ##: ####: ##: ##::::::: ##:::..::::: ##:::: ##: ##::::::: ##:::: ##: ##:::..:: ##:'##::: ##:::..:: ##:::: ##::'##:. ##::: ##:: ####: ##:
 ######::: ######::: ## ## ##: ## ## ##: ######::: ##:::::::::: ########:: ##::::::: ##:::: ##: ##::::::: #####:::: ##::::::: #########:'##:::. ##:: ##:: ## ## ##:
 ##...:::: ##...:::: ##. ####: ##. ####: ##...:::: ##:::::::::: ##.... ##: ##::::::: ##:::: ##: ##::::::: ##. ##::: ##::::::: ##.... ##: #########:: ##:: ##. ####:
 ##::::::: ##::::::: ##:. ###: ##:. ###: ##::::::: ##::: ##:::: ##:::: ##: ##::::::: ##:::: ##: ##::: ##: ##:. ##:: ##::: ##: ##:::: ##: ##.... ##:: ##:: ##:. ###:
 ##::::::: ########: ##::. ##: ##::. ##: ########:. ######::::: ########:: ########:. #######::. ######:: ##::. ##:. ######:: ##:::: ##: ##:::: ##:'####: ##::. ##:
..::::::::........::..::::..::..::::..::........:::......::::::........:::........:::.......::::......:::..::::..:::......:::..:::::..::..:::::..::....::..::::..::
*/


// Copyright (c) 2010 Satoshi Nakamoto
// Fennec Developers - TylerAnderson T.A
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <fennec.h>

#include <fennecseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


#include "uint256.h"
#include "arith_uint256.h"
#include "hash.h"
#include "streams.h"
#include <time.h>



static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}


static CBlock CreateGenesisBlockMainNet(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Founded November 21st, 2022";
    const CScript genesisOutputScript = CScript() << ParseHex("04cfd3e005016735e8bd7c4280e854a746744b5757011bec994f621cf77ccfb157e5bcce3e14a76f4a88080ddc45bb01cf05848a4e05e412f4ff8b032013538e1e") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateGenesisBlockTestNet(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Founded November 21st, 2022";
    const CScript genesisOutputScript = CScript() << ParseHex("0455e6a7f6f18d161b7769145fab887d6ced3a45b852245c58eb276f59eae9f928c332cb71f61d9d7d6b86d8d2f0837cc019a3360061ab511e4ff58086105a3c6d") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateGenesisBlockRegTest(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Founded November 21st, 2022";
    const CScript genesisOutputScript = CScript() << ParseHex("049bc36749883b688292f1118333c2b2c220931266a450f41a73f11edd289d95dcee4e42c1c91ceddec1dbd1e3fc3d357ca72b0ab288e09dfbc5764980e1908247") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


/**
'##::::'##::::'###::::'####:'##::: ##:'##::: ##:'########:'########:
 ###::'###:::'## ##:::. ##:: ###:: ##: ###:: ##: ##.....::... ##..::
 ####'####::'##:. ##::: ##:: ####: ##: ####: ##: ##:::::::::: ##::::
 ## ### ##:'##:::. ##:: ##:: ## ## ##: ## ## ##: ######:::::: ##::::
 ##. #: ##: #########:: ##:: ##. ####: ##. ####: ##...::::::: ##::::
 ##:.:: ##: ##.... ##:: ##:: ##:. ###: ##:. ###: ##:::::::::: ##::::
 ##:::: ##: ##:::: ##:'####: ##::. ##: ##::. ##: ########:::: ##::::
..:::::..::..:::::..::....::..::::..::..::::..::........:::::..:::::
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1176055;
        consensus.BIP16Exception = uint256S("0x7834578baec6d4a5457d80bc1ce707d90a110ed7459ab2a5eecec681b1980c65");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x7834578baec6d4a5457d80bc1ce707d90a110ed7459ab2a5eecec681b1980c65");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0; 
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2.5 * 60;
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowTRDHeight = 0;
        consensus.nRuleChangeActivationThreshold = 3600;
        consensus.nMinerConfirmationWindow = 4800;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000009f6935dd20");
        consensus.defaultAssumeValid = uint256S("0x3aa3e0845f03c4ab8644f9892a3012a277842e3a508bf3ce91f8cb14f3940376");

        pchMessageStart[0] = 0xb1;
        pchMessageStart[1] = 0xb1;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0xb1;
        nDefaultPort = 8338;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlockMainNet(1669000888, 4935, 0x20000fff, 1, 6.25 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7834578baec6d4a5457d80bc1ce707d90a110ed7459ab2a5eecec681b1980c65"));
        assert(genesis.hashMerkleRoot == uint256S("0xb09c67dc55c54f73e9b1f5ebd37bdcc06fdc8dd0533f52c3b7b0158362408cc9"));


        vSeeds.emplace_back("seed1.fennecblockchain.com");
        vSeeds.emplace_back("seed2.fennecblockchain.com");
        vSeeds.emplace_back("seed3.fennecblockchain.com");
        vSeeds.emplace_back("seed4.fennecblockchain.com");
        vSeeds.emplace_back("seed5.fennecblockchain.com");
        vSeeds.emplace_back("seed6.fennecblockchain.com");


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,35);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,36);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xF4};

        bech32_hrp = "fennec";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;

        checkpointData = {
            {
                { 0, uint256S("0x7834578baec6d4a5457d80bc1ce707d90a110ed7459ab2a5eecec681b1980c65")},
                {1, uint256S("0xafd6d4cd59066f5d35f33a1ae7adc089a0399cfbcacfd3627b5d27ce43b4917a")},
                {2, uint256S("0xd11abc073734adf145a8487b6f9e747cb56a93c597913482be2357652797d062")},
                {10, uint256S("0xb7fd0be5304e0f7a9b2eaec266da1102c9bcdf4558443dea652627719f453874")},
                {45, uint256S("0x49175470369fcc41e8a0430f1e3af1e523c0cb4b3551ec1b4cd36bc52e64be24")},
                {100, uint256S("0x93bbc1a13fac281358ca5471b0ae9c08e18e107b1da1ebf3bd64ac5fb56bc817")},
                {101, uint256S("0x77a09c409bf10d0cccdd2554a1dbbc2e617ff6ae1f2b3ce688e83f36f3b380a0")},
                {102, uint256S("0x7c950f9a0e1932157d40b0269c3b4f68224d8301db0b0b3721e37b35ef488da1")},
                {232, uint256S("0xff5e5181da3f696a474048fc033c54158d51dc30360fd5ec2df3ce60eff5fe96")},
                {345, uint256S("0xff69c121d000d2749777c3e009a96adbe68ad4e1475926539400a6e6b954fcda")},
                {684, uint256S("0x6f590bf83581ebd196dd685a2b808bc42747235ad3077dfe2f48df682969d38a")},
                {1312, uint256S("0x3aa3e0845f03c4ab8644f9892a3012a277842e3a508bf3ce91f8cb14f3940376")},
            }
        };

        chainTxData = ChainTxData{ 1669237456, 1483, 0.00730049};
    }
};


/**
'########:'########::'######::'########:'##::: ##:'########:'########:
... ##..:: ##.....::'##... ##:... ##..:: ###:: ##: ##.....::... ##..::
::: ##:::: ##::::::: ##:::..::::: ##:::: ####: ##: ##:::::::::: ##::::
::: ##:::: ######:::. ######::::: ##:::: ## ## ##: ######:::::: ##::::
::: ##:::: ##...:::::..... ##:::: ##:::: ##. ####: ##...::::::: ##::::
::: ##:::: ##:::::::'##::: ##:::: ##:::: ##:. ###: ##:::::::::: ##::::
::: ##:::: ########:. ######::::: ##:::: ##::. ##: ########:::: ##::::
:::..:::::........:::......::::::..:::::..::::..::........:::::..:::::
 */

class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 200000;
        consensus.BIP16Exception = uint256S("0x4dc996d10092dfb9928d08be2dec761b2ebcb1079ad426e7af45d57d3a9cb688");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x4dc996d10092dfb9928d08be2dec761b2ebcb1079ad426e7af45d57d3a9cb688");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2.5 * 60;
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 180;
        consensus.nMinerConfirmationWindow = 240;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000002001");
        consensus.defaultAssumeValid = uint256S("0xfdb993479c7c048e5a4d7006268295fe86f763808295713c44088b731aa17811");

        pchMessageStart[0] = 0xa8;
        pchMessageStart[1] = 0xa8;
        pchMessageStart[2] = 0xa8;
        pchMessageStart[3] = 0xa8;
        nDefaultPort = 18338;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 15;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlockTestNet(1669000887, 11103, 0x20000fff, 1, 6.25 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x4dc996d10092dfb9928d08be2dec761b2ebcb1079ad426e7af45d57d3a9cb688"));
        assert(genesis.hashMerkleRoot == uint256S("0x5a835c1ec5e1c35670716224a71a7187238e0878f0770ff5703e802512ca97cf"));

        vFixedSeeds.clear();
        vSeeds.clear();

      //  vSeeds.emplace_back("");


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,65);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,140);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x07, 0x45, 0x83, 0xCD};
        base58Prefixes[EXT_SECRET_KEY] = {0x07, 0x45, 0x87, 0x98};

        bech32_hrp = "tfennec";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;


        checkpointData = {
            {
                {0, uint256S("0x4dc996d10092dfb9928d08be2dec761b2ebcb1079ad426e7af45d57d3a9cb688")},
      //          {1, uint256S("0xfdb993479c7c048e5a4d7006268295fe86f763808295713c44088b731aa17811")},
      //          {2, uint256S("0x1fccae08251b0e9349688ea5b1744f333b18a09c873f519efc2003540810a875")},
            }
        };

        chainTxData = ChainTxData{ 1669000887, 0, 0.00000};
    }
};


/**
'########::'########::'######:::'########:'########::'######::'########:
 ##.... ##: ##.....::'##... ##::... ##..:: ##.....::'##... ##:... ##..::
 ##:::: ##: ##::::::: ##:::..:::::: ##:::: ##::::::: ##:::..::::: ##::::
 ########:: ######::: ##::'####:::: ##:::: ######:::. ######::::: ##::::
 ##.. ##::: ##...:::: ##::: ##::::: ##:::: ##...:::::..... ##:::: ##::::
 ##::. ##:: ##::::::: ##::: ##::::: ##:::: ##:::::::'##::: ##:::: ##::::
 ##:::. ##: ########:. ######:::::: ##:::: ########:. ######::::: ##::::
..:::::..::........:::......:::::::..:::::........:::......::::::..:::::
 */

class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 0;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 0.25 * 60;
        consensus.nPowTargetSpacing = 0.25 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108;
        consensus.nMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000200020");
        consensus.defaultAssumeValid = uint256S("0xa51fd3ec622c97e083571a5f3347875ee5a44cfade6c189e1c933b771e34ecf1");

        pchMessageStart[0] = 0xa6;
        pchMessageStart[1] = 0xa6;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0xa6;
        nDefaultPort = 28338;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        genesis = CreateGenesisBlockRegTest(1669000886, 0, 0x207fffff, 1, 6.25 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
   //     assert(consensus.hashGenesisBlock == uint256S("0xa51fd3ec622c97e083571a5f3347875ee5a44cfade6c189e1c933b771e34ecf1"));
  //      assert(genesis.hashMerkleRoot == uint256S("0x0f266ce2d9d762fd4b004c66f7012bf39c92ead9773ce478efaf8c1b8f30116d"));

        vFixedSeeds.clear();
        vSeeds.clear();

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {
            {
                {0, uint256S("0xa51fd3ec622c97e083571a5f3347875ee5a44cfade6c189e1c933b771e34ecf1")},
            }
        };

        chainTxData = ChainTxData{ 1666411203, 0, 0.00000};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,61);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,60);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,121);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCB};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x92};

        bech32_hrp = "rfennec";
    }

    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
