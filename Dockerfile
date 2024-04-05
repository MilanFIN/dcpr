FROM devkitpro/devkitarm:latest
COPY ./ /dcpr
RUN cd dcpr && make
