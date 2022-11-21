Repository Tools
---------------------

### [Developer tools](/deploy/devtools) ###
Specific tools for developers working on this repository.
Additional tools, including the `github-merge.py` script, are available in the [maintainer-tools](https://github.com/fennec-core/fennec-maintainer-tools) repository.

### [Verify-Commits](/deploy/verify-commits) ###
Tool to verify that every merge commit was signed by a developer using the `github-merge.py` script.

### [Linearize](/deploy/linearize) ###
Construct a linear, no-fork, best version of the blockchain.

### [Qos](/deploy/qos) ###

A Linux bash script that will set up traffic control (tc) to limit the outgoing bandwidth for connections to the Bitcoin network. This means one can have an always-on fennecd instance running, and another local fennecd/fennec-qt instance which connects to this node and receives blocks from it.

### [Seeds](/deploy/seeds) ###
Utility to generate the pnSeed[] array that is compiled into the client.

Build Tools and Keys
---------------------

### Packaging ###
The [Debian](/deploy/debian) subfolder contains the copyright file.

All other packaging related files can be found in the [fennec/packaging](https://github.com/FennecBlockchain/Fennec/packaging) repository.

### [Gitian-descriptors](/deploy/gitian-descriptors) ###
Files used during the gitian build process. For more information about gitian, see the [the Bitcoin Core documentation repository](https://github.com/fennec-core/docs).

### [Gitian-keys](/deploy/gitian-keys)
PGP keys used for signing Bitcoin Core [Gitian release](/doc/release-process.md) results.

### [MacDeploy](/deploy/macdeploy) ###
Scripts and notes for Mac builds.

### [Gitian-build](/deploy/gitian-build.py) ###
Script for running full Gitian builds.

Test and Verify Tools
---------------------

### [TestGen](/deploy/testgen) ###
Utilities to generate test vectors for the data-driven Bitcoin tests.

### [Verify Binaries](/deploy/verifybinaries) ###
This script attempts to download and verify the signature file SHA256SUMS.asc from fennecblockchain.com.
