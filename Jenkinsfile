pipeline {
	agent {
		docker {
			image 'gcc:latest'
		}
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
					sh 'cd Ness1 && make'
				}
			}
		}
	}
}
