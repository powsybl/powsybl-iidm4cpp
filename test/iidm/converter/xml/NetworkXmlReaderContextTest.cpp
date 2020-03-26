/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(NetworkXmlReaderContextTest)

BOOST_AUTO_TEST_CASE(Constructor) {
    std::stringstream istream;
    istream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
               "<test />";
    powsybl::xml::XmlStreamReader reader(istream);
    ImportOptions importOptions;
    FakeAnonymizer anonymizer;
    NetworkXmlReaderContext context(anonymizer, reader, importOptions, IidmXmlVersion::CURRENT_IIDM_XML_VERSION);
    BOOST_CHECK_EQUAL(importOptions.isThrowExceptionIfExtensionNotFound(), context.getOptions().isThrowExceptionIfExtensionNotFound());
    BOOST_CHECK(stdcxx::areSame(anonymizer, context.getAnonymizer()));
    BOOST_CHECK(stdcxx::areSame(reader, context.getReader()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

