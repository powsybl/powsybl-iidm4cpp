/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>

#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/cast.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace stdcxx {

BOOST_AUTO_TEST_SUITE(CaseTestSuite)

class BaseClass {
public:
    BaseClass() = default;

    virtual ~BaseClass() = default;

    int getBaseAttr() const;

    void setBaseAttr(int newValue);

private:
    int m_baseAttr = 1;
};

int BaseClass::getBaseAttr() const {
    return m_baseAttr;
}

void BaseClass::setBaseAttr(int newValue) {
    m_baseAttr = newValue;
}

class DerivedClass : public BaseClass {
public:
    DerivedClass() = default;

    ~DerivedClass() override = default;

    int getDerivedAttr() const;

    void setDerivedAttr(int newValue);

private:
    int m_derivedAttr = 2;
};

int DerivedClass::getDerivedAttr() const {
    return m_derivedAttr;
}

void DerivedClass::setDerivedAttr(int newValue) {
    m_derivedAttr = newValue;
}

BOOST_AUTO_TEST_CASE(unique_ptrCastTest) {
    std::unique_ptr<BaseClass> basePtr = stdcxx::make_unique<DerivedClass>();
    BOOST_CHECK_EQUAL(1, basePtr->getBaseAttr());
    basePtr->setBaseAttr(11);
    BOOST_CHECK_EQUAL(11, basePtr->getBaseAttr());

    std::unique_ptr<DerivedClass> derivedPtr = stdcxx::downcast<DerivedClass>(basePtr);
    BOOST_CHECK(!basePtr);
    BOOST_CHECK_EQUAL(11, derivedPtr->getBaseAttr());
    BOOST_CHECK_EQUAL(2, derivedPtr->getDerivedAttr());
    derivedPtr->setBaseAttr(111);
    derivedPtr->setDerivedAttr(22);
    BOOST_CHECK_EQUAL(22, derivedPtr->getDerivedAttr());

    std::unique_ptr<BaseClass> otherBasePtr = stdcxx::upcast<BaseClass>(derivedPtr);
    BOOST_CHECK(!derivedPtr);
    BOOST_CHECK_EQUAL(111, otherBasePtr->getBaseAttr());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace stdcxx
