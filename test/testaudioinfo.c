/*
  Copyright (C) 1997-2022 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#include <SDL3/SDL.h>

static void
print_devices(int iscapture)
{
    SDL_AudioSpec spec;
    const char *typestr = ((iscapture) ? "capture" : "output");
    int n = SDL_GetNumAudioDevices(iscapture);

    SDL_Log("Found %d %s device%s:\n", n, typestr, n != 1 ? "s" : "");

    if (n == -1) {
        SDL_Log("  Driver can't detect specific %s devices.\n\n", typestr);
    } else if (n == 0) {
        SDL_Log("  No %s devices found.\n\n", typestr);
    } else {
        int i;
        for (i = 0; i < n; i++) {
            const char *name = SDL_GetAudioDeviceName(i, iscapture);
            if (name != NULL) {
                SDL_Log("  %d: %s\n", i, name);
            } else {
                SDL_Log("  %d Error: %s\n", i, SDL_GetError());
            }

            if (SDL_GetAudioDeviceSpec(i, iscapture, &spec) == 0) {
                SDL_Log("     Sample Rate: %d\n", spec.freq);
                SDL_Log("     Channels: %d\n", spec.channels);
                SDL_Log("     SDL_AudioFormat: %X\n", spec.format);
            }
        }
        SDL_Log("\n");
    }
}

int main(int argc, char **argv)
{
    char *deviceName = NULL;
    SDL_AudioSpec spec;
    int n;

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Load the SDL library */
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    /* Print available audio drivers */
    n = SDL_GetNumAudioDrivers();
    if (n == 0) {
        SDL_Log("No built-in audio drivers\n\n");
    } else {
        int i;
        SDL_Log("Built-in audio drivers:\n");
        for (i = 0; i < n; ++i) {
            SDL_Log("  %d: %s\n", i, SDL_GetAudioDriver(i));
        }
        SDL_Log("Select a driver with the SDL_AUDIODRIVER environment variable.\n");
    }

    SDL_Log("Using audio driver: %s\n\n", SDL_GetCurrentAudioDriver());

    print_devices(0);
    print_devices(1);

    if (SDL_GetDefaultAudioInfo(&deviceName, &spec, 0) < 0) {
        SDL_Log("Error when calling SDL_GetDefaultAudioInfo: %s\n", SDL_GetError());
    } else {
        SDL_Log("Default Output Name: %s\n", deviceName != NULL ? deviceName : "unknown");
        SDL_free(deviceName);
        SDL_Log("Sample Rate: %d\n", spec.freq);
        SDL_Log("Channels: %d\n", spec.channels);
        SDL_Log("SDL_AudioFormat: %X\n", spec.format);
    }

    if (SDL_GetDefaultAudioInfo(&deviceName, &spec, 1) < 0) {
        SDL_Log("Error when calling SDL_GetDefaultAudioInfo: %s\n", SDL_GetError());
    } else {
        SDL_Log("Default Capture Name: %s\n", deviceName != NULL ? deviceName : "unknown");
        SDL_free(deviceName);
        SDL_Log("Sample Rate: %d\n", spec.freq);
        SDL_Log("Channels: %d\n", spec.channels);
        SDL_Log("SDL_AudioFormat: %X\n", spec.format);
    }

    SDL_Quit();
    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */
