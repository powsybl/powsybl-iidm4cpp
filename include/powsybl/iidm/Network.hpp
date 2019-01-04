/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_HPP
#define POWSYBL_IIDM_NETWORK_HPP

#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/ObjectStore.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

namespace powsybl {

namespace iidm {

class BusbarSection;
class Connectable;
class DanglingLine;
class Generator;
class HvdcConverterStation;
class LccConverterStation;
class Line;
class LineAdder;
class Load;
class ShuntCompensator;
class StaticVarCompensator;
class Substation;
class TieLineAdder;
class VoltageLevel;
class VscConverterStation;

class Network : public Container, public VariantManagerHolder {
public: // VariantManagerHolder
    unsigned long getVariantIndex() const override;

    const VariantManager& getVariantManager() const override;

    VariantManager& getVariantManager() override;

public:
    Network(const std::string& id, const std::string& sourceFormat);

    Network(Network&&) = default;

    ~Network() noexcept override = default;

    template <typename T> ObjectStore::iterator<T> begin() {
        return m_objectStore.begin<T>();
    }

    template <typename T> ObjectStore::const_iterator<T> cbegin() const {
        return m_objectStore.cbegin<T>();
    }

    template <typename T> ObjectStore::const_iterator<T> cend() const {
        return m_objectStore.cend<T>();
    }

    template <typename T> T& checkAndAdd(std::unique_ptr<T>&& identifiable) {
        return m_objectStore.checkAndAdd(std::move(identifiable));
    }

    template <typename T> ObjectStore::iterator<T> end() {
        return m_objectStore.end<T>();
    }

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    T& get(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    unsigned long getObjectCount() const;

    BusbarSection& getBusbarSection(const std::string& id) const;

    unsigned long getBusbarSectionCount() const;

    Connectable& getConnectable(const std::string& id) const;

    DanglingLine& getDanglingLine(const std::string& id) const;

    unsigned long getDanglingLineCount() const;

    int getForecastDistance() const;

    Generator& getGenerator(const std::string& id) const;

    unsigned long getGeneratorCount() const;

    HvdcConverterStation& getHvdcConverterStation(const std::string& id) const;

    unsigned long getHvdcConverterStationCount() const;

    LccConverterStation& getLccConverterStation(const std::string& id) const;

    unsigned long getLccConverterStationCount() const;

    Line& getLine(const std::string& id) const;

    unsigned long getLineCount() const;

    Load& getLoad(const std::string& id) const;

    unsigned long getLoadCount() const;

    ShuntCompensator& getShuntCompensator(const std::string& id) const;

    unsigned long getShuntCompensatorCount() const;

    const std::string& getSourceFormat() const;

    StaticVarCompensator& getStaticVarCompensator(const std::string& id) const;

    unsigned long getStaticVarCompensatorCount() const;

    Substation& getSubstation(const std::string& id) const;

    unsigned long getSubstationCount() const;

    VoltageLevel& getVoltageLevel(const std::string& id) const;

    unsigned long getVoltageLevelCount() const;

    VscConverterStation& getVscConverterStation(const std::string& id) const;

    unsigned long getVscConverterStationCount() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value>::type>
    stdcxx::Reference<T> find(const std::string& id) const;

    LineAdder newLine();

    SubstationAdder newSubstation();

    TieLineAdder newTieLine();

    void remove(Identifiable& identifiable);

    Network& setForecastDistance(int forecastDistance);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    std::string m_sourceFormat;

    int m_forecastDistance;

    ObjectStore m_objectStore;

    VariantManager m_variantManager;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Network.hxx>

#endif  // POWSYBL_IIDM_NETWORK_HPP
