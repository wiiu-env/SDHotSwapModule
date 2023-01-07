FROM wiiuenv/devkitppc:20221228

COPY --from=wiiuenv/wiiumodulesystem:20230106 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libsdutils:20220903 /artifacts $DEVKITPRO

WORKDIR project
