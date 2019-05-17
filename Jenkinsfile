/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

def maintainers = "mathieu.bague@rte-france.com"

// Enable code-coverage only for the master branch
def withSonar = (env.gitlabActionType == null) || (env.gitlabSourceBranch == "master")

/**
 * Build powsybl-iidm inside a docker image, using the specified compiler
 */
def build(os, compilerFamily, withQualityCheck, withCodeCoverage) {
    // Define the docker image
    def dockerImage = 'powsybl-iidm:' + os

    // Define the C++ compiler
    def compiler = null
    switch (compilerFamily) {
    case 'gcc':
        compiler = 'g++'
        break
    case 'clang':
        compiler = 'clang++'
        break
    default:
        throw new IllegalArgumentException('Unsupported compiler family: ' + compilerFamily)
    }

    // Define the stage name
    def stage = os + ' (' + compiler + ')'

    // Define the build directory
    def buildDirectory = 'build-' + os + '-' + compilerFamily

    // Build options
    def clangTidy = withQualityCheck ? 'clang-tidy' : ''
    def buildType = withCodeCoverage ? 'Debug' : 'Release'
    def codeCoverage = withCodeCoverage ? 'TRUE' : 'FALSE'
    def dockerOptions = withCodeCoverage ? '-v /home/jenkins/tools/build-wrapper-linux-x86:/home/jenkins/tools' : ''
    def buildWrapper = withCodeCoverage ? '/home/jenkins/tools/build-wrapper-linux-x86-64 --out-dir ./output' : ''

    docker.image(dockerImage).inside(dockerOptions) { c ->

        // Build
        stageDevin(stage) {
            dir(buildDirectory) {
                // Remove old build directory
                deleteDir()

                // Compile and run tests
                sh """
                cmake -DCMAKE_BUILD_TYPE=${buildType} -DCODE_COVERAGE=${codeCoverage} -DCMAKE_CXX_COMPILER=${compiler} -DCMAKE_CXX_CLANG_TIDY=${clangTidy} ..
                ${buildWrapper} make -j4
                make tests
                """

                // Run tests, compute code coverage results and stash the reports
                if (withCodeCoverage) {
                    sh """
                    make code-coverage

                    # Clean non relevant files
                    rm -f coverage/reports/#usr#*
                    """
                }
            }
        }
    }

    // Run sonar analysis
    if (withCodeCoverage) {
        stageDevin('Sonarqube') {
            sonar {
                useMaven = false
            }
        }
    }
}

node('powsybl-rh72') {

    try {
        stageDevin('Cloning git repository') {
            gitCheckout {}
        }

        // Alpine
        build('alpine', 'gcc', false, false)
        build('alpine', 'clang', false, false)

        // CentOS
        build('centos', 'gcc', false, false)
        build('centos', 'clang', false, false)

        // Ubuntu
        build('ubuntu', 'gcc', true, false)
        build('ubuntu', 'clang', true, false)

        // Sonar
        if (withSonar) {
            build('sonar', 'gcc', false, true)
        }

    } catch (Exception e) {
        notify {
            to = maintainers
            errorMsg = e.toString()
        }
        throw e
    } finally {
        step([$class: 'WsCleanup'])
    }
}
