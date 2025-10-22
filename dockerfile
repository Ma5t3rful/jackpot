FROM ubuntu:25.04
RUN apt update -y && apt upgrade -y && apt install -y gcc-15 g++-15 cmake make libssl-dev libzstd-dev
COPY . /usr/shuffler-src
RUN rm -fr /usr/shuffler-src/build/ /usr/shuffler-src/.cache/
WORKDIR /usr/shuffler-src/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build . --config Release 
CMD /usr/bin/python3 -m http.server -d ../website -b 0.0.0.0 &\
    ./shufflerbot || :&&\
    echo now lets kill python&&\
    killall -s KILL python3