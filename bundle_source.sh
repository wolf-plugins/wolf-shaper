PLUGIN_NAME=wolf-shaper
PLUGIN_VERSION=0.1.6
TARBALL_NAME="$PLUGIN_NAME-v$PLUGIN_VERSION-source.tar.gz"

rm -rf "$PLUGIN_NAME"
git clone --recursive "https://github.com/pdesaulniers/$PLUGIN_NAME.git" --branch "v$PLUGIN_VERSION" --single-branch
cd "$PLUGIN_NAME"
find . -name ".git" -type f -delete
rm -rf .git
find . -name ".gitmodules" -type f -delete
cd ..
tar -zcvf "$TARBALL_NAME" "$PLUGIN_NAME/"
rm -rf "$PLUGIN_NAME/"

gpg --armor --detach-sign "$TARBALL_NAME"
