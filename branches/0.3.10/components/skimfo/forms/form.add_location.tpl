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
<form method="post" action="/" class="admin_form admin_skimfo_location_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="location_name">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="location_name" id="location_name" value="{$FORMERROR.record.spot_title}"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="skimspot_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add_location" />
    <input type="hidden" name="component" value="skimfo" />
</form>