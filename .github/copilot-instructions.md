<!-- Brief, focused instructions to help AI agents be productive in this repo -->
# BathGuardian — Copilot instructions

These notes are targeted at AI coding assistants working on the BathGuardian ESP-IDF project. They focus on concrete, discoverable patterns, entry points, and conventions so changes are safe and consistent.

- Project type: ESP-IDF C application (FreeRTOS tasks, esp-netif, esp_event, httpd, LED control).
- Key directories: `main/` (application code), `build/` (generated), `partition_table/`, `esp-idf/` (vendor components).

Important entry files and examples
- `main/main.c` — boot flow: NVS init then calls `wifi_app_task_start()`; keep changes to app startup minimal.
- `main/wifi_app.c`, `main/wifi_app.h` — primary network logic and task-based message queue. Use the `wifi_app_send_message()` API to communicate with the WiFi task. Message enums: `WIFI_APP_MSG_START_HTTP_SERVER`, `WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER`, `WIFI_APP_MSG_STA_CONNECTED_GOT_IP`.
- `main/http_server.c` — embedded static resources (index.html, app.js, jquery, favicon) served by the esp_http_server. Handlers register URIs and call functions in other modules (e.g., send queue messages to `wifi_app`). See `http_server_start()` / `http_server_stop()` for lifecycle.
- `main/tasks_common.h` — canonical task stack sizes, priorities, and core IDs. Respect these macros when creating or modifying tasks so priorities remain consistent (WiFi task priority is higher than HTTP server).

Architecture and cross-component patterns
- Single-process FreeRTOS: each major feature runs as a pinned task (WiFi task, HTTP server). Inter-task communication uses FreeRTOS queues and explicit send APIs (e.g., `wifi_app_send_message`).
- Network lifecycle: `wifi_app_task()` initializes esp-netif, registers event handlers (`wifi_app_event_handler`), starts WiFi (AP+STA mode) and then triggers the HTTP server by sending `WIFI_APP_MSG_START_HTTP_SERVER` to itself.
- HTTP server lifecycle: created/stopped via `http_server_start()` and `http_server_stop()` in `main/http_server.c`. The server uses embedded binary blobs for static content via the build pipeline's `binary_*` symbols.

Conventions and gotchas specific to this repo
- Use the existing message enums and queue APIs for cross-task signals instead of spinning new global variables. See `wifi_app_send_message()` in `main/wifi_app.c` and `wifi_app.h`.
- Task priorities: WIFI_TASK_PRIORITY (5) > HTTP_SERVER_TASK_PRIORITY (4) > HTTP_SERVER_MONITOR_TASK_PRIORITY (3). Don't change priorities without updating `tasks_common.h` and justifying the change.
- Embedded resources are referenced by linker symbols named like `_binary_<filename>_start`/`_end`. Handlers use these pointers and compute lengths by subtraction. Ensure build embeds the files (check `CMakeLists.txt` if adding new assets).
- Logging: use `ESP_LOG*` macros and module `TAG` strings (e.g., `static const char *TAG = "wifi_app";`). Some modules silence logs (see `esp_log_level_set("wifi", ESP_LOG_NONE)` in `wifi_app.c`).
- Network config constants (SSID, password, IP) live in `wifi_app.h`. Modify here for quick config changes.

Build / debug / workflow notes
- Standard build and flash: use ESP-IDF tooling (`idf.py -p <PORT> flash monitor`) from the project root. Before building, run `idf.py set-target <chip>` if needed (see README).
- Serial monitor: exit with Ctrl-]. Use `idf.py monitor` for logs; adjust log level with `make menuconfig` / `idf.py menuconfig` or `esp_log_level_set()` in code for targeted suppression.
- If adding new static assets (HTML/JS/CSS), update the `CMakeLists.txt` in `main/` to include the files so the binary symbols are generated (look at how `index.html` and `jquery` are embedded already in `http_server.c`).

Testing and quick verification
- Unit tests: none present. Preferred quick runtime test: flash to dev board and use the AP named `BathGuardian-AP` to connect and open `http://192.168.0.1/` for the web UI.
- Runtime checks: confirm WiFi AP comes up (logs: `WiFi AP started`) and HTTP server registers URIs (logs from `http_server_configure`).

Small examples to follow
- Registering a URI in `http_server_configure()` (see `main/http_server.c`): create `httpd_uri_t` with `.uri`, `.method`, `.handler`, then call `httpd_register_uri_handler()`.
- Sending a message to WiFi task from any module: call `wifi_app_send_message(WIFI_APP_MSG_START_HTTP_SERVER);` (prototype in `main/wifi_app.h`).

When in doubt
- Follow existing macros and naming in `main/` (e.g., `TAG` names, `*_start()`/`*_stop()` lifecycle functions). Small, incremental PRs are preferred — avoid replacing whole modules in one change.

If anything is missing or unclear, tell me which file or flow you want documented and I will expand these instructions.
