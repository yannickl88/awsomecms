{if $FORMERROR}{$FORMERROR}{/if}
<h2>{"edit"|text|ucfirst} {$record->image_title}</h2>
<form method="post" action="/" class="admin_form admin_gallery_add" enctype="multipart/form-data">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_title">{"field_image_title"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_title" id="image_title" value="{$record->image_title}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_image">{"field_image_image"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_image" id="image_image"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_thumb">{"field_image_thumb"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_thumb" id="image_thumb"/>
            <input type="checkbox" name="image_tumb_generate" id="image_tumb_generate"/> {"generate"|text|ucfirst}
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_tag">{"field_image_tag"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_tag" id="image_tag" value="{$record->image_tag}"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|ucfirst}" id="user_submit" class="admin_form_submit" />
    </div>
    <input type="hidden" name="image_id" value="{$record->image_id}" />
    <input type="hidden" name="action" value="edit" />
    <input type="hidden" name="component" value="gallery" />
</form>
<script type="text/javascript">
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
        $("#image_tumb_generate").change(function(e) {
            toggleDisabled(this, 'image_thumb');
        });
        //check if it was already set somehow
        $("#image_thumb").attr("disabled", ($("#image_tumb_generate").attr("checked"))? "disabled" : "");
    });
    {/literal}
</script>