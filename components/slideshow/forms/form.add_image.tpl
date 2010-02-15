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
<form method="post" action="/" class="admin_form admin_gallery_add" enctype="multipart/form-data">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_title" id="image_title" value="{$FORMERROR.record.image_title}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_image">Image:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_image" id="image_image"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_image">Size:</label>
        </div>
        <div class="admin_form_field">
            <input type="radio" name="image_size" id="image_size" value="1"/>Small
            <input type="radio" name="image_size" id="image_size" value="2" checked="checked"/>Large
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add_image" />
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