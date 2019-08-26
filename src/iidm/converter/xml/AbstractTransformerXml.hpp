/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP

#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TapChangerStep.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename T, typename A>
class AbstractTransformerXml : public AbstractConnectableXml<T, A, Substation> {
public:
    ~AbstractTransformerXml() noexcept override = default;

protected:
    AbstractTransformerXml() = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractTransformerXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP

