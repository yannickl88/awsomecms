<?php /* Smarty version 2.6.22, created on 2009-09-12 00:43:49
         compiled from file:D:%5Cwww%5Cskimfo%5Chtdocs%5Ccomponents%5Cnews/forms/form.add.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'html_select_date', 'file:D:\\www\\skimfo\\htdocs\\components\\news/forms/form.add.tpl', 39, false),)), $this); ?>
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
<form method="post" action="/" class="admin_form admin_news_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="news_title" id="news_title" value="<?php echo $this->_tpl_vars['FORMERROR']['record']['news_title']; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_user">User:</label>
        </div>
        <div class="admin_form_field">
            <select name="news_user" id="news_user">
                <?php $_from = $this->_tpl_vars['users']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['user']):
?>
                    <option value="<?php echo $this->_tpl_vars['user']->user_id; ?>
" <?php if ($this->_tpl_vars['FORMERROR']['record']['news_user'] == $this->_tpl_vars['user']->user_id): ?>selected="selected"<?php endif; ?>><?php echo $this->_tpl_vars['user']->user_name; ?>
</option>
                <?php endforeach; endif; unset($_from); ?>
            </select>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_date_Month">Date:</label>
        </div>
        <div class="admin_form_field">
            <?php echo smarty_function_html_select_date(array('prefix' => 'news_date_','time' => $this->_tpl_vars['FORMERROR']['record']['news_date'],'day_extra' => "id=news_date_Day",'month_extra' => "id=news_date_Month",'year_extra' => "id=news_date_Year"), $this);?>

        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_text">Content:</label>
        </div>
        <div class="admin_form_field">
            <textarea name="news_text" id="news_text" rows="5" cols="20"></textarea>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="news_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="news" />
</form>