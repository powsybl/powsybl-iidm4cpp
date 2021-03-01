/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(NetworkXmlWriterContextTest)

BOOST_AUTO_TEST_CASE(Constructor) {
    std::stringstream istream;
    powsybl::xml::XmlStreamWriter writer(istream, true);
    ExportOptions exportOptions;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    Network network("id_network", "name_network");
    const BusFilter& filter = BusFilter::create(network, exportOptions);
    NetworkXmlWriterContext context(std::move(anonymizer), writer, exportOptions, filter, IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    BOOST_CHECK_EQUAL(static_cast<int>(exportOptions.getTopologyLevel()), static_cast<int>(context.getOptions().getTopologyLevel()));
    BOOST_CHECK_EQUAL(exportOptions.isAnonymized(), context.getOptions().isAnonymized());
    BOOST_CHECK_EQUAL(exportOptions.isIndent(), context.getOptions().isIndent());
    BOOST_CHECK_EQUAL(exportOptions.isOnlyMainCc(), context.getOptions().isOnlyMainCc());
    BOOST_CHECK_EQUAL(exportOptions.isThrowExceptionIfExtensionNotFound(), context.getOptions().isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK_EQUAL(exportOptions.isWithBranchSV(), context.getOptions().isWithBranchSV());
    BOOST_CHECK_EQUAL(&writer, &context.getWriter());

    BOOST_CHECK(context.getExportedEquipments().empty());
    const Substation& substation = network.newSubstation().setTso("tso_test").setCountry(Country::FR).setId("id_test").setName("name_test").add();
    context.addExportedEquipment(substation);
    BOOST_CHECK_EQUAL(1L, context.getExportedEquipments().size());
    BOOST_CHECK(context.getExportedEquipments().find("id_test") != context.getExportedEquipments().end());
    BOOST_CHECK(context.getExportedEquipments().find("id_fake") == context.getExportedEquipments().end());

    const Substation& substation2 = network.newSubstation().setTso("tso_test").setCountry(Country::FR).setId("id_test2").setName("name_test").add();
    context.addExportedEquipment(substation2);
    BOOST_CHECK_EQUAL(2L, context.getExportedEquipments().size());
    BOOST_CHECK(context.getExportedEquipments().find("id_test2") != context.getExportedEquipments().end());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

