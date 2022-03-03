FROM wiiuenv/devkitppc:20220303

COPY --from=wiiuenv/libkernel:20211031 /artifacts $DEVKITPRO
COPY --from=wiiuenv/wiiumodulesystem:20220123 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libsdutils:20220303 /artifacts $DEVKITPRO

WORKDIR project