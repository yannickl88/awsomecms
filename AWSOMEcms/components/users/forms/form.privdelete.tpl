<form method="post" action="/" class="admin_form admin_users_privdelete">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this item? <br />
            <ul>
                {foreach from=$record item=row}
                    <li><img src="/img/icons/user.png" alt="">{$row->privilege_name}</li>
                    <input type="hidden" name="privilege_name[]" value="{$row->privilege_name}" />
                {/foreach}
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="priv_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="privdelete" />
    <input type="hidden" name="component" value="users" />
</form>