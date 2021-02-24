/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTTERMINALTOPOLOGYVISITOR_HPP
#define POWSYBL_IIDM_ABSTRACTTERMINALTOPOLOGYVISITOR_HPP

#include <powsybl/iidm/TopologyVisitor.hpp>

namespace powsybl {

namespace iidm {

class Injection;

class AbstractTerminalTopologyVisitor : public TopologyVisitor {
public:  // TopologyVisitor
    void visitBattery(const Battery& battery) override;

    void visitBusbarSection(const BusbarSection& section) override;

    void visitDanglingLine(const DanglingLine& danglingLine) override;

    void visitGenerator(const Generator& generator) override;

    void visitHvdcConverterStation(const HvdcConverterStation& converterStation) override;

    void visitLine(const Line& line, const Branch::Side& side) override;

    void visitLoad(const Load& load) override;

    void visitShuntCompensator(const ShuntCompensator& sc) override;

    void visitStaticVarCompensator(const StaticVarCompensator& staticVarCompensator) override;

    void visitThreeWindingsTransformer(const ThreeWindingsTransformer& transformer, const ThreeWindingsTransformer::Side& side) override;

    void visitTwoWindingsTransformer(const TwoWindingsTransformer& transformer, const Branch::Side& side) override;

public:
    AbstractTerminalTopologyVisitor() = default;

    ~AbstractTerminalTopologyVisitor() noexcept override = default;

    AbstractTerminalTopologyVisitor(const AbstractTerminalTopologyVisitor&) = default;

    AbstractTerminalTopologyVisitor(AbstractTerminalTopologyVisitor&&) noexcept = default;

    AbstractTerminalTopologyVisitor& operator=(const AbstractTerminalTopologyVisitor&) = default;

    AbstractTerminalTopologyVisitor& operator=(AbstractTerminalTopologyVisitor&&) noexcept = default;

    virtual void visitTerminal(const Terminal& terminal) = 0;

private:
    void visitBranch(const Branch& branch, const Branch::Side& side);

    void visitInjection(const Injection& injection);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTTERMINALTOPOLOGYVISITOR_HPP
