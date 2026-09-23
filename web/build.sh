#!/bin/bash
#
# Builds The Invasion for WebAssembly/WebGL with Emscripten.
#
# Allegro 5 has no Emscripten backend of its own. We use its SDL2 backend
# (src/sdl/), which is the route documented in the upstream README_sdl.txt,
# combined with Emscripten's SDL2 port.
#
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
GAME="$ROOT/The Invasion"
OUT="$ROOT/web/dist"
ALLEGRO="$ROOT/web/allegro5"
BUILD="$ALLEGRO/build_emscripten"

# Arch Linux installs the toolchain outside PATH.
if ! command -v emcc >/dev/null && [ -x /usr/lib/emscripten/emcc ]; then
    export PATH="/usr/lib/emscripten:$PATH"
fi

command -v emcc >/dev/null || {
    echo "ERROR: emscripten not found. Install it with: sudo pacman -S emscripten"
    exit 1
}

EM_CACHE="$(em-config CACHE)"
echo ">> emscripten: $(emcc --version | head -1)"
echo ">> cache:      $EM_CACHE"

# Emscripten writes sdl2.pc here but does not create the tree itself.
mkdir -p "$EM_CACHE/sysroot/lib/pkgconfig" "$EM_CACHE/sysroot/include"

# Allegro looks for a separate libvorbisfile, but Emscripten's vorbis port
# ships vorbis, vorbisenc and vorbisfile inside a single archive. Without
# this the acodec addon silently drops Ogg Vorbis and the soundtrack
# fails to load at runtime.
EM_LIB="$EM_CACHE/sysroot/lib/wasm32-emscripten"

echo ">> building emscripten ports..."
embuilder build sdl2 freetype vorbis ogg libpng libjpeg

# USE_VORBIS and USE_OGG are what give us the .ogg soundtrack in the browser.
FLAGS=(
    -sUSE_SDL=2
    -sUSE_FREETYPE=1
    -sUSE_VORBIS=1
    -sUSE_OGG=1
    -sUSE_LIBPNG=1
    -sUSE_LIBJPEG=1
    -O3
)

# ---------- 1. Allegro 5 for wasm ----------
if [ ! -f "$BUILD/lib/liballegro_monolith-static.a" ]; then
    [ -d "$ALLEGRO" ] || git clone --depth 1 https://github.com/liballeg/allegro5.git "$ALLEGRO"
    mkdir -p "$BUILD"
    cd "$BUILD"

    # ALLEGRO_WAIT_EVENT_SLEEP makes al_wait_for_event yield to the browser,
    # which is what lets the blocking event loop survive the port.
    emcmake cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DALLEGRO_SDL=ON \
        -DSHARED=OFF \
        -DWANT_MONOLITH=ON \
        -DWANT_ALLOW_SSE=OFF \
        -DWANT_DOCS=OFF -DWANT_TESTS=OFF -DWANT_EXAMPLES=OFF -DWANT_DEMO=OFF \
        -DWANT_OPENAL=OFF \
        -DALLEGRO_WAIT_EVENT_SLEEP=ON \
        -DSDL2_INCLUDE_DIR="$EM_CACHE/sysroot/include" \
        -DVORBISFILE_LIBRARY="$EM_LIB/libvorbis.a" \
        -DVORBIS_LIBRARY="$EM_LIB/libvorbis.a" \
        -DOGG_LIBRARY="$EM_LIB/libogg.a" \
        -DCMAKE_C_FLAGS="${FLAGS[*]}" \
        -DCMAKE_CXX_FLAGS="${FLAGS[*]}"
    make -j"$(nproc)"
else
    echo ">> Allegro already built, skipping"
fi

# ---------- 2. The game ----------
# Allegro keeps each addon's public header under addons/<name>/allegro5/,
# not in include/. A build from the source tree has to name them all.
ADDON_INCLUDES=()
for addon in font ttf image audio acodec primitives; do
    ADDON_INCLUDES+=(-I "$ALLEGRO/addons/$addon")
done

echo ">> compiling the game..."
mkdir -p "$OUT"
cd "$GAME"

# Derived from the tree so this list cannot drift from CMakeLists.txt.
mapfile -t SOURCES < <(find src -name '*.cpp' | sort)
echo ">> ${#SOURCES[@]} translation units"


em++ "${SOURCES[@]}" -o "$OUT/index.html" \
    -std=c++17 \
    -Isrc \
    -I "$ALLEGRO/include" -I "$BUILD/include" \
    "${ADDON_INCLUDES[@]}" \
    -L "$BUILD/lib" -lallegro_monolith-static \
    "${FLAGS[@]}" \
    -sFULL_ES2=1 \
    -sASYNCIFY \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=268435456 \
    -lidbfs.js \
    --preload-file Assets \
    --preload-file fonts \
    --preload-file sounds \
    --shell-file "$ROOT/web/shell.html"

echo
echo ">> done: $OUT/index.html"
echo ">> serve it with:  cd '$OUT' && python3 -m http.server 8080"
