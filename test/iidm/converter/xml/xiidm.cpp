/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define BOOST_TEST_MODULE iidm-xml
#include <boost/test/unit_test.hpp>

#include "extensions/LoadBarXmlSerializer.hpp"
#include "extensions/LoadFooXmlSerializer.hpp"
#include "extensions/LoadMockExtXmlSerializer.hpp"
#include "extensions/LoadQuxXmlSerializer.hpp"
#include "extensions/TerminalMockXmlSerializer.hpp"
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/test/ExtensionFixture.hpp>
#include <powsybl/test/XmlFixture.hpp>

namespace xml = powsybl::iidm::converter::xml;
namespace ext = powsybl::iidm::converter::xml::extensions;

namespace powsybl {

namespace test {

class ExtensionFixtureXiidm : public ResourceFixture {
 public:
  ExtensionFixtureXiidm();

  ExtensionFixtureXiidm(const ExtensionFixtureXiidm&) = default;

  ExtensionFixtureXiidm(ExtensionFixtureXiidm&&) = default;

  ~ExtensionFixtureXiidm() = default;

  ExtensionFixtureXiidm& operator=(const ExtensionFixtureXiidm&) = default;

  ExtensionFixtureXiidm& operator=(ExtensionFixtureXiidm&&) = default;
};

ExtensionFixtureXiidm::ExtensionFixtureXiidm() {
  auto& extProvider = powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>::getInstance();
  extProvider.registerExtension(stdcxx::make_unique<ext::LoadBarXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::LoadFooXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::LoadMockExtXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::LoadQuxXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::TerminalMockXmlSerializer>());
}

BOOST_TEST_GLOBAL_FIXTURE(ExtensionFixtureXiidm);
BOOST_TEST_GLOBAL_FIXTURE(XmlFixture);

}  // namespace test

}  // namespace powsybl
