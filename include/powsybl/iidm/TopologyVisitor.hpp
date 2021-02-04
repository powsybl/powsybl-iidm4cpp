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
    virtual void visitBattery(const Battery& battery) = 0;

    virtual void visitBusbarSection(const BusbarSection& section) = 0;

    virtual void visitDanglingLine(const DanglingLine& danglingLine) = 0;

    virtual void visitGenerator(const Generator& generator) = 0;

    virtual void visitHvdcConverterStation(const HvdcConverterStation& converterStation) = 0;

    virtual void visitLine(const Line& line, const Branch::Side& side) = 0;

    virtual void visitLoad(const Load& load) = 0;

    virtual void visitShuntCompensator(const ShuntCompensator& sc) = 0;

    virtual void visitStaticVarCompensator(const StaticVarCompensator& staticVarCompensator) = 0;

    virtual void visitThreeWindingsTransformer(const ThreeWindingsTransformer& transformer, const ThreeWindingsTransformer::Side& side) = 0;

    virtual void visitTwoWindingsTransformer(const TwoWindingsTransformer& transformer, const Branch::Side& side) = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TOPOLOGYVISITOR_HPP
