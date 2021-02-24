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

const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                "<network/>";

BOOST_AUTO_TEST_CASE(testReadMaximumVersion) {
    std::stringstream ss;
    ss << networkStr;
    powsybl::xml::XmlStreamReader reader(ss);
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    NetworkXmlReaderContext context(std::move(anonymizer), reader, ImportOptions(), IidmXmlVersion::V1_1());

    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), context), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be <= 1.0");
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context));
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context));
}

BOOST_AUTO_TEST_CASE(testWriteMaximumVersion) {
    std::stringstream ss;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.1");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION);
    NetworkXmlWriterContext contextThrow(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_1());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be <= 1.0");

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    NetworkXmlWriterContext contextLog(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_1());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextLog));
}

BOOST_AUTO_TEST_CASE(testReadStrictMaximumVersion) {
    std::stringstream ss;
    ss << networkStr;
    powsybl::xml::XmlStreamReader reader(ss);
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    NetworkXmlReaderContext context(std::move(anonymizer), reader, ImportOptions(), IidmXmlVersion::V1_1());

    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertStrictMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be < 1.1");
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertStrictMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context));
}

BOOST_AUTO_TEST_CASE(testWriteStrictMaximumVersion) {
    std::stringstream ss;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.1");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION);
    NetworkXmlWriterContext contextThrow(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_1());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertStrictMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be < 1.0");

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    NetworkXmlWriterContext contextLog(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_1());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertStrictMaximumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), contextLog));
}

BOOST_AUTO_TEST_CASE(testReadMinimumVersion) {
    std::stringstream ss;
    ss << networkStr;
    powsybl::xml::XmlStreamReader reader(ss);
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    NetworkXmlReaderContext context(std::move(anonymizer), reader, ImportOptions(), IidmXmlVersion::V1_1());

    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), context));
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context));
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be >= 1.2");
}

BOOST_AUTO_TEST_CASE(testWriteMinimumVersion) {
    std::stringstream ss;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.0");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION);
    NetworkXmlWriterContext contextThrow(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_0());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    NetworkXmlWriterContext contextLog(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_0());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersion(ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextLog));
}

BOOST_AUTO_TEST_CASE(testReadMinimumVersionIfNotDefault) {
    std::stringstream ss;
    ss << networkStr;
    powsybl::xml::XmlStreamReader reader(ss);
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    NetworkXmlReaderContext context(std::move(anonymizer), reader, ImportOptions(), IidmXmlVersion::V1_1());

    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), context));
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context));
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be >= 1.2");
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(false, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), context));
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(false, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context));
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(false, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context));
}

BOOST_AUTO_TEST_CASE(testWriteMinimumVersionIfNotDefault) {
    std::stringstream ss;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.0");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION);
    NetworkXmlWriterContext contextThrow(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_0());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextThrow), PowsyblException, "root.element is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    NetworkXmlWriterContext contextLog(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_0());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextLog));
}

BOOST_AUTO_TEST_CASE(testReadMinimumVersionAndRunIfNotDefault) {
    std::stringstream ss;
    ss << networkStr;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    powsybl::xml::XmlStreamReader reader(ss);
    ImportOptions options;
    NetworkXmlReaderContext context(std::move(anonymizer), reader, options, IidmXmlVersion::V1_1());

    int variable = 0;
    auto runnable = [&variable]() {
        variable++;
    };

    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), context, runnable));
    BOOST_CHECK_EQUAL(1, variable);
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, runnable));
    BOOST_CHECK_EQUAL(2, variable);
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context, runnable), PowsyblException, "root.element is not supported for IIDM-XML version 1.1. IIDM-XML version should be >= 1.2");
    BOOST_CHECK_EQUAL(2, variable);
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(false, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_0(), context, runnable));
    BOOST_CHECK_EQUAL(2, variable);
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(false, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, runnable));
    BOOST_CHECK_EQUAL(2, variable);
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(false, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context, runnable));
    BOOST_CHECK_EQUAL(2, variable);
}

BOOST_AUTO_TEST_CASE(testWriteMinimumVersionAndRunIfNotDefault) {
    std::stringstream ss;
    std::unique_ptr<FakeAnonymizer> anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    powsybl::xml::XmlStreamWriter writer(ss, true);
    ExportOptions options = ExportOptions().setVersion("1.0");
    Network network("test", "test");
    BusFilter busFilter = BusFilter::create(network, options);

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION);
    NetworkXmlWriterContext contextThrow(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_0());
    POWSYBL_ASSERT_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextThrow, [](){}), PowsyblException, "root.element is not supported for IIDM-XML version 1.0. IIDM-XML version should be >= 1.1");

    options.setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
    NetworkXmlWriterContext contextLog(std::move(anonymizer), writer, options, busFilter, IidmXmlVersion::V1_0());
    BOOST_CHECK_NO_THROW(IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(true, ROOT, ELEMENT, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), contextLog, [](){}));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
