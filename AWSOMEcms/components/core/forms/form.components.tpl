<form method="post" action="/" class="admin_form admin_core_components">
    <table>
        <tr class="header">
            <th>Component</th>
            <th>Public Access</th>
        </tr>
        {foreach from=$components item=component}
            <tr class="row">
                <td>{$component->component_name}</td>
                <td>
                    <input name="auth_{$component->component_name}[]" value="{$auth.auth_select}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_select} checked="checked"{/if} /> Select
                    <input name="auth_{$component->component_name}[]" value="{$auth.auth_insert}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_insert} checked="checked"{/if} /> Insert
                    <input name="auth_{$component->component_name}[]" value="{$auth.auth_update}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_update} checked="checked"{/if} /> Update
                    <input name="auth_{$component->component_name}[]" value="{$auth.auth_delete}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_delete} checked="checked"{/if} /> Delete
                </td>
            </tr>
        {/foreach}
    </table>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="component_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="components" />
    <input type="hidden" name="component" value="core" />
</form>