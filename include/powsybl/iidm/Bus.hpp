/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUS_HPP
#define POWSYBL_IIDM_BUS_HPP

#include <functional>
#include <string>
#include <vector>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class Component;
class DanglingLine;
class Generator;
class LccConverterStation;
class Line;
class Load;
class ShuntCompensator;
class StaticVarCompensator;
class Terminal;
class ThreeWindingsTransformer;
class TopologyVisitor;
class TwoWindingsTransformer;
class VoltageLevel;
class VscConverterStation;

class Bus : public Identifiable {
public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

public:
    ~Bus() noexcept override = default;

    virtual double getAngle() const = 0;

    stdcxx::const_range<Battery> getBatteries() const;

    stdcxx::range<Battery> getBatteries();

    virtual stdcxx::CReference<Component> getConnectedComponent() const = 0;

    virtual stdcxx::Reference<Component> getConnectedComponent() = 0;

    virtual unsigned long getConnectedTerminalCount() const = 0;

    virtual stdcxx::const_range<Terminal> getConnectedTerminals() const = 0;

    virtual stdcxx::range<Terminal> getConnectedTerminals() = 0;

    stdcxx::const_range<DanglingLine> getDanglingLines() const;

    stdcxx::range<DanglingLine> getDanglingLines();

    stdcxx::const_range<Generator> getGenerators() const;

    stdcxx::range<Generator> getGenerators();

    stdcxx::const_range<LccConverterStation> getLccConverterStations() const;

    stdcxx::range<LccConverterStation> getLccConverterStations();

    stdcxx::const_range<Line> getLines() const;

    stdcxx::range<Line> getLines();

    stdcxx::const_range<Load> getLoads() const;

    stdcxx::range<Load> getLoads();

    stdcxx::const_range<ShuntCompensator> getShuntCompensators() const;

    stdcxx::range<ShuntCompensator> getShuntCompensators();

    stdcxx::const_range<StaticVarCompensator> getStaticVarCompensators() const;

    stdcxx::range<StaticVarCompensator> getStaticVarCompensators();

    virtual stdcxx::CReference<Component> getSynchronousComponent() const = 0;

    virtual stdcxx::Reference<Component> getSynchronousComponent() = 0;

    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    virtual double getV() const = 0;

    virtual const VoltageLevel& getVoltageLevel() const = 0;

    virtual VoltageLevel& getVoltageLevel() = 0;

    stdcxx::const_range<VscConverterStation> getVscConverterStations() const;

    stdcxx::range<VscConverterStation> getVscConverterStations();

    virtual bool isInMainConnectedComponent() const;

    virtual bool isInMainSynchronousComponent() const;

    virtual Bus& setAngle(double angle) = 0;

    virtual Bus& setV(double v) = 0;

    void visitConnectedEquipments(TopologyVisitor& visitor);

    virtual void visitConnectedOrConnectableEquipments(TopologyVisitor& visitor) = 0;

protected:
    Bus(const std::string& id, const std::string& name, bool fictitious);

    template <typename T>
    void visitEquipments(const stdcxx::const_range<T>& terminals, TopologyVisitor& visitor) const;

private:  // Identifiable
    const std::string& getTypeDescription() const override;

private:
    template <typename T>
    stdcxx::const_range<T> getAll() const;

    template <typename T>
    stdcxx::range<T> getAll();

    virtual void setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber) = 0;

    virtual void setSynchronousComponentNumber(const stdcxx::optional<unsigned long>& synchronousComponentNumber) = 0;

    friend class ConnectedComponentsManager;

    friend class SynchronousComponentsManager;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Bus.hxx>

#endif  // POWSYBL_IIDM_BUS_HPP
