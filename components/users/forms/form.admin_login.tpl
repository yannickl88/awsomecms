{if $FORMERROR}{$FORMERROR}{/if}
<form method="post" action="/" class="admin_form admin_login">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_name">{"field_user_name"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="user_name" id="user_name"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_pass">{"field_user_pass"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="password" name="user_pass" id="user_pass"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"login"|text}" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="admin_login" />
    <input type="hidden" name="component" value="users" />
</form>