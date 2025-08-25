/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCOMPLEXIDENTIFIABLEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCOMPLEXIDENTIFIABLEXML_HPP

#include <powsybl/iidm/converter/xml/AbstractIdentifiableXml.hpp>

#include <functional>
#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;

template <typename Added, typename Adder, typename Parent>
class AbstractComplexIdentifiableXml : public AbstractIdentifiableXml<Added, Adder, Parent> {

public:
    virtual void read(Parent& parent, NetworkXmlReaderContext& context) const override;

protected:
    virtual void readSubElements(const std::string& id, std::vector<std::function<void(Identifiable&)>>& toApply , NetworkXmlReaderContext& context) const;

    virtual void readRootElementAttributes(Adder& adder, std::vector<std::function<void(Identifiable&)>>& toApply ,NetworkXmlReaderContext& context) const = 0;

    virtual void readSubElements(const std::string& id, Adder& adder, std::vector<std::function<void(Identifiable&)>>& toApply, NetworkXmlReaderContext& context) const = 0;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractComplexIdentifiableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCOMPLEXIDENTIFIABLEXML_HPP


