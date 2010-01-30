<?php
  function smarty_function_renderdebug($params, &$smarty)
  {
      return Debugger::getInstance();
  }