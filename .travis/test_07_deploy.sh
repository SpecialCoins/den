#!/usr/bin/env bash

export LC_ALL=C.UTF-8

OUTDIR=$TRAVIS_BUILD_DIR/out/$TRAVIS_PULL_REQUEST/$TRAVIS_JOB_NUMBER-$HOST
mkdir -p $OUTDIR/bin

ARCHIVE_CMD="zip"

if [[ $HOST = "i686-w64-mingw32" ]]; then
  ARCHIVE_NAME="windows32.zip"
elif [[ $HOST = "x86_64-w64-mingw32" ]]; then
    ARCHIVE_NAME+="windows64.zip"
elif [[ $HOST = "x86_64-pc-linux-gnu" ]]; then
    ARCHIVE_NAME+="linux-pc.tar.gz"
ARCHIVE_CMD="tar -czf"
elif [[ $HOST = "x86_64-unknown-linux-gnu" ]]; then
    ARCHIVE_NAME="linux-unknown.tar.gz"
    ARCHIVE_CMD="tar -czf"
elif [[ $HOST = "arm-linux-gnueabihf" ]]; then
    ARCHIVE_NAME="arm-x86.tar.gz"
    ARCHIVE_CMD="tar -czf"
elif [[ $HOST = "aarch64-linux-gnu" ]]; then
    ARCHIVE_NAME="arm-x64.tar.gz"
    ARCHIVE_CMD="tar -czf"
elif [[ $HOST = "x86_64-apple-darwin16" ]]; then
    ARCHIVE_NAME+="osx.zip"
fi
cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/qt/pivx-qt $OUTDIR/bin/ || cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/qt/pivx-qt.exe $OUTDIR/bin/ || echo "no QT Wallet"
cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/qt/bcz-qt $OUTDIR/bin/ || cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/qt/bcz-qt.exe $OUTDIR/bin/ || echo "no QT Wallet"
cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/bczd $OUTDIR/bin/ || cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/bczd.exe $OUTDIR/bin/ || echo "no Daemon"
cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/bcz-cli $OUTDIR/bin/ || cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/bcz-cli.exe $OUTDIR/bin/ || echo "no Cli"
cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/bcz-tx $OUTDIR/bin/ || cp $TRAVIS_BUILD_DIR/build/bcz-$HOST/src/bcz-tx.exe $OUTDIR/bin/ || echo "no TX"
strip "$OUTDIR/bin"/*
ls -lah $OUTDIR/bin

cd $OUTDIR/bin || return
ARCHIVE_CMD="$ARCHIVE_CMD $ARCHIVE_NAME *"
eval $ARCHIVE_CMD

mkdir -p $OUTDIR/zip
mv $ARCHIVE_NAME $OUTDIR/zip

sleep $(( ( RANDOM % 6 ) + 1 ))s
