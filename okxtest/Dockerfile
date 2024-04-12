FROM ubuntu

COPY install_tools.sh .
RUN chmod +x install_tools.sh
RUN ./install_tools.sh

ARG username=user
ARG home_dir=/home/${username}

RUN mkdir test
COPY *.sh test/

COPY installboost.sh /test/
RUN chmod +x /test/installboost.sh
RUN /test/installboost.sh

RUN chmod +x test/installwebsocket.sh
RUN test/installwebsocket.sh 0.8.1

COPY src ${home_dir}/src
COPY include ${home_dir}/include
COPY main.cpp ${home_dir}
COPY CMakeLists.txt ${home_dir}

RUN mkdir ${home_dir}/build
WORKDIR ${home_dir}/build
RUN cmake .. && cat ${home_dir}/build/CMakeFiles/CMakeOutput.log
RUN cmake --build build

CMD ["./okxtest"]
