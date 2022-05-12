FROM wiiuenv/devkitppc:20220507

COPY --from=wiiuenv/wiiumodulesystem:20220512 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libsdutils:20220428 /artifacts $DEVKITPRO

WORKDIR project
