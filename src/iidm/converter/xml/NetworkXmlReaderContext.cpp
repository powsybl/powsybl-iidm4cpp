/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

NetworkXmlReaderContext::NetworkXmlReaderContext(const Anonymizer& anonymizer, powsybl::xml::XmlStreamReader& reader, const ImportOptions& options) :
    m_reader(reader),
    m_anonymizer(anonymizer),
    m_options(options) {

}

const Anonymizer& NetworkXmlReaderContext::getAnonymizer() const {
    return m_anonymizer;
}

const ImportOptions& NetworkXmlReaderContext::getOptions() const {
    return m_options;
}

const powsybl::xml::XmlStreamReader& NetworkXmlReaderContext::getReader() const {
    return m_reader;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
