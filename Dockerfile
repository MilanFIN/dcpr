FROM devkitpro/devkitarm:latest
COPY ./ /gba-raycast-engine
RUN cd gba-raycast-engine && make
