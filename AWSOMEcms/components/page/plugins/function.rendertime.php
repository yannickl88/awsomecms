<?php
  function smarty_function_rendertime($params, &$smarty)
  {
      $start = $params['start'];
      
      return round(microtime(true) - $start, 3);
  }