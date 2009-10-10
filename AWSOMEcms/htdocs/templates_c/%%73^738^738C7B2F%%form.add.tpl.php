<?php /* Smarty version 2.6.22, created on 2009-09-12 12:02:18
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cusers/forms/form.add.tpl */ ?>
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
<form method="post" action="/admin/users/add" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="user_name" id="user_name" value="<?php echo $this->_tpl_vars['FORMERROR']['record']['user_name']; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_pass">Password:</label>
        </div>
        <div class="admin_form_field">
            <input type="password" name="user_pass" id="user_pass"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="user_pass2">Again:</label>
        </div>
        <div class="admin_form_field">
            <input type="password" name="user_pass2" id="user_pass2"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="user_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="users" />
</form>