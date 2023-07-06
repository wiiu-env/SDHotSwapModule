FROM ghcr.io/wiiu-env/devkitppc:20230621

COPY --from=ghcr.io/wiiu-env/wiiumodulesystem:20230622 /artifacts $DEVKITPRO
COPY --from=ghcr.io/wiiu-env/libsdutils:20230621 /artifacts $DEVKITPRO

WORKDIR project
