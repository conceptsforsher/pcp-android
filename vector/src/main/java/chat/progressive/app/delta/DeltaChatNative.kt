package chat.progressive.app.delta

object DeltaChatNative {
    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                isLoaded = true
            } catch (e: UnsatisfiedLinkError) {
                isLoaded = false
            }
        }
    }

    fun isAvailable(): Boolean {
        ensureLoaded()
        return isLoaded
    }
}
