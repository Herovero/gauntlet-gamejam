#!/bin/bash
echo "Syncing code and assets to Raymob..."
cp *.cpp *.hpp raymob/app/src/main/cpp/

# Create a nested assets folder inside Raymob's asset directory
mkdir -p raymob/app/src/main/assets/assets
# Copy the contents into the nested folder
cp -r assets/* raymob/app/src/main/assets/assets/

echo "Compiling APK..."
cd raymob
./gradlew assembleDebug