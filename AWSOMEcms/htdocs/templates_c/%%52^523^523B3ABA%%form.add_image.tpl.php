<?php /* Smarty version 2.6.22, created on 2009-09-20 00:08:31
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cslideshow/forms/form.add_image.tpl */ ?>
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
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add_image" />
    <input type="hidden" name="component" value="slideshow" />
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