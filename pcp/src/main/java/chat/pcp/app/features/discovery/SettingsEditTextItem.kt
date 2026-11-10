/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */
package chat.pcp.app.features.discovery

import android.view.KeyEvent
import android.view.inputmethod.EditorInfo
import android.widget.EditText
import android.widget.TextView
import androidx.core.widget.doOnTextChanged
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import com.google.android.material.textfield.TextInputLayout
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.extensions.setTextOrHide
import chat.pcp.app.core.extensions.showKeyboard

@EpoxyModelClass
abstract class SettingsEditTextItem : PcpEpoxyModel<SettingsEditTextItem.Holder>(R.layout.item_settings_edit_text) {

    @EpoxyAttribute var hint: String? = null
    @EpoxyAttribute var value: String? = null
    @EpoxyAttribute var requestFocus = false
    @EpoxyAttribute var descriptionText: String? = null
    @EpoxyAttribute var errorText: String? = null
    @EpoxyAttribute var inProgress: Boolean = false
    @EpoxyAttribute var inputType: Int? = null

    @EpoxyAttribute
    var interactionListener: Listener? = null

    private val textChangeListener: (text: CharSequence?, start: Int, count: Int, after: Int) -> Unit = { text, _, _, _ ->
        text?.let { interactionListener?.onTextChange(it.toString()) }
    }

    private val editorActionListener = object : TextView.OnEditorActionListener {
        override fun onEditorAction(v: TextView?, actionId: Int, event: KeyEvent?): Boolean {
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                interactionListener?.onValidate()
                return true
            }
            return false
        }
    }

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.textView.setTextOrHide(descriptionText)

        holder.editText.isEnabled = !inProgress

        if (errorText.isNullOrBlank()) {
            holder.textInputLayout.error = null
        } else {
            holder.textInputLayout.error = errorText
        }
        holder.textInputLayout.hint = hint
        inputType?.let { holder.editText.inputType = it }

        holder.editText.doOnTextChanged(textChangeListener)
        holder.editText.setOnEditorActionListener(editorActionListener)
        if (value != null) {
            holder.editText.setText(value)
        }
        if (requestFocus) {
            holder.editText.showKeyboard(andRequestFocus = true)
        }
    }

    class Holder : PcpEpoxyHolder() {
        val textView by bind<TextView>(R.id.settings_item_edit_text_description)
        val editText by bind<EditText>(R.id.settings_item_edit_text)
        val textInputLayout by bind<TextInputLayout>(R.id.settings_item_edit_text_til)
    }

    interface Listener {
        fun onValidate()
        fun onTextChange(text: String)
    }
}
