pipeline {
	agent any


	stages {
		stage ('clean') {
			dir ('Ness1') {
				steps {
					sh 'make clean'
				}
			}
		}
		stage ('build 1') {
			dir ('Ness1') {
				steps {
					sh 'cd Ness1 && make'
				}
			}
		}
	}
}
