/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERHOLDER_HPP
#define POWSYBL_IIDM_TAPCHANGERHOLDER_HPP

#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;
class Network;

class TapChangerHolder : public virtual Validable {
public:
    ~TapChangerHolder() noexcept override = default;

    virtual const Network& getNetwork() const = 0;

    virtual Network& getNetwork() = 0;

    virtual bool hasPhaseTapChanger() const = 0;

    virtual bool hasRatioTapChanger() const = 0;

protected:
    TapChangerHolder() noexcept = default;

private:
    virtual unsigned long getRegulatingTapChangerCount() const = 0;

private:
    friend class PhaseTapChangerAdder;

    friend class RatioTapChangerAdder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGERHOLDER_HPP
