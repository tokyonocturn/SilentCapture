#include <obs/obs-module.h>
#include <windows.h>
#include <shellapi.h>
#include <thread>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("silent-capture", "en-US")

struct SilentCapture {
	obs_output_t *current_output = nullptr;
	bool notifications_suppressed = false;
	std::thread monitor_thread;
	bool should_exit = false;

	SilentCapture() = default;
	~SilentCapture() = default;
};

static SilentCapture *g_silent_capture = nullptr;

bool IsDisplayCaptureActive()
{
	obs_frontend_source_list source_list = {};
	obs_frontend_get_sources(&source_list);

	bool has_display_capture = false;
	for (size_t i = 0; i < source_list.sources.num; i++) {
		obs_source_t *source = source_list.sources.array[i];
		const char *source_id = obs_source_get_id(source);

		if (strcmp(source_id, "monitor_capture") == 0 ||
		    strcmp(source_id, "game_capture") == 0) {
			if (obs_source_enabled(source)) {
				has_display_capture = true;
				break;
			}
		}
	}

	obs_frontend_source_list_free(&source_list);
	return has_display_capture;
}

bool GetFocusAssistStatus()
{
	HKEY hKey;
	LONG result = RegOpenKeyExW(
		HKEY_CURRENT_USER,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Notifications\\Settings",
		0, KEY_READ, &hKey);

	if (result != ERROR_SUCCESS)
		return false;

	DWORD value = 0;
	DWORD size = sizeof(value);
	result = RegQueryValueExW(hKey, L"NOC_GLOBAL_SETTING_TOASTS_ENABLED", NULL,
				  NULL, (LPBYTE)&value, &size);
	RegCloseKey(hKey);

	return result == ERROR_SUCCESS && value == 0;
}

void SetFocusAssist(bool enable)
{
	HKEY hKey;
	LONG result = RegOpenKeyExW(
		HKEY_CURRENT_USER,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Notifications\\Settings",
		0, KEY_WRITE, &hKey);

	if (result != ERROR_SUCCESS)
		return;

	DWORD value = enable ? 1 : 0;
	RegSetValueExW(hKey, L"NOC_GLOBAL_SETTING_TOASTS_ENABLED", 0, REG_DWORD,
		       (const BYTE *)&value, sizeof(value));
	RegCloseKey(hKey);
}

void MonitorThread()
{
	while (!g_silent_capture->should_exit) {
		bool recording = obs_frontend_recording_active() ||
				  obs_frontend_streaming_active();
		bool display_capture_active = IsDisplayCaptureActive();
		bool should_suppress = recording && display_capture_active;

		if (should_suppress && !g_silent_capture->notifications_suppressed) {
			SetFocusAssist(false);
			g_silent_capture->notifications_suppressed = true;
			blog(LOG_INFO, "SilentCapture: Notifications suppressed");
		} else if (!should_suppress && g_silent_capture->notifications_suppressed) {
			SetFocusAssist(true);
			g_silent_capture->notifications_suppressed = false;
			blog(LOG_INFO, "SilentCapture: Notifications restored");
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

static void FrontendEventCallback(enum obs_frontend_event event, void *private_data)
{
	switch (event) {
	case OBS_FRONTEND_EVENT_RECORDING_STARTED:
	case OBS_FRONTEND_EVENT_STREAMING_STARTED:
	case OBS_FRONTEND_EVENT_RECORDING_STOPPED:
	case OBS_FRONTEND_EVENT_STREAMING_STOPPED:
		break;
	default:
		break;
	}
}

bool obs_module_load(void)
{
	g_silent_capture = new SilentCapture();
	g_silent_capture->should_exit = false;
	g_silent_capture->monitor_thread = std::thread(MonitorThread);

	obs_frontend_add_event_callback(FrontendEventCallback, nullptr);

	blog(LOG_INFO, "SilentCapture loaded successfully");
	return true;
}

void obs_module_unload(void)
{
	if (g_silent_capture) {
		g_silent_capture->should_exit = true;
		if (g_silent_capture->monitor_thread.joinable())
			g_silent_capture->monitor_thread.join();

		if (g_silent_capture->notifications_suppressed)
			SetFocusAssist(true);

		obs_frontend_remove_event_callback(FrontendEventCallback, nullptr);
		delete g_silent_capture;
		g_silent_capture = nullptr;
	}

	blog(LOG_INFO, "SilentCapture unloaded");
}

MODULE_EXPORT const char *obs_module_name(void)
{
	return "SilentCapture";
}

MODULE_EXPORT const char *obs_module_description(void)
{
	return "Automatically suppresses Windows notifications during OBS recording and streaming";
}
