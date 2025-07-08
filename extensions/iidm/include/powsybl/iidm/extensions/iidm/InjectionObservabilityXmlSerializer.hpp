/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITYXMLSERIALIZER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITYXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/AbstractExtensionXmlSerializer.hpp>

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}  // namespace xml

namespace iidm {

namespace extensions {

namespace iidm {

class ObservabilityQuality;

class InjectionObservabilityXmlSerializer : public converter::xml::AbstractExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    Extension& read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const override;

public:
    InjectionObservabilityXmlSerializer();

    ~InjectionObservabilityXmlSerializer() noexcept override = default;

private:
    void writeOptionalQuality(const std::string& elementName, const stdcxx::CReference<ObservabilityQuality>& quality, xml::XmlStreamWriter& writer) const;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITYXMLSERIALIZER_HPP
