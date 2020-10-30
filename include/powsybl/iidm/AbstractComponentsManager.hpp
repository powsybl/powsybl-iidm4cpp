/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP

#include <map>
#include <string>
#include <vector>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Network;

template <typename C>
class AbstractComponentsManager {
public:
    stdcxx::CReference<C> getComponent(const stdcxx::optional<unsigned long>& num) const;

    stdcxx::const_range<C> getConnectedComponents() const;

    stdcxx::range<C> getConnectedComponents();

    void invalidate();

    void update();

protected:
    AbstractComponentsManager() = default;

    virtual ~AbstractComponentsManager() noexcept = default;

    void addToAdjacencyList(const stdcxx::CReference<Bus>& bus1, const stdcxx::CReference<Bus>& bus2, const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;

    virtual std::unique_ptr<C> createComponent(unsigned long num, unsigned long size) = 0;

    virtual void fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;

    virtual const Network& getNetwork() const = 0;

    virtual Network& getNetwork() = 0;

    virtual void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) = 0;

private:
    virtual const std::string& getComponentLabel() const = 0;

private:
    std::vector<std::unique_ptr<C>> m_components;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/AbstractComponentsManager.hxx>

#endif  // POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP
