/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADQUXEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADQUXEXT_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Extendable;
class Load;

namespace converter {

namespace xml {

namespace extensions {

class LoadQuxExt : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit LoadQuxExt(Load& load);

    ~LoadQuxExt() override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;
};

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADQUXEXT_HPP
