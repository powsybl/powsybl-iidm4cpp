/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTSXMLSERIALIZER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTSXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/AbstractExtensionXmlSerializer.hpp>

namespace powsybl {

namespace xml {

class XmlStreamReader;

}  // namespace xml

namespace iidm {

namespace extensions {

namespace iidm {

class DiscreteMeasurements;

class DiscreteMeasurementsXmlSerializer : public converter::xml::AbstractExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    bool isSerializable(const Extension& extension) const override;

    Extension& read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const override;

public:
    DiscreteMeasurementsXmlSerializer();

    ~DiscreteMeasurementsXmlSerializer() noexcept override = default;

private:
    static void readDiscreteMeasurement(DiscreteMeasurements& discreteMeasurements, const xml::XmlStreamReader& reader);
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTSXMLSERIALIZER_HPP
