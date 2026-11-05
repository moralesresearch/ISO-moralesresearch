#!/bin/sh

# Consistency checks that do not need FreeBSD, so they can run in CI on any host.
# Catches the kind of breakage that renaming overlays or scripts can cause:
# build.sh and settings/script.* refer to files by name, and nothing
# fails until hours into a build if one of those names goes stale

cd "$(dirname "$0")/.." || exit 1
errors=0

fail()
{
  echo "ERROR: $*" 1>&2
  errors=$((errors + 1))
}

# build.sh reads the overlay lists with 'while read', which silently
# skips a last line that has no trailing newline
for f in settings/*; do
  if [ -f "${f}" ] && [ -s "${f}" ] && [ -n "$(tail -c 1 "${f}")" ] ; then
    fail "${f} does not end with a newline"
  fi
done

# Every overlay named in settings/overlays.* must exist
for list in settings/overlays.*; do
  # '|| [ -n ]' also reads a last line without a newline, which build.sh would skip
  while read -r p || [ -n "${p}" ]; do
    [ -z "${p}" ] && continue
    [ -f "overlays/uzip/${p}/manifest" ] || fail "${list} lists '${p}', but overlays/uzip/${p}/manifest does not exist"
    [ -d "overlays/uzip/${p}/files" ] || fail "${list} lists '${p}', but overlays/uzip/${p}/files does not exist"
  done < "${list}"
done

# Overlay programs that settings/script.* runs inside the image must exist in an overlay
for script in settings/script.*; do
  for path in $(sed -n 's|.*chroot "${uzip}" \(/usr/local/[a-z]*bin/[^ ]*\).*|\1|p;s|.*chroot "${uzip}" \(/usr/local/libexec/[^ ]*\).*|\1|p' "${script}" | sort -u); do
    ls overlays/uzip/*/files"${path}" >/dev/null 2>&1 || fail "${script} runs ${path}, which no overlay provides"
  done
done

# Overlay programs must be executable: pkg create takes file modes from the
# checked-out files, so a program committed without +x cannot run in the image
# (a heredoc rather than a pipe, so that fail() runs in this shell and counts)
while read -r f; do
  [ -z "${f}" ] && continue
  [ -x "${f}" ] || fail "${f} is not executable"
done <<EOF
$(find overlays/uzip/*/files -type f \( -path '*/bin/*' -o -path '*/sbin/*' -o -path '*/libexec/*' \) ! -name .gitkeep)
EOF

if [ "${errors}" -gt 0 ] ; then
  echo "${errors} problem(s) found" 1>&2
  exit 1
fi
echo "Tree is consistent"
