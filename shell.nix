{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    cmake
    ninja
    pkg-config
    git
    tree
    vscodium

    alsa-lib

    vulkan-headers
    vulkan-loader

    xorg.libX11
    xorg.libXext
    xorg.libXrandr
    xorg.libXcursor
    xorg.libXi
    xorg.libXinerama
    xorg.libXfixes
    xorg.libXScrnSaver
    xorg.libXtst

    xorg.libxcb
    xorg.xcbutil
    xorg.xcbutilwm
    xorg.xcbutilimage
    xorg.xcbutilkeysyms
    xorg.xcbutilrenderutil

    wayland
    wayland-protocols
    libxkbcommon
  ];
}
