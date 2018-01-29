/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ASSERTIONUTILS_HPP
#define POWSYBL_IIDM_ASSERTIONUTILS_HPP

#include <sstream>



#include <gtest/gtest.h>

#define POWSYBL_ASSERT_THROW(statement, expected_exception, expected_message) \
try { \
    statement; \
    FAIL(); \
} catch (const expected_exception& e) { \
    ASSERT_STREQ(expected_message, e.what()); \
} catch (const std::exception& e) { \
    std::ostringstream oss; \
    oss << "Expected: " << #statement << " throws an exception of type "; \
    oss << stdcxx::demangle(typeid(expected_exception)) << std::endl; \
    oss << "  Actual: it throws an exception of type "; \
    oss << stdcxx::demangle(e); \
    GTEST_FATAL_FAILURE_(oss.str().c_str()); \
} catch (...) { \
    std::ostringstream oss; \
    oss << "Expected: " << #statement << " throws an exception of type "; \
    oss << stdcxx::demangle(typeid(expected_exception)) << std::endl; \
    oss << "  Actual: it throws an exception of unknown type"; \
    GTEST_FATAL_FAILURE_(oss.str().c_str()); \
}

#endif  // POWSYBL_IIDM_ASSERTIONUTILS_HPP
