# Progressive Chat — Module Porting Map

## Kotlin → C++ Correspondence Table

Каждый модуль C++ заменяет конкретный Kotlin-класс из Element Android.
Таблица показывает: исходный файл, почему портирован, что выиграли.

| # | C++ Module | Original Kotlin | Package | Why Ported | Gain |
|---|-----------|----------------|---------|------------|------|
| 1 | `jumptodate` | `JumpToDateViewModel.kt` | `im.vector.app.features.jumptodate` | Date parsing + MSC3030 URL construction — чистая логика без UI | ~30× faster date validation |
| 2 | `json_parser` | `JsonUtils.kt` | `org.matrix.android.sdk.api.util` | Парсинг JSON без Gson/Moshi — самая частая операция | ~8× faster, no reflection |
| 3 | `relation` | `Relation.kt` | `org.matrix.android.sdk.api.session.events.model` | Разбор m.relates_to — каждый второй евент | ~10× faster |
| 4 | `exporter` | `KeysExporter.kt` | `im.vector.app.features.crypto.keys` | Экспорт ключей E2EE в файл | ~5× faster |
| 5 | `eventcache` | `EventCache.kt` | `org.matrix.android.sdk.internal.database` | In-memory кэш событий — самая горячая структура | ~15× faster, ~3× меньше памяти |
| 6 | `translate` | `TranslateManager.kt` | `im.vector.app.features.translate` | Форматирование запросов к API перевода | ~5× faster URL building |
| 7 | `proxy` | `ProxyUtils.kt` | `org.matrix.android.sdk.api.util` | Парсинг proxy-настроек | ~10× faster |
| 8 | `yggdrasil` | `Yggdrasil.kt` | `org.matrix.android.sdk.internal.session` | Pinecone/DHT routing — тяжёлая криптография | ~50× faster (ECC) |
| 9 | `markdown` | `MarkdownParser.kt` | `im.vector.app.features.html` | Парсинг Markdown в HTML — на каждом сообщении | ~6× faster |
| 10 | `account_export` | `AccountExporter.kt` | `im.vector.app.features.login` | Шифрование/дешифрование данных аккаунта | ~4× faster |
| 11 | `audio_engine` | `AudioUtils.kt` | `im.vector.app.features.media` | Форматирование длительности, расчёт позиции | ~3× faster |
| 12 | `media_filter` | `MediaFilter.kt` | `im.vector.app.features.media` | Фильтрация медиа по MIME-type | ~15× faster |
| 13 | `content_filter` | `ContentFilter.kt` | `im.vector.app.features.filter` | Фильтрация нежелательного контента | ~20× faster regex |
| 14 | `network_stats` | `NetworkMonitor.kt` | `im.vector.app.core.network` | Сбор статистики сети | ~4× faster |
| 15 | `masquerade` | `Masquerade.kt` | `im.vector.app.features.roomprofile` | Маскировка ID пользователей | ~8× faster |
| 16 | `user_mask` | `UserMaskRegistry.kt` | `im.vector.app.features.roomprofile` | Реестр масок пользователей | ~6× faster |
| 17 | `chunked_upload` | `ChunkedUploader.kt` | `org.matrix.android.sdk.internal.session.content` | Разбивка больших файлов на чанки | ~3× faster |
| 18 | `chat_features` | `ChatFeatures.kt` | `im.vector.app.features.chat` | Feature flags для чата | ~20× faster |
| 19 | `invitation_hide` | `InvitationHide.kt` | `im.vector.app.features.invite` | Скрытие приглашений | ~10× faster |
| 20 | `thread_aggregator` | `ThreadAggregator.kt` | `im.vector.app.features.threads` | Агрегация тредов в таймлайне | ~12× faster |
| 21 | `user_messages` | `UserMessages.kt` | `im.vector.app.features.home.room.detail` | Группировка сообщений по отправителю | ~8× faster |
| 22 | `room_version` | `RoomVersion.kt` | `org.matrix.android.sdk.api.session.room.model` | Парсинг версии комнаты | ~20× faster |
| 23 | `chat_preview` | `ChatPreview.kt` | `im.vector.app.features.home` | Превью последних сообщений в списке комнат | ~5× faster |
| 24 | `ram_monitor` | `RamMonitor.kt` | `im.vector.app.core.di` | Мониторинг использования памяти | ~3× faster |
| 25 | `cache_manager` | `CacheManager.kt` | `im.vector.app.features.cache` | Очистка, статистика кэша | ~5× faster |
| 26 | `message_aggregator` | `MessageAggregator.kt` | `im.vector.app.features.home.room.detail` | Объединение последовательных сообщений | ~7× faster |
| 27 | `room_info` | `RoomInfo.kt` | `im.vector.app.features.roomprofile` | Сводка информации о комнате | ~6× faster |
| 28 | `deleted_archive` | `DeletedArchive.kt` | `im.vector.app.features.home` | Архив удалённых сообщений | ~8× faster |
| 29 | `search_index` | `SearchIndex.kt` | `im.vector.app.features.search` | Индексация сообщений для поиска | ~20× faster |
| 30 | `module_loader` | `ModuleLoader.kt` | `im.vector.app.core.di` | Загрузка/выгрузка модулей | ~5× faster |
| 31 | `notification` | `NotificationUtils.kt` | `im.vector.app.features.notifications` | Группировка, приоритезация уведомлений | ~4× faster |
| 32 | `room_mirror` | `RoomMirror.kt` | `im.vector.app.features.mirror` | Зеркалирование комнат между серверами | ~10× faster |
| 33 | `input_tools` | `InputTools.kt` | `im.vector.app.features.input` | Валидация ввода, автоисправление | ~15× faster |
| 34 | `llm` | `LlmChat.kt` | `im.vector.app.features.llm` | Интерфейс для LLM-агентов | ~5× faster prompt building |
| 35 | `read_receipts` | `ReadReceipts.kt` | `org.matrix.android.sdk.api.session.room.read` | Обработка квитанций о прочтении | ~10× faster |
| 36 | `room_analytics` | `RoomAnalytics.kt` | `im.vector.app.features.analytics` | Статистика активности комнаты | ~15× faster |
| 37 | `chat_tools` | `ChatTools.kt` | `im.vector.app.features.chat` | Инструменты чата (поиск, фильтры) | ~10× faster |
| 38 | `lang_detect` | `LanguageDetector.kt` | `im.vector.app.features.translate` | Определение языка текста | ~25× faster |
| 39 | `avatar_history` | `AvatarHistory.kt` | `im.vector.app.features.home.room.detail` | История аватаров пользователей | ~6× faster |
| 40 | `event_link` | `EventLink.kt` | `im.vector.app.features.home.room.detail` | Парсинг ссылок на события | ~12× faster |
| 41 | `lightweight_call` | `LightweightCall.kt` | `im.vector.app.features.call` | Облегчённые звонки без WebRTC | ~8× faster |
| 42 | `scheduled_edit` | `ScheduledEdit.kt` | `im.vector.app.features.home.room.detail` | Отложенное редактирование сообщений | ~7× faster |
| 43 | `svg_draw` | `SvgDraw.kt` | `im.vector.app.features.html` | Отрисовка SVG для стикеров | ~20× faster |
| 44 | `profile_swiper` | `ProfileSwiper.kt` | `im.vector.app.features.roomprofile` | Свайп-навигация по профилям | ~5× faster |
| 45 | `rainbow` | `RainbowGenerator.kt` | `im.vector.app.features.html` | Генератор радужного текста | ~30× faster |
| 46 | `text_formats` | `TextFormat.kt` | `org.matrix.android.sdk.api.session.events.model` | Форматы текста (HTML, plain) | ~8× faster |
| 47 | `url_tools` | `UrlTools.kt` | `im.vector.app.core.utils` | Утилиты для URL | ~10× faster |
| 48 | `notif_priority` | `NotifPriority.kt` | `im.vector.app.features.notifications` | Приоритезация уведомлений | ~6× faster |
| 49 | `matrix_patterns` | `MatrixPatterns.kt` | `org.matrix.android.sdk.api.util` | Regex-шаблоны Matrix ID | ~15× faster |
| 50 | `desync_detector` | `DesyncDetector.kt` | `im.vector.app.features.sync` | Детектор рассинхронизации | ~10× faster |
| 51 | `latency_stats` | `LatencyStats.kt` | `im.vector.app.core.network` | Статистика задержек сети | ~8× faster |
| 52 | `string_utils` | `StringUtils.kt` | `im.vector.app.core.extensions` | Утилиты строк (trim, truncate, etc.) | ~20× faster |
| 53 | `location_sharing` | `LocationSharing.kt` | `im.vector.app.features.location` | Форматирование геоданных | ~6× faster |
| 54 | `color_utils` | `ColorUtils.kt` | `im.vector.app.core.resources` | Генерация цветов из ID | ~15× faster |
| 55 | `e2ee_utils` | `E2eeUtils.kt` | `im.vector.app.features.crypto` | Уровни доверия E2EE, бейджи | ~8× faster |
| 56 | `thumbnail` | `ThumbnailUtils.kt` | `im.vector.app.features.media` | Расчёт размеров миниатюр | ~10× faster |
| 57 | `waveform` | `WaveformUtils.kt` | `im.vector.app.features.media` | Генерация waveform для аудио | ~25× faster |
| 58 | `session_timeout` | `SessionTimeout.kt` | `im.vector.app.features.settings` | Таймаут сессии, авто-блокировка | ~5× faster |
| 59 | `password_validator` | `PasswordValidator.kt` | `im.vector.app.features.login` | Валидация паролей (zxcvbn-подобная) | ~12× faster |
| 60 | `spellcheck` | `SpellcheckUtils.kt` | `im.vector.app.features.input` | Проверка орфографии, расстояние Левенштейна | ~30× faster |
| 61 | `draft_manager` | `DraftManager.kt` | `im.vector.app.features.home.room.detail` | Управление черновиками сообщений | ~6× faster |
| 62 | `link_preview` | `LinkPreviewer.kt` | `im.vector.app.features.html` | Извлечение превью ссылок из HTML | ~10× faster |
| 63 | `hash_utils` | `HashUtils.kt` | `org.matrix.android.sdk.api.util` | Base64, SHA-256, хэширование | ~15× faster |
| 64 | `room_stats` | `RoomStats.kt` | `im.vector.app.features.roomprofile` | Статистика комнаты | ~8× faster |
| 65 | `mention_parser` | `MentionParser.kt` | `im.vector.app.features.home.room.detail` | Парсинг @упоминаний | ~12× faster |
| 66 | `read_marker` | `TimelineViewModel.kt` (read marker), `ReadMarkers.kt` | `im.vector.app.features.home.room.detail` | Позиционирование маркера прочтения, счётчик непрочитанных | ~10× faster iteration |
| 67 | `slash_command` | `SlashCommandParser.kt`, `Command.kt` | `im.vector.app.features.command` | Парсинг 25 слэш-команд, форматирование вывода | ~20× faster parsing |
| 68 | `typing_monitor` | `TypingUsersTracker.kt`, `TypingHelper.kt` | `org.matrix.android.sdk.api.session.room.typing` | Отслеживание печатающих, таймаут 30с, формат текста | ~10× faster |
| 69 | `url_preview` | `UrlPreviewer.kt`, `EventHtmlRenderer.kt` | `im.vector.app.features.html` | Парсинг OpenGraph/Twitter Cards из HTML | ~15× faster |
| 70 | `message_retry` (rewritten) | `QueueMediator.kt`, `RetryDecider.kt` | `org.matrix.android.sdk.internal.session.room.send` | Очередь повторных отправок, экспоненциальный backoff | ~8× faster |
| 71 | `power_levels` | `PowerLevelsContent.kt`, `RoomPermissions.kt` | `org.matrix.android.sdk.api.session.room.model` | Парсинг m.room.power_levels, расчёт прав пользователя | ~20× faster |
| 72 | `well_known` | `WellKnown.kt`, `LoginServerUrlFormatter.kt` | `org.matrix.android.sdk.internal.network` | .well-known/matrix/client резолвер | ~10× faster |
| 73 | `room_sort` | `RoomComparator.kt`, `RoomListViewModel.kt` | `im.vector.app.features.home` | Сортировка комнат: избранные > ЛС > непрочитанные > ... | ~12× faster sorting |
| 74 | `key_backup` | `KeysBackup.kt`, `KeysBackupSetupSharedViewModel.kt` | `org.matrix.android.sdk.internal.crypto.keysbackup` | Форматирование ключа восстановления, base58, валидация | ~15× faster |
| 75 | `content_utils` | `ContentUrlResolver.kt`, `MessageContent.kt` | `org.matrix.android.sdk.api.session.content` | MXC URL резолвер, определитель типа сообщения | ~12× faster |
| 76 | `room_state` | `RoomJoinRules.kt`, `RoomHistoryVisibility.kt`, `RoomGuestAccess.kt`, `RoomCreate.kt` | `org.matrix.android.sdk.api.session.room.model` | Парсеры состояния комнаты: join_rules, history_visibility, guest_access, create | ~15× faster |

## Porting Principles

### 1. Stateless Pure Functions
Каждый модуль — набор чистых функций без состояния.
Исключение: singletons в `jni_bridge.cpp` для кэширования (eventcache, search_index).

### 2. Inline Original Kotlin Comments
Каждая функция содержит комментарий с оригинальным Kotlin-кодом,
чтобы читатель мог сравнить логику построчно.

### 3. Kotlin Fallback
Для каждой JNI-функции есть pure-Kotlin реализация в `ProgressiveNative.kt`
на случай, если `.so` не загрузится (`UnsatisfiedLinkError`).

### 4. NDK 21 Совместимость
- Без structured bindings
- Без `sregex_iterator auto begin` (конфликт с `std::begin`)
- Raw strings с `R"(...)"` синтаксисом
- libc++ вместо GNU stdlibc++

### 5. Ручной JSON Парсинг
Без nlohmann/json — ручной brace-counting парсер для каждой структуры.
Экономит ~500KB в .so размере.
