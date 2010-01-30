<form method="post" action="/" class="admin_form admin_groups_delete">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this group? <br />
            <ul>
                {foreach from=$record item=row}
                    <li><img src="/img/icons/group.png" alt="">{$row->group_name}</li>
                    <input type="hidden" name="group_id[]" value="{$row->group_id}" />
                {/foreach}
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="group_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="delete" />
    <input type="hidden" name="component" value="groups" />
</form>