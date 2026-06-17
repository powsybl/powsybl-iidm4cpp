/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MANUALFREQUENCYRESTORATIONRESERVE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MANUALFREQUENCYRESTORATIONRESERVE_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Injection;

namespace extensions {

namespace iidm {

class ManualFrequencyRestorationReserve : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~ManualFrequencyRestorationReserve() noexcept override = default;

    bool isParticipate() const;

    ManualFrequencyRestorationReserve& setParticipate(bool participate);

protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    ManualFrequencyRestorationReserve(Injection& injection, bool participate);
    friend class ManualFrequencyRestorationReserveAdder;

private:
    /**
     * Indicates if the extended injection participates in the manual frequency restoration reserve
     */
    bool m_participate;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MANUALFREQUENCYRESTORATIONRESERVE_HPP
