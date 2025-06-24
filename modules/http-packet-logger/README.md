# Project Overview

This repository provides tools and resources for customizing and building a Linux kernel with a focus on packet filtering using the Netfilter subsystem. It also includes scripts and configurations for setting up a testing environment within virtual machines.

---

## Repository Structure

### 1. `kernel` Folder
Contains the kernel configuration, source code, and a custom kernel module. The `Makefile` in this folder supports various tasks:

- **config**: Copies the kernel configuration file into the Linux kernel source directory.
- **kbuild**: Builds the entire Linux kernel.
- **install**: Creates a symbolic link to the compiled kernel (`bzImage`) inside the `tests/vm` folder, enabling the VM to boot with this custom kernel.
- **kmodule**: Builds the custom kernel module and copies the resulting `.ko` file into the `tests/vm/shared` folder. This allows the VM's guest OS to load the module using `insmod`.

**Note:** The `modules` subfolder contains the source code for the custom kernel module and a `Makefile` to compile and install it into the shared VM folder. Refer to the specific `README.md` within the `modules` folder for instructions on recompiling and copying the module after modifications.

---

### 2. `podman` Folder
Includes scripts and configurations to set up a containerized environment for building and running the kernel and modules.

**Note:** For setup instructions, see the `README.md` inside the `podman` folder.

---

### 3. `tests` Folder
Contains:
- `vm`: The root filesystem used by the virtual machine to run the guest OS with the custom kernel. It also includes scripts to build, run, and connect to the VM.
- `scripts`: Collection of scripts for testing and various use cases within the VM environment.

---

## How to Get Started

1. Navigate to the `podman` folder:

   ```bash
   cd podman
   ```

2. Follow the instructions provided in the `README.md` within the `podman` folder to set up the containerized environment.

---

## Additional Notes
- After setting up the environment, you can build and install the kernel and modules using the provided make targets.
- Use the scripts inside the `tests/scripts` folder for testing specific functionalities within the VM.

---

## Summary
This repository is designed to facilitate customizing the Linux kernel, building kernel modules, and testing them within virtual machines, all orchestrated through containerized environments. Follow the provided instructions in each subfolder's `README.md` files to properly set up and operate the environment.

---

*For further assistance or questions, refer to the individual README files within each folder.*
---
## M3 Project Submission: Advanced HTTP Packet Logger

This repository contains the source code for the advanced HTTP Packet Logger kernel module. The code is located in the `/modules/http-packet-logger/` directory.

A full report detailing the project results, replication instructions, and design notes is available at the link below.

### [Click Here to View Full Project Documentation](https://docs.google.com/document/d/1QJcuk91UCle3sdX3GfGloij6Md_4XN3uvEQtN9633lM/edit?usp=sharing)


