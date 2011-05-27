#include "transactionrecord.h"


/* Return positive answer if transaction should be shown in list.
 */
bool TransactionRecord::showTransaction(const CWalletTx &wtx)
{
    if (wtx.IsCoinBase())
    {
        // Don't show generated coin until confirmed by at least one block after it
        // so we don't get the user's hopes up until it looks like it's probably accepted.
        //
        // It is not an error when generated blocks are not accepted.  By design,
        // some percentage of blocks, like 10% or more, will end up not accepted.
        // This is the normal mechanism by which the network copes with latency.
        //
        // We display regular transactions right away before any confirmation
        // because they can always get into some block eventually.  Generated coins
        // are special because if their block is not accepted, they are not valid.
        //
        if (wtx.GetDepthInMainChain() < 2)
        {
            return false;
        }
    }
    return true;
}

/* Decompose CWallet transaction to model transaction records.
 */
QList<TransactionRecord> TransactionRecord::decomposeTransaction(const CWalletTx &wtx)
{
    QList<TransactionRecord> parts;
    int64 nTime = wtx.nTimeDisplayed = wtx.GetTxTime();
    int64 nCredit = wtx.GetCredit(true);
    int64 nDebit = wtx.GetDebit();
    int64 nNet = nCredit - nDebit;
    uint256 hash = wtx.GetHash();
    std::map<std::string, std::string> mapValue = wtx.mapValue;

    // Find the block the tx is in
    CBlockIndex* pindex = NULL;
    std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(wtx.hashBlock);
    if (mi != mapBlockIndex.end())
        pindex = (*mi).second;

    // Determine transaction status
    TransactionStatus status;
    // Sort order, unrecorded transactions sort to the top
    status.sortKey = strprintf("%010d-%01d-%010u",
        (pindex ? pindex->nHeight : INT_MAX),
        (wtx.IsCoinBase() ? 1 : 0),
        wtx.nTimeReceived);
    status.confirmed = wtx.IsConfirmed();
    status.depth = wtx.GetDepthInMainChain();

    if (!wtx.IsFinal())
    {
        if (wtx.nLockTime < 500000000)
        {
            status.status = TransactionStatus::OpenUntilBlock;
            status.open_for = nBestHeight - wtx.nLockTime;
        } else {
            status.status = TransactionStatus::OpenUntilDate;
            status.open_for = wtx.nLockTime;
        }
    }
    else
    {
        if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0)
        {
            status.status = TransactionStatus::Offline;
        } else if (status.depth < 6)
        {
            status.status = TransactionStatus::Unconfirmed;
        } else
        {
            status.status = TransactionStatus::HaveConfirmations;
        }
    }

    if (showTransaction(wtx))
    {
        if (nNet > 0 || wtx.IsCoinBase())
        {
            //
            // Credit
            //
            TransactionRecord sub(hash, nTime, status);

            sub.credit = nNet;

            if (wtx.IsCoinBase())
            {
                // Generated
                sub.type = TransactionRecord::Generated;

                if (nCredit == 0)
                {
                    sub.status.maturity = TransactionStatus::Immature;

                    int64 nUnmatured = 0;
                    BOOST_FOREACH(const CTxOut& txout, wtx.vout)
                        nUnmatured += txout.GetCredit();
                    sub.credit = nUnmatured;

                    if (wtx.IsInMainChain())
                    {
                        sub.status.maturity = TransactionStatus::MaturesIn;
                        sub.status.matures_in = wtx.GetBlocksToMaturity();

                        // Check if the block was requested by anyone
                        if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0)
                            sub.status.maturity = TransactionStatus::MaturesWarning;
                    }
                    else
                    {
                        sub.status.maturity = TransactionStatus::NotAccepted;
                    }
                }
            }
            else if (!mapValue["from"].empty() || !mapValue["message"].empty())
            {
                // Received by IP connection
                sub.type = TransactionRecord::RecvFromIP;
                if (!mapValue["from"].empty())
                    sub.address = mapValue["from"];
            }
            else
            {
                // Received by Bitcoin Address
                sub.type = TransactionRecord::RecvFromAddress;
                BOOST_FOREACH(const CTxOut& txout, wtx.vout)
                {
                    if (txout.IsMine())
                    {
                        std::vector<unsigned char> vchPubKey;
                        if (ExtractPubKey(txout.scriptPubKey, true, vchPubKey))
                        {
                            sub.address = PubKeyToAddress(vchPubKey);
                        }
                        break;
                    }
                }
            }
            parts.append(sub);
        }
        else
        {
            bool fAllFromMe = true;
            BOOST_FOREACH(const CTxIn& txin, wtx.vin)
                fAllFromMe = fAllFromMe && txin.IsMine();

            bool fAllToMe = true;
            BOOST_FOREACH(const CTxOut& txout, wtx.vout)
                fAllToMe = fAllToMe && txout.IsMine();

            if (fAllFromMe && fAllToMe)
            {
                // Payment to self
                int64 nChange = wtx.GetChange();

                parts.append(TransactionRecord(hash, nTime, status, TransactionRecord::SendToSelf, "",
                                -(nDebit - nChange), nCredit - nChange));
            }
            else if (fAllFromMe)
            {
                //
                // Debit
                //
                int64 nTxFee = nDebit - wtx.GetValueOut();

                for (int nOut = 0; nOut < wtx.vout.size(); nOut++)
                {
                    const CTxOut& txout = wtx.vout[nOut];
                    TransactionRecord sub(hash, nTime, status);

                    if (txout.IsMine())
                    {
                        // Sent to self
                        sub.type = TransactionRecord::SendToSelf;
                        sub.credit = txout.nValue;
                    } else if (!mapValue["to"].empty())
                    {
                        // Sent to IP
                        sub.type = TransactionRecord::SendToIP;
                        sub.address = mapValue["to"];
                    } else {
                        // Sent to Bitcoin Address
                        sub.type = TransactionRecord::SendToAddress;
                        uint160 hash160;
                        if (ExtractHash160(txout.scriptPubKey, hash160))
                            sub.address = Hash160ToAddress(hash160);
                    }

                    int64 nValue = txout.nValue;
                    /* Add fee to first output */
                    if (nTxFee > 0)
                    {
                        nValue += nTxFee;
                        nTxFee = 0;
                    }
                    sub.debit = nValue;
                    sub.status.sortKey += strprintf("-%d", nOut);

                    parts.append(sub);
                }
            } else {
                //
                // Mixed debit transaction, can't break down payees
                //
                bool fAllMine = true;
                BOOST_FOREACH(const CTxOut& txout, wtx.vout)
                    fAllMine = fAllMine && txout.IsMine();
                BOOST_FOREACH(const CTxIn& txin, wtx.vin)
                    fAllMine = fAllMine && txin.IsMine();

                parts.append(TransactionRecord(hash, nTime, status, TransactionRecord::Other, "", nNet, 0));
            }
        }
    }

    return parts;
}
