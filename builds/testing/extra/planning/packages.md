## **OVERVIEW OF C PACKAGES AND INSTALLATIONS**

**DEBIAN BASED SYSTEM**

- A popular OS that uses unix Kernel - such as Linux - alongside other GNU components
- MacOS is debian-based since it is built off of linux

**PACKAGE MANAGEMENT SYSTEMS**

- Most Debian-based systems use the APT package manager
- MacOS does not use APT, instead use Homebrew as a replacement

**PACKAGES**

_Vulkan_

- A way to leverage the low-level, granular control of OpenGL on MacOS

  | term            | Defintion                                                                                                                                                          |
  | --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
  | ICD             | Installable Client Driver: A Vulkan compatible display driver                                                                                                      |
  | GLSL            | OpenGL Shading Language                                                                                                                                            |
  | Vulkan Instance | The Vulkan API object that stores all per-application states                                                                                                       |
  | Vulkan Layer    | A library designed to work as a plug-in for the loader. It usually serves to provide validation and debugging functionality to applications                        |
  | Vulkan Loader   | A library which implements the Vulkan API entry points and manages layers, extensions, and drivers. It is found in the independent hardware vendor driver installs |
  | SPIR-V          | Standard Portable Intermediate Representation is a cross-API intermediate language (IL) that natively represents parallel compute and graphics programs            |

- MoltenVK is a way to use Vulkan on Apple Operating Systems

  - for Desktop Applications (the preferred and feature rich way of using Molten), use the MoletnVK Dynamic Library in combination with the Vulkan Loader

  - for
