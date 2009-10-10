<?php /* Smarty version 2.6.22, created on 2009-09-12 23:11:42
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cskimfo/forms/form.add_skimspot.tpl */ ?>
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
<form method="post" action="/" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="spot_title" id="spot_title" value="<?php echo $this->_tpl_vars['FORMERROR']['record']['spot_title']; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_text">Text:</label>
        </div>
        <div class="admin_form_field">
            <textarea name="spot_text" id="spot_text" rows="5" cols="20"><?php echo $this->_tpl_vars['FORMERROR']['record']['spot_text']; ?>
</textarea>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="spot_location">Location:</label>
        </div>
        <div class="admin_form_field">
            <select name="spot_location" id="spot_location">
                <?php $_from = $this->_tpl_vars['locations']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['location']):
?>
                    <option value="<?php echo $this->_tpl_vars['location']->location_id; ?>
" <?php if ($this->_tpl_vars['FORMERROR']['record']['spot_location'] == $this->_tpl_vars['location']->location_id): ?>selected="selected"<?php endif; ?>><?php echo $this->_tpl_vars['location']->location_name; ?>
</option>
                <?php endforeach; endif; unset($_from); ?>
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
        <input type="submit" value="Save" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="page" />
</form>