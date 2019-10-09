pipeline {
	agent any


	stages {
		dir ('Ness1') {
			stage ('clean') {
				steps {
					sh 'make clean'
				}
			}
		
			stage ('build 1') {
				steps {
					sh 'cd Ness1 && make'
				}
			}
		}
	}
}
