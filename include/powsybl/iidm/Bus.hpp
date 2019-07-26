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
#include <powsybl/iidm/bits/Bus.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class DanglingLine;
class Generator;
class LccConverterStation;
class Line;
class Load;
class ShuntCompensator;
class StaticVarCompensator;
class Terminal;
class ThreeWindingsTransformer;
class TwoWindingsTransformer;
class VoltageLevel;
class VscConverterStation;

class Bus : public Identifiable {
public:
    template <typename T>
    using const_range = typename bus::range_traits<T>::const_range;

    template <typename T>
    using range = typename bus::range_traits<T>::range;

public:
    ~Bus() noexcept override = default;

    virtual double getAngle() const = 0;

    const_range<Battery> getBatteries() const;

    range<Battery> getBatteries();

    virtual unsigned long getConnectedTerminalCount() const = 0;

    virtual bus::Terminals getConnectedTerminals() const = 0;

    const_range<DanglingLine> getDanglingLines() const;

    range<DanglingLine> getDanglingLines();

    const_range<Generator> getGenerators() const;

    range<Generator> getGenerators();

    const_range<LccConverterStation> getLccConverterStations() const;

    range<LccConverterStation> getLccConverterStations();

    const_range<Line> getLines() const;

    range<Line> getLines();

    const_range<Load> getLoads() const;

    range<Load> getLoads();

    const_range<ShuntCompensator> getShuntCompensators() const;

    range<ShuntCompensator> getShuntCompensators();

    const_range<StaticVarCompensator> getStaticVarCompensators() const;

    range<StaticVarCompensator> getStaticVarCompensators();

    const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    range<TwoWindingsTransformer> getTwoWindingsTransformers();

    virtual double getV() const = 0;

    virtual VoltageLevel& getVoltageLevel() const = 0;

    const_range<VscConverterStation> getVscConverterStations() const;

    range<VscConverterStation> getVscConverterStations();

    virtual Bus& setAngle(double angle) = 0;

    virtual Bus& setV(double v) = 0;

protected:
    explicit Bus(const std::string& id);

    Bus(const std::string& id, const std::string& name);

private: // Identifiable
    const std::string& getTypeDescription() const override;

    template <typename T>
    const_range<T> getAll() const;

    template <typename T>
    range<T> getAll();

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUS_HPP
