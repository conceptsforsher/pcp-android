# Slash Commands Reference

Все 32 команды из оригинального `Command.kt` + `CommandParser.kt`.
Портированы в `slash_command.hpp/cpp`.

## Команды сообщений (работают в тредах)

| Команда | Параметры | Описание |
|---------|-----------|----------|
| `/me` | `<message>` | Действие от имени пользователя (`* Alice машет рукой`) |
| `/rainbow` | `<message>` | Радужный текст |
| `/rainbowme` | `<message>` | Радужное действие |
| `/spoiler` | `<message>` | Скрытый текст `\|\|спойлер\|\|` |
| `/shrug` | `<message>` | Добавляет `¯\_(ツ)_/¯` |
| `/lenny` | `<message>` | Добавляет `( ͡° ͜ʖ ͡°)` |
| `/tableflip` | `<message>` | Добавляет `(╯°□°）╯︵ ┻━┻` |
| `/plain` | `<message>` | Текст без форматирования Markdown |
| `/whois` | `<user-id>` | Показать информацию о пользователе |

## Команды модерации

| Команда | Параметры | Описание |
|---------|-----------|----------|
| `/ban` | `<user-id> [reason]` | Забанить пользователя |
| `/unban` | `<user-id> [reason]` | Разбанить пользователя |
| `/ignore` | `<user-id> [reason]` | Игнорировать пользователя |
| `/unignore` | `<user-id>` | Перестать игнорировать |
| `/op` | `<user-id> [<power-level>]` | Установить уровень прав |
| `/deop` | `<user-id>` | Сбросить уровень прав |
| `/remove` | `<user-id> [reason]` | Удалить пользователя из комнаты (алиас: `/kick`) |
| `/invite` | `<user-id> [reason]` | Пригласить пользователя |

## Команды комнат

| Команда | Параметры | Описание |
|---------|-----------|----------|
| `/join` | `<room-address> [reason]` | Войти в комнату (алиасы: `/j`, `/goto`) |
| `/part` | `[<room-address>]` | Покинуть комнату |
| `/roomname` | `<name>` | Изменить название комнаты |
| `/topic` | `<topic>` | Изменить тему комнаты |
| `/nick` | `<display-name>` | Изменить свой никнейм |
| `/myroomnick` | `<display-name>` | Никнейм только для этой комнаты (алиас: `/roomnick`) |
| `/roomavatar` | `<mxc_url>` | Сменить аватар комнаты (dev) |
| `/myroomavatar` | `<mxc_url>` | Свой аватар для комнаты (dev) |
| `/markdown` | `<on\|off>` | Включить/выключить Markdown |
| `/discardsession` | | Сбросить сессию |
| `/leave` | `<roomId?>` | Покинуть комнату по ID (dev) |
| `/upgraderoom` | `newVersion` | Обновить версию комнаты (dev) |
| `/jumptodate` | `<YYYY-MM-DD> [HH:MM]` | Перейти к дате в истории (Labs) |

## AI Агент

| Команда | Параметры | Описание |
|---------|-----------|----------|
| `/agent` | `<task description>` | AI-агент: анализирует чат, ищет сообщения, выполняет действия. Например: `/agent найди сообщения от Алисы про будущее` |

## Команды пространств (dev)

| Команда | Параметры | Описание |
|---------|-----------|----------|
| `/createspace` | `<name> <invitee>*` | Создать пространство |
| `/addToSpace` | `spaceId` | Добавить комнату в пространство |
| `/joinSpace` | `spaceId` | Войти в пространство |

## Dev-команды

| Команда | Параметры | Описание |
|---------|-----------|----------|
| `/crash` | | Уронить приложение (только dev-режим) |
| `/devtools` | | Открыть инструменты разработчика |
| `/clear_scalar_token` | | Очистить токен Scalar |
| `/confetti` | `<message>` | Конфетти-эффект |
| `/snowfall` | `<message>` | Снегопад-эффект |

## Как использовать в коде

### Kotlin (через JNI)
```kotlin
// Распарсить команду
val json = ProgressiveNative.nativeParseSlashCommand(text)
val result = JSONObject(json)
if (result.getBoolean("isSlashCommand")) {
    when (result.getString("command")) {
        "/me" -> sendEmote(result.getString("arguments"))
        "/join" -> joinRoom(result.getString("roomId"))
        ...
    }
}
```

### C++ (напрямую)
```cpp
auto cmd = progressive::parseSlashCommand("/me привет всем", "", false);
// cmd.resultType == ParseResultType::SendEmote
// cmd.message == "привет всем"
```

### Поиск команды по префиксу
```cpp
auto suggestions = progressive::getSlashCommandSuggestions("/j");
// → ["/join", "/j", "/joinSpace", "/jumptodate"]
```
