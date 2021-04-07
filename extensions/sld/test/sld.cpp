/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define BOOST_TEST_MODULE sld
#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/extensions/sld/BusbarSectionPositionXmlSerializer.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePositionXmlSerializer.hpp>
#include <powsybl/test/ExtensionFixture.hpp>
#include <powsybl/test/XmlFixture.hpp>

namespace xml = powsybl::iidm::converter::xml;
namespace ext = powsybl::iidm::extensions::sld;

namespace powsybl {

namespace test {

class ExtensionFixtureSld : public ResourceFixture {
 public:
  ExtensionFixtureSld();

  ExtensionFixtureSld(const ExtensionFixtureSld&) = default;

  ExtensionFixtureSld(ExtensionFixtureSld&&) = default;

  ~ExtensionFixtureSld() = default;

  ExtensionFixtureSld& operator=(const ExtensionFixtureSld&) = default;

  ExtensionFixtureSld& operator=(ExtensionFixtureSld&&) = default;
};

ExtensionFixtureSld::ExtensionFixtureSld() {
  auto& extProvider = powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>::getInstance();
  extProvider.registerExtension(stdcxx::make_unique<ext::BusbarSectionPositionXmlSerializer>());
  extProvider.registerExtension(stdcxx::make_unique<ext::ConnectablePositionXmlSerializer>());
}

BOOST_TEST_GLOBAL_FIXTURE(ExtensionFixtureSld);
BOOST_TEST_GLOBAL_FIXTURE(XmlFixture);

}  // namespace test

}  // namespace powsybl
