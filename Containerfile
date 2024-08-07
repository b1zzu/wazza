FROM gcc:14
COPY . /usr/src/wazza
WORKDIR /usr/src/wazza
RUN echo "TODO: build wazza" && exit 1

FROM scratch
COPY --from=0 /bin/wazza /bin/wazza
CMD ["/bin/wazza"]
