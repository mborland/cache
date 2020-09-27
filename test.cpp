// Copyright 2020 Matt Borland
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "include/cache_size.hpp"
#include <boost/core/lightweight_test.hpp>

// Tests need to be modified for actual system
int main(void)
{
    #if defined (__linux__)
    BOOST_TEST_EQ(cache_line_size(), 64);
    BOOST_TEST_EQ(L1I_cache_size(), 32'768);
    BOOST_TEST_EQ(L1D_cache_size(), 32'768);
    BOOST_TEST_EQ(L2_cache_size(), 262'144);
    BOOST_TEST_EQ(L3_cache_size(), 12'582'912);
    BOOST_TEST_EQ(L4_cache_size(), 0);
    #endif
    
    #if defined(__APPLE__)
    BOOST_TEST_EQ(cache_line_size(), 64);
    BOOST_TEST_EQ(L1I_cache_size(), 32'768);
    BOOST_TEST_EQ(L1D_cache_size(), 32'768);
    BOOST_TEST_EQ(L2_cache_size(), 262'144);
    BOOST_TEST_EQ(L3_cache_size(), 3'145'728);
    #endif 

    boost::report_errors();
}

