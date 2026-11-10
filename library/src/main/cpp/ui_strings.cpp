/**
 * ui_strings.cpp - Localized UI string system implementation.
 *
 * Embeds default translations for 15+ languages. Supports external JSON
 * loading for extensibility. All common chat client strings are covered.
 */

#include "ui_strings.hpp"
#include "core_utils.hpp"
#include <android/log.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <cstring>

#define LOG_TAG "PcpUIStrings"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace pcp {
namespace library {

using json = nlohmann::json;

// ============================================================================
// Embedded default translations
// ============================================================================

namespace {

struct LangEntry {
    const char* key;
    const char* english;
    const char* spanish;
    const char* french;
    const char* german;
    const char* italian;
    const char* portuguese;
    const char* russian;
    const char* chinese;
    const char* japanese;
    const char* korean;
    const char* arabic;
    const char* hindi;
    const char* dutch;
    const char* polish;
    const char* turkish;
    const char* ukrainian;
    const char* swedish;
};

// Column index mapping for LangEntry
enum LangCol {
    LC_EN = 0, LC_ES, LC_FR, LC_DE, LC_IT, LC_PT, LC_RU,
    LC_ZH, LC_JA, LC_KO, LC_AR, LC_HI, LC_NL, LC_PL, LC_TR, LC_UK, LC_SV
};

constexpr const char* kLangCodes[] = {
    "en", "es", "fr", "de", "it", "pt", "ru",
    "zh", "ja", "ko", "ar", "hi", "nl", "pl", "tr", "uk", "sv"
};

constexpr const char* kLangNativeNames[] = {
    "English", "Español", "Français", "Deutsch", "Italiano",
    "Português", "Русский", "中文", "日本語", "한국어",
    "العربية", "हिन्दी", "Nederlands", "Polski",
    "Türkçe", "Українська", "Svenska"
};

// All embedded translation strings. Each entry has the key and translations
// for all 17 languages. Index by LangCol values.
const LangEntry kEmbeddedStrings[] = {
    // Common buttons
    {"cancel",     "Cancel",     "Cancelar",    "Annuler",       "Abbrechen",   "Annulla",     "Cancelar",    "Отмена",       "取消",        "キャンセル",   "취소",        "إلغاء",        "रद्द करें",         "Annuleren",  "Anuluj",      "İptal",       "Скасувати",        "Avbryt"},
    {"ok",         "OK",         "Aceptar",     "OK",            "OK",          "OK",          "OK",          "ОК",           "确定",        "OK",           "확인",        "موافق",       "ठीक है",             "OK",         "OK",          "Tamam",       "ОК",               "OK"},
    {"send",       "Send",       "Enviar",      "Envoyer",       "Senden",      "Invia",       "Enviar",      "Отправить",    "发送",        "送信",         "보내기",      "إرسال",       "भेजें",              "Verzenden",  "Wyślij",      "Gönder",      "Надіслати",        "Skicka"},
    {"search",     "Search",     "Buscar",      "Rechercher",    "Suchen",      "Cerca",       "Pesquisar",   "Поиск",        "搜索",        "検索",         "검색",        "بحث",         "खोजें",              "Zoeken",     "Szukaj",      "Ara",         "Пошук",            "Sök"},
    {"settings",   "Settings",   "Configuración","Paramètres",    "Einstellungen","Impostazioni","Configurações","Настройки",    "设置",        "設定",         "설정",        "الإعدادات",   "सेटिंग्स",           "Instellingen","Ustawienia",  "Ayarlar",     "Налаштування",     "Inställningar"},
    {"logout",     "Log Out",    "Cerrar sesión","Déconnexion",   "Abmelden",    "Esci",        "Sair",        "Выйти",        "退出登录",    "ログアウト",   "로그아웃",    "تسجيل خروج",  "लॉग आउट",            "Uitloggen",  "Wyloguj",     "Çıkış",       "Вийти",            "Logga ut"},
    {"save",       "Save",       "Guardar",     "Enregistrer",   "Speichern",   "Salva",       "Salvar",      "Сохранить",    "保存",        "保存",         "저장",        "حفظ",         "सहेजें",             "Opslaan",    "Zapisz",      "Kaydet",      "Зберегти",         "Spara"},
    {"delete",     "Delete",     "Eliminar",    "Supprimer",     "Löschen",     "Elimina",     "Excluir",     "Удалить",      "删除",        "削除",         "삭제",        "حذف",         "हटाएं",              "Verwijderen","Usuń",        "Sil",         "Видалити",         "Radera"},
    {"edit",       "Edit",       "Editar",      "Modifier",      "Bearbeiten",  "Modifica",    "Editar",      "Редактировать","编辑",        "編集",         "편집",        "تعديل",       "संपादित करें",       "Bewerken",   "Edytuj",      "Düzenle",     "Редагувати",       "Redigera"},
    {"copy",       "Copy",       "Copiar",      "Copier",        "Kopieren",    "Copia",       "Copiar",      "Копировать",   "复制",        "コピー",       "복사",        "نسخ",         "कॉपी करें",          "Kopiëren",   "Kopiuj",      "Kopyala",     "Копіювати",        "Kopiera"},
    {"share",      "Share",      "Compartir",   "Partager",      "Teilen",      "Condividi",   "Compartilhar","Поделиться",   "分享",        "共有",         "공유",        "مشاركة",      "साझा करें",          "Delen",      "Udostępnij",  "Paylaş",      "Поділитися",       "Dela"},
    {"retry",      "Retry",      "Reintentar",  "Réessayer",     "Wiederholen", "Riprova",     "Tentar novamente","Повторить","重试",        "再試行",       "재시도",      "إعادة محاولة","पुनः प्रयास करें",   "Opnieuw",    "Ponów",       "Tekrar dene", "Повторити",        "Försök igen"},
    {"close",      "Close",      "Cerrar",      "Fermer",        "Schließen",   "Chiudi",      "Fechar",      "Закрыть",      "关闭",        "閉じる",       "닫기",        "إغلاق",       "बंद करें",           "Sluiten",    "Zamknij",     "Kapat",       "Закрити",          "Stäng"},
    {"loading",    "Loading...", "Cargando...", "Chargement...",  "Laden...",    "Caricamento...","Carregando...","Загрузка...", "加载中...",   "読み込み中...","로딩 중...",  "جار التحميل...","लोड हो रहा है...","Laden...",  "Ładowanie...","Yükleniyor...","Завантаження...","Laddar..."},

    // Navigation
    {"back",       "Back",       "Atrás",       "Retour",        "Zurück",      "Indietro",    "Voltar",      "Назад",        "返回",        "戻る",         "뒤로",        "رجوع",        "वापस",               "Terug",      "Wstecz",      "Geri",        "Назад",            "Tillbaka"},
    {"forward",    "Forward",    "Adelante",    "Suivant",       "Weiter",      "Avanti",      "Avançar",     "Вперед",       "前进",        "進む",         "앞으로",      "تقدم",        "आगे",                "Vooruit",    "Dalej",       "İleri",       "Вперед",           "Framåt"},
    {"next",       "Next",       "Siguiente",   "Suivant",       "Nächste",     "Successivo",  "Próximo",     "Далее",        "下一个",      "次へ",         "다음",        "التالي",      "अगला",               "Volgende",   "Następny",    "Sonraki",     "Далі",             "Nästa"},
    {"previous",   "Previous",   "Anterior",    "Précédent",     "Vorherige",   "Precedente",  "Anterior",    "Предыдущий",   "上一个",      "前へ",         "이전",        "السابق",      "पिछला",              "Vorige",     "Poprzedni",   "Önceki",      "Попередній",       "Föregående"},

    // Chat UI
    {"message",    "Message",    "Mensaje",     "Message",       "Nachricht",   "Messaggio",   "Mensagem",    "Сообщение",    "消息",        "メッセージ",   "메시지",      "رسالة",       "संदेश",              "Bericht",    "Wiadomość",   "Mesaj",       "Повідомлення",     "Meddelande"},
    {"messages",   "Messages",   "Mensajes",    "Messages",      "Nachrichten", "Messaggi",    "Mensagens",   "Сообщения",    "消息",        "メッセージ",   "메시지",      "رسائل",       "संदेश",              "Berichten",  "Wiadomości",  "Mesajlar",    "Повідомлення",     "Meddelanden"},
    {"room",       "Room",       "Sala",        "Salon",         "Raum",        "Stanza",      "Sala",        "Комната",      "房间",        "ルーム",       "방",          "غرفة",        "कक्ष",               "Kamer",      "Pokój",       "Oda",         "Кімната",          "Rum"},
    {"rooms",      "Rooms",      "Salas",       "Salons",        "Räume",       "Stanze",      "Salas",       "Комнаты",      "房间",        "ルーム",       "방",          "غرف",         "कक्ष",               "Kamers",     "Pokoje",      "Odalar",      "Кімнати",          "Rum"},
    {"file",       "File",       "Archivo",     "Fichier",       "Datei",       "File",        "Arquivo",     "Файл",         "文件",        "ファイル",     "파일",        "ملف",         "फ़ाइल",              "Bestand",    "Plik",        "Dosya",       "Файл",             "Fil"},
    {"files",      "Files",      "Archivos",    "Fichiers",      "Dateien",     "File",        "Arquivos",    "Файлы",        "文件",        "ファイル",     "파일",        "ملفات",       "फ़ाइलें",            "Bestanden",  "Pliki",       "Dosyalar",    "Файли",            "Filer"},
    {"image",      "Image",      "Imagen",      "Image",         "Bild",        "Immagine",    "Imagem",      "Изображение",  "图片",        "画像",         "이미지",      "صورة",        "छवि",                "Afbeelding", "Obraz",       "Görsel",      "Зображення",       "Bild"},
    {"images",     "Images",     "Imágenes",    "Images",        "Bilder",      "Immagini",    "Imagens",     "Изображения",  "图片",        "画像",         "이미지",      "صور",         "छवियाँ",             "Afbeeldingen","Obrazy",      "Görseller",   "Зображення",       "Bilder"},
    {"video",      "Video",      "Video",       "Vidéo",         "Video",       "Video",       "Vídeo",       "Видео",        "视频",        "動画",         "비디오",      "فيديو",       "वीडियो",             "Video",      "Wideo",       "Video",       "Відео",            "Video"},
    {"audio",      "Audio",      "Audio",       "Audio",         "Audio",       "Audio",       "Áudio",       "Аудио",        "音频",        "音声",         "오디오",      "صوت",         "ऑडियो",              "Audio",      "Dźwięk",      "Ses",         "Аудіо",            "Ljud"},
    {"attachment", "Attachment","Adjunto",      "Pièce jointe",  "Anhang",      "Allegato",    "Anexo",       "Вложение",     "附件",        "添付ファイル", "첨부파일",    "مرفق",        "अनुलग्नक",           "Bijlage",    "Załącznik",   "Ek",          "Вкладення",        "Bilaga"},

    // People
    {"user",       "User",       "Usuario",     "Utilisateur",   "Benutzer",    "Utente",      "Usuário",     "Пользователь", "用户",        "ユーザー",     "사용자",      "مستخدم",      "उपयोगकर्ता",         "Gebruiker",  "Użytkownik",  "Kullanıcı",   "Користувач",       "Användare"},
    {"users",      "Users",      "Usuarios",    "Utilisateurs",  "Benutzer",    "Utenti",      "Usuários",    "Пользователи","用户",        "ユーザー",     "사용자",      "مستخدمون",    "उपयोगकर्ता",         "Gebruikers", "Użytkownicy", "Kullanıcılar","Користувачі",       "Användare"},
    {"contact",    "Contact",    "Contacto",    "Contact",       "Kontakt",     "Contatto",    "Contato",     "Контакт",      "联系人",      "連絡先",       "연락처",      "جهة اتصال",   "संपर्क",             "Contact",    "Kontakt",     "Kişi",        "Контакт",          "Kontakt"},
    {"profile",    "Profile",    "Perfil",      "Profil",        "Profil",      "Profilo",     "Perfil",      "Профиль",      "个人资料",    "プロフィール", "프로필",      "الملف الشخصي","प्रोफ़ाइल",          "Profiel",    "Profil",      "Profil",      "Профіль",          "Profil"},
    {"status",     "Status",     "Estado",      "Statut",        "Status",      "Stato",       "Status",      "Статус",       "状态",        "ステータス",   "상태",        "الحالة",      "स्थिति",             "Status",     "Status",      "Durum",       "Статус",           "Status"},

    // Actions
    {"add",        "Add",        "Añadir",      "Ajouter",       "Hinzufügen",  "Aggiungi",    "Adicionar",   "Добавить",     "添加",        "追加",         "추가",        "إضافة",       "जोड़ें",             "Toevoegen",  "Dodaj",       "Ekle",        "Додати",           "Lägg till"},
    {"remove",     "Remove",     "Eliminar",    "Supprimer",     "Entfernen",   "Rimuovi",     "Remover",     "Удалить",      "移除",        "削除",         "제거",        "إزالة",       "हटाएं",              "Verwijderen","Usuń",        "Kaldır",      "Видалити",         "Ta bort"},
    {"create",     "Create",     "Crear",       "Créer",         "Erstellen",   "Crea",        "Criar",       "Создать",      "创建",        "作成",         "만들기",      "إنشاء",       "बनाएं",              "Maken",      "Utwórz",      "Oluştur",     "Створити",         "Skapa"},
    {"join",       "Join",       "Unirse",      "Rejoindre",     "Beitreten",   "Unisciti",    "Entrar",      "Присоединиться","加入",      "参加",         "참가",        "انضمام",      "शामिल हों",          "Deelnemen",  "Dołącz",      "Katıl",       "Приєднатися",      "Gå med"},
    {"leave",      "Leave",      "Salir",       "Quitter",       "Verlassen",   "Lascia",      "Sair",        "Покинуть",     "离开",        "退出",         "나가기",      "مغادرة",      "छोड़ें",             "Verlaten",   "Opuść",       "Ayrıl",       "Покинути",         "Lämna"},
    {"invite",     "Invite",     "Invitar",     "Inviter",       "Einladen",    "Invita",      "Convidar",    "Пригласить",   "邀请",        "招待",         "초대",        "دعوة",        "आमंत्रित करें",      "Uitnodigen", "Zaproś",      "Davet et",    "Запросити",        "Bjud in"},
    {"block",      "Block",      "Bloquear",    "Bloquer",       "Blockieren",  "Blocca",      "Bloquear",    "Заблокировать","屏蔽",        "ブロック",     "차단",        "حظر",         "अवरोधित करें",       "Blokkeren",  "Zablokuj",    "Engelle",     "Заблокувати",      "Blockera"},
    {"report",     "Report",     "Reportar",    "Signaler",      "Melden",      "Segnala",     "Reportar",    "Пожаловаться", "举报",        "報告",         "신고",        "إبلاغ",       "रिपोर्ट करें",       "Rapporteren","Zgłoś",       "Şikayet et",  "Поскаржитися",     "Rapportera"},
    {"mute",       "Mute",       "Silenciar",   "Mettre en sourdine","Stummschalten","Silenzia", "Silenciar",   "Откл. звук",   "静音",        "ミュート",     "음소거",      "كتم",         "म्यूट करें",         "Dempen",     "Wycisz",      "Sessize al",  "Вимкнути звук",    "Tysta"},
    {"unmute",     "Unmute",     "Activar sonido","Réactiver son","Stumm. aufheben","Riattiva","Reativar som","Вкл. звук",    "取消静音",    "ミュート解除", "음소거 해제", "إلغاء الكتم", "अनम्यूट करें",       "Dempen opheffen","Włącz dźwięk","Sesi aç",   "Увімкнути звук",   "Slå på ljud"},

    // Time
    {"today",      "Today",      "Hoy",         "Aujourd'hui",   "Heute",       "Oggi",        "Hoje",        "Сегодня",      "今天",        "今日",         "오늘",        "اليوم",       "आज",                 "Vandaag",    "Dziś",        "Bugün",       "Сьогодні",         "Idag"},
    {"yesterday",  "Yesterday",  "Ayer",        "Hier",          "Gestern",     "Ieri",        "Ontem",       "Вчера",        "昨天",        "昨日",         "어제",        "أمس",         "कल",                 "Gisteren",   "Wczoraj",     "Dün",         "Вчора",            "Igår"},
    {"tomorrow",   "Tomorrow",   "Mañana",      "Demain",        "Morgen",      "Domani",      "Amanhã",      "Завтра",       "明天",        "明日",         "내일",        "غداً",        "कल",                 "Morgen",     "Jutro",       "Yarın",       "Завтра",           "Imorgon"},
    {"now",        "Now",        "Ahora",       "Maintenant",    "Jetzt",       "Ora",         "Agora",       "Сейчас",       "现在",        "今",           "지금",        "الآن",        "अभी",                "Nu",         "Teraz",       "Şimdi",       "Зараз",            "Nu"},
    {"minute",     "minute",     "minuto",      "minute",        "Minute",      "minuto",      "minuto",      "минута",       "分钟",        "分",           "분",          "دقيقة",       "मिनट",               "minuut",     "minuta",      "dakika",      "хвилина",          "minut"},
    {"hour",       "hour",       "hora",        "heure",         "Stunde",      "ora",         "hora",        "час",          "小时",        "時間",         "시간",        "ساعة",        "घंटा",               "uur",        "godzina",     "saat",        "година",           "timme"},
    {"day",        "day",        "día",         "jour",          "Tag",         "giorno",      "dia",         "день",         "天",          "日",           "일",          "يوم",         "दिन",                "dag",        "dzień",       "gün",         "день",             "dag"},
    {"week",       "week",       "semana",      "semaine",       "Woche",       "settimana",   "semana",      "неделя",       "周",          "週",           "주",          "أسبوع",       "सप्ताह",             "week",       "tydzień",     "hafta",       "тиждень",          "vecka"},

    // Messages
    {"typing",     "typing...",  "escribiendo...","en train d'écrire...","schreibt...","scrive...","digitando...","печатает...","正在输入...","入力中...","입력 중...","يكتب...","टाइप कर रहे हैं...","typt...","pisze...","yazıyor...","друкує...","skriver..."},
    {"online",     "Online",     "En línea",    "En ligne",      "Online",      "Online",      "Online",      "В сети",       "在线",        "オンライン",   "온라인",      "متصل",        "ऑनलाइन",             "Online",     "Online",      "Çevrimiçi",   "Онлайн",           "Online"},
    {"offline",    "Offline",    "Desconectado","Hors ligne",    "Offline",     "Offline",     "Offline",     "Не в сети",    "离线",        "オフライン",   "오프라인",    "غير متصل",    "ऑफ़लाइन",            "Offline",    "Offline",     "Çevrimdışı",  "Офлайн",           "Offline"},
    {"no_messages","No messages","Sin mensajes","Aucun message", "Keine Nachrichten","Nessun messaggio","Sem mensagens","Нет сообщений","没有消息","メッセージなし","메시지 없음","لا توجد رسائل","कोई संदेश नहीं","Geen berichten","Brak wiadomości","Mesaj yok","Немає повідомлень","Inga meddelanden"},
    {"new_message","New message","Nuevo mensaje","Nouveau message","Neue Nachricht","Nuovo messaggio","Nova mensagem","Новое сообщение","新消息","新しいメッセージ","새 메시지","رسالة جديدة","नया संदेश","Nieuw bericht","Nowa wiadomość","Yeni mesaj","Нове повідомлення","Nytt meddelande"},

    // Errors / states
    {"error",      "Error",      "Error",       "Erreur",        "Fehler",      "Errore",      "Erro",        "Ошибка",       "错误",        "エラー",       "오류",        "خطأ",         "त्रुटि",             "Fout",       "Błąd",        "Hata",        "Помилка",          "Fel"},
    {"unknown_error","Unknown error","Error desconocido","Erreur inconnue","Unbekannter Fehler","Errore sconosciuto","Erro desconhecido","Неизвестная ошибка","未知错误","不明なエラー","알 수 없는 오류","خطأ غير معروف","अज्ञात त्रुटि","Onbekende fout","Nieznany błąd","Bilinmeyen hata","Невідома помилка","Okänt fel"},
    {"retry_error","An error occurred. Please try again.","Ocurrió un error. Inténtelo de nuevo.","Une erreur s'est produite. Veuillez réessayer.","Ein Fehler ist aufgetreten. Bitte versuchen Sie es erneut.","Si è verificato un errore. Riprova.","Ocorreu um erro. Tente novamente.","Произошла ошибка. Пожалуйста, попробуйте снова.","发生错误，请重试。","エラーが発生しました。再試行してください。","오류가 발생했습니다. 다시 시도해주세요.","حدث خطأ. يرجى المحاولة مرة أخرى.","एक त्रुटि हुई। कृपया पुनः प्रयास करें।","Er is een fout opgetreden. Probeer het opnieuw.","Wystąpił błąd. Spróbuj ponownie.","Bir hata oluştu. Lütfen tekrar deneyin.","Сталася помилка. Будь ласка, спробуйте знову.","Ett fel uppstod. Försök igen."},
    {"no_internet","No internet connection","Sin conexión a internet","Pas de connexion internet","Keine Internetverbindung","Nessuna connessione internet","Sem conexão com a internet","Нет интернет-соединения","无网络连接","インターネット接続なし","인터넷 연결 없음","لا يوجد اتصال بالإنترنت","कोई इंटरनेट कनेक्शन नहीं","Geen internetverbinding","Brak połączenia z internetem","İnternet bağlantısı yok","Немає з'єднання з інтернетом","Ingen internetanslutning"},

    // Notifications
    {"notification","Notification","Notificación","Notification","Benachrichtigung","Notifica","Notificação","Уведомление","通知","通知","알림","إشعار","सूचना","Melding","Powiadomienie","Bildirim","Сповіщення","Notis"},
    {"notifications","Notifications","Notificaciones","Notifications","Benachrichtigungen","Notifiche","Notificações","Уведомления","通知","通知","알림","إشعارات","सूचनाएं","Meldingen","Powiadomienia","Bildirimler","Сповіщення","Notiser"},

    // Plural examples (embedded as {count} placeholder)
    {"member_count","{0} member","{0} miembro","{0} membre","{0} Mitglied","{0} membro","{0} membro","{0} участник","{0} 位成员","{0} メンバー","{0} 명","{0} عضو","{0} सदस्य","{0} lid","{0} członek","{0} üye","{0} учасник","{0} medlem"},
    {"room_count",  "{0} room","{0} sala","{0} salon","{0} Raum","{0} stanza","{0} sala","{0} комната","{0} 个房间","{0} ルーム","{0} 개 방","{0} غرفة","{0} कक्ष","{0} kamer","{0} pokój","{0} oda","{0} кімната","{0} rum"},

    // Accessibility
    {"content_description_image","Image","Imagen","Image","Bild","Immagine","Imagem","Изображение","图片","画像","이미지","صورة","छवि","Afbeelding","Obraz","Görsel","Зображення","Bild"},
    {"content_description_button","Button","Botón","Bouton","Schaltfläche","Pulsante","Botão","Кнопка","按钮","ボタン","버튼","زر","बटन","Knop","Przycisk","Düğme","Кнопка","Knapp"},
};

constexpr size_t kEmbeddedStringCount = sizeof(kEmbeddedStrings) / sizeof(kEmbeddedStrings[0]);

/** Get the translation column for a language code. Returns {column index, true} or {0, false}. */
std::pair<int, bool> getColumnForLangCode(const std::string& code) {
    for (int i = 0; i < 17; ++i) {
        if (code == kLangCodes[i]) return {i + 1, true}; // +1 to skip key col
    }
    return {LC_EN + 1, false}; // fallback to English column
}

// Plural rules per language code
PluralRule getPluralRuleForCode(const std::string& code) {
    if (code == "ru" || code == "uk" || code == "pl") return PluralRule::ONE_FEW_OTHER;
    if (code == "ar") return PluralRule::ONE_TWO_OTHER;
    if (code == "pt") return PluralRule::ONE_OTHER_ZERO;
    if (code == "zh" || code == "ja" || code == "ko" || code == "tr") return PluralRule::OTHER;
    if (code == "hi") return PluralRule::ONE_MANY_OTHER;
    return PluralRule::ONE_OTHER; // en, es, fr, de, it, nl, sv
}

} // anonymous namespace

// ============================================================================
// UIStrings Implementation
// ============================================================================

UIStrings& UIStrings::instance() {
    static UIStrings inst;
    return inst;
}

UIStrings::UIStrings() {
    loadEmbeddedDefaults();
    LOGD("UIStrings initialized with %zu keys in %zu languages",
         kEmbeddedStringCount, 17);
}

void UIStrings::loadEmbeddedDefaults() {
    for (const auto& entry : kEmbeddedStrings) {
        const char* values[] = {
            entry.english, entry.spanish, entry.french, entry.german,
            entry.italian, entry.portuguese, entry.russian, entry.chinese,
            entry.japanese, entry.korean, entry.arabic, entry.hindi,
            entry.dutch, entry.polish, entry.turkish, entry.ukrainian,
            entry.swedish
        };
        for (int i = 0; i < 17; ++i) {
            translations_[kLangCodes[i]][entry.key] = values[i];
        }
    }
}

void UIStrings::setLanguage(Language lang) {
    std::string code = languageToCode(lang);
    setLanguage(code);
}

void UIStrings::setLanguage(const std::string& langCode) {
    std::string lower = toLower(langCode);
    // Accept both "en" and "en-us" style codes
    if (lower.size() > 2 && lower[2] == '-') {
        lower = lower.substr(0, 2);
    }
    currentLanguage_ = codeToLanguage(lower);
    currentPluralRule_ = getPluralRuleForCode(lower);

    // Ensure we have translations loaded for this language
    if (translations_.find(lower) == translations_.end()) {
        LOGW("No translations loaded for language: %s, using English fallback", lower.c_str());
        currentLanguage_ = Language::EN;
        currentPluralRule_ = PluralRule::ONE_OTHER;
    }
    LOGD("Language set to: %s (plural rule: %d)", lower.c_str(),
         static_cast<int>(currentPluralRule_));
}

std::string UIStrings::getString(const std::string& key) const {
    std::string langCode = getLanguageCode();
    auto langIt = translations_.find(langCode);
    if (langIt != translations_.end()) {
        auto keyIt = langIt->second.find(key);
        if (keyIt != langIt->second.end()) {
            return keyIt->second;
        }
    }
    // Fallback: try English
    auto enIt = translations_.find("en");
    if (enIt != translations_.end() && langCode != "en") {
        auto keyIt = enIt->second.find(key);
        if (keyIt != enIt->second.end()) {
            return keyIt->second;
        }
    }
    LOGW("Missing translation key: %s", key.c_str());
    return key;
}

bool UIStrings::hasString(const std::string& key) const {
    std::string langCode = getLanguageCode();
    auto langIt = translations_.find(langCode);
    if (langIt != translations_.end() && langIt->second.find(key) != langIt->second.end()) {
        return true;
    }
    auto enIt = translations_.find("en");
    return enIt != translations_.end() && enIt->second.find(key) != enIt->second.end();
}

Language UIStrings::getLanguage() const {
    return currentLanguage_;
}

std::string UIStrings::getLanguageCode() const {
    return languageToCode(currentLanguage_);
}

std::vector<std::string> UIStrings::getAvailableLanguages() const {
    std::vector<std::string> codes;
    codes.reserve(translations_.size());
    for (const auto& [code, _] : translations_) {
        codes.push_back(code);
    }
    std::sort(codes.begin(), codes.end());
    return codes;
}

bool UIStrings::loadStrings(const std::string& jsonData) {
    try {
        json j = json::parse(jsonData);
        if (!j.is_object()) {
            LOGE("loadStrings: root must be an object, got %s", j.type_name());
            return false;
        }
        int loaded = 0;
        for (auto& [langCode, translations] : j.items()) {
            if (!translations.is_object()) {
                LOGW("loadStrings: skipping non-object for lang '%s'", langCode.c_str());
                continue;
            }
            auto& langMap = translations_[toLower(langCode)];
            for (auto& [key, value] : translations.items()) {
                if (value.is_string()) {
                    langMap[key] = value.get<std::string>();
                    ++loaded;
                }
            }
        }
        LOGD("loadStrings: loaded %d translations", loaded);
        return loaded > 0;
    } catch (const json::exception& e) {
        LOGE("loadStrings JSON error: %s", e.what());
        return false;
    }
}

bool UIStrings::loadStringsForLanguage(const std::string& langCode,
                                        const std::string& jsonData) {
    try {
        json j = json::parse(jsonData);
        if (!j.is_object()) {
            LOGE("loadStringsForLanguage: data must be an object");
            return false;
        }
        std::string lower = toLower(langCode);
        auto& langMap = translations_[lower];
        int loaded = 0;
        for (auto& [key, value] : j.items()) {
            if (value.is_string()) {
                langMap[key] = value.get<std::string>();
                ++loaded;
            }
        }
        LOGD("loadStringsForLanguage: loaded %d entries for '%s'", loaded, lower.c_str());
        return loaded > 0;
    } catch (const json::exception& e) {
        LOGE("loadStringsForLanguage JSON error: %s", e.what());
        return false;
    }
}

int UIStrings::getPluralIndex(int count) const {
    switch (currentPluralRule_) {
        case PluralRule::ONE_OTHER:
            return (count == 1) ? 0 : 1;
        case PluralRule::ONE_FEW_OTHER: {
            // Russian/Ukrainian/Polish rules
            int n = count % 100;
            int n10 = n % 10;
            if (n10 == 1 && n != 11) return 0;        // one
            if (n10 >= 2 && n10 <= 4 && !(n >= 12 && n <= 14)) return 1; // few
            return 2;                                   // other
        }
        case PluralRule::ONE_TWO_OTHER: {
            // Arabic: 0 -> other(4), 1 -> one(0), 2 -> two(1), 3-10 -> few(2), 11-99 -> many(3), 100+ -> other(4)
            // Simplified: 0->0, 1->1, 2->2, otherwise 3
            if (count == 0) return 0;
            if (count == 1) return 1;
            if (count == 2) return 2;
            if (count >= 3 && count <= 10) return 3;
            return 4;
        }
        case PluralRule::ONE_OTHER_ZERO:
            // Portuguese: count==0 -> 2, count==1 -> 0, else -> 1
            if (count == 0) return 2;
            return (count == 1) ? 0 : 1;
        case PluralRule::OTHER:
            return 0; // Chinese/Japanese/Korean/Turkish: always singular form
        case PluralRule::ONE_MANY_OTHER:
            // Hindi: count==1 -> 0, else -> 1
            return (count == 1) ? 0 : 1;
    }
    return 0;
}

std::string UIStrings::getPlural(const std::string& key, int count) const {
    int idx = getPluralIndex(count);
    std::string pluralKey = key + "_" + std::to_string(idx);

    // Try plural-specific key first
    std::string langCode = getLanguageCode();
    auto langIt = translations_.find(langCode);
    if (langIt != translations_.end()) {
        auto keyIt = langIt->second.find(pluralKey);
        if (keyIt != langIt->second.end()) {
            return keyIt->second;
        }
    }

    // Fallback to base key with {0} as count placeholder
    std::string base = getString(key);
    // If base contains {0}, replace with count
    if (contains(base, "{0}")) {
        return replaceAll(base, "{0}", std::to_string(count));
    }
    return base;
}

PluralRule UIStrings::getPluralRule() const {
    return currentPluralRule_;
}

std::string UIStrings::languageToCode(Language lang) {
    switch (lang) {
        case Language::EN: return "en";
        case Language::ES: return "es";
        case Language::FR: return "fr";
        case Language::DE: return "de";
        case Language::IT: return "it";
        case Language::PT: return "pt";
        case Language::RU: return "ru";
        case Language::ZH: return "zh";
        case Language::JA: return "ja";
        case Language::KO: return "ko";
        case Language::AR: return "ar";
        case Language::HI: return "hi";
        case Language::NL: return "nl";
        case Language::PL: return "pl";
        case Language::TR: return "tr";
        case Language::UK: return "uk";
        case Language::SV: return "sv";
        case Language::CUSTOM: return "custom";
    }
    return "en";
}

Language UIStrings::codeToLanguage(const std::string& code) {
    for (int i = 0; i < 17; ++i) {
        if (code == kLangCodes[i]) return static_cast<Language>(i);
    }
    return Language::CUSTOM;
}

std::string UIStrings::getLanguageNativeName(Language lang) {
    int idx = static_cast<int>(lang);
    if (idx >= 0 && idx < 17) return kLangNativeNames[idx];
    return "Unknown";
}

PluralRule UIStrings::getPluralRuleForLanguage(const std::string& langCode) {
    return getPluralRuleForCode(langCode);
}

// Template instantiations for common types
// (ensures formatString works with the most common argument types)
template std::string UIStrings::formatString<std::string>(const std::string&, std::string&&) const;
template std::string UIStrings::formatString<const char*>(const std::string&, const char*&&) const;
template std::string UIStrings::formatString<int>(const std::string&, int&&) const;
template std::string UIStrings::formatString<long>(const std::string&, long&&) const;
template std::string UIStrings::formatString<long long>(const std::string&, long long&&) const;
template std::string UIStrings::formatString<unsigned int>(const std::string&, unsigned int&&) const;
template std::string UIStrings::formatString<double>(const std::string&, double&&) const;

} // namespace library
} // namespace pcp
