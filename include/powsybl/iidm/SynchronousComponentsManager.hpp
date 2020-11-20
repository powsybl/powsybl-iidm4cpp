/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SYNCHRONOUSCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_SYNCHRONOUSCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/AbstractComponentsManager.hpp>

namespace powsybl {

namespace iidm {

class SynchronousComponentsManager : public AbstractComponentsManager {
public:
    explicit SynchronousComponentsManager(Network& network);

    SynchronousComponentsManager(Network& network, SynchronousComponentsManager&& manager) noexcept;

protected:  // AbstractComponentsManager
    void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) override;

private:  // AbstractComponentsManager
    const std::string& getComponentLabel() const override;

private:
    static std::unique_ptr<Component> createComponent(Network& network, unsigned long num, unsigned long size);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SYNCHRONOUSCOMPONENTSMANAGER_HPP
