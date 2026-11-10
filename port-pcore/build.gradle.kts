// Experimental pcore bridge module — intentionally NOT wired into settings.gradle
// until Phase 1 exit criteria (see docs/PCORE_PORT_PLAN.md).
plugins { id("com.android.library"); id("org.jetbrains.kotlin.android") }
android {
    namespace = "chat.pcp.app.pcore"
    compileSdk = 34
    defaultConfig { minSdk = 19; externalNativeBuild { cmake {} } }
    externalNativeBuild { cmake { path = "src/main/cpp/CMakeLists.txt" } }
}
dependencies { implementation("org.jetbrains.kotlinx:kotlinx-coroutines-android:1.8.0") }
