FIRMWARE_DIR=firmware

INFO=$(make info RELEASE=1)
VERSION=$(echo $INFO | sed -e 's/.*Version:[[:space:]]*[^[:space:]]*[[:space:]]*//; s/-.*$//')
TARGET=$(echo $INFO | sed -e 's/.*Target:[[:space:]]*[^[:space:]]*[[:space:]]*//; s/[[:space:]].*$//')
BUILD_DIR=$(echo $INFO | sed -e 's/.*Build to:[[:space:]]*[^[:space:]]*[[:space:]]*//; s/[[:space:]].*$//')

make build RELEASE=1
echo $BUILD_DIR
cp $BUILD_DIR/$TARGET.hex $FIRMWARE_DIR/${TARGET}_v$VERSION.hex

git add $FIRMWARE_DIR/${TARGET}_v$VERSION.hex
git commit --amend --no-edit
