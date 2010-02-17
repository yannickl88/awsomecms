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
            <input type="file" name="image_image" id="image_image" />
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_image">{"field_image_size"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="radio" name="image_size" id="image_size" value="1" {if $record->image_size eq 1}checked="checked"{/if}/>{"small"|text|ucfirst}
            <input type="radio" name="image_size" id="image_size" value="2" {if $record->image_size eq 2}checked="checked"{/if}/>{"large"|text|ucfirst}
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|ucfirst}" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="image_id" value="{$record->image_id}" />
    <input type="hidden" name="action" value="edit_image" />
    <input type="hidden" name="component" value="slideshow" />
</form>
<script>
    {literal}
    $().ready(function(e) {
        $("#image_image").change(function(e) {
            if($("#image_title").val() == "")
            {
                var title = $("#image_image").val();
                $("#image_title").val(title.substr(0, title.lastIndexOf(".")));
            }
        });
    });
    {/literal}
</script>