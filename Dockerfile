FROM gcc:latest

RUN apt-get update

RUN apt-get install -y python python-pip gcovr

