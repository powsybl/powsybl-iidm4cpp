/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWFICTITIOUSINJECTIONXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWFICTITIOUSINJECTIONXML_HPP

namespace powsybl {

namespace iidm {

class VoltageLevel;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class NodeBreakerViewFictitiousInjectionXml {
public:
    static const NodeBreakerViewFictitiousInjectionXml& getInstance();

public:
    void read(VoltageLevel& voltageLevel, const NetworkXmlReaderContext& context) const;

    void write(unsigned long node, double p0, double q0, NetworkXmlWriterContext& context) const;

private:
    NodeBreakerViewFictitiousInjectionXml() = default;

    ~NodeBreakerViewFictitiousInjectionXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWFICTITIOUSINJECTIONXML_HPP

