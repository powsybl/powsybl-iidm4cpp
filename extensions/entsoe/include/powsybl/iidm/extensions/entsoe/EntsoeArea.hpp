/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEAREA_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEAREA_HPP

#include <string>

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeGeographicalCode.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class Substation;

namespace extensions {

namespace entsoe {

class IIDM_DECLSPEC EntsoeArea : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    EntsoeArea(Substation& substation, const EntsoeGeographicalCode& code);

    ~EntsoeArea() noexcept override = default;

    const EntsoeGeographicalCode& getCode() const;

    EntsoeArea& setCode(const EntsoeGeographicalCode& code);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    EntsoeGeographicalCode m_code;
};

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEAREA_HPP
