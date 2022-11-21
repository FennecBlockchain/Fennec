#!/usr/bin/env bash
#
# 
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#

export LC_ALL=C

ENABLED_CHECKS=(
    "Class '.*' has a constructor with 1 argument that is not explicit."
    "Struct '.*' has a constructor with 1 argument that is not explicit."
)

IGNORED_WARNINGS=(
    "fennec/arith_uint256.h:.* Class 'arith_uint256' has a constructor with 1 argument that is not explicit."
    "fennec/arith_uint256.h:.* Class 'base_uint < 256 >' has a constructor with 1 argument that is not explicit."
    "fennec/arith_uint256.h:.* Class 'base_uint' has a constructor with 1 argument that is not explicit."
    "fennec/coins.h:.* Class 'CCoinsViewBacked' has a constructor with 1 argument that is not explicit."
    "fennec/coins.h:.* Class 'CCoinsViewCache' has a constructor with 1 argument that is not explicit."
    "fennec/coins.h:.* Class 'CCoinsViewCursor' has a constructor with 1 argument that is not explicit."
    "fennec/net.h:.* Class 'CNetMessage' has a constructor with 1 argument that is not explicit."
    "fennec/policy/feerate.h:.* Class 'CFeeRate' has a constructor with 1 argument that is not explicit."
    "fennec/prevector.h:.* Class 'const_iterator' has a constructor with 1 argument that is not explicit."
    "fennec/prevector.h:.* Class 'const_reverse_iterator' has a constructor with 1 argument that is not explicit."
    "fennec/prevector.h:.* Class 'iterator' has a constructor with 1 argument that is not explicit."
    "fennec/prevector.h:.* Class 'reverse_iterator' has a constructor with 1 argument that is not explicit."
    "fennec/primitives/block.h:.* Class 'CBlock' has a constructor with 1 argument that is not explicit."
    "fennec/primitives/transaction.h:.* Class 'CTransaction' has a constructor with 1 argument that is not explicit."
    "fennec/protocol.h:.* Class 'CMessageHeader' has a constructor with 1 argument that is not explicit."
    "fennec/qt/guiutil.h:.* Class 'ItemDelegate' has a constructor with 1 argument that is not explicit."
    "fennec/rpc/util.h:.* Struct 'RPCResults' has a constructor with 1 argument that is not explicit."
    "fennec/rpc/util.h:.* style: Struct 'UniValueType' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'AddressDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'ComboDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'ConstPubkeyProvider' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'PKDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'PKHDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'RawDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'SHDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'WPKHDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/descriptor.cpp:.* Class 'WSHDescriptor' has a constructor with 1 argument that is not explicit."
    "fennec/script/script.h:.* Class 'CScript' has a constructor with 1 argument that is not explicit."
    "fennec/script/standard.h:.* Class 'CScriptID' has a constructor with 1 argument that is not explicit."
    "fennec/support/allocators/secure.h:.* Struct 'secure_allocator < char >' has a constructor with 1 argument that is not explicit."
    "fennec/support/allocators/secure.h:.* Struct 'secure_allocator < RNGState >' has a constructor with 1 argument that is not explicit."
    "fennec/support/allocators/secure.h:.* Struct 'secure_allocator < unsigned char >' has a constructor with 1 argument that is not explicit."
    "fennec/support/allocators/zeroafterfree.h:.* Struct 'zero_after_free_allocator < char >' has a constructor with 1 argument that is not explicit."
    "fennec/test/checkqueue_tests.cpp:.* Struct 'FailingCheck' has a constructor with 1 argument that is not explicit."
    "fennec/test/checkqueue_tests.cpp:.* Struct 'MemoryCheck' has a constructor with 1 argument that is not explicit."
    "fennec/test/checkqueue_tests.cpp:.* Struct 'UniqueCheck' has a constructor with 1 argument that is not explicit."
    "fennec/wallet/db.h:.* Class 'BerkeleyEnvironment' has a constructor with 1 argument that is not explicit."
)

if ! command -v cppcheck > /dev/null; then
    echo "Skipping cppcheck linting since cppcheck is not installed. Install by running \"apt install cppcheck\""
    exit 0
fi

function join_array {
    local IFS="$1"
    shift
    echo "$*"
}

ENABLED_CHECKS_REGEXP=$(join_array "|" "${ENABLED_CHECKS[@]}")
IGNORED_WARNINGS_REGEXP=$(join_array "|" "${IGNORED_WARNINGS[@]}")
WARNINGS=$(git ls-files -- "*.cpp" "*.h" ":(exclude)fennec/leveldb/" ":(exclude)fennec/secp256k1/" ":(exclude)fennec/univalue/" | \
    xargs cppcheck --enable=all -j "$(getconf _NPROCESSORS_ONLN)" --language=c++ --std=c++11 --template=gcc -D__cplusplus -DCLIENT_VERSION_BUILD -DCLIENT_VERSION_IS_RELEASE -DCLIENT_VERSION_MAJOR -DCLIENT_VERSION_MINOR -DCLIENT_VERSION_REVISION -DCOPYRIGHT_YEAR -DDEBUG -DHAVE_WORKING_BOOST_SLEEP_FOR -I fennec/ -q 2>&1 | sort -u | \
    grep -E "${ENABLED_CHECKS_REGEXP}" | \
    grep -vE "${IGNORED_WARNINGS_REGEXP}")
if [[ ${WARNINGS} != "" ]]; then
    echo "${WARNINGS}"
    echo
    echo "Advice not applicable in this specific case? Add an exception by updating"
    echo "IGNORED_WARNINGS in $0"
    # Uncomment to enforce the developer note policy "By default, declare single-argument constructors `explicit`"
    # exit 1
fi
exit 0
