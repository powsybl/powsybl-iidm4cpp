/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ASSERTIONUTILS_HPP
#define POWSYBL_IIDM_ASSERTIONUTILS_HPP

#include <sstream>

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/demangle.hpp>

#include "boost/unit_test.hpp"

#define POWSYBL_ASSERT_THROW(statement, expected_exception, expected_message) \
do { \
    try { \
        statement; \
        std::ostringstream oss; \
        oss << "Expected: " << #statement << " throws an exception of type "; \
        oss << stdcxx::demangle(typeid(expected_exception)) << std::endl; \
        oss << "  Actual: no exception thrown"; \
        BOOST_ERROR(oss.str().c_str()); \
    } catch (const expected_exception& e) { \
        if (expected_message) { \
            BOOST_CHECK_EQUAL(expected_message, e.what()); \
        } \
    } catch (const std::exception& e) { \
        std::ostringstream oss; \
        oss << "Expected: " << #statement << " throws an exception of type "; \
        oss << stdcxx::demangle(typeid(expected_exception)) << std::endl; \
        oss << "  Actual: it throws an exception of type "; \
        oss << stdcxx::demangle(e); \
        BOOST_FAIL(oss.str().c_str()); \
    } catch (...) { \
        std::ostringstream oss; \
        oss << "Expected: " << #statement << " throws an exception of type "; \
        oss << stdcxx::demangle(typeid(expected_exception)) << std::endl; \
        oss << "  Actual: it throws an exception of unknown type"; \
        BOOST_FAIL(oss.str().c_str()); \
    } \
} while (boost::test_tools::tt_detail::dummy_cond())

// for stdcxx::Reference or CReference
#define POWSYBL_ASSERT_REF_TRUE(reference) \
BOOST_TEST(static_cast<bool>(reference))

// for stdcxx::Reference or CReference
#define POWSYBL_ASSERT_REF_FALSE(reference) \
BOOST_TEST(!static_cast<bool>(reference))

template <typename T, bool = std::is_enum<T>::value>
bool isSameEnumValue(const T& val1, const T& val2) {
    using type = typename std::underlying_type<T>::type;
    return static_cast<type>(val1) == static_cast<type>(val2);
}

// for enum values
#define POWSYBL_ASSERT_ENUM_EQ(left, right) \
BOOST_TEST(isSameEnumValue((left), (right)))

#endif  // POWSYBL_IIDM_ASSERTIONUTILS_HPP
