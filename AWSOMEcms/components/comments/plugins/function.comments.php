<?php
  function smarty_function_comments($params, &$smarty)
  {
      $hook = $_GET['url'];
      
      if(isset($params['hook']))
      {
          $hook = $params['hook'];
      }
      if(isset($params['redirect']))
      {
          $smarty->assign("comment_redirect", $params['redirect']);
      }
      
      include_once Config::get('websiteroot', '.').'/../components/comments/class.CommentsComponent.inc';
      $commentsComponent = new CommentsComponent();
      
      $smarty->assign("comments", $commentsComponent->select(array('hook' => $hook)));
      $smarty->assign("comment_hook", $hook);
      
      return $smarty->fetch("comments/comments.tpl");
  }