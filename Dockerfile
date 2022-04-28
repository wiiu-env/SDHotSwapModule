FROM wiiuenv/devkitppc:20220417

COPY --from=wiiuenv/wiiumodulesystem:20220123 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libsdutils:20220428 /artifacts $DEVKITPRO

WORKDIR project