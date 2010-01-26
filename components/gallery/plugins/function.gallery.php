<?php
  function smarty_function_gallery($params, &$smarty)
  {
      $galleryComponent = Component::init('gallery');
      $data = array();
      
      if(isset($params['max']))
      {
          $data['max'] = $params['max'];
      }
      if(isset($params['tag']))
      {
          $data['image_tag'] = $params['tag'];
      }
      
      $smarty->assign("gallery", $galleryComponent->select($data));
      
      return $smarty->fetch("gallery/gallery.tpl");
  }