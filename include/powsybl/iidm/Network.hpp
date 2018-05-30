/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_HPP
#define POWSYBL_IIDM_NETWORK_HPP

#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/iidm/ObjectStore.hpp>
#include <powsybl/iidm/StateManager.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>

namespace powsybl {

namespace iidm {

class BusbarSection;
class Connectable;
class Load;
class Substation;
class VoltageLevel;

class Network : public Container, public MultiStateObject {
public: // MultiStateObject
    unsigned long getStateIndex() const override;

    const StateManager& getStateManager() const override;

    StateManager& getStateManager() override;

public:
    Network(const std::string& id, const std::string& sourceFormat);

    Network(Network&&) = default;

    virtual ~Network() = default;

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

    BusbarSection& getBusbarSection(const std::string& id) const;

    unsigned long getBusbarSectionCount() const;

    Connectable& getConnectable(const std::string& id) const;

    int getForecastDistance() const;

    Load& getLoad(const std::string& id) const;

    unsigned long getLoadCount() const;

    const std::string& getSourceFormat() const;

    Substation& getSubstation(const std::string& id) const;

    unsigned long getSubstationCount() const;

    VoltageLevel& getVoltageLevel(const std::string& id) const;

    unsigned long getVoltageLevelCount() const;

    SubstationAdder newSubstation();

    void remove(Identifiable& identifiable);

    Network& setForecastDistance(int forecastDistance);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    std::string m_sourceFormat;

    int m_forecastDistance;

    ObjectStore m_objectStore;

    StateManager m_stateManager;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORK_HPP
