/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSIMPLEIDENTIFIABLEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSIMPLEIDENTIFIABLEXML_HPP

#include <powsybl/iidm/converter/xml/AbstractIdentifiableXml.hpp>

#include <string>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;

template <typename Added, typename Adder, typename Parent>
class AbstractSimpleIdentifiableXml : public AbstractIdentifiableXml<Added, Adder, Parent> {

public:
    virtual void read(Parent& parent, NetworkXmlReaderContext& context) const override;

protected:
    virtual void readSubElements(Added& identifiable, NetworkXmlReaderContext& context) const;

    virtual Added& readRootElementAttributes(Adder& adder, NetworkXmlReaderContext& context) const = 0;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractSimpleIdentifiableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSIMPLEIDENTIFIABLEXML_HPP


