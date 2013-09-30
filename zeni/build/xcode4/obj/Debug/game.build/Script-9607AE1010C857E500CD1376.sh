#!/bin/sh
rsync -av --exclude '.*' ../../dev/pc_/ ../..
if [ "${CONFIGURATION}" = "Debug Native" ]; then
/usr/libexec/PlistBuddy -c "Delete :LSMinimumSystemVersion" ../../Info_d.plist
mkdir -p ../../game_d.app/Contents
rsync -av --exclude '.*' ../../lib/univ_d/ ../../game_d.app/Contents/MacOS
rsync -av --exclude '.*' --delete ../../assets/ ../../game_d.app/Contents/assets
rsync -av --exclude '.*' --delete ../../Resources/ ../../game_d.app/Contents/Resources
rsync -av --exclude '.*' ../../Info_d.plist ../../game_d.app/Contents/Info.plist
fi
if [ "${CONFIGURATION}" = "Debug Universal" ]; then
/usr/libexec/PlistBuddy -c "Delete :LSMinimumSystemVersion" ../../Info_d.plist
mkdir -p ../../game_d.app/Contents
rsync -av --exclude '.*' ../../lib/univ_d/ ../../game_d.app/Contents/MacOS
rsync -av --exclude '.*' --delete ../../assets/ ../../game_d.app/Contents/assets
rsync -av --exclude '.*' --delete ../../Resources/ ../../game_d.app/Contents/Resources
rsync -av --exclude '.*' ../../Info_d.plist ../../game_d.app/Contents/Info.plist
fi
if [ "${CONFIGURATION}" = "Release Native" ]; then
/usr/libexec/PlistBuddy -c "Delete :LSMinimumSystemVersion" ../../Info.plist
mkdir -p ../../game.app/Contents
rsync -av --exclude '.*' ../../lib/univ/ ../../game.app/Contents/MacOS
rsync -av --exclude '.*' --delete ../../assets/ ../../game.app/Contents/assets
rsync -av --exclude '.*' --delete ../../Resources/ ../../game.app/Contents/Resources
rsync -av --exclude '.*' ../../Info.plist ../../game.app/Contents/Info.plist
fi
if [ "${CONFIGURATION}" = "Release Universal" ]; then
/usr/libexec/PlistBuddy -c "Delete :LSMinimumSystemVersion" ../../Info.plist
mkdir -p ../../game.app/Contents
rsync -av --exclude '.*' ../../lib/univ/ ../../game.app/Contents/MacOS
rsync -av --exclude '.*' --delete ../../assets/ ../../game.app/Contents/assets
rsync -av --exclude '.*' --delete ../../Resources/ ../../game.app/Contents/Resources
rsync -av --exclude '.*' ../../Info.plist ../../game.app/Contents/Info.plist
fi
