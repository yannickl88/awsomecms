<form method="post" action="/" class="admin_form admin_users_delete">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this item? <br />
            <ul>
                {foreach from=$record item=row}
                    <li><img src="/img/icons/user.png" alt="">{$row->user_name}</li>
                    <input type="hidden" name="user_id[]" value="{$row->user_id}" />
                {/foreach}
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="delete" />
    <input type="hidden" name="component" value="users" />
</form>