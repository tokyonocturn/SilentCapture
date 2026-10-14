# SilentCapture

An OBS Studio plugin that automatically hides Windows notifications during recording and streaming, while keeping them visible on your actual monitor.

## Features

- **Automatic notification suppression** during OBS recording and streaming
- **Display/Game capture detection** - only suppresses when actively capturing
- **Transparent** - no UI dialogs or configuration needed
- **Non-intrusive** - notifications queue and appear after recording ends
- **Registry-safe** - uses the same mechanism as Windows Focus Assist

## How It Works

SilentCapture monitors your OBS recording and streaming status. When both conditions are true:
1. OBS is recording or streaming
2. A display or game capture source is active

The plugin automatically enables Focus Assist, which suppresses system notifications. Your notifications stay visible on your monitor but won't appear in the capture output.

When recording stops or the capture source is disabled, notifications are automatically restored.

## Installation

### Prerequisites

- OBS Studio 28.0 or later
- Windows 10/11
- OBS development environment set up

### Build from Source

1. Clone the repository:
```bash
git clone https://github.com/tokyonocturn/SilentCapture.git
cd SilentCapture
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

4. Copy the built plugin to your OBS plugins directory:
   - **Windows**: `%APPDATA%/obs-studio/plugins/silent-capture/bin/64bit/`

5. Restart OBS Studio

## Usage

No configuration needed. Once installed:

1. Start OBS
2. Add a Display Capture or Game Capture source to your scene
3. Start recording or streaming
4. Windows notifications will automatically be suppressed
5. Your monitor still shows notifications normally
6. Notifications are restored when recording/streaming stops

## Technical Details

- **Language**: C++ (C++17)
- **Platform**: Windows (10/11)
- **Registry Access**: Writes to `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Notifications\Settings`
- **Thread Model**: Single background monitoring thread (500ms polling interval)
- **OBS API**: Uses frontend API for recording/streaming state detection

## Requirements Met

- Monitors display/game capture sources
- Checks recording and streaming state
- No notifications visible in capture output
- Notifications remain visible on monitor
- Automatic enable/disable on record start/stop

## Troubleshooting

**Notifications still appear in capture:**
- Ensure a Display Capture or Game Capture source is active in your scene
- Check that the source is enabled (not muted)
- Verify OBS has registry write permissions

**Notifications don't return after recording:**
- Manually re-enable notifications in Windows Settings > System > Focus Assist
- Restart OBS

**Plugin fails to load:**
- Verify OBS development libraries are installed correctly
- Check OBS logs for error messages: `%APPDATA%/obs-studio/logs/`

## License

MIT License - see LICENSE file for details

## Credits

Co-edited by Akari

---

Built for smooth, distraction-free streaming and recording on Windows.
