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
<form method="post" action="/admin/users/add" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="user_name" id="user_name" value="{$FORMERROR.record.user_name}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_pass">Password:</label>
        </div>
        <div class="admin_form_field">
            <input type="password" name="user_pass" id="user_pass"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_pass2">Again:</label>
        </div>
        <div class="admin_form_field">
            <input type="password" name="user_pass2" id="user_pass2"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="users" />
</form>