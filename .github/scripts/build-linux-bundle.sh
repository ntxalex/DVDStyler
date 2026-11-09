#!/usr/bin/env bash
# Build a self-contained ("Blender-style") Linux x86_64 tarball of DVDStyler.
#
# Meant to run inside an ubuntu:22.04 container (glibc 2.35) from the root
# of a DVDStyler checkout, as root. FFmpeg, wxWidgets and wxSVG are built
# from source into $DEPS_PREFIX and bundled in lib/ with RPATH=$ORIGIN, so
# the result does not depend on the host distro's FFmpeg or wx sonames.
# glibc, libstdc++, GTK3, GStreamer, X11/Wayland, fontconfig etc. are taken
# from the host and listed in DEPENDENCIES.txt.
#
# Environment overrides:
#   FFMPEG_VERSION WX_VERSION WXSVG_VERSION  dependency versions
#   (if you change the dependency configure flags below, bump
#   DEPS_CACHE_REV in release.yml so CI rebuilds them)
#   SKIP_APT=1    skip the apt-get step (packages already installed)
#   DEPS_PREFIX   where the dependencies are installed (cacheable)
#   OUT_DIR       where the finished tarball is written
#   JOBS          parallel make jobs
set -euo pipefail

FFMPEG_VERSION=${FFMPEG_VERSION:-9.0.2}
WX_VERSION=${WX_VERSION:-3.2.12}
WXSVG_VERSION=${WXSVG_VERSION:-1.5.25}
DEPS_PREFIX=${DEPS_PREFIX:-/opt/dvdstyler-deps}
OUT_DIR=${OUT_DIR:-$PWD/dist}
JOBS=${JOBS:-$(nproc)}
SRC_DIR=$PWD
WORK=${WORK:-/tmp/dvdstyler-bundle-work}

VERSION=$(sed -n 's/^AC_INIT(\[DVDStyler\],\[\([^]]*\)\].*/\1/p' configure.ac)
[ -n "$VERSION" ] || { echo "cannot read version from configure.ac" >&2; exit 1; }
NAME=DVDStyler-$VERSION-linux-x86_64

export PATH=$DEPS_PREFIX/bin:$PATH
export PKG_CONFIG_PATH=$DEPS_PREFIX/lib/pkgconfig
export LD_LIBRARY_PATH=$DEPS_PREFIX/lib

step() { echo; echo "=== $* ==="; }

install_build_deps() {
  step "Installing build packages (apt)"
  export DEBIAN_FRONTEND=noninteractive
  apt-get update
  apt-get install -y --no-install-recommends \
    build-essential autoconf automake libtool pkg-config git ca-certificates \
    curl xz-utils bzip2 file patchelf nasm flex bison gettext xmlto docbook-xsl zip \
    libgtk-3-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
    libexpat1-dev libcairo2-dev libpango1.0-dev libexif-dev libfontconfig-dev \
    libudev-dev libdbus-1-dev zlib1g-dev \
    dvdauthor genisoimage dvd+rw-tools
}

fetch() { # url file
  [ -f "$2" ] || curl -fsSL --retry 3 -o "$2" "$1"
}

build_deps() {
  if [ -f "$DEPS_PREFIX/.built-$FFMPEG_VERSION-$WX_VERSION-$WXSVG_VERSION" ]; then
    step "Dependencies already in $DEPS_PREFIX (cache hit)"
    return
  fi
  mkdir -p "$WORK" "$DEPS_PREFIX"
  cd "$WORK"

  step "FFmpeg $FFMPEG_VERSION"
  fetch "https://ffmpeg.org/releases/ffmpeg-$FFMPEG_VERSION.tar.xz" ffmpeg.tar.xz
  rm -rf ffmpeg-src && mkdir ffmpeg-src && tar -xJf ffmpeg.tar.xz -C ffmpeg-src --strip-components=1
  (cd ffmpeg-src && ./configure --prefix="$DEPS_PREFIX" \
      --enable-shared --disable-static --enable-gpl --enable-version3 \
      --disable-autodetect --enable-zlib --enable-iconv \
      --disable-doc --disable-debug --disable-ffplay \
    && make -j"$JOBS" && make install)

  step "wxWidgets $WX_VERSION"
  fetch "https://github.com/wxWidgets/wxWidgets/releases/download/v$WX_VERSION/wxWidgets-$WX_VERSION.tar.bz2" wx.tar.bz2
  rm -rf wx-src && mkdir wx-src && tar -xjf wx.tar.bz2 -C wx-src --strip-components=1
  (cd wx-src && mkdir -p build-gtk3 && cd build-gtk3 && ../configure --prefix="$DEPS_PREFIX" \
      --with-gtk=3 --enable-unicode --enable-mediactrl \
      --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin \
      --with-zlib=sys --with-expat=sys --with-regex=builtin \
      --without-opengl --without-libnotify --without-sdl --disable-webview \
      --disable-webrequest --disable-secretstore --disable-tests \
    && make -j"$JOBS" && make install)

  step "wxSVG $WXSVG_VERSION"
  fetch "https://downloads.sourceforge.net/wxsvg/wxsvg-$WXSVG_VERSION.tar.bz2" wxsvg.tar.bz2
  rm -rf wxsvg-src && mkdir wxsvg-src && tar -xjf wxsvg.tar.bz2 -C wxsvg-src --strip-components=1
  (cd wxsvg-src && autoreconf -fi && ./configure --prefix="$DEPS_PREFIX" \
      --with-wx-config="$DEPS_PREFIX/bin/wx-config" --enable-shared --disable-static \
    && make -j"$JOBS" && make install)

  touch "$DEPS_PREFIX/.built-$FFMPEG_VERSION-$WX_VERSION-$WXSVG_VERSION"
}

build_dvdstyler() {
  step "DVDStyler $VERSION"
  # DVDStyler does not support out-of-tree (VPATH) builds, so build in a
  # scratch copy of the source and keep the checkout clean.
  rm -rf "$WORK/build" "$WORK/stage" && mkdir -p "$WORK/build"
  tar -C "$SRC_DIR" --exclude=./.git --exclude=./dist -cf - . | tar -C "$WORK/build" -xf -
  cd "$WORK/build"
  ./autogen.sh
  ./configure --prefix=/usr/local --with-wx-config="$DEPS_PREFIX/bin/wx-config"
  make -j"$JOBS"
  make install DESTDIR="$WORK/stage"
}

# System libraries that are deliberately NOT bundled (matched on soname).
is_system_lib() {
  case "$1" in
    linux-vdso.so*|ld-linux*|libc.so*|libm.so*|libdl.so*|libpthread.so*|librt.so*|libresolv.so*|\
    libstdc++.so*|libgcc_s.so*|libz.so*|libexpat.so*|\
    libgtk-3.so*|libgdk-3.so*|libgdk_pixbuf-2.0.so*|libglib-2.0.so*|libgobject-2.0.so*|libgio-2.0.so*|libgmodule-2.0.so*|libgthread-2.0.so*|\
    libpango-1.0.so*|libpangocairo-1.0.so*|libpangoft2-1.0.so*|libcairo.so*|libcairo-gobject.so*|libharfbuzz.so*|libatk-1.0.so*|\
    libfontconfig.so*|libfreetype.so*|libX11.so*|libXxf86vm.so*|libSM.so*|libICE.so*|libXtst.so*|libwayland-*.so*|libxkbcommon.so*|\
    libgstreamer-1.0.so*|libgstbase-1.0.so*|libgstvideo-1.0.so*|libgstaudio-1.0.so*|libgstpbutils-1.0.so*|libgsttag-1.0.so*|libgstplayer-1.0.so*|\
    libudev.so*|libdbus-1.so*)
      return 0 ;;
  esac
  return 1
}

assemble_bundle() {
  step "Assembling $NAME"
  local B="$WORK/$NAME" S="$WORK/stage/usr/local"
  rm -rf "$B" && mkdir -p "$B/bin" "$B/lib"
  install -m755 "$S/bin/dvdstyler" "$B/bin/dvdstyler"
  install -m755 "$DEPS_PREFIX/bin/ffmpeg" "$B/bin/ffmpeg"
  install -m755 "$DEPS_PREFIX/bin/ffprobe" "$B/bin/ffprobe"
  # Data dirs sit next to bin/ because DVDStyler looks for <exe dir>/../<dir>
  # (wxFindDataDirectory) and <exe dir>/../locale for translations.
  cp -a "$S/share/dvdstyler/." "$B/"
  cp -a "$S/share/locale" "$B/locale"
  mkdir -p "$B/share" && cp -a "$S/share/man" "$S/share/applications" "$S/share/pixmaps" "$B/share/" 2>/dev/null || true
  cp "$SRC_DIR/COPYING" "$B/COPYING"

  # Bundle exactly the non-system libraries the executables need
  # (FFmpeg, wxWidgets, wxSVG built above, plus libexif), following NEEDED
  # entries recursively so unused wx libraries are left out.
  local queue=("$B"/bin/*) f so src
  while [ ${#queue[@]} -gt 0 ]; do
    f=${queue[0]}; queue=("${queue[@]:1}")
    for so in $(objdump -p "$f" | awk '/NEEDED/{print $2}'); do
      [ -e "$B/lib/$so" ] && continue
      is_system_lib "$so" && continue
      if [ -e "$DEPS_PREFIX/lib/$so" ]; then src="$DEPS_PREFIX/lib/$so"
      else src=$(ldconfig -p | awk -v so="$so" '$1==so && /x86-64/{print $NF; exit}')
      fi
      [ -n "$src" ] || { echo "cannot find $so (needed by $f)" >&2; exit 1; }
      cp -L "$src" "$B/lib/$so"
      queue+=("$B/lib/$so")
    done
  done

  chmod 755 "$B"/lib/*
  for f in "$B"/bin/*; do patchelf --set-rpath '$ORIGIN/../lib' "$f"; done
  for f in "$B"/lib/*.so.*; do patchelf --set-rpath '$ORIGIN' "$f"; done
  strip --strip-unneeded "$B"/bin/* "$B"/lib/*.so.* 2>/dev/null || true

  cat > "$B/dvdstyler" <<'LAUNCH'
#!/bin/sh
# Launcher for the self-contained DVDStyler build. Puts the bundled ffmpeg
# first in PATH so DVDStyler's transcoding uses the same FFmpeg it links to.
HERE=$(dirname "$(readlink -f "$0")")
PATH="$HERE/bin:$PATH"
export PATH
exec "$HERE/bin/dvdstyler" "$@"
LAUNCH
  chmod 755 "$B/dvdstyler"

  write_dependencies "$B"
  verify_bundle "$B"

  mkdir -p "$OUT_DIR"
  tar -C "$WORK" -cJf "$OUT_DIR/$NAME.tar.xz" "$NAME"
  (cd "$OUT_DIR" && sha256sum "$NAME.tar.xz" > "$NAME.tar.xz.sha256")
  ls -l "$OUT_DIR"
}

# Debian/Ubuntu and Arch package for each system soname we depend on.
pkg_for() {
  case "$1" in
    libc.so*|libm.so*|libdl.so*|libpthread.so*|librt.so*|libresolv.so*|ld-linux*) echo "libc6|glibc" ;;
    libstdc++.so*) echo "libstdc++6|gcc-libs" ;;
    libgcc_s.so*) echo "libgcc-s1|gcc-libs" ;;
    libz.so*) echo "zlib1g|zlib" ;;
    libexpat.so*) echo "libexpat1|expat" ;;
    libgtk-3.so*|libgdk-3.so*) echo "libgtk-3-0 (libgtk-3-0t64)|gtk3" ;;
    libgdk_pixbuf-2.0.so*) echo "libgdk-pixbuf-2.0-0|gdk-pixbuf2" ;;
    libglib-2.0.so*|libgobject-2.0.so*|libgio-2.0.so*|libgmodule-2.0.so*|libgthread-2.0.so*) echo "libglib2.0-0 (libglib2.0-0t64)|glib2" ;;
    libpango*) echo "libpango-1.0-0, libpangocairo-1.0-0, libpangoft2-1.0-0|pango" ;;
    libcairo*) echo "libcairo2, libcairo-gobject2|cairo" ;;
    libharfbuzz.so*) echo "libharfbuzz0b|harfbuzz" ;;
    libatk-1.0.so*) echo "libatk1.0-0 (libatk1.0-0t64)|at-spi2-core" ;;
    libfontconfig.so*) echo "libfontconfig1|fontconfig" ;;
    libfreetype.so*) echo "libfreetype6|freetype2" ;;
    libX11.so*) echo "libx11-6|libx11" ;;
    libXxf86vm.so*) echo "libxxf86vm1|libxxf86vm" ;;
    libSM.so*) echo "libsm6|libsm" ;;
    libICE.so*) echo "libice6|libice" ;;
    libXtst.so*) echo "libxtst6|libxtst" ;;
    libwayland-*) echo "libwayland-client0 / -cursor0 / -egl1|wayland" ;;
    libxkbcommon.so*) echo "libxkbcommon0|libxkbcommon" ;;
    libgst*) echo "libgstreamer1.0-0, libgstreamer-plugins-base1.0-0|gstreamer, gst-plugins-base-libs" ;;
    libudev.so*) echo "libudev1|systemd-libs (Artix: eudev or libudev provider)" ;;
    libdbus-1.so*) echo "libdbus-1-3|dbus (Artix: dbus or libdbus)" ;;
    *) echo "?|?" ;;
  esac
}

write_dependencies() {
  local B=$1 D="$1/DEPENDENCIES.txt" needed glibc
  needed=$(for f in "$B"/bin/* "$B"/lib/*.so.*; do objdump -p "$f" 2>/dev/null | awk '/NEEDED/{print $2}'; done | sort -u)
  glibc=$(for f in "$B"/bin/* "$B"/lib/*.so.*; do objdump -T "$f" 2>/dev/null; done | grep -o 'GLIBC_[0-9.]*' | sort -uV | tail -1)
  glibcxx=$(for f in "$B"/bin/* "$B"/lib/*.so.*; do objdump -T "$f" 2>/dev/null; done | grep -o 'GLIBCXX_[0-9.]*' | sort -uV | tail -1)
  {
    echo "DVDStyler $VERSION - self-contained Linux x86_64 build"
    echo "======================================================"
    echo
    echo "Unpack anywhere and run ./dvdstyler (the launcher script) from this folder."
    echo "Nothing needs installing except the system libraries and tools below."
    echo
    echo "Built on: $(. /etc/os-release; echo "$PRETTY_NAME"), $(ldd --version | head -1)"
    echo "Minimum glibc on the host: ${glibc#GLIBC_}   (libstdc++ symbol version: ${glibcxx})"
    echo
    echo "Bundled in lib/ (RPATH \$ORIGIN/../lib, no system copy needed):"
    echo "  FFmpeg $FFMPEG_VERSION, wxWidgets $WX_VERSION (GTK3; libjpeg/png/tiff built in), wxSVG $WXSVG_VERSION, libexif"
    (cd "$B/lib" && ls -1 *.so.* | sed 's/^/    /')
    echo "  bin/ffmpeg and bin/ffprobe are the matching FFmpeg $FFMPEG_VERSION command-line tools."
    echo
    echo "Required from the system (soname -> Debian/Ubuntu package | Arch/Artix package):"
    for so in $needed; do
      [ -e "$B/lib/$so" ] && continue
      printf '  %-28s %s\n' "$so" "$(pkg_for "$so" | sed 's/|/  |  /')"
    done
    echo
    echo "Also needed at run time (external programs DVDStyler calls):"
    echo "  dvdauthor, spumux     -> Debian/Ubuntu: dvdauthor        | Arch: dvdauthor"
    echo "  mkisofs/genisoimage   -> Debian/Ubuntu: genisoimage      | Arch: cdrtools"
    echo "  growisofs             -> Debian/Ubuntu: dvd+rw-tools     | Arch: dvd+rw-tools"
    echo "  GStreamer plugins for the media preview: gstreamer1.0-plugins-good / gst-plugins-good"
    echo
    echo "One-line installs:"
    echo "  Debian/Ubuntu: sudo apt install libgtk-3-0 libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 gstreamer1.0-plugins-good dvdauthor genisoimage dvd+rw-tools"
    echo "  Arch/Artix:    sudo pacman -S --needed gtk3 gstreamer gst-plugins-base-libs gst-plugins-good dvdauthor cdrtools dvd+rw-tools"
  } > "$D"
  cat "$D"
}

verify_bundle() {
  local B=$1 bad=0 f so
  step "Checking the bundle"
  # Every direct dependency must be either bundled or on the system list.
  for f in "$B"/bin/* "$B"/lib/*.so.*; do
    file -b "$f" | grep -q ELF || continue
    for so in $(objdump -p "$f" | awk '/NEEDED/{print $2}'); do
      [ -e "$B/lib/$so" ] || is_system_lib "$so" || { echo "UNBUNDLED: $f -> $so"; bad=1; }
    done
    if env -u LD_LIBRARY_PATH ldd "$f" | grep -q 'not found'; then
      echo "UNRESOLVED: $f"; env -u LD_LIBRARY_PATH ldd "$f" | grep 'not found'; bad=1
    fi
    # Bundled libraries must come from lib/, not from the build prefix.
    if env -u LD_LIBRARY_PATH ldd "$f" | grep -q "$DEPS_PREFIX"; then echo "LEAKS BUILD PREFIX: $f"; bad=1; fi
  done
  [ $bad = 0 ] || { echo "bundle check failed" >&2; exit 1; }
  env -u LD_LIBRARY_PATH "$B/bin/ffmpeg" -hide_banner -encoders 2>/dev/null | grep -E ' (mpeg2video|ac3|mp2) ' \
    || { echo "ffmpeg lacks DVD encoders" >&2; exit 1; }
  echo "bundle check OK"
}

[ "${SKIP_APT:-0}" = 1 ] || install_build_deps
build_deps
build_dvdstyler
assemble_bundle
