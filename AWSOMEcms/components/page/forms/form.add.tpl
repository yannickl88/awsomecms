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
<form method="post" action="/" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="page_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="page_name" id="page_name" value="{$FORMERROR.record.page_name}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="page_location">Location:</label>
        </div>
        <div class="admin_form_field">
            {admintree javascript="page_location" folderonly=true}
            <input type="text" name="page_location" id="page_location" value="{$FORMERROR.record.page_location}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="page_template">Content:</label>
        </div>
        <div class="admin_form_field">
            <textarea name="page_template" id="page_template" rows="5" cols="20" class="htmleditor">{$FORMERROR.record.page_template}</textarea>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="page" />
</form>