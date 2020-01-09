/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/CurrentLimitsPerSeason.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Connectable.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

std::unique_ptr<CurrentLimits> clone(const std::unique_ptr<CurrentLimits>& currentLimits) {
    std::unique_ptr<CurrentLimits> clone;
    if (currentLimits) {
        clone = stdcxx::make_unique<CurrentLimits>(*currentLimits);
    }
    return clone;
}

CurrentLimitsPerSeason::CurrentLimitsTriple::CurrentLimitsTriple(std::unique_ptr<CurrentLimits> currentLimits, std::unique_ptr<CurrentLimits> currentLimits1,
                                                                 std::unique_ptr<CurrentLimits> currentLimits2, std::unique_ptr<CurrentLimits> currentLimits3) :
    m_currentLimits(std::move(currentLimits)),
    m_currentLimits1(std::move(currentLimits1)),
    m_currentLimits2(std::move(currentLimits2)),
    m_currentLimits3(std::move(currentLimits3)) {
}

CurrentLimitsPerSeason::CurrentLimitsTriple::CurrentLimitsTriple(const CurrentLimitsTriple& other) :
    m_currentLimits(clone(other.m_currentLimits)),
    m_currentLimits1(clone(other.m_currentLimits1)),
    m_currentLimits2(clone(other.m_currentLimits2)),
    m_currentLimits3(clone(other.m_currentLimits3)) {
}

CurrentLimitsPerSeason::CurrentLimitsTriple& CurrentLimitsPerSeason::CurrentLimitsTriple::operator=(const CurrentLimitsTriple& other) {
    if (!stdcxx::areSame(*this, other)) {
        m_currentLimits = clone(other.m_currentLimits);
        m_currentLimits1 = clone(other.m_currentLimits1);
        m_currentLimits2 = clone(other.m_currentLimits2);
        m_currentLimits3 = clone(other.m_currentLimits3);
    }
    return *this;
}

stdcxx::CReference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits() const {
    return m_currentLimits ? stdcxx::cref<CurrentLimits>(*m_currentLimits) : stdcxx::cref<CurrentLimits>();
}

stdcxx::Reference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits() {
    return m_currentLimits ? stdcxx::ref<CurrentLimits>(*m_currentLimits) : stdcxx::ref<CurrentLimits>();
}

stdcxx::CReference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits1() const {
    return m_currentLimits1 ? stdcxx::cref<CurrentLimits>(*m_currentLimits1) : stdcxx::cref<CurrentLimits>();
}

stdcxx::Reference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits1() {
    return m_currentLimits1 ? stdcxx::ref<CurrentLimits>(*m_currentLimits1) : stdcxx::ref<CurrentLimits>();
}

stdcxx::CReference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits2() const {
    return m_currentLimits2 ? stdcxx::cref<CurrentLimits>(*m_currentLimits2) : stdcxx::cref<CurrentLimits>();
}

stdcxx::Reference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits2() {
    return m_currentLimits2 ? stdcxx::ref<CurrentLimits>(*m_currentLimits2) : stdcxx::ref<CurrentLimits>();
}

stdcxx::CReference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits3() const {
    return m_currentLimits3 ? stdcxx::cref<CurrentLimits>(*m_currentLimits3) : stdcxx::cref<CurrentLimits>();
}

stdcxx::Reference<CurrentLimits> CurrentLimitsPerSeason::CurrentLimitsTriple::getCurrentLimits3() {
    return m_currentLimits3 ? stdcxx::ref<CurrentLimits>(*m_currentLimits3) : stdcxx::ref<CurrentLimits>();
}

CurrentLimitsAdder<std::nullptr_t, CurrentLimitsPerSeason::CurrentLimitsTriple> CurrentLimitsPerSeason::CurrentLimitsTriple::newCurrentLimits() {
    return CurrentLimitsAdder<std::nullptr_t, CurrentLimitsTriple>(nullptr, *this);
}

CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple> CurrentLimitsPerSeason::CurrentLimitsTriple::newCurrentLimits1() {
    return CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsTriple>(ThreeWindingsTransformer::Side::ONE, *this);
}

CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple> CurrentLimitsPerSeason::CurrentLimitsTriple::newCurrentLimits2() {
    return CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsTriple>(ThreeWindingsTransformer::Side::TWO, *this);
}

CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple> CurrentLimitsPerSeason::CurrentLimitsTriple::newCurrentLimits3() {
    return CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsTriple>(ThreeWindingsTransformer::Side::THREE, *this);
}

void CurrentLimitsPerSeason::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Connectable>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Connectable>()));
    }
}

void CurrentLimitsPerSeason::CurrentLimitsTriple::setCurrentLimits(const ThreeWindingsTransformer::Side& side, std::unique_ptr<CurrentLimits> limits) {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            m_currentLimits1 = stdcxx::make_unique<CurrentLimits>(*limits);
            break;

        case ThreeWindingsTransformer::Side::TWO:
            m_currentLimits2 = stdcxx::make_unique<CurrentLimits>(*limits);
            break;

        case ThreeWindingsTransformer::Side::THREE:
            m_currentLimits2 = stdcxx::make_unique<CurrentLimits>(*limits);
            break;

        default:
            throw AssertionError("Should not happen");
    }
}

void CurrentLimitsPerSeason::CurrentLimitsTriple::setCurrentLimits(std::nullptr_t /*side*/, std::unique_ptr<CurrentLimits> limits) {
    m_currentLimits = stdcxx::make_unique<CurrentLimits>(*limits);
}

CurrentLimitsPerSeason::CurrentLimitsPerSeason(Connectable& connectable, const std::map<std::string, CurrentLimitsPerSeason::CurrentLimitsTriple>& currentLimits) :
    Extension(connectable),
    m_currentLimits(currentLimits) {
}

CurrentLimitsPerSeason::CurrentLimitsPerSeason(Connectable& connectable) :
    Extension(connectable) {
}

const std::map<std::string, typename CurrentLimitsPerSeason::CurrentLimitsTriple>& CurrentLimitsPerSeason::getCurrentLimits() const {
    return m_currentLimits;
}

std::map<std::string, typename CurrentLimitsPerSeason::CurrentLimitsTriple>& CurrentLimitsPerSeason::getCurrentLimits() {
    return m_currentLimits;
}

std::string CurrentLimitsPerSeason::CurrentLimitsTriple::getMessageHeader() const {
    return "currentLimitsPerSeason";
}

const std::string& CurrentLimitsPerSeason::getName() const {
    static std::string s_name = "currentLimitsPerSeason";
    return s_name;
}

stdcxx::CReference<CurrentLimitsPerSeason::CurrentLimitsTriple> CurrentLimitsPerSeason::getSeason(const std::string& name) const {
    const auto& it = m_currentLimits.find(name);
    return it != m_currentLimits.end() ? stdcxx::cref<CurrentLimitsTriple>(it->second) : stdcxx::cref<CurrentLimitsTriple>();
}

stdcxx::Reference<CurrentLimitsPerSeason::CurrentLimitsTriple> CurrentLimitsPerSeason::getSeason(const std::string& name) {
    const auto& it = m_currentLimits.find(name);
    return it != m_currentLimits.end() ? stdcxx::ref<CurrentLimitsTriple>(it->second) : stdcxx::ref<CurrentLimitsTriple>();
}

const std::type_index& CurrentLimitsPerSeason::getType() const {
    static std::type_index s_type = typeid(CurrentLimitsPerSeason);
    return s_type;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

