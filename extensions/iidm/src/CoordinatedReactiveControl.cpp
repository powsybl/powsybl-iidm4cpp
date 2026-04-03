/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControl.hpp>

#include <cmath>

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

CoordinatedReactiveControl::CoordinatedReactiveControl(Generator& generator, double qPercent) :
    AbstractMultiVariantIdentifiableExtension(generator) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_qPercent.resize(variantArraySize, checkQPercent(generator, qPercent));
}

void CoordinatedReactiveControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

double CoordinatedReactiveControl::checkQPercent(const Generator& generator, double qPercent) {
    if (std::isnan(qPercent)) {
        throw ValidationException(generator, stdcxx::format("Undefined value (%1%) for qPercent", qPercent));
    }
    if (qPercent < 0.0 || qPercent > 100.0) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<CoordinatedReactiveControl>();
        logger.debug(stdcxx::format("qPercent value of generator %1% does not seem to be a valid percent: %2%", generator.getId(), qPercent));
    }
    return qPercent;
}

const std::string& CoordinatedReactiveControl::getName() const {
    static std::string s_name = "coordinatedReactiveControl";
    return s_name;
}

double CoordinatedReactiveControl::getQPercent() const {
    return m_qPercent[getVariantIndex()];
}

const std::type_index& CoordinatedReactiveControl::getType() const {
    static std::type_index s_type = typeid(CoordinatedReactiveControl);
    return s_type;
}

CoordinatedReactiveControl& CoordinatedReactiveControl::setQPercent(double qPercent) {
    m_qPercent[getVariantIndex()] = checkQPercent(getExtendable<Generator>(), qPercent);
    return *this;
}

void CoordinatedReactiveControl::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_qPercent[index] = m_qPercent[sourceIndex];
    }
}

void CoordinatedReactiveControl::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void CoordinatedReactiveControl::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_qPercent.resize(m_qPercent.size() + number, m_qPercent[sourceIndex]);
}

void CoordinatedReactiveControl::reduceVariantArraySize(unsigned long number) {
    m_qPercent.resize(m_qPercent.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
