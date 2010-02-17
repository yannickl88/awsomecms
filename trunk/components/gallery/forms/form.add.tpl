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
<h2>{"addto"|text|ucfirst} {"comp_gallery"|text}</h2>
<form method="post" action="/" class="admin_form admin_gallery_add" enctype="multipart/form-data">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_title">{"field_image_title"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_title" id="image_title" value="{$FORMERROR.record->image_title}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_image">{"field_image_image"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_image" id="image_image" />
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_thumb">{"field_image_thumb"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_thumb" id="image_thumb" {if $FORMERROR.record->image_tumb_generate}disabled="disabled"{/if}/>
            <input type="checkbox" name="image_tumb_generate" id="image_tumb_generate" {if $FORMERROR.record->image_tumb_generate}checked="checked"{/if}/> {"generate"|text|ucfirst}
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_tag">{"field_image_tag"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_tag" id="image_tag" value="{$FORMERROR.record->image_tag}"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|ucfirst}" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="gallery" />
</form>
<script>
    {literal}
    function toggleDisabled(checkbox, fieldID)
    {
        var field = $('#'+fieldID);
        
        if($(checkbox).val())
        {
            if(field.attr('disabled'))
            {
                field.removeAttr('disabled');
            }
            else
            {
                field.attr('disabled', 'disabled');
            }
        }
    }
    $().ready(function(e) {
        $("#image_image").change(function(e) {
            if($("#image_title").val() == "")
            {
                var title = $("#image_image").val();
                $("#image_title").val(title.substr(0, title.lastIndexOf(".")));
            }
        });
        $("#image_tumb_generate").change(function(e) {
            toggleDisabled(this, 'image_thumb');
        });
    });
    {/literal}
</script>