<?php /* Smarty version 2.6.22, created on 2009-09-12 17:50:00
         compiled from news/newsitem.tpl */ ?>
<h1><?php echo $this->_tpl_vars['news']->news_title; ?>
</h1>
<div>
<?php echo $this->_tpl_vars['news']->news_text; ?>

</div>
<div style="text-align: right;">
    Door <?php echo $this->_tpl_vars['news']->user_name; ?>
 op <?php echo $this->_tpl_vars['news']->news_date; ?>

</div>