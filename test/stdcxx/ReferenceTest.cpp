/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace stdcxx {

BOOST_AUTO_TEST_SUITE(ReferenceTestSuite)

BOOST_AUTO_TEST_CASE(refFromStandardRef) {
    int variable = 2;
    int& variableRef = variable;
    const int& cVariableRef = variable;

    auto refFromRef = ref(variableRef);
    BOOST_CHECK_EQUAL(2, refFromRef.get());

    auto cRefFromRef = cref(variableRef);
    BOOST_CHECK_EQUAL(2, cRefFromRef.get());

    auto refFromCRef = ref(cVariableRef);
    BOOST_CHECK_EQUAL(2, refFromCRef.get());

    auto cRefFromCRef = cref(cVariableRef);
    BOOST_CHECK_EQUAL(2, cRefFromCRef.get());
}

BOOST_AUTO_TEST_CASE(emptyRef) {
    BOOST_CHECK(!ref<int>());
    BOOST_CHECK(!cref<int>());
}

BOOST_AUTO_TEST_CASE(refFromStdcxxRef) {
    // invalid reference case
    Reference<int> myRef;
    CReference<int> myCRef;

    Reference<int> refFromInvalidRef = ref<int>(myRef);
    BOOST_CHECK(!refFromInvalidRef);

    Reference<int> refFromInvalidCRef = ref<int>(myCRef);
    BOOST_CHECK(!refFromInvalidCRef);

    CReference<int> cRefFromInvalidRef = cref<int>(myRef);
    BOOST_CHECK(!cRefFromInvalidRef);

    // valid reference case
    int myVar = 2;
    myRef = ref(myVar);
    myCRef = cref(myVar);

    Reference<int> refFromValidRef = ref<int>(myRef);
    BOOST_CHECK_EQUAL(2, refFromValidRef.get());

    CReference<int> cRefFromValidRef = cref<int>(myRef);
    BOOST_CHECK_EQUAL(2, cRefFromValidRef.get());

    Reference<int> refFromValidCRef = ref<int>(myCRef);
    BOOST_CHECK_EQUAL(2, refFromValidCRef.get());
}

BOOST_AUTO_TEST_CASE(refFromPtr) {
    // invalid pointer case
    std::unique_ptr<int> ptr;

    auto ptrRef = ref<int>(ptr);
    BOOST_CHECK(!ptrRef);

    auto cPtrRef = cref<int>(ptr);
    BOOST_CHECK(!cPtrRef);

    // valid pointer case
    ptr.reset(new int(2));

    ptrRef = ref<int>(ptr);
    BOOST_CHECK(ptrRef);
    BOOST_CHECK_EQUAL(2, ptrRef.get());

    cPtrRef = cref<int>(ptr);
    BOOST_CHECK(cPtrRef);
    BOOST_CHECK_EQUAL(2, cPtrRef.get());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
