{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    cmake
    gdb
    valgrind
    ninja
    pkg-config
    git

    alsa-lib

    vulkan-headers
    vulkan-loader

    libX11
    libXext
    libXrandr
    libXcursor
    libXi
    libXinerama
    libXfixes
    libXScrnSaver
    libXtst

    libxcb
    xcbutil
    xcbutilwm
    xcbutilimage
    xcbutilkeysyms
    xcbutilrenderutil

    wayland
    wayland-protocols
    libxkbcommon

    libGL
    libGLU

    doxygen
    perf

    luajit
  ];

  shellHook = ''
  export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [
    pkgs.vulkan-loader
    pkgs.mesa
    pkgs.libGL
    pkgs.luajit
  ]}:$LD_LIBRARY_PATH
'';
}
