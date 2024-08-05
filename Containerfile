FROM gcc:14
COPY . /usr/src/wazza
WORKDIR /usr/src/wazza
RUN gcc -static -o /bin/wazza src/main.c

FROM scratch
COPY --from=0 /bin/wazza /bin/wazza
CMD ["/bin/wazza"]
