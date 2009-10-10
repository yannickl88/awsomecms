{if $FORMERROR}
    <div class="form_error">
        The form was not filled in correctly. Please check the following fields:
        <ul id="errorlist">
        </ul>
    </div>
    <script>
        {foreach from=$FORMERROR.fields key=field item=error}
            addFieldError('{$field}', '{$error}');
        {/foreach}
    </script>
{/if}
<form method="post" action="/" class="form contact_send">
    {foreach from=$contactform->fields item=row}
        <div class="form_row">
            <div class="form_label">
                <label for="{$row->field_code}">{$row->field_name}{if $row->field_required == 1}<span class="required">*</span>{/if}:</label>
            </div>
            <div class="form_field">
                {assign var=fieldcode value=$row->field_code}
                {if $row->field_type == 1}
                    <input type="text" name="{$fieldcode}" id="{$fieldcode}" value="{$FORMERROR.record.$fieldcode}"/>
                {elseif $row->field_type == 2}
                    <input type="password" name="{$fieldcode}" id="{$fieldcode}"/>
                {elseif $row->field_type == 3}
                    <textarea name="{$fieldcode}" id="{$fieldcode}" rows="5" cols="30">{$FORMERROR.record.$fieldcode}</textarea>
                {elseif $row->field_type == 4}
                    <select name="{$fieldcode}" id="{$fieldcode}">
                        <option {if $FORMERROR.record.$fieldcode == 'Male'}selected="selected"{/if}>Male</option>
                        <option {if $FORMERROR.record.$fieldcode == 'Female'}selected="selected"{/if}>Female{$FORMERROR.record.$fieldcode}</option>
                    </select>
                {/if}
            </div>
        </div>
    {/foreach}
    <div class="form_row form_submit">
        <input type="submit" value="Send" id="contact_submit" class="form_submit" />
    </div>
    <input type="hidden" name="form_id" value="{$contactform->form_id}" />
    <input type="hidden" name="url" value="{$URL}" />
    <input type="hidden" name="action" value="send" />
    <input type="hidden" name="component" value="contact" />
    <br />
    Fields marked as <span class="required">*</span> are required. 
</form>