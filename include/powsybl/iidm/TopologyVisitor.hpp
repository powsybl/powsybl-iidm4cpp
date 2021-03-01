/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TOPOLOGYVISITOR_HPP
#define POWSYBL_IIDM_TOPOLOGYVISITOR_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class BusbarSection;
class DanglingLine;
class Generator;
class HvdcConverterStation;
class Line;
class Load;
class StaticVarCompensator;
class ShuntCompensator;
class TwoWindingsTransformer;

class TopologyVisitor {
public:
    TopologyVisitor() = default;

    TopologyVisitor(const TopologyVisitor&) = default;

    TopologyVisitor(TopologyVisitor&&) noexcept = default;

    virtual ~TopologyVisitor() noexcept = default;

    TopologyVisitor& operator=(const TopologyVisitor&) = default;

    TopologyVisitor& operator=(TopologyVisitor&&) noexcept = default;

    virtual void visitBattery(const Battery& battery);

    virtual void visitBusbarSection(const BusbarSection& section);

    virtual void visitDanglingLine(const DanglingLine& danglingLine);

    void visitEquipments(const stdcxx::const_range<Terminal>& terminals);

    virtual void visitGenerator(const Generator& generator);

    virtual void visitHvdcConverterStation(const HvdcConverterStation& converterStation);

    virtual void visitLine(const Line& line, const Branch::Side& side);

    virtual void visitLoad(const Load& load);

    virtual void visitShuntCompensator(const ShuntCompensator& sc);

    virtual void visitStaticVarCompensator(const StaticVarCompensator& staticVarCompensator);

    virtual void visitThreeWindingsTransformer(const ThreeWindingsTransformer& transformer, const ThreeWindingsTransformer::Side& side);

    virtual void visitTwoWindingsTransformer(const TwoWindingsTransformer& transformer, const Branch::Side& side);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TOPOLOGYVISITOR_HPP
