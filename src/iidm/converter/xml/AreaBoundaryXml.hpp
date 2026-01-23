/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_AREABOUNDARYXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_AREABOUNDARYXML_HPP

#include <string>
#include <functional>

namespace powsybl {

namespace iidm {

class Network;
class Area;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class AreaBoundaryXml {
public:
    static const AreaBoundaryXml& getInstance();

protected:
    void read(Area& area, NetworkXmlReaderContext& context) const;
    void write(const Area& area, NetworkXmlWriterContext& context) const;

friend class AreaXml;

private:
    AreaBoundaryXml() = default;

    ~AreaBoundaryXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_BOUNDARYREFXML_HPP

