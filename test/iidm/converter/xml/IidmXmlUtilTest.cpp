/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(IidmXmlUtilTestSuite)

static constexpr const char* const ELEMENT = "element";
static constexpr const char* const ROOT = "root";

BOOST_AUTO_TEST_CASE(testWriteMaximumVersion) {
    std::stringstream ss;
    FakeAnonymizer anonymizer;
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.1");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    NetworkXmlWriterContext contextThrow(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION), busFilter, IidmXmlVersion::V1_1());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be <= 1.0");

    NetworkXmlWriterContext contextLog(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR), busFilter, IidmXmlVersion::V1_1());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextLog));
}

BOOST_AUTO_TEST_CASE(testWriteStrictMaximumVersion) {
    std::stringstream ss;
    FakeAnonymizer anonymizer;
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.1");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    NetworkXmlWriterContext contextThrow(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION), busFilter, IidmXmlVersion::V1_1());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertStrictMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be < 1.0");

    NetworkXmlWriterContext contextLog(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR), busFilter, IidmXmlVersion::V1_1());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertStrictMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextLog));
}

BOOST_AUTO_TEST_CASE(testWriteMinimumVersion) {
    std::stringstream ss;
    FakeAnonymizer anonymizer;
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.0");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    NetworkXmlWriterContext contextThrow(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION), busFilter, IidmXmlVersion::V1_0());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");

    NetworkXmlWriterContext contextLog(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR), busFilter, IidmXmlVersion::V1_0());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextLog));
}

BOOST_AUTO_TEST_CASE(testWriteMinimumVersionIfNotDefault) {
    std::stringstream ss;
    FakeAnonymizer anonymizer;
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.0");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    NetworkXmlWriterContext contextThrow(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION), busFilter, IidmXmlVersion::V1_0());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");

    NetworkXmlWriterContext contextLog(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR), busFilter, IidmXmlVersion::V1_0());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextLog));
}

BOOST_AUTO_TEST_CASE(testWriteMinimumVersionAndRunIfNotDefault) {
    std::stringstream ss;
    FakeAnonymizer anonymizer;
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.0");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    NetworkXmlWriterContext contextThrow(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION), busFilter, IidmXmlVersion::V1_0());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextThrow, [](){}), PowsyblException, "root.element is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");

    NetworkXmlWriterContext contextLog(anonymizer, writer, options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR), busFilter, IidmXmlVersion::V1_0());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextLog, [](){}));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
