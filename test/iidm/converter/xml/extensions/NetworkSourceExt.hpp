/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_NETWORKSOURCEEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_NETWORKSOURCEEXT_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Extendable;
class Network;

namespace converter {

namespace xml {

namespace extensions {

class NetworkSourceExt : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit NetworkSourceExt(Network& network, const std::string& sourceData);

    ~NetworkSourceExt() override = default;

    const std::string& getSourceData() const;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::string m_sourceData;
};

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_NETWORKSOURCEEXT_HPP
