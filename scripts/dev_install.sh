echo "Installing dev kit helper"
echo "All right reserved to Hugo Ponsin"

check_nix_or_exit() {
    if ! command -v nix-shell &> /dev/null && ! command -v nix &> /dev/null; then
        echo "Nix not available on this device" >&2
        echo "   Please install nix." >&2
        exit 1
    fi
    echo "Nix detected"
}

download_git_submodule() {
    echo "Installing git submodule..."
    
    if git submodule update --init --recursive; then
        echo "Installation completed"
    else
        echo "Error: Failed to update git submodules" >&2
        return 1
    fi
}

compile_shaderc(){
    echo "Compiling shaderc..."
    if cd ./external/bgfx/ && make shaderc -j $(nproc); then
        echo "Compiling completed"
    else
        echo "Error: Failed to compile shaderc" >&2
        return 1
    fi
}

check_nix_or_exit
download_git_submodule
compile_shaderc
