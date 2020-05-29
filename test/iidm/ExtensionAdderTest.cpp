/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

class SimpleExtendable : public Extendable {
};

class SimpleExtensionImpl : public Extension {
public:
    explicit SimpleExtensionImpl(Extendable& extendable) :
        Extension(extendable) {
    }

    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override {
        if (extendable && !stdcxx::isInstanceOf<SimpleExtendable>(extendable.get())) {
            throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<SimpleExtendable>()));
        }
    }

    const std::string& getName() const override {
        static std::string s_name = "SimpleExtension";
        return s_name;
    }

    const std::type_index& getType() const override {
        static std::type_index s_type = typeid(SimpleExtensionImpl);
        return s_type;
    }
};

class SimpleExtensionAdder : public ExtensionAdder {
public:
    explicit SimpleExtensionAdder(Extendable& extendable) :
        ExtensionAdder(extendable) {
    }

    std::unique_ptr<Extension> createExtension(Extendable& extendable) override {
        return stdcxx::make_unique<SimpleExtensionImpl>(extendable);
    }
};

BOOST_AUTO_TEST_SUITE(ExtensionAdderTestSuite)

BOOST_AUTO_TEST_CASE(TestAddingBase) {
    SimpleExtendable sExtendable;
    Extendable& extendable = sExtendable.newExtension<SimpleExtensionAdder>().add();
    sExtendable.getExtension<SimpleExtensionImpl>();
    BOOST_CHECK_EQUAL(1, boost::size(sExtendable.getExtensions()));
    sExtendable.removeExtension<SimpleExtensionImpl>();
    BOOST_CHECK_EQUAL(0, boost::size(sExtendable.getExtensions()));
    BOOST_CHECK(stdcxx::areSame(extendable, sExtendable));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
