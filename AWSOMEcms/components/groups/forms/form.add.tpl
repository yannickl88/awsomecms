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
<form method="post" action="/admin/groups/add" class="admin_form admin_group_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="group_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="group_name" id="group_name" value="{$FORMERROR.record.group_name}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label>Privileges:</label>
        </div>
        <div class="admin_form_field">
            {foreach from=$privileges item=priv}
                <input type="checkbox" name="privileges[]" value="{$priv->privilege_int}" {if $record->group_privileges|hasflag:$priv->privilege_int} checked="checked"{/if}/>{$priv->privilege_name}<br />
            {/foreach}
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="groups_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="groups" />
</form>