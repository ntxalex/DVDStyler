#!/usr/bin/env bash
# Build the DVDStyler source release tarball from the current git checkout.
#
# Same steps as build_release.sh (which clones upstream from SourceForge),
# but uses this checkout's HEAD so a tag on the fork produces a tarball of
# exactly that tag. `make dist` cannot be used: locale/Makefile.in is
# hand-written and has no distdir target.
#
# Needs: git, autoconf, automake, bison, flex, tar, bzip2.
set -euo pipefail

OUT_DIR=${OUT_DIR:-$PWD/dist}
VERSION=$(sed -n 's/^AC_INIT(\[DVDStyler\],\[\([^]]*\)\].*/\1/p' configure.ac)
[ -n "$VERSION" ] || { echo "cannot read version from configure.ac" >&2; exit 1; }
MODULE=DVDStyler-$VERSION
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

git archive --format=tar --prefix="$MODULE/" HEAD | tar -x -C "$TMP"
cd "$TMP/$MODULE"
rm -rf .github
rm -f docs/*.pdf
chmod -x src/*.h src/*.cpp buttons/*.xml buttons/deprecated/*.xml backgrounds/*.jpg
./autogen.sh
(cd src && bison -y -o dvdvmy.c -d -p dvdvm dvdvmy.y)
(cd src && flex -s -B -Cem -odvdvml.c -Pdvdvm dvdvml.l)
rm -rf autom4te.cache

mkdir -p "$OUT_DIR"
tar -C "$TMP" -cjf "$OUT_DIR/$MODULE.tar.bz2" "$MODULE"
(cd "$OUT_DIR" && sha256sum "$MODULE.tar.bz2" > "$MODULE.tar.bz2.sha256")
ls -l "$OUT_DIR/$MODULE.tar.bz2"
