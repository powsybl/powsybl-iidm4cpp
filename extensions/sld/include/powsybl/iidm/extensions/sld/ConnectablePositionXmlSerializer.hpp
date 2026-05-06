/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITIONXMLSERIALIZER_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITIONXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePosition.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePositionAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

class ConnectablePositionXmlSerializer : public converter::xml::AbstractVersionableExtensionXmlSerializer {
public:  // AbstractVersionableExtensionXmlSerializer
    Extension& read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const override;

public:
    ConnectablePositionXmlSerializer();

    ~ConnectablePositionXmlSerializer() noexcept override = default;

private:
    void readPosition(const converter::xml::NetworkXmlReaderContext& context, ConnectablePositionAdder::FeederAdder& feederAdder) const;

    void writePosition(const std::string& connectableId, const ConnectablePosition::Feeder& feeder, converter::xml::NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>()) const;
};

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITIONXMLSERIALIZER_HPP
