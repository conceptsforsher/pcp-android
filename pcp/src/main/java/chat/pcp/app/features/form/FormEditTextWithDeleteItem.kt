/*
 * Copyright 2026-2026 PCP
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-PCP
 * Please see LICENSE files in the repository root for full details.
 */

package chat.pcp.app.features.form

import android.text.Editable
import android.text.InputFilter
import android.view.inputmethod.EditorInfo
import android.widget.ImageButton
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import chat.pcp.app.R
import chat.pcp.app.core.epoxy.ClickListener
import chat.pcp.app.core.epoxy.TextListener
import chat.pcp.app.core.epoxy.PcpEpoxyHolder
import chat.pcp.app.core.epoxy.PcpEpoxyModel
import chat.pcp.app.core.epoxy.addTextChangedListenerOnce
import chat.pcp.app.core.epoxy.onClick
import chat.pcp.app.core.extensions.setTextIfDifferent
import chat.pcp.app.core.platform.SimpleTextWatcher

@EpoxyModelClass
abstract class FormEditTextWithDeleteItem : PcpEpoxyModel<FormEditTextWithDeleteItem.Holder>(R.layout.item_form_text_input_with_delete) {

    @EpoxyAttribute
    var hint: String? = null

    @EpoxyAttribute
    var value: String? = null

    @EpoxyAttribute
    var enabled: Boolean = true

    @EpoxyAttribute
    var singleLine: Boolean = true

    @EpoxyAttribute
    var imeOptions: Int? = null

    @EpoxyAttribute
    var maxLength: Int? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var onTextChange: TextListener? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var onDeleteClicked: ClickListener? = null

    private val onTextChangeListener = object : SimpleTextWatcher() {
        override fun afterTextChanged(s: Editable) {
            onTextChange?.invoke(s.toString())
        }
    }

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.textInputLayout.isEnabled = enabled
        holder.textInputLayout.hint = hint

        if (maxLength != null) {
            holder.textInputEditText.filters = arrayOf(InputFilter.LengthFilter(maxLength!!))
            holder.textInputLayout.counterMaxLength = maxLength!!
        } else {
            holder.textInputEditText.filters = arrayOf()
        }
        holder.textInputEditText.setTextIfDifferent(value)

        holder.textInputEditText.isEnabled = enabled
        holder.textInputEditText.isSingleLine = singleLine

        holder.textInputEditText.imeOptions =
                imeOptions ?: when (singleLine) {
                    true -> EditorInfo.IME_ACTION_NEXT
                    false -> EditorInfo.IME_ACTION_NONE
                }

        holder.textInputEditText.addTextChangedListenerOnce(onTextChangeListener)

        holder.textInputDeleteButton.onClick(onDeleteClicked)
    }

    override fun shouldSaveViewState() = false

    override fun unbind(holder: Holder) {
        super.unbind(holder)
        holder.textInputEditText.removeTextChangedListener(onTextChangeListener)
    }

    class Holder : PcpEpoxyHolder() {
        val textInputLayout by bind<TextInputLayout>(R.id.formTextInputTextInputLayout)
        val textInputEditText by bind<TextInputEditText>(R.id.formTextInputTextInputEditText)
        val textInputDeleteButton by bind<ImageButton>(R.id.formTextInputDeleteButton)
    }
}
