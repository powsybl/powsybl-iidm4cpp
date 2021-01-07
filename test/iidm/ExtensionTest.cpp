/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/network/EurostagFactory.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ExtensionTestSuite)

class LoadExtension : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit LoadExtension(Load& load, double value);

    ~LoadExtension() override = default;

    double getValue() const;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    double m_value;
};

LoadExtension::LoadExtension(Load& load, double value) :
    Extension(load),
    m_value(value) {
}

void LoadExtension::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Load>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Load>()));
    }
}

const std::string& LoadExtension::getName() const {
    static std::string s_name = "loadExtension";
    return s_name;
}

const std::type_index& LoadExtension::getType() const {
    static std::type_index s_type = typeid(LoadExtension);
    return s_type;
}

double LoadExtension::getValue() const {
    return m_value;
}

class BatteryExtension : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit BatteryExtension(Battery& battery);

    ~BatteryExtension() override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;
};

BatteryExtension::BatteryExtension(Battery& battery) :
    Extension(battery) {
}

const std::string& BatteryExtension::getName() const {
    static std::string s_name = "batteryExtension";
    return s_name;
}

const std::type_index& BatteryExtension::getType() const {
    static std::type_index s_type = typeid(BatteryExtension);
    return s_type;
}

void BatteryExtension::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>()));
    }
}

BOOST_AUTO_TEST_CASE(getExtensionByName) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");
    load.addExtension(stdcxx::make_unique<LoadExtension>(load, 1.1));
    const Load& cLoad = load;

    BOOST_CHECK(!cLoad.getExtensionByName<LoadExtension>("test"));
    BOOST_CHECK(!load.getExtensionByName<LoadExtension>("test"));

    BOOST_CHECK(!cLoad.getExtensionByName<BatteryExtension>("test"));
    BOOST_CHECK(!load.getExtensionByName<BatteryExtension>("test"));

    auto cExt = cLoad.getExtensionByName<LoadExtension>("loadExtension");
    auto ext = load.getExtensionByName<LoadExtension>("loadExtension");
    BOOST_CHECK(cExt);
    BOOST_CHECK(ext);
    BOOST_CHECK_CLOSE(1.1, cExt.get().getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.1, ext.get().getValue(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
