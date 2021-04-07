/**
 * Copyright (c) 2029, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define BOOST_TEST_MODULE entsoe
#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeAreaXmlSerializer.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnodeXmlSerializer.hpp>
#include <powsybl/iidm/extensions/entsoe/XnodeXmlSerializer.hpp>
#include <powsybl/test/ExtensionFixture.hpp>
#include <powsybl/test/XmlFixture.hpp>

namespace xml = powsybl::iidm::converter::xml;
namespace ext = powsybl::iidm::extensions::entsoe;

namespace powsybl {

namespace test {

class ExtensionFixtureEnstsoe : public ResourceFixture {
 public:
  ExtensionFixtureEnstsoe();

  ExtensionFixtureEnstsoe(const ExtensionFixtureEnstsoe&) = default;

  ExtensionFixtureEnstsoe(ExtensionFixtureEnstsoe&&) = default;

  ~ExtensionFixtureEnstsoe() = default;

  ExtensionFixtureEnstsoe& operator=(const ExtensionFixtureEnstsoe&) = default;

  ExtensionFixtureEnstsoe& operator=(ExtensionFixtureEnstsoe&&) = default;
};

ExtensionFixtureEnstsoe::ExtensionFixtureEnstsoe() {
  auto& extProvider = powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>::getInstance();
  extProvider.registerExtension(stdcxx::make_unique<ext::EntsoeAreaXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::MergedXnodeXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::XnodeXmlSerializer>());
}

BOOST_TEST_GLOBAL_FIXTURE(ExtensionFixtureEnstsoe);
BOOST_TEST_GLOBAL_FIXTURE(XmlFixture);

}  // namespace test

}  // namespace powsybl
