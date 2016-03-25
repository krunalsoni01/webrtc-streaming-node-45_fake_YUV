/* test_old_uniform_int.cpp
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id: test_old_uniform_int.cpp 68647 2011-02-05 05:54:11Z steven_watanabe $
 *
 */

#include <boost/random/uniform_int.hpp>
#include <boost/math/distributions/uniform.hpp>

#define BOOST_RANDOM_DISTRIBUTION boost::uniform_int<>
#define BOOST_RANDOM_DISTRIBUTION_NAME uniform_int
#define BOOST_MATH_DISTRIBUTION boost::math::uniform
#define BOOST_RANDOM_ARG1_TYPE int
#define BOOST_RANDOM_ARG1_NAME b
#define BOOST_RANDOM_ARG1_DEFAULT 1000
#define BOOST_RANDOM_ARG1_DISTRIBUTION(n) boost::uniform_int<>(0, n)
#define BOOST_RANDOM_DISTRIBUTION_INIT (0, b)
#define BOOST_MATH_DISTRIBUTION_INIT (0, b+1)
#define BOOST_RANDOM_DISTRIBUTION_MAX b

#include "test_real_distribution.ipp"