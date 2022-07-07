# BareBoot

A bare-metal bootloader for low-power embedded systems.

## Description

This project reflects the current state of my master thesis. 
The goal is to develop a bootloader to allow safe and secure In-Ortit software updates for cubesats. 

In order to reach this goal the bootloader is one component that manages, loads and verifies multiple images slots and picks a good image.

Updating a new image at runtime is the other component, not part of this repository, but also in the scope of my thesis. It makes use of RODOS(Realtime Onboard Dependable Operating System) and DOSIS and might be published at a later point in time. 

## Features

- Verify Images are still in desired state using CRC
- Authenticate Images are send by us using message authentication codes
- Transmission is expected to be unencrypted
- Space on devices is limited and depends on the microcontroller.

## Target

The initial target hardware is VA41620 from Vorage with an ARM CPU. 


