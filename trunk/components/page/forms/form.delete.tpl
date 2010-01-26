<form method="post" action="/" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this item? <br />
            <ul>
                <li><img src="/img/icons/page.png" alt="">{$record->page_location}{$record->page_name}</li>
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="page_id" value="{$record->page_id}" />
    <input type="hidden" name="action" value="delete" />
    <input type="hidden" name="component" value="page" />
</form>