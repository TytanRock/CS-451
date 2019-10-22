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
		stage ('Cover 1') {
			steps {
				dir('Ness1') {
					sh 'pip install gcovr'
					sh 'make gcov/5ps'
					dir('Ness1/gcov') {
						sh 'gcovr -r . --html'
					}
				}
			}
		}
	}

	post {
		always {
			junit 'Ness1/*.xml'
		}
	}
}
