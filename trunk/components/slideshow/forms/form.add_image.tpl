{if $FORMERROR}{$FORMERROR}{/if}
<h2>{"addto"|text|ucfirst} {"comp_slideshow"|text}</h2>
<form method="post" action="/" class="admin_form admin_gallery_add" enctype="multipart/form-data">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_title">{"field_image_title"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_title" id="image_title"/>
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
            <label for="image_image">{"field_image_size"|text}:</label>
        </div>
        <div class="admin_form_field">
            <input type="radio" name="image_size" id="image_size1" value="1"/>{"small"|text|ucfirst}
            <input type="radio" name="image_size" id="image_size2" value="2" checked="checked"/>{"large"|text|ucfirst}
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|ucfirst}" id="user_submit" class="admin_form_submit" />
    </div>
    <input type="hidden" name="action" value="add_image" />
    <input type="hidden" name="component" value="slideshow" />
</form>
<script type='text/javascript'>
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