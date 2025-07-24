/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERTOBEESTIMATEDADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERTOBEESTIMATEDADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerToBeEstimatedAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ThreeWindingsTransformerToBeEstimatedAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    ThreeWindingsTransformerToBeEstimatedAdder(const ThreeWindingsTransformerToBeEstimatedAdder&) = default;

    /**
     * Move constructor
     */
    ThreeWindingsTransformerToBeEstimatedAdder(ThreeWindingsTransformerToBeEstimatedAdder&&) = default;

    /**
     * Destructor
     */
    ~ThreeWindingsTransformerToBeEstimatedAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    ThreeWindingsTransformerToBeEstimatedAdder& operator=(const ThreeWindingsTransformerToBeEstimatedAdder&) = delete;

    /**
     * Move assignment operator
     */
    ThreeWindingsTransformerToBeEstimatedAdder& operator=(ThreeWindingsTransformerToBeEstimatedAdder&&) = delete;

    ThreeWindingsTransformerToBeEstimatedAdder& withPhaseTapChanger1Status(bool toBeEstimated);

    ThreeWindingsTransformerToBeEstimatedAdder& withPhaseTapChanger2Status(bool toBeEstimated);

    ThreeWindingsTransformerToBeEstimatedAdder& withPhaseTapChanger3Status(bool toBeEstimated);

    ThreeWindingsTransformerToBeEstimatedAdder& withRatioTapChanger1Status(bool toBeEstimated);

    ThreeWindingsTransformerToBeEstimatedAdder& withRatioTapChanger2Status(bool toBeEstimated);

    ThreeWindingsTransformerToBeEstimatedAdder& withRatioTapChanger3Status(bool toBeEstimated);

protected:
    /**
     * Creates the ThreeWindingsTransformerToBeEstimated extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    bool m_ptc1Status = false;

    bool m_ptc2Status = false;

    bool m_ptc3Status = false;

    bool m_rtc1Status = false;

    bool m_rtc2Status = false;

    bool m_rtc3Status = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERTOBEESTIMATEDADDER_HPP
