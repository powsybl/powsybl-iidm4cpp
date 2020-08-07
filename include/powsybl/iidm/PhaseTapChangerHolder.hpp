/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERHOLDER_HPP
#define POWSYBL_IIDM_PHASETAPCHANGERHOLDER_HPP

#include <powsybl/iidm/TapChangerHolder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class PhaseTapChanger;
class PhaseTapChangerAdder;

class PhaseTapChangerHolder : public virtual TapChangerHolder {
public:
    ~PhaseTapChangerHolder() noexcept override = default;

    stdcxx::CReference<PhaseTapChanger> getOptionalPhaseTapChanger() const;

    stdcxx::Reference<PhaseTapChanger> getOptionalPhaseTapChanger();

    virtual const PhaseTapChanger& getPhaseTapChanger() const = 0;

    virtual PhaseTapChanger& getPhaseTapChanger() = 0;

    virtual PhaseTapChangerAdder newPhaseTapChanger() = 0;

protected:
    PhaseTapChangerHolder() noexcept = default;

private:
    virtual void setPhaseTapChanger(std::unique_ptr<PhaseTapChanger> ratioTapChanger) = 0;

private:
    friend class PhaseTapChanger;

    friend class PhaseTapChangerAdder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERHOLDER_HPP
