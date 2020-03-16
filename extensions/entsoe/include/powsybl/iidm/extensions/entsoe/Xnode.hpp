/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_XNODE_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_XNODE_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeExport.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine;

namespace extensions {

namespace entsoe {

class EXT_ENTSOE_EXPORT Xnode : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    Xnode(DanglingLine& dl, const std::string& code);

    ~Xnode() noexcept override = default;

    const std::string& getCode() const;

    Xnode& setCode(const std::string& code);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::string m_code;
};

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_ENTSOE_XNODE_HPP

