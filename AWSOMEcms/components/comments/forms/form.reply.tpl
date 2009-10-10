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
    <div class="form_row">
        <div class="form_label">
            <label for="comment_username">Name/E-mail<span class="required">*</span>:</label>
        </div>
        <div class="form_field">
            <input type="text" name="comment_username" id="comment_username" value="{$FORMERROR.record.comment_username}"/>
        </div>
    </div>
    <div class="form_row">
        <div class="form_label">
            <label for="comment_text">Reply<span class="required">*</span>:</label>
        </div>
        <div class="form_field">
            <textarea name="comment_text" id="comment_text" rows="5" cols="30">{$FORMERROR.record.$fieldcode}</textarea>
        </div>
    </div>
    <div class="form_row form_submit">
        <input type="submit" value="Send" id="contact_submit" class="form_submit">
    </div>
    <input type="hidden" name="comment_hook" value="{$comment_hook}" />
    <input type="hidden" name="action" value="reply" />
    <input type="hidden" name="component" value="comments" />
    <br />
    Fields marked as <span class="required">*</span> are required. 
</form>