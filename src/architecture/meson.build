# Architecture dependent 
architecture = host_machine.cpu()
supported_architectures = ['armv7']

# Collect sources based on architecture
if architecture in supported_architectures
    subdir(architecture)
else 
    subdir('dummy')
endif
