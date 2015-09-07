// Copyright (c) 2012-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "leveldbwrapper.h"
#include "uint256.h"
#include "random.h"
#include "test/test_bitcoin.h"

#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>
                    
using namespace std;
using namespace boost::assign; // bring 'operator+=()' into scope
using namespace boost::filesystem;
         
// Test if a string consists entirely of null characters
bool is_null_key(const vector<unsigned char>& key) {
    bool isnull = true;

    for (unsigned int i = 0; i < key.size(); i++)
        isnull &= (key[i] == '\x00');

    return isnull;
}
 
BOOST_FIXTURE_TEST_SUITE(leveldbwrapper_tests, BasicTestingSetup)
                       
BOOST_AUTO_TEST_CASE(leveldbwrapper)
{
    // Perform tests both obfuscated and non-obfuscated.
    for (int i = 0; i < 2; i++) {
        bool obfuscate = (bool)i;
        path ph = temp_directory_path() / unique_path();
        CLevelDBWrapper dbw(ph, (1 << 20), true, false, obfuscate);
        char key = 'k';
        uint256 in = GetRandHash();
        uint256 res;

        // Ensure that we're doing real obfuscation when obfuscate=true
        BOOST_CHECK(obfuscate != is_null_key(dbw.GetObfuscateKey()));

        BOOST_CHECK(dbw.Write(key, in));
        BOOST_CHECK(dbw.Read(key, res));
        BOOST_CHECK_EQUAL(res.ToString(), in.ToString());
    }
}
                       
// Test that we do not obfuscation if there is existing data.
BOOST_AUTO_TEST_CASE(existing_data_no_obfuscate)
{
    // We're going to share this path between two wrappers
    path ph = temp_directory_path() / unique_path();
    create_directories(ph);

    // Set up a non-obfuscated wrapper to write some initial data.
    CLevelDBWrapper* dbw = new CLevelDBWrapper(ph, (1 << 10), false, false, false);
    char key = 'k';
    uint256 in = GetRandHash();
    uint256 res;

    BOOST_CHECK(dbw->Write(key, in));
    BOOST_CHECK(dbw->Read(key, res));
    BOOST_CHECK_EQUAL(res.ToString(), in.ToString());

    // Call the destructor to free leveldb LOCK
    delete dbw;

    // Now, set up another wrapper that wants to obfuscate the same directory
    CLevelDBWrapper odbw(ph, (1 << 10), false, false, true);

    // Check that the key/val we wrote with unobfuscated wrapper exists and 
    // is readable.
    uint256 res2;
    BOOST_CHECK(odbw.Read(key, res2));
    BOOST_CHECK_EQUAL(res2.ToString(), in.ToString());

    BOOST_CHECK(!odbw.IsEmpty()); // There should be existing data
    BOOST_CHECK(is_null_key(odbw.GetObfuscateKey())); // The key should be an empty string

    uint256 in2 = GetRandHash();
    uint256 res3;
 
    // Check that we can write successfully
    BOOST_CHECK(odbw.Write(key, in2));
    BOOST_CHECK(odbw.Read(key, res3));
    BOOST_CHECK_EQUAL(res3.ToString(), in2.ToString());
}
                        
// Ensure that we start obfuscating during a reindex.
BOOST_AUTO_TEST_CASE(existing_data_reindex)
{
    // We're going to share this path between two wrappers
    path ph = temp_directory_path() / unique_path();
    create_directories(ph);

    // Set up a non-obfuscated wrapper to write some initial data.
    CLevelDBWrapper* dbw = new CLevelDBWrapper(ph, (1 << 10), false, false, false);
    char key = 'k';
    uint256 in = GetRandHash();
    uint256 res;

    BOOST_CHECK(dbw->Write(key, in));
    BOOST_CHECK(dbw->Read(key, res));
    BOOST_CHECK_EQUAL(res.ToString(), in.ToString());

    // Call the destructor to free leveldb LOCK
    delete dbw;

    // Simulate a -reindex by wiping the existing data store
    CLevelDBWrapper odbw(ph, (1 << 10), false, true, true);

    // Check that the key/val we wrote with unobfuscated wrapper doesn't exist
    uint256 res2;
    BOOST_CHECK(!odbw.Read(key, res2));
    BOOST_CHECK(!is_null_key(odbw.GetObfuscateKey()));

    uint256 in2 = GetRandHash();
    uint256 res3;
 
    // Check that we can write successfully
    BOOST_CHECK(odbw.Write(key, in2));
    BOOST_CHECK(odbw.Read(key, res3));
    BOOST_CHECK_EQUAL(res3.ToString(), in2.ToString());
}
 
BOOST_AUTO_TEST_SUITE_END()
