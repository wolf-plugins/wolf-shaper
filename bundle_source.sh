PLUGIN_NAME=wolf-shaper
PLUGIN_VERSION=0.1.6

rm -rf "$PLUGIN_NAME"
git clone --recursive "https://github.com/pdesaulniers/$PLUGIN_NAME.git" --branch "v$PLUGIN_VERSION" --single-branch
cd "$PLUGIN_NAME"
find . -name ".git" -type f -delete
rm -rf .git
find . -name ".gitmodules" -type f -delete
cd ..
tar -zcvf "$PLUGIN_NAME-v$PLUGIN_VERSION-source.tar.gz" "$PLUGIN_NAME/"
rm -rf "$PLUGIN_NAME/"
