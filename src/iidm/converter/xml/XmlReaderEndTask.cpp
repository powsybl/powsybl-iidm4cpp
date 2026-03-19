/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/XmlReaderEndTask.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<converter::xml::XmlReaderEndTask::Step>() {
    static std::initializer_list<std::string> s_typesNames {
        "BEFORE_EXTENSIONS",
        "AFTER_EXTENSIONS"
    };
    return s_typesNames;
}

}  // namespace Enum

namespace converter {

namespace xml {

std::ostream& operator<<(std::ostream& stream, const XmlReaderEndTask::Step& value) {
    stream << Enum::toString(value);
    return stream;
}

XmlReaderEndTask::XmlReaderEndTask(const XmlReaderEndTask::Step& step, const std::function<void()>& task) :
    m_step(step),
    m_task(task),
    m_processed(false) {

}

const XmlReaderEndTask::Step& XmlReaderEndTask::getStep() const {
    return m_step;
}

const std::function<void()>& XmlReaderEndTask::getTask() const {
    return m_task;
}

void XmlReaderEndTask::runTask() {
    if(!m_processed){
        m_processed = true;
        m_task();
    }
}

bool XmlReaderEndTask::isProcessed() const {
    return m_processed;
}
void XmlReaderEndTask::setProcessed(bool processed) {
    m_processed = processed;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl