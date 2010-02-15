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
<form method="post" action="/" class="admin_form admin_slideshow_group_add" enctype="multipart/form-data">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="group_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="group_title" id="group_title" value="{$FORMERROR.record->image_title}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="group_header">Header:</label>
        </div>
        <div class="admin_form_field">
            <select id="image_image_select" name="image_image_select" style="width: 125px;">
                {foreach from=$images item=image}
                <option value="{$image->image_id}">{$image->image_title}</option>
                {/foreach}
            </select>
            or <input type="file" name="image_image" id="group_header" />
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_groups">Images:</label>
        </div>
        <div class="admin_form_field">
            <ul id="group_images">
            </ul>
            <div>
                <select id="image_groups" style="width: 325px;">
                    {foreach from=$images item=image}
                    <option value="{$image->image_id}">{$image->image_title}</option>
                    {/foreach}
                </select><br/>
                <button id="add_image" type="button" style="width: 70px;" onclick="addImage();">Add</button><br />
                Or upload a new one:<br/>
                <div style="margin-top: 10px;">
                    <div>
                        <div style="float: left; width: 75px;">Title</div><input type="text" id="image_title" style="width: 250px;"/>
                    </div>
                    <div id="image_size">
                        <input type="radio" value="1" name="image_size_dummy"/>Small
                        <input type="radio" value="2" name="image_size_dummy" checked="checked"/>Large
                    </div>
                    <button id="upload_image" type="button" style="width: 70px;">Upload</button> (You will be promted for the file)
                </div>
            </div>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add_group" />
    <input type="hidden" name="component" value="slideshow" />
</form>
<script>
    {literal}
    var upload = new AjaxUpload('#upload_image', 
    {
        action: '/',
        name: 'image_image',
        responseType: 'json',
        autoSubmit: true,
        onChange: function(file, extension){
            if($('#image_title').val() == '')
            {
              $("#image_title").val(file.substr(0, file.lastIndexOf(".")));
            }
        },
        onSubmit: function(file, extension){
            var size;
            
            $("#image_size input").each(function(key, value) {
                if(value.checked)
                {
                    size = $(value).val();
                }
            });
            
            this.setData({
                action : 'add_image',
                component : 'slideshow',
                ajax: 'true',
                image_title: $('#image_title').val(),
                image_size: size
            });
        },
        onComplete: function(file, response) {
            //console.log(file, response);
            if(response.error)
            {
                alert("There was an error when uploading the file: '"+response.error+"'.");
                return false;
            }
            
            var html = "";
            
            html += "<li id='image" + response[0].image_id + "'>";
            html += "<a href='javascript: removeImage(" + response[0].image_id + ");'><img src='/img/icons/delete.png'/></a> ";
            html += response[0].image_title;
            html += "<input type='hidden' name='images[]' value='" + response[0].image_id + "'>";
            html += "</li>";
            
            //add to the list
            $('#group_images').append(html);
            
            //clear the title field
            $('#image_title').val('');
        }
    });
    
    function addImage()
    {
        var select = $('#image_groups');
        
        select.find("option").each(function(key, value) {
            if($(value).val() == select.val())
            {
                var html = "";
                
                html += "<li id='image" + $(value).val() + "'>";
                html += "<a href='javascript: removeImage(" + $(value).val() + ");'><img src='/img/icons/delete.png'/></a> ";
                html += $(value).text();
                html += "<input type='hidden' name='images[]' value='" + $(value).val() + "'>";
                html += "</li>";
                
                //add to the list
                $('#group_images').append(html);
                
                //remove from dropdown
                $(value).remove();
            }
        });
    }
    
    function removeImage(id)
    {
        var item = $('#image'+id);
        
        //add to dropdown
        $('#image_groups').append("<option value='" + id + "'>" + item.text() + "</option>");
        
        //remove from list
        item.remove();
    }
    
    {/literal}
</script>