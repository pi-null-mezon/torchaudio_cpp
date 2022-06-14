# step 1 - download torchaudio sources from https://github.com/pytorch/audio/releases
# step 2 - build torchaudio
linux {
    TORCHAUDIO_SOURCES = "/home/alex/Programming/3rdParties/torchaudio/audio-0.11.0-cxx11"
    old_cxx11_abi: TORCHAUDIO_SOURCES = "/home/alex/Programming/3rdParties/torchaudio/audio-0.11.0"
    TORCHAUDIO_INSTALL = $${TORCHAUDIO_SOURCES}/third_party/install

    SOURCES += $${TORCHAUDIO_SOURCES}/torchaudio/csrc/sox/io.cpp \
               $${TORCHAUDIO_SOURCES}/torchaudio/csrc/sox/effects.cpp \
               $${TORCHAUDIO_SOURCES}/torchaudio/csrc/sox/effects_chain.cpp \
               $${TORCHAUDIO_SOURCES}/torchaudio/csrc/sox/utils.cpp

    INCLUDEPATH += $${TORCHAUDIO_SOURCES} \
                   $${TORCHAUDIO_INSTALL}/include

    LIBS += -L$${TORCHAUDIO_SOURCES}/build/torchaudio/csrc \
            -L$${TORCHAUDIO_INSTALL}/lib \
            -lFLAC \
            -lmad \
            -lopencore-amrnb \
            -lopencore-amrwb \
            -lsox \
            -lopusurl \
            -lopusfile \
            -lopus \
            -lvorbisenc \
            -lvorbisfile \
            -lvorbis \
            -logg \
            -lmp3lame \
            -ltorchaudio
}
