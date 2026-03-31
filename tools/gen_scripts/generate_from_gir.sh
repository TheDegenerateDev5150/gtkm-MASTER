#!/bin/bash

# The script assumes that it resides in the tools/gen_scripts directory and
# the XML and defs files will be placed in gdk/src, gsk/src and gtk/src.

# To update the defs files:
# 1. ./generate_from_gir.sh
#    Generates g?k/src/*.defs.orig and g?k/src/*.defs.
#    If any hunks from the patch files fail to apply, apply them manually to
#    the defs files, if required.
# 2. Optional: Remove g?k/src/*.defs.orig.

# To update the patch files:
# 1. Like step 1 when updating the g?k/src/*.defs files.
# 2. Apply new patches manually to the g?k/src/*.defs files.
# 3. ./generate_from_gir.sh --make-patch
# 4. Like step 2 when updating the g?k/src/*.defs file.

source "$(dirname "$0")/init_generate.sh"

gir_dir="$GMMPROC_GEN_INSTALL_DIR/share/gir-1.0"
out_dir_gdk="$root_dir/gdk/src"
out_dir_gsk="$root_dir/gsk/src"
out_dir_gtk="$root_dir/gtk/src"

if [ $# -eq 0 ]
then
  echo ===== Gdk, Gsk and Gtk: Documentation
  "$(dirname "$0")"/gdk_generate_docs.sh
  "$(dirname "$0")"/gsk_generate_docs.sh
  "$(dirname "$0")"/gtk_generate_docs.sh

  echo; echo Gdk: Enums and functions
  "$gen_with_mmgir" --gir "$gir_dir"/Gdk-4.0.gir \
    --gir-search-dir "$gir_dir" \
    --enum-defs "$out_dir_gdk"/gdk_enums.defs \
    --function-defs "$out_dir_gdk"/gdk_methods.defs

  echo; echo GdkPixbuf: Enums and functions
  "$gen_with_mmgir" --gir "$gir_dir"/GdkPixbuf-2.0.gir --gir "$gir_dir"/GdkPixdata-2.0.gir \
    --gir-search-dir "$gir_dir" \
    --enum-defs "$out_dir_gdk"/gdk_pixbuf_enums.defs \
    --function-defs "$out_dir_gdk"/gdk_pixbuf_methods.defs

  echo; echo Graphene: Functions
  "$gen_with_mmgir" --gir "$gir_dir"/Graphene-1.0.gir \
    --gir-search-dir "$gir_dir" \
    --enum-defs /dev/null \
    --function-defs "$out_dir_gdk"/gdk_graphene_methods.defs

  echo; echo Gdk: Signals and vfuncs
  "$gen_with_mmgir" --gir "$gir_dir"/Gdk-4.0.gir \
    --gir "$gir_dir"/GdkPixbuf-2.0.gir --gir "$gir_dir"/GdkPixdata-2.0.gir \
    --gir-search-dir "$gir_dir" \
    --enum-defs /dev/null \
    --function-defs /dev/null \
    --signal-defs "$out_dir_gdk"/gdk_signals.defs \
    --vfunc-defs "$out_dir_gdk"/gdk_vfuncs.defs

  echo; echo Gsk: Enums, functions, signals and vfuncs
  "$gen_with_mmgir" --gir "$gir_dir"/Gsk-4.0.gir \
    --gir-search-dir "$gir_dir" \
    --enum-defs "$out_dir_gsk"/gsk_enums.defs \
    --function-defs "$out_dir_gsk"/gsk_methods.defs \
    --signal-defs "$out_dir_gsk"/gsk_signals.defs \
    --vfunc-defs "$out_dir_gsk"/gsk_vfuncs.defs

  echo; echo Gtk: Enums, functions, signals and vfuncs
  "$gen_with_mmgir" --gir "$gir_dir"/Gtk-4.0.gir \
    --gir-search-dir "$gir_dir" \
    --enum-defs "$out_dir_gtk"/gtk_enums.defs \
    --function-defs "$out_dir_gtk"/gtk_methods.defs \
    --signal-defs "$out_dir_gtk"/gtk_signals.defs \
    --vfunc-defs "$out_dir_gtk"/gtk_vfuncs.defs

  echo; echo ===== Patching defs files
  patch_options="--backup --version-control=simple --suffix=.orig"
  # Execute in a subshell. The effect of the cd command is undone when the subshell ends.
  (
    cd "$out_dir_gdk"
    for file in gdk_signals.defs
    do
      patch $patch_options $file $file.girpatch
    done
  )
  (
    cd "$out_dir_gtk"
    for file in gtk_enums.defs gtk_signals.defs
    do
      patch $patch_options $file $file.girpatch
    done
  )
elif [ "$1" = "--make-patch" ]
then
  (
    cd "$out_dir_gdk"
    for file in gdk_signals.defs
    do
      diff --unified=5 $file.orig $file > $file.girpatch
    done
  )
  (
    cd "$out_dir_gtk"
    for file in gtk_enums.defs gtk_signals.defs
    do
      diff --unified=5 $file.orig $file > $file.girpatch
    done
  )
else
  echo "Usage: $0 [--make-patch]"
  exit 1
fi
