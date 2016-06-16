(note: this is a temporary file, to be added-to by anybody, and moved to
release-notes at release time)

Bitcoin Core version *version* is now available from:

  <https://bitcoin.org/bin/bitcoin-core-*version*/>

This is a new major version release, including new features, various bugfixes
and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github:

  <https://github.com/bitcoin/bitcoin/issues>

To receive security and update notifications, please subscribe to:

  <https://bitcoincore.org/en/list/announcements/join/>

Notable changes
===============

Example item
----------------


bitcoin-cli: arguments privacy
--------------------------------

The RPC command line client gained a new argument, `-stdin`
to read extra arguments from standard input, one per line until EOF/Ctrl-D.
For example:

    $ echo -e "mysecretcode\n120" | src/bitcoin-cli -stdin walletpassphrase

It is recommended to use this for sensitive information such as wallet
passphrases, as command-line arguments can usually be read from the process
table by any user on the system.

RPC low-level changes
----------------------

- `gettxoutsetinfo` UTXO hash (`hash_serialized`) has changed. There was a divergence between
  32-bit and 64-bit platforms, and the txids were missing in the hashed data. This has been
  fixed, but this means that the output will be different than from previous versions.

- Full UTF-8 support in the RPC API. Non-ASCII characters in, for example,
  wallet labels have always been malformed because they weren't taken into account
  properly in JSON RPC processing. This is no longer the case. This also affects
  the GUI debug console.

C++11 and Python 3
-------------------

Various code modernizations have been done. The Bitcoin Core code base has
started using C++11. This means that a C++11-capable compiler is now needed for
building. Effectively this means GCC 4.7 or higher, or Clang 3.3 or higher.

When cross-compiling for a target that doesn't have C++11 libraries, configure with
`./configure --enable-glibc-back-compat ... LDFLAGS=-static-libstdc++`.

For running the functional tests in `qa/rpc-tests`, Python3.4 or higher is now
required.

0.13.0 Change log
=================

Detailed release notes follow. This overview includes changes that affect
behavior, not code moves, refactors and string updates. For convenience in locating
the code changes and accompanying discussion, both the pull request and
git merge commit are mentioned.

### RPC and REST

Asm script outputs now contain OP_CHECKLOCKTIMEVERIFY in place of OP_NOP2
-------------------------------------------------------------------------

OP_NOP2 has been renamed to OP_CHECKLOCKTIMEVERIFY by [BIP 
65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki)

The following outputs are affected by this change:
- RPC `getrawtransaction` (in verbose mode)
- RPC `decoderawtransaction`
- RPC `decodescript`
- REST `/rest/tx/` (JSON format)
- REST `/rest/block/` (JSON format when including extended tx details)
- `bitcoin-tx -json`

New mempool information RPC calls
---------------------------------

RPC calls have been added to output detailed statistics for individual mempool
entries, as well as to calculate the in-mempool ancestors or descendants of a
transaction: see `getmempoolentry`, `getmempoolancestors`, `getmempooldescendants`.

### ZMQ

Each ZMQ notification now contains an up-counting sequence number that allows
listeners to detect lost notifications.
The sequence number is always the last element in a multi-part ZMQ notification and
therefore backward compatible.
Each message type has its own counter.
(https://github.com/bitcoin/bitcoin/pull/7762)

### Configuration and command-line options

### Block and transaction handling

### P2P protocol and network code

The p2p alert system has been removed in #7692 and the 'alert' message is no longer supported.


Fee filtering of invs (BIP 133)
------------------------------------

The optional new p2p message "feefilter" is implemented and the protocol
version is bumped to 70013. Upon receiving a feefilter message from a peer,
a node will not send invs for any transactions which do not meet the filter
feerate. [BIP 133](https://github.com/bitcoin/bips/blob/master/bip-0133.mediawiki)

### Validation

### Build system

### Wallet

Hierarchical Deterministic Key Generation
-----------------------------------------
Newly created wallets will use hierarchical deterministic key generation
according to BIP32 (keypath m/0'/0'/k').
Existing wallets will still use traditional key generation.

Backups of HD wallets, regardless of when they have been created, can
therefore be used to re-generate all possible private keys, even the
ones which haven't already been generated during the time of the backup.

HD key generation for new wallets can be disabled by `-usehd=0`. Keep in
mind that this flag only has affect on newly created wallets.
You can't disable HD key generation once you have created a HD wallet.

There is no distinction between internal (change) and external keys.

[Pull request](https://github.com/bitcoin/bitcoin/pull/8035/files), [BIP 32](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki)

### GUI

### Tests

### Miscellaneous

