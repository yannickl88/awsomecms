<?php /* Smarty version 2.6.22, created on 2009-09-12 17:52:41
         compiled from news/news_side.tpl */ ?>
<img src="/img/nieuws.gif" alt="Nieuws"/>
<ul>
<?php $_from = $this->_tpl_vars['news']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['item']):
?>
    <li><a href="/news?id=<?php echo $this->_tpl_vars['item']->news_id; ?>
"><?php echo $this->_tpl_vars['item']->news_title; ?>
</a></li>
<?php endforeach; endif; unset($_from); ?>
</ul>