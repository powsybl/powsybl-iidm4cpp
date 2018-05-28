/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

def maintainers = "mathieu.bague@rte-france.com"

// Enable code-coverage only for the master branch
def withSonar = ((env.gitlabActionType == null) || (env.gitlabSourceBranch == "master"))
def buildType = withSonar ? "Debug" : "Release"
def codeCoverage = withSonar ? "TRUE" : "FALSE"

def gitCheckout() {
    if (env.gitlabActionType == null) {
        // Checkout the master branch
        checkout scm
    } else {
        switch(env.gitlabActionType) {
        case 'MERGE':
            // Checkout the source branch and merge with the target branch
            checkout changelog: true, poll: true, scm: [
                $class: 'GitSCM',
                branches: [[name: "origin/${env.gitlabSourceBranch}"]],
                doGenerateSubmoduleConfigurations: false,
                extensions: [
                    [$class: 'UserIdentity', email: 'no-reply@rte-france.com', name: 'jenkins'],
                    [$class: 'PreBuildMerge', options: [fastForwardMode: 'NO_FF', mergeRemote: 'origin', mergeStrategy: 'default', mergeTarget: "${env.gitlabTargetBranch}"]]
                ],
                submoduleCfg: [],
                userRemoteConfigs: [[credentialsId: 'jenkins-gitlab-devin', name: 'origin', url: "${env.gitlabSourceRepoHttpUrl}"]]
            ]
            break

        case 'PUSH':
            checkout changelog: true, poll: true, scm: [
                $class: 'GitSCM',
                branches: [[name: "origin/${env.gitlabSourceBranch}"]],
                doGenerateSubmoduleConfigurations: false,
                submoduleCfg: [],
                userRemoteConfigs: [[credentialsId: 'jenkins-gitlab-devin', name: 'origin', url: "${env.gitlabSourceRepoHttpUrl}"]]
            ]
            break

        default:
            println "Unsupported GitLab action: ${env.gitlabActionType}"
            currentBuild.result = 'ERROR'
        }
    }
}

node('power-system-tools') {

    try {
        stage('Cloning git repository') {
            gitCheckout()
        }

        stage('Building with GCC') {
            gitlabCommitStatus('GCC') {
                dir('build') {
                    // Remove old build directory
                    deleteDir()

                    sh """
                    cmake -DCMAKE_BUILD_TYPE=${buildType} -DCODE_COVERAGE=${codeCoverage} -DCMAKE_CXX_COMPILER=g++ ..
                    make -j4
                    make tests
                    """
                }
            }
        }

        /*
        stage('Building with Clang') {
            gitlabCommitStatus('Clang') {
                dir('build-clang') {
                    // Remove old build directory
                    deleteDir()

                    sh """
                    cmake -DCMAKE_CXX_COMPILER=clang++ ..
                    make -j4
                    make tests
                    """
                }
            }
        }
        */

        if (withSonar) {
            stage('Code-coverage') {
                gitlabCommitStatus('code-coverage') {
                    // Run tests and compute code coverage results
                    dir('build') {
                        sh """
                        make code-coverage

                        # Clean non relevant files
                        rm -f coverage/reports/#usr#*
                        """

                        // Stash
                        stash includes: 'coverage/reports/*.gcov', name: 'code-coverage'
                    }
                }
            }
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

node('build') {

    try {
        if (withSonar) {
            stage('Sonarqube') {
                gitlabCommitStatus('sonar') {
                    gitCheckout()

                    dir('build') {
                        // Clean old code coverage results
                        deleteDir()

                        // Unstash new code coverage results
                        unstash "code-coverage"
                    }

                    // Run sonar analysis
                    sonar {
                        useMaven = "false"
                    }
                }
            }
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
