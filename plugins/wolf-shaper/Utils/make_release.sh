set -e

PLUGIN_NAME="$(basename -s .git $(git config --get remote.origin.url))"
PLUGIN_VERSION=$(git describe --tags --abbrev=0)

PLATFORM_NAME=linux

export BUILD_VST2=true 
export BUILD_LV2=true 
export BUILD_JACK=true 

EXTENSION=so

if [ "$WIN32" = true ]; then
    PLATFORM_NAME=windows

    export CC=x86_64-w64-mingw32-gcc 
    export AR=x86_64-w64-mingw32-ar 
    export CXX=x86_64-w64-mingw32-g++

    BUILD_VST2=true 
    BUILD_LV2=false 
    BUILD_JACK=false

    EXTENSION=dll
fi

make clean && make

cd bin

ARCHIVE_NAME="$PLUGIN_NAME-$PLUGIN_VERSION-$PLATFORM_NAME"

if [ "$BUILD_VST2" = true ]; then
    if [ "$PLATFORM_NAME" = "linux" ]; then
        tar -zcvf "$ARCHIVE_NAME-vst2.tar.gz" "$PLUGIN_NAME-vst.$EXTENSION"
    else
        zip -r "$ARCHIVE_NAME-vst2.zip" "$PLUGIN_NAME-vst.$EXTENSION"
    fi
fi

if [ "$BUILD_LV2" = true ]; then
    tar -zcvf "$ARCHIVE_NAME-lv2.tar.gz" "$PLUGIN_NAME.lv2"
fi

if [ "$BUILD_JACK" = true ]; then
    tar -zcvf "$ARCHIVE_NAME-jack.tar.gz" "$PLUGIN_NAME"
fi

cd ..

mkdir -p ./.releases/

if [ "$PLATFORM_NAME" = "linux" ]; then
    mv bin/*.tar.gz .releases/
else
    mv bin/*.zip .releases/
fi
