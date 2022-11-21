# fennec/node/

The [`fennec/node/`](./) directory contains code that needs to access node state
(state in `CChain`, `CBlockIndex`, `CCoinsView`, `CTxMemPool`, and similar
classes).

Code in [`fennec/node/`](./) is meant to be segregated from code in
[`fennec/wallet/`](../wallet/) and [`fennec/qt/`](../qt/), to ensure wallet and GUI
code changes don't interfere with node operation, to allow wallet and GUI code
to run in separate processes, and to perhaps eventually allow wallet and GUI
code to be maintained in separate source repositories.

As a rule of thumb, code in one of the [`fennec/node/`](./),
[`fennec/wallet/`](../wallet/), or [`fennec/qt/`](../qt/) directories should avoid
calling code in the other directories directly, and only invoke it indirectly
through the more limited [`fennec/interfaces/`](../interfaces/) classes.

The [`fennec/node/`](./) directory is a new directory introduced in
[#14978](https://github.com/bitcoin/bitcoin/pull/14978) and at the moment is
sparsely populated. Eventually more substantial files like
[`fennec/fennecblockchain.cpp`](../fennecblockchain.cpp) and
[`fennec/txmempool.cpp`](../txmempool.cpp) might be moved there.
