<?php /* Smarty version 2.6.22, created on 2009-09-20 22:55:03
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cpage/forms/form.edit.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'admintree', 'file:D:\\www\\skimfo\\components\\page/forms/form.edit.tpl', 27, false),)), $this); ?>
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
            <label for="page_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="page_name" id="page_name" value="<?php echo $this->_tpl_vars['record']->page_name; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="page_location">Location:</label>
        </div>
        <div class="admin_form_field">
            <?php echo smarty_function_admintree(array('javascript' => 'page_location','folderonly' => true), $this);?>

            <input type="text" name="page_location" id="page_location" value="<?php echo $this->_tpl_vars['record']->page_location; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="page_template">Content:</label>
        </div>
        <div class="admin_form_field">
            <textarea name="page_template" id="page_template" rows="5" cols="20" class="htmleditor"><?php echo $this->_tpl_vars['record']->page_template; ?>
</textarea>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="page_id" value="<?php echo $this->_tpl_vars['record']->page_id; ?>
" />
    <input type="hidden" name="action" value="edit" />
    <input type="hidden" name="component" value="page" />
</form>