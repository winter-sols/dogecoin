Contrib Index
---------------------

### [BitRPC](/contrib/bitrpc) ###
Added bitrpc.py which allows for sending of all standard Bitcoin commands via RPC rather than as command line args.

### [Debian](/contrib/debian) ###
Contains files used to package bitcoind/bitcoin-qt
for Debian-based Linux systems. If you compile bitcoind/bitcoin-qt yourself, there are some useful files here.

### [Gitian-descriptors](/contrib/gitian-descriptors) ###
Gavin's notes on getting gitian builds up and running using KVM.

### [Gitian-downloader](/contrib/gitian-downloader)
Various PGP files of core developers. 

### [Linearize](/contrib/linearize) ###
Construct a linear, no-fork, best version of the blockchain.

### [MacDeploy](/contrib/macdeploy) ###
Scripts and notes for Mac builds. 

### [PyMiner](/contrib/pyminer) ###

This is a 'getwork' CPU mining client for Bitcoin. It is pure-python, and therefore very, very slow.  The purpose is to provide a reference implementation of a miner, for study.

### [Qos](/contrib/qos) ###

A Linux bash script that will set up tc to limit the outgoing bandwidth for connections to the Bitcoin network. This means one can have an always-on bitcoind instance running, and another local bitcoind/bitcoin-qt instance which connects to this node and receives blocks from it.

### [Seeds](/contrib/seeds) ###
Utility to generate the pnSeed[] array that is compiled into the client.

### [SpendFrom](/contrib/spendfrom) ###

Use the raw transactions API to send coins received on a particular
address (or addresses).

### [TestGen](/contrib/testgen) ###
Utilities to generate test vectors for the data-driven Bitcoin tests.

### [Test Patches](/contrib/test-patches) ###
These patches are applied when the automated pull-tester
tests each pull and when master is tested using jenkins.

### [Verify SF Binaries](/contrib/verifysfbinaries) ###
This script attempts to download and verify the signature file SHA256SUMS.asc from SourceForge.

### [Wallet Tools](/contrib/wallettools) ###
Contains a wallet change password and unlock script.
