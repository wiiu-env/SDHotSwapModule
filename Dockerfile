FROM wiiuenv/devkitppc:20220806

COPY --from=wiiuenv/wiiumodulesystem:20220904 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libsdutils:20220903 /artifacts $DEVKITPRO

WORKDIR project
