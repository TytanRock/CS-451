pipeline {
	agent {
		dockerfile true
	}


	stages {
		stage ('clean') {
			steps {
				dir('Ness1') {
					sh 'make clean'
				}
			}
		}
		stage ('build 1') {
			steps {
				dir('Ness1') {
					sh 'make'
				}
			}
		}
		stage ('Test 1') {
			steps {
				dir('Ness1') {
					sh 'make bin/5ps_test'
					sh './bin/5ps_test'
				}
			}
		}
		stage ('Coverage 1') {
			steps {
				dir('Ness1') {
					sh 'make gcov/5ps'
					dir('gcov') {
						sh 'gcovr -r . --xml -o rep.xml'
					}
				}
			}
		}
	}

	post {
		always {
			junit 'Ness1/*.xml'
			cobertura coberturaReportFile: 'Ness1/gcov/*.xml'
		}
	}
}
