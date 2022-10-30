# BareBoot

A bare-metal bootloader for embedded systems.
Mirrored from a private (and soon public) gitlab repository.

Currently supported platforms:
- Vorago VA41620

## Description

This project reflects the current state of my master thesis. 
The goal is to develop a bootloader to allow safe and secure On-Ortit software updates for cubesats. 

In order to reach this goal the bootloader is one component that manages, loads and verifies multiple images slots and picks a good image.

Updating a new image at runtime is done by the bootmanager, which is not part of this repository, but also in the scope of my thesis. It makes use of RODOS(Realtime Onboard Dependable Operating System) and might be published at a later point in time. 

## Features

- Verify Images are still in desired state using CRC
- Authenticate Images using message authentication codes (HMAC-SHA256)
- Transmission is expected to be unencrypted
- Space on devices is limited and depends on the microcontroller

## Resources

- [Thesis: Reliable Software Updates for On-Orbit CubeSats](https://www.this-is-patrick.net/assets/thesis.pdf)
- [RODOS](https://gitlab.com/rodos/rodos)
- [Initial-Metadata Script](https://github.com/ThisIsPatrickM/bareboot-initial-metadata)
