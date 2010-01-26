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
<form method="post" action="/" class="admin_form admin_skimfo_skimspot_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="spot_title" id="spot_title" value="{$FORMERROR.record.spot_title}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_text">Text:</label>
        </div>
        <div class="admin_form_field">
            <textarea name="spot_text" id="spot_text" rows="5" cols="20">{$FORMERROR.record.spot_text}</textarea>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_location">Location:</label>
        </div>
        <div class="admin_form_field">
            <select name="spot_location" id="spot_location">
                {foreach from=$locations item=location}
                    <option value="{$location->location_id}" {if $FORMERROR.record.spot_location == $location->location_id}selected="selected"{/if}>{$location->location_name}</option>
                {/foreach}
            </select>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_header">Header:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="spot_header" id="spot_header" />
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_img">Image:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="spot_img" id="spot_img" />
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="skimspot_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add_skimspot" />
    <input type="hidden" name="component" value="skimfo" />
</form>