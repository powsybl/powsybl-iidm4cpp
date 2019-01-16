/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGER_HPP
#define POWSYBL_IIDM_TAPCHANGER_HPP

#include <set>
#include <string>
#include <vector>

#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;
class Network;
class Terminal;
class VariantManagerHolder;

template<typename H, typename C, typename S>
class TapChanger : public MultiVariantObject {
public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    ~TapChanger() noexcept override = default;

    const S& getCurrentStep() const;

    S& getCurrentStep();

    long getHighTapPosition() const;

    long getLowTapPosition() const;

    stdcxx::CReference<Terminal> getRegulationTerminal() const;

    stdcxx::Reference<Terminal>& getRegulationTerminal();

    const S& getStep(long tapPosition) const;

    S& getStep(long tapPosition);

    unsigned int getStepCount() const;

    long getTapPosition() const;

    bool isRegulating() const;

    virtual void remove() = 0;

    virtual C& setRegulating(bool regulating);

    virtual C& setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal);

    C& setTapPosition(long tapPosition);

protected:
    TapChanger(VariantManagerHolder& network, H& parent, long lowTapPosition, const std::vector<S>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
               long tapPosition, bool regulating);

    virtual const Network& getNetwork() const;

    virtual Network& getNetwork();

    const H& getParent() const;

    H& getParent();

    virtual const std::string& getTapChangerAttribute() const = 0;

    virtual Identifiable& getTransformer();

    virtual const Identifiable& getTransformer() const;

private:
    stdcxx::Reference<VariantManagerHolder> m_network;

    H& m_parent;

    long m_lowTapPosition;

    std::vector<S> m_steps;

    stdcxx::Reference<Terminal> m_regulationTerminal;

    std::vector<long> m_tapPosition;

    std::vector<bool> m_regulating;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/TapChanger.hxx>

#endif  // POWSYBL_IIDM_TAPCHANGER_HPP
