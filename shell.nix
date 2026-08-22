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

    libGL
    libGLU

    doxygen

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
