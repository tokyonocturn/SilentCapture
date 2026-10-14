# GitHub Setup Instructions for SilentCapture

Follow these steps to upload SilentCapture to your GitHub account.

## 1. Create a new repository on GitHub

Go to https://github.com/new and create a new public repository named `SilentCapture` with:
- Owner: `tokyonocturn`
- Repository name: `SilentCapture`
- Description: "OBS plugin that automatically hides Windows notifications during recording/streaming"
- Public
- Do NOT initialize with README, .gitignore, or license (we have these)

## 2. Initialize and push from this directory

```bash
cd SilentCapture
git init
git add .
git commit -m "Initial commit: SilentCapture OBS plugin

Co-edited by Akari
- Automatic notification suppression during recording/streaming
- Display/Game capture detection
- Registry-based Focus Assist integration
- Cross-platform OBS compatibility"

git branch -M main
git remote add origin https://github.com/tokyonocturn/SilentCapture.git
git push -u origin main
```

## 3. Verify on GitHub

Your repo should now be live at: https://github.com/tokyonocturn/SilentCapture

Check that all files are present:
- CMakeLists.txt
- LICENSE (MIT)
- README.md
- .gitignore
- logo.svg
- src/notification-hider.cpp

## Build Instructions for Users

Users can now clone and build with:
```bash
git clone https://github.com/tokyonocturn/SilentCapture.git
cd SilentCapture
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

Done! Your plugin is now on GitHub with proper documentation and credits.
