# step 1 - download C++ torch binaries from https://pytorch.org
linux {
    TORCH_DISTRIB = "/home/alex/Programming/3rdParties/torchaudio/libtorch-cxx11-abi-shared-with-deps-1.11.0+cpu/libtorch"
    old_cxx11_abi: TORCH_DISTRIB = "/home/alex/Programming/3rdParties/torchaudio/libtorch-shared-with-deps-1.11.0+cpu/libtorch"

    INCLUDEPATH += $${TORCH_DISTRIB} \
                   $${TORCH_DISTRIB}/include

    LIBS += -L$${TORCH_DISTRIB}/lib \
            -lc10 \
            -ltorch_cpu

    QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=1
    old_cxx11_abi: QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0
}
