<?php
  function smarty_function_rendertime($params, &$smarty)
  {
      global $start;
      
      return round(microtime(true) - $start, 3);
  }