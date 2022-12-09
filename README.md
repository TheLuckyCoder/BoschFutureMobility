# BoschFutureMobility

[![ci](https://github.com/TheLuckyCoder/BoschFutureMobility/actions/workflows/ci.yml/badge.svg)](https://github.com/TheLuckyCoder/BoschFutureMobility/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/TheLuckyCoder/BoschFutureMobility/branch/main/graph/badge.svg)](https://codecov.io/gh/TheLuckyCoder/BoschFutureMobility)
[![CodeQL](https://github.com/TheLuckyCoder/BoschFutureMobility/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/TheLuckyCoder/BoschFutureMobility/actions/workflows/codeql-analysis.yml)

## About BoschFutureMobility



## More Details

 * [Dependency Setup](README_dependencies.md)
 * [Building Details](README_building.md)
 * [Troubleshooting](README_troubleshooting.md)
 * [Docker](README_docker.md)

Deci daca nu aveti Linux cel mai usor faceti cu WSL:
- instalati WSL si ubuntu din Microsoft Store
- ca sa instalati compiler/cmake/conan etc rulati asta
wget "https://github.com/aminya/setup-cpp/releases/download/v0.23.0/setup_cpp_linux"
chmod +x setup_cpp_linux
sudo ./setup_cpp_linux --compiler llvm --cmake true --ninja true --ccache true --conan true --cppcheck true --clangtidy true

- instalati CLion
- conectati CLion la WSL (https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-product.html)
