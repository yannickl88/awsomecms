<?php /* Smarty version 2.6.22, created on 2009-09-20 12:10:18
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cslideshow/forms/form.edit_image.tpl */ ?>
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
            <input type="text" name="image_title" id="image_title" value="<?php echo $this->_tpl_vars['record']->image_title; ?>
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
            <label for="image_image">Size:</label>
        </div>
        <div class="admin_form_field">
            <input type="radio" name="image_size" id="image_size" value="1" <?php if ($this->_tpl_vars['record']->image_size == 1): ?>checked="checked"<?php endif; ?>/>Small
            <input type="radio" name="image_size" id="image_size" value="2" <?php if ($this->_tpl_vars['record']->image_size == 2): ?>checked="checked"<?php endif; ?>/>Large
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="image_id" value="<?php echo $this->_tpl_vars['record']->image_id; ?>
" />
    <input type="hidden" name="action" value="edit_image" />
    <input type="hidden" name="component" value="slideshow" />
</form>