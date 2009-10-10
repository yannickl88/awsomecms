<form method="post" action="/" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this item? <br />
            <ul>
                {foreach from=$record item=row}
                    <li><img src="/img/icons/form.png" alt="">{$row->form_name}</li>
                    <input type="hidden" name="form_id[]" value="{$row->form_id}" />
                {/foreach}
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="form_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="delete" />
    <input type="hidden" name="component" value="contact" />
</form>