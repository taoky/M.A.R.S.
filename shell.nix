{ pkgs ? import <nixpkgs> {} }:

with pkgs; mkShell {
    buildInputs = [
        sfml
        cmake
        fribidi
        taglib
        mesa
        libGLU
    ];
}