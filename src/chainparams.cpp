// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The GermanCC Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>
#include "chainparamsseeds.h"
#include "arith_uint256.h"

extern bool fPrintToConsole;




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
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Lächerlich! Der Schiri hat Geschenke verteilt";
    const CScript genesisOutputScript = CScript() << ParseHex("04d9ce6c0611fad1c6b6cb156ed2c73363f61593eb4169510343581a3d285c8182ba5622eb265de576eabe10913c19a6d3381f3612b8e1c7f7dce02fa21ebff0df") << OP_CHECKSIG;
    														 //04d9ce6c0611fad1c6b6cb156ed2c73363f61593eb4169510343581a3d285c8182ba5622eb265de576eabe10913c19a6d3381f3612b8e1c7f7dce02fa21ebff0df
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

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


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nBlockRewardInit = 100; // 100 COINS
        consensus.nPremineCoins=30000000;  // Blockreward firstblock
        consensus.nBlocksPerDay = 1000;
        consensus.nSubsidyHalvingInterval = consensus.nBlocksPerDay * 352; // 1 year maxmoney = 70000000 + 30000000 (premine)
        consensus.nMasternodeCoins=100000; // 100000 Coins für Masternode
        consensus.nMasternodePaymentsStartBlock = 10;//
        consensus.nMasternodeFixPaymentsPercent = 50;// 0 = disable fix payments use proto ...
        consensus.nMasternodePaymentsIncreaseBlock = consensus.nBlocksPerDay * 30;// 1 month
        consensus.nMasternodePaymentsIncreasePeriod = consensus.nBlocksPerDay * 30;// 1 month
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsBlockSharePercent = 0; // 0 disable budget
        consensus.nBudgetProposalEstablishingTime = 60*60*24;// 1 day
        consensus.nSuperblockStartBlock = 10;//1;
        consensus.nSuperblockCycle = consensus.nBlocksPerDay * 30; // 1 month
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15; //15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00");
        consensus.powLimit   =  uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        //consensus.powLimit =  uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4*60*60; //  4hours
        consensus.nPowTargetSpacing = 24*60*60 / consensus.nBlocksPerDay; //~90 sec
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nMinerConfirmationWindow = consensus.nPowTargetTimespan / consensus.nPowTargetSpacing;
        consensus.nRuleChangeActivationThreshold = consensus.nMinerConfirmationWindow * 75 / 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1508025600; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout =   1571090400; // Oct 15th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0x00000be21196755f811b7a7c078002954b8774ec4229bf9b66f730bf8fc26eef");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x9f;
        pchMessageStart[1] = 0xeb;
        pchMessageStart[2] = 0x7a;
        pchMessageStart[3] = 0xad;

        vAlertPubKey =   ParseHex("04fc027a938ff12a421a4aac9d84e8830938ba89bc8b8f807d802438037ad47be3ea72f0c9a9d03c7c3267e14fe786addf9e610932767bfe3f1b2de8e6c0141eab");
        						 //04fc027a938ff12a421a4aac9d84e8830938ba89bc8b8f807d802438037ad47be3ea72f0c9a9d03c7c3267e14fe786addf9e610932767bfe3f1b2de8e6c0141eab
        nDefaultPort = 9966;
        nMaxTipAge = 24*60*60; // 1day
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1529145167,37783,0x1f00ffff,1,consensus.nBlockRewardInit * COIN);//
        if (false) // genesis block miner
        	{
            bool fNegative;
            bool fOverflow;
            arith_uint256 bnTarget;
            bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
        	fPrintToConsole = true; // no debug.log at this state

        	if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(consensus.powLimit))
        		LogPrintf("nBits error \n");
        	else
        		{

        			// mining genesis block cut and paste nonce, hash and merkle
        			LogPrintf(" main: mining genesis block wait \n");
        			LogPrintf(" main: powlimit %u nbit_target %u \n",UintToArith256(consensus.powLimit).GetCompact(),bnTarget.GetCompact());
                    for(genesis.nNonce = 0; UintToArith256(genesis.GetHash()) > bnTarget ; genesis.nNonce++)
                    {
                    	//LogPrintf(" nonce %u genesishash %u target %u \n",genesis.nNonce,UintToArith256(genesis.GetHash()).GetCompact(),bnTarget.GetCompact());
                    }
        			LogPrintf(" main nonce: %u \n", genesis.nNonce);
                    LogPrintf(" main time: %u \n", genesis.nTime);
                    LogPrintf(" main hashgenesis: %s \n", genesis.GetHash().ToString());
                    LogPrintf(" main hashmerkle: %s \n", genesis.hashMerkleRoot.ToString());
        		}
            fPrintToConsole = false;
        	}

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000be21196755f811b7a7c078002954b8774ec4229bf9b66f730bf8fc26eef"));
        assert(genesis.hashMerkleRoot ==     uint256S("0x3a7b22b20dad056ae67daba51192732ad17b35b2702191e232677da303730745 "));


        vSeeds.clear();
        vFixedSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("germancc.org", "dnsseed.germancc.org"));
        //vSeeds.push_back(CDNSSeedData("germanccdot.io", "dnsseed.germanccdot.io"));
        //vSeeds.push_back(CDNSSeedData("germanccpay.io", "dnsseed.germanccpay.io"));


        // GermanCC addresses start
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38); // wallet address beginn's with G....
        // GermanCC script addresses start
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,14);
        // GermanCC private keys start
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,123);
        // GermanCC BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // GermanCC BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // GermanCC BIP44 coin type is '5'
        nExtCoinType = 5;
        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
                strSporkPubKey = "04957b06fc4abafe56faf888c9faa48222a02f3c978ed6757d9aa044bd7535461e0c11a4923dd79b771f5a1132fe3648a62092020b45ae00ed92b9551ebc8d4502";
                			   //"04d8abf3ef3056240b22465cc80fd9dbba5078616bbe10ab869ed777beeb207c26562bcc6a1915f9259f1460a58c9b594bcc13c17ae36b009997c4b919894251c9";
        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of

            ( 0, uint256S("0x00000be21196755f811b7a7c078002954b8774ec4229bf9b66f730bf8fc26eef")),
			1529145167, // UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
                     //   (the tx=... number in the SetBestChain debug.log lines)
            1000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nBlockRewardInit = 100; // 100 COINS
        consensus.nPremineCoins = 0;  // Blockreward firstblock
        consensus.nBlocksPerDay = 1000;
        consensus.nSubsidyHalvingInterval = consensus.nBlocksPerDay * 352; // 1 year ~maxmoney = SubsidyHalvingInterval * nBlockRewardInit * 2 + nPremineCoin
        consensus.nMasternodeCoins=1500; // 1500 Coins für Masternode
        consensus.nMasternodePaymentsStartBlock = 1;//
        consensus.nMasternodeFixPaymentsPercent = 0;// 0 = disable
        consensus.nMasternodePaymentsIncreaseBlock = consensus.nBlocksPerDay * 30;// 1 month
        consensus.nMasternodePaymentsIncreasePeriod = consensus.nBlocksPerDay * 30;// 1 month
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nBudgetPaymentsBlockSharePercent = 1;// 0-100
        consensus.nSuperblockStartBlock = 1;
        consensus.nSuperblockCycle = 40; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00");
        consensus.powLimit =  uint256S("007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4*60*60; //  4hours
        consensus.nPowTargetSpacing = 24*60*60 / consensus.nBlocksPerDay; //~90 sec
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // 0

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x004d9ecdf6f6b5d77463e8feb6c7fde4926f56e837ad2cd957c75bca6e0b3f5f"); // 0

        pchMessageStart[0] = 0x97;
        pchMessageStart[1] = 0xe4;
        pchMessageStart[2] = 0x77;
        pchMessageStart[3] = 0x93;
        vAlertPubKey = ParseHex("04d9ce6c0611fad1c6b6cb156ed2c73363f61593eb4169510343581a3d285c8182ba5622eb265de576eabe10913c19a6d3381f3612b8e1c7f7dce02fa21ebff0df");
                               //04fc027a938ff12a421a4aac9d84e8830938ba89bc8b8f807d802438037ad47be3ea72f0c9a9d03c7c3267e14fe786addf9e610932767bfe3f1b2de8e6c0141eab
        nDefaultPort = 19966;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1528103493,82,0x1f00ffff,1,consensus.nBlockRewardInit * COIN);
        if (false) // genesis block miner
        	{
            bool fNegative;
            bool fOverflow;
            arith_uint256 bnTarget;
            bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
        	fPrintToConsole = true; // no debug.log at this state

        	if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(consensus.powLimit))
        		LogPrintf("nBits error \n");
        	else
        		{

        			// mining genesis block cut and paste nonce, hash and merkle
        			LogPrintf(" test: mining genesis block wait \n");
        			LogPrintf(" test: powlimit %u nbit_target %u \n",UintToArith256(consensus.powLimit).GetCompact(),bnTarget.GetCompact());
                    //for(genesis.nNonce = 0; UintToArith256(genesis.GetHash()) > bnTarget ; genesis.nNonce++) {}
        			for(genesis.nNonce = 0; UintToArith256(genesis.GetHash()) > UintToArith256(consensus.powLimit) ; genesis.nNonce++) {} // powallowmindiff = true
                    LogPrintf(" test: nonce: %u \n", genesis.nNonce);
                    LogPrintf(" test: time: %u \n", genesis.nTime);
                    LogPrintf(" test; hashgenesis: %s \n", genesis.GetHash().ToString());
                    LogPrintf(" test: hashmerkle: %s \n", genesis.hashMerkleRoot.ToString());
        		}
            fPrintToConsole = false;
        	}

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x004d9ecdf6f6b5d77463e8feb6c7fde4926f56e837ad2cd957c75bca6e0b3f5f"));
        assert(genesis.hashMerkleRoot ==     uint256S("0x3a7b22b20dad056ae67daba51192732ad17b35b2702191e232677da303730745"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("germanccdot.io",  "testnet-seed.germanccdot.io"));
        //vSeeds.push_back(CDNSSeedData("masternode.io", "test.dnsseed.masternode.io"));

        // Testnet GermanCC addresses start
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,117);
        // Testnet GermanCC script addresses start
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,199);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,246);
        // Testnet GermanCC BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet GermanCC BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet GermanCC BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04b21edb07c2fcfb9adc144c8e0f6a19c4dc35dfd4abbdcf5d5272254fd58f1d86b24a644381e6661b29ba22639945417efe0890df05dc2fe01f0fd023fa445695";
                       //"046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of

            (   0, uint256S("0x004d9ecdf6f6b5d77463e8feb6c7fde4926f56e837ad2cd957c75bca6e0b3f5f")),

			1528103493, //UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                   //   (the tx=... number in the SetBestChain debug.log lines)
            10     // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBlockRewardInit = 100; // 100 COINS
        consensus.nPremineCoins=0;  // Blockreward firstblock
        consensus.nBlocksPerDay = 1000;
        consensus.nSubsidyHalvingInterval = consensus.nBlocksPerDay * 352; // 1 year ~maxmoney = SubsidyHalvingInterval * nBlockRewardInit * 2 + nPremineCoin
        consensus.nMasternodeCoins=1500; // 1500 Coins für Masternode
        consensus.nMasternodePaymentsStartBlock = 1;//
        consensus.nMasternodeFixPaymentsPercent = 0;// 0 = disable fix payments use proto ...
        consensus.nMasternodePaymentsIncreaseBlock = consensus.nBlocksPerDay * 30;// 1 month
        consensus.nMasternodePaymentsIncreasePeriod = consensus.nBlocksPerDay * 30;// 1 month
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsBlockSharePercent = 1;// 0-100
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4*60*60; //  4hours
        consensus.nPowTargetSpacing = 24*60*60 / consensus.nBlocksPerDay; //~90 sec
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x39d034bf812ec7e44d9fe91ad50bf7cd7410aa6fc61e16b75429bea146940dcb");

        pchMessageStart[0] = 0xad;
        pchMessageStart[1] = 0x5b;
        pchMessageStart[2] = 0xfb;
        pchMessageStart[3] = 0xaf;
        nMaxTipAge = 24 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 8335;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1528103493,0,0x1f00ffff,1,consensus.nBlockRewardInit * COIN);
        if (false) // genesis block miner
        	{
            bool fNegative;
            bool fOverflow;
            arith_uint256 bnTarget;
            bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
        	fPrintToConsole = true; // no debug.log at this state

        	if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(consensus.powLimit))
        		LogPrintf(" reg: nBits error \n");
        	else
        		{

        			// mining genesis block cut and paste nonce, hash and merkle
        			LogPrintf(" reg: mining genesis block wait \n");
        			LogPrintf(" reg: powlimit %u nbit_target %u \n",UintToArith256(consensus.powLimit).GetCompact(),bnTarget.GetCompact());
                    //for(genesis.nNonce = 0; UintToArith256(genesis.GetHash()) > bnTarget ; genesis.nNonce++) {}
        			for(genesis.nNonce = 0; UintToArith256(genesis.GetHash()) > UintToArith256(consensus.powLimit) ; genesis.nNonce++) {} // powallowmindiff = true
                    LogPrintf(" reg: nonce: %u \n", genesis.nNonce);
                    LogPrintf(" reg: time: %u \n", genesis.nTime);
                    LogPrintf(" reg: hashgenesis: %s \n", genesis.GetHash().ToString());
                    LogPrintf(" reg: hashmerkle: %s \n", genesis.hashMerkleRoot.ToString());
        		}
            fPrintToConsole = false;
        	}

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x39d034bf812ec7e44d9fe91ad50bf7cd7410aa6fc61e16b75429bea146940dcb"));
        assert(genesis.hashMerkleRoot == uint256S("0x3a7b22b20dad056ae67daba51192732ad17b35b2702191e232677da303730745"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x39d034bf812ec7e44d9fe91ad50bf7cd7410aa6fc61e16b75429bea146940dcb")),
			1528103493,
            0,
            0
        };
        // Regtest GermanCC addresses start
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,141);
        // Regtest GermanCC script addresses start
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,191);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,238);
        // Regtest GermanCC BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest GermanCC BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest GermanCC BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
