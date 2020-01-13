/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_BUSBARSECTIONPOSITION_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_BUSBARSECTIONPOSITION_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class BusbarSection;

namespace extensions {

namespace cvg {

class BusbarSectionPosition : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    BusbarSectionPosition(BusbarSection& busbarSection, unsigned long busbarIndex, unsigned long sectionIndex);

    unsigned long getBusbarIndex() const;

    unsigned long getSectionIndex() const;

    BusbarSectionPosition& setBusbarIndex(unsigned long busbarIndex);

    BusbarSectionPosition& setSectionIndex(unsigned long sectionIndex);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    unsigned long m_busbarIndex;

    unsigned long m_sectionIndex;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_BUSBARSECTIONPOSITION_HPP

