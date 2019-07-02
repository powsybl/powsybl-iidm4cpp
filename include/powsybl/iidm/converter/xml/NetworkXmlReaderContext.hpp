/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLREADERCONTEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLREADERCONTEXT_HPP

#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace xml {
class XmlStreamReader;
}  // namespace xml

namespace iidm {

namespace converter {

class Anonymizer;

namespace xml {

class NetworkXmlReaderContext {
public:
    NetworkXmlReaderContext(const stdcxx::CReference<Anonymizer>& anonymizer, powsybl::xml::XmlStreamReader& reader, const ImportOptions& options);

    const stdcxx::CReference<Anonymizer>& getAnonymizer() const;

    const ImportOptions& getOptions() const;

    const powsybl::xml::XmlStreamReader& getReader() const;

private:
    powsybl::xml::XmlStreamReader& m_reader;

    stdcxx::CReference<Anonymizer> m_anonymizer;

    ImportOptions m_options;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLREADERCONTEXT_HPP
