<?php /* Smarty version 2.6.22, created on 2009-09-12 23:15:21
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cgallery/forms/form.add.tpl */ ?>
<?php if ($this->_tpl_vars['FORMERROR']): ?>
    <div class="form_error">
        The form was not filled in correctly. Please check the following fields:
        <ul id="errorlist">
        </ul>
    </div>
    <script>
        <?php $_from = $this->_tpl_vars['FORMERROR']['fields']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['field'] => $this->_tpl_vars['error']):
?>
            addFieldError('<?php echo $this->_tpl_vars['field']; ?>
', '<?php echo $this->_tpl_vars['error']; ?>
');
        <?php endforeach; endif; unset($_from); ?>
    </script>
<?php endif; ?>
<form method="post" action="/" class="admin_form admin_gallery_add" enctype="multipart/form-data">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_title" id="image_title" value="<?php echo $this->_tpl_vars['FORMERROR']['record']['image_title']; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_image">Image:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_image" id="image_image" />
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_thumb">Tumb:</label>
        </div>
        <div class="admin_form_field">
            <input type="file" name="image_thumb" id="image_thumb" <?php if ($this->_tpl_vars['FORMERROR']['record']['image_tumb_generate']): ?>disabled="disabled"<?php endif; ?>/>
            <input type="checkbox" name="image_tumb_generate" id="image_tumb_generate" <?php if ($this->_tpl_vars['FORMERROR']['record']['image_tumb_generate']): ?>checked="checked"<?php endif; ?> onchange="toggleDisabled(this, 'image_thumb');"/> Generate
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="image_tag">Tag:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="image_tag" id="image_tag" value="<?php echo $this->_tpl_vars['FORMERROR']['record']['image_tag']; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="gallery" />
</form>
<script>
    <?php echo '
    function toggleDisabled(checkbox, fieldID)
    {
        var field = $(\'#\'+fieldID);
        
        if($(checkbox).val())
        {
            if(field.attr(\'disabled\'))
            {
                field.removeAttr(\'disabled\');
            }
            else
            {
                field.attr(\'disabled\', \'disabled\');
            }
        }
    }
    '; ?>

</script>