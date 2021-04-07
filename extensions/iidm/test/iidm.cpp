/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define BOOST_TEST_MODULE iidm
#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClockXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlXmlSerializer.hpp>
#include <powsybl/test/ExtensionFixture.hpp>
#include <powsybl/test/XmlFixture.hpp>

namespace xml = powsybl::iidm::converter::xml;
namespace ext = powsybl::iidm::extensions::iidm;

namespace powsybl {

namespace test {

class ExtensionFixtureIidm : public ResourceFixture {
 public:
  ExtensionFixtureIidm();

  ExtensionFixtureIidm(const ExtensionFixtureIidm&) = default;

  ExtensionFixtureIidm(ExtensionFixtureIidm&&) = default;

  ~ExtensionFixtureIidm() = default;

  ExtensionFixtureIidm& operator=(const ExtensionFixtureIidm&) = default;

  ExtensionFixtureIidm& operator=(ExtensionFixtureIidm&&) = default;
};

ExtensionFixtureIidm::ExtensionFixtureIidm() {
  auto& extProvider = powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>::getInstance();
  extProvider.registerExtension(stdcxx::make_unique<ext::ActivePowerControlXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::CoordinatedReactiveControlXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::ThreeWindingsTransformerPhaseAngleClockXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::TwoWindingsTransformerPhaseAngleClockXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::VoltagePerReactivePowerControlXmlSerializer>());
}

BOOST_TEST_GLOBAL_FIXTURE(ExtensionFixtureIidm);
BOOST_TEST_GLOBAL_FIXTURE(XmlFixture);

}  // namespace test

}  // namespace powsybl
