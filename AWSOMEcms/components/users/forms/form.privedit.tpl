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
<form method="post" action="/admin/users/privedit" class="admin_form admin_priv_edit">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="privilege_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="privilege_name" id="privilege_name" value="{$record->privilege_name}"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="priv_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="privilege_name_old" value="{$record->privilege_name}" />
    <input type="hidden" name="action" value="privedit" />
    <input type="hidden" name="component" value="users" />
</form>