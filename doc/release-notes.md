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

Compatibility
==============

Bitcoin Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows Vista and later.

Microsoft ended support for Windows XP on [April 8th, 2014](https://www.microsoft.com/en-us/WindowsForBusiness/end-of-xp-support).
No attempt is made to prevent installing or running the software on Windows XP, you
can still do so at your own risk but be aware that there are known instabilities.
Please do not report issues about Windows XP to the issue tracker.

Bitcoin Core should also work on most other Unix-like systems but is not
frequently tested on them.

Notable changes
===============

Low-level RPC changes
----------------------

- `importprunedfunds` only accepts two required arguments. Some versions accept
  an optional third arg, which was always ignored. Make sure to never pass more
  than two arguments.

Removal of Priority Estimation
------------------------------

- Estimation of "priority" needed for a transaction to be included within a target
  number of blocks has been removed.  The rpc calls are deprecated and will either
  return -1 or 1e24 appropriately. The format for `fee_estimates.dat` has also
  changed to no longer save these priority estimates. It will automatically be
  converted to the new format which is not readable by prior versions of the
  software.

- The concept of "priority" transactions is planned to be removed in the next
  major version. To prepare for this, the default for the rate limit of priority
  transactions (`-limitfreerelay`) has been set to `0` kB/minute.

P2P connection management
--------------------------

- Peers manually added through the addnode option or addnode RPC now have their own
  limit of eight connections which does not compete with other inbound or outbound
  connection usage and is not subject to the maxconnections limitation.

- New connections to manually added peers are much faster.


0.14.0 Change log
=================

Detailed release notes follow. This overview includes changes that affect
behavior, not code moves, refactors and string updates. For convenience in locating
the code changes and accompanying discussion, both the pull request and
git merge commit are mentioned.

### RPC and REST

UTXO set query (`GET /rest/getutxos/<checkmempool>/<txid>-<n>/<txid>-<n>/.../<txid>-<n>.<bin|hex|json>`) responses
were changed to return status code HTTP_BAD_REQUEST (400) instead of HTTP_INTERNAL_SERVER_ERROR (500) when requests
contain invalid parameters.

The first boolean argument to `getaddednodeinfo` has been removed. This is an incompatible change.

Call "getmininginfo" loses the "testnet" field in favor of the more generic "chain" (which has been present for years).

### Configuration and command-line options

### Block and transaction handling

### P2P protocol and network code

### Validation

### Build system

### Wallet

### GUI

### Tests

### Miscellaneous

Credits
=======

Thanks to everyone who directly contributed to this release:


As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/p/bitcoin/).
