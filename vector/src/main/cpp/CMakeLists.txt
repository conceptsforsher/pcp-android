cmake_minimum_required(VERSION 3.22.1)
project("progressive_native")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    progressive_native
    SHARED
    src/jumptodate.cpp
    src/json_parser.cpp
    src/relation.cpp
    src/exporter.cpp
    src/eventcache.cpp
    src/translate.cpp
    src/proxy.cpp
    src/yggdrasil.cpp
    src/markdown.cpp
    src/matrix_api.cpp
    src/account_data_utils.cpp
    src/account_export.cpp
    src/audio_engine.cpp
    src/auth_models.cpp
    src/media_filter.cpp
    src/content_filter.cpp
    src/network_stats.cpp
    src/masquerade.cpp
    src/megolm_decryptor.cpp
    src/olm_session.cpp
    src/sas_verification.cpp
    src/user_mask.cpp
    src/chunked_upload.cpp
    src/chat_features.cpp
    src/debug_service.cpp
    src/invitation_hide.cpp
    src/thread_aggregator.cpp
    src/user_messages.cpp
    src/room_version.cpp
    src/chat_preview.cpp
    src/ram_monitor.cpp
    src/cache_manager.cpp
    src/call_models.cpp
    src/canonical_json.cpp
    src/message_aggregator.cpp
    src/room_info.cpp
    src/deleted_archive.cpp
    src/search_index.cpp
    src/module_loader.cpp
    src/notification.cpp
    src/room_mirror.cpp
    src/input_tools.cpp
    src/llm.cpp
    src/read_receipts.cpp
    src/room_analytics.cpp
    src/room_content.cpp
    src/room_counter.cpp
    src/chat_tools.cpp
    src/lang_detect.cpp
    src/avatar_history.cpp
    src/event_link.cpp
    src/lightweight_call.cpp
    src/scheduled_edit.cpp
    src/svg_draw.cpp
    src/profile_swiper.cpp
    src/rainbow.cpp
    src/text_formats.cpp
    src/tls_bridge.cpp
    src/url_tools.cpp
    src/notif_priority.cpp
    src/matrix_patterns.cpp
    src/desync_detector.cpp
    src/latency_stats.cpp
    src/lightweight_settings.cpp
    src/live_draft.cpp
    src/string_utils.cpp
    src/location_sharing.cpp
    src/color_utils.cpp
    src/e2ee_utils.cpp
    src/thumbnail.cpp
    src/waveform.cpp
    src/session_timeout.cpp
    src/password_validator.cpp
    src/spellcheck.cpp
    src/draft_manager.cpp
    src/link_preview.cpp
    src/hash_utils.cpp
    src/http_client.cpp
    src/room_stats.cpp
    src/mention_parser.cpp
    src/poll_utils.cpp
    src/reaction_utils.cpp
    src/file_validator.cpp
    src/graph_utils.cpp
    src/date_utils.cpp
    src/message_queue.cpp
    src/pinned_events.cpp
    src/server_capabilities.cpp
    src/username_validator.cpp
    src/emoji_analyzer.cpp
    src/identity_utils.cpp
    src/notif_analyzer.cpp
    src/sync_analyzer.cpp
    src/sync_models.cpp
    src/user_rating.cpp
    src/event_timeline.cpp
    src/event_utils.cpp
    src/room_directory.cpp
    src/widget_utils.cpp
    src/widget_manager.cpp
    src/user_directory.cpp
    src/transparent_overlay.cpp
    src/thread_manager.cpp
    src/text_undo_manager.cpp
    src/terms_manager.cpp
    src/sync_filter.cpp
    src/spoiler_manager.cpp
    src/session_manager_full.cpp
    src/server_notice_manager.cpp
    src/room_state.cpp
    src/room_permissions_manager.cpp
    src/room_name.cpp
    src/room_directory_manager.cpp
    src/profiler.cpp
    src/poll_manager.cpp
    src/pin_manager.cpp
    src/oidc_manager.cpp
    src/media_viewer.cpp
    src/media_upload_manager.cpp
    src/login_flow.cpp
    src/live_location.cpp
    src/key_backup_manager.cpp
    src/identity_server_manager.cpp
    src/event_relations_manager.cpp
    src/draft_manager_full.cpp
    src/device_naming.cpp
    src/device_manager_full.cpp
    src/cross_signing_manager.cpp
    src/content_utils.cpp
    src/composer_manager.cpp
    src/call_manager.cpp
    src/webrtc_utils.cpp
    src/sso_utils.cpp
    src/backup_utils.cpp
    src/device_manager.cpp
    src/presence_utils.cpp
    src/room_permissions.cpp
    src/room_summary.cpp
    src/room_tombstone.cpp
    src/membership_utils.cpp
    src/event_validator.cpp
    src/room_encryption.cpp
    src/login_utils.cpp
    src/account_utils.cpp
    src/connection_monitor.cpp
    src/create_room.cpp
    src/push_rules.cpp
    src/raw_service.cpp
    src/space_utils.cpp
    src/event_relations.cpp
    src/e2ee_decoration.cpp
    src/room_list.cpp
    src/room_uploads.cpp
    src/media_utils.cpp
    src/notif_settings.cpp
    src/invite_utils.cpp
    src/verification_utils.cpp
    src/session_manager.cpp
    src/signout_service.cpp
    src/auth_utils.cpp
    src/content_scanner.cpp
    src/event_encryption.cpp
    src/event_models.cpp
    src/federation_version.cpp
    src/report_utils.cpp
    src/message_retry.cpp
    src/sync_utils.cpp
    src/event_display.cpp
    src/permalink.cpp
    src/network_monitor.cpp
    src/client_info.cpp
    src/keyshare.cpp
    src/displayname_utils.cpp
    src/message_location.cpp
    src/timeline_chunk.cpp
    src/timeline_utils.cpp
    src/cross_signing.cpp
    src/crypto_algorithms.cpp
    src/crypto_models.cpp
    src/terms_service.cpp
    src/edit_history.cpp
    src/read_marker.cpp
    src/slash_command.cpp
    src/sliding_sync.cpp
    src/sqlite_wrapper.cpp
    src/typing_monitor.cpp
    src/unified_push.cpp
    src/url_preview.cpp
    src/web_search.cpp
    src/power_levels.cpp
    src/well_known.cpp
    src/room_sort.cpp
    src/key_backup.cpp
    src/notif_format.cpp
    src/notif_formatter.cpp
    src/matrix_error.cpp
    src/message_content.cpp
    src/message_extras.cpp
    src/agent_executor.cpp
    src/push_condition.cpp
    src/sender_notif_filter.cpp
    src/string_order.cpp
    src/event_classifier.cpp
    src/encrypted_file.cpp
    src/content_guard.cpp
    src/user_status.cpp
    src/offline_cache.cpp
    src/oidc_auth.cpp
    src/olm.cpp
    src/openid_token.cpp
    src/jni_bridge.cpp
    src/eventdb.cpp
)

# --- Download SQLite3 amalgamation at configure time ---
# sqlite3.c (255k lines) is NOT stored in the repo.
# Downloaded once, cached in build dir.
set(SQLITE3_DIR "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3450200")
set(SQLITE3_SRC "${SQLITE3_DIR}/sqlite3.c")
if(NOT EXISTS "${SQLITE3_SRC}")
    message(STATUS "Downloading SQLite3 amalgamation from sqlite.org...")
    file(DOWNLOAD
        "https://sqlite.org/2024/sqlite-amalgamation-3450200.zip"
        "${CMAKE_BINARY_DIR}/sqlite3.zip"
        SHOW_PROGRESS
        STATUS download_status
        TIMEOUT 120
    )
    list(GET download_status 0 download_code)
    if(NOT download_code EQUAL 0)
        list(GET download_status 1 download_err)
        message(FATAL_ERROR "Failed to download SQLite: ${download_err}")
    endif()
    message(STATUS "Extracting SQLite amalgamation...")
    file(MAKE_DIRECTORY "${SQLITE3_DIR}")
    execute_process(
        COMMAND python3 -c "import zipfile,sys; zipfile.ZipFile(sys.argv[1]).extractall(sys.argv[2])" "${CMAKE_BINARY_DIR}/sqlite3.zip" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        RESULT_VARIABLE extract_result
        ERROR_VARIABLE extract_err
    )
    if(NOT extract_result EQUAL 0)
        message(FATAL_ERROR "SQLite extraction failed (${extract_result}): ${extract_err}")
    endif()
    if(NOT EXISTS "${SQLITE3_SRC}")
        file(GLOB extracted_files "${CMAKE_BINARY_DIR}/sqlite*")
        message(FATAL_ERROR "SQLite extraction failed — sqlite3.c not found. Extracted files in ${CMAKE_BINARY_DIR}: ${extracted_files}")
    endif()
    message(STATUS "SQLite3 amalgamation extracted to ${SQLITE3_DIR}")
endif()
# Add to build after download+extraction
target_sources(progressive_native PRIVATE "${SQLITE3_SRC}")
target_include_directories(progressive_native PUBLIC "${SQLITE3_DIR}")

target_include_directories(
    progressive_native
    PUBLIC
    include
)

find_library(log_lib log)

# --- libolm E2EE crypto (C/C++ native) ---
add_subdirectory(libolm)

target_link_libraries(
    progressive_native
    ${log_lib}
    olm
)

target_compile_definitions(progressive_native PRIVATE PROGRESSIVE_HAS_OLM=1)

# SQLite3 amalgamation compile flags (optimize for Android, downloaded at build time)
set_source_files_properties(
    "${SQLITE3_DIR}/sqlite3.c"
    PROPERTIES COMPILE_FLAGS "-DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 -DSQLITE_LIKE_DOESNT_MATCH_BLOBS -DSQLITE_MAX_EXPR_DEPTH=0 -DSQLITE_OMIT_DECLTYPE -DSQLITE_OMIT_DEPRECATED -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_SHARED_CACHE -DSQLITE_USE_ALLOCA -Os"
)

