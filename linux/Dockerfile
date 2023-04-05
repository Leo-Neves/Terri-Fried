FROM ubuntu:latest

#RUN apt-get purge binutils
#RUN apt-get remove make
RUN apt-get autoremove
RUN apt-get update && apt-get install -y build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN ls -lah /app
RUN cd /app
#RUN ls -lah ./ && cmake
RUN make
CMD ["cd", "/app", "&&", "make", "&&"]