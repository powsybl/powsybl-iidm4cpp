/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYVISITOR_HPP
#define POWSYBL_IIDM_DCTOPOLOGYVISITOR_HPP

#include <powsybl/iidm/TerminalNumber.hpp>
#include <powsybl/iidm/TwoSides.hpp>

#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class AcDcConverter;
class DcGround;
class DcLine;
class DcTerminal;

class DcTopologyVisitor {
public:
    static void visitDcEquipments(const stdcxx::const_range<DcTerminal>& dcTerminals, DcTopologyVisitor& visitor);

public:
    DcTopologyVisitor() = default;

    DcTopologyVisitor(const DcTopologyVisitor&) = default;

    DcTopologyVisitor(DcTopologyVisitor&&) noexcept = default;

    virtual ~DcTopologyVisitor() noexcept = default;

    DcTopologyVisitor& operator=(const DcTopologyVisitor&) = default;

    DcTopologyVisitor& operator=(DcTopologyVisitor&&) noexcept = default;


    virtual void visitAcDcConverter(const AcDcConverter& converter, const TerminalNumber& terminalNumber);

    virtual void visitDcGround(const DcGround& dcGround);

    virtual void visitDcLine(const DcLine& dcLine, const TwoSides& side);

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYVISITOR_HPP
