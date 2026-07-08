/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_DCCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/AbstractComponentsManager.hpp>

namespace powsybl {

namespace iidm {

class DcComponentsManager : public AbstractComponentsManager {
public:
    explicit DcComponentsManager(Network& network);

    DcComponentsManager(Network& network, DcComponentsManager&& manager) noexcept;

private:  // AbstractComponentsManager
    const std::string& getComponentLabel() const override;

protected:  // AbstractComponentsManager
    virtual void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) override;
    virtual void setComponentNumber(DcBus& dcBus, const stdcxx::optional<unsigned long>& num) override;

private:
    static std::unique_ptr<Component> createComponent(Network& network, unsigned long num, unsigned long size);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCCOMPONENTSMANAGER_HPP
