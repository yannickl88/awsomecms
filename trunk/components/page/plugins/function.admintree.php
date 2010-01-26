<?php
  function smarty_function_admintree($params, &$smarty)
  {
      $javascript = ''.$params['javascript'];
      $folderOnly = ($params['folderonly'] === true);
      if(isset($params['hideadmin']))
      {
          $hideAdmin = ($params['hideadmin'] === true);
      }
      else
      {
          $hideAdmin = Config::get("hideadmintree", true, "admin");
      }
      
      $tree = array();
      
      $currentFolder = '/';
      
      if($hideAdmin)
      {
          $result = Controller::makeRequest('page', "GET", array("where" => array('page_location', "/admin/%", "NOT LIKE")));
      }
      else
      {
          $result = Controller::makeRequest('page');
      }
      
      foreach($result as $row)
      {
          pages_addToTree(substr($row->page_location, 1), $row, $tree['items'], '/');
      }
      
      include_once Config::get('websiteroot', '.').'/../components/page/util/class.TreeSorter.inc';
      
      $sorter = new TreeSorter($tree);
      $tree = $sorter->sort();
      
      $html = '<div class="treeNode" style="padding: 0;">';
      $html .= pages_renderTree($tree, $theme, true, $javascript, $folderOnly);
      $html .= '</div>';
      
      return $html;
  }
  
  function pages_addToTree($location, $item, &$tree, $parentLoc)
  {
      if($location == '')
      {
          if($item->page_isfolder == 1)
          {
              $tree[$item->page_name]['items'] = array();
              $tree[$item->page_name]['location'] = $parentLoc.$item->page_name.'/';
          }
          else
          {
            $tree[] = $item;
          }
      }
      else
      {
          $locationArray = explode('/', $location);
          $folder = array_shift($locationArray);
          
          $tree[$folder]['location'] = $parentLoc.$folder.'/';
          
          pages_addToTree(implode('/', $locationArray), $item, &$tree[$folder]['items'], $tree[$folder]['location']);
      }
  }

  function pages_renderTree($tree, $theme, $hidden, $javascript = '', $folderOnly = false)
  {
      $html = '';
      
      if($tree['items'] && is_array($tree['items']))
      {
          foreach($tree['items'] as $key => $node)
          {
              $html .= '<div class="hideIcons" onMouseOver="$(this).removeClass(\'hideIcons\');" onMouseOut="$(this).addClass(\'hideIcons\');">';
              
              if(!is_numeric($key))
              {
                  $id = md5($node['location']);
                  
                  if($hidden && !isset($_COOKIE['menu'.$id]))
                  {
                    $html .= '<a id="link'.$id.'" class="folder link'.$id.'" href="javascript: void(0)" onClick="admin_toggleTree(\''.$id.'\')" onMouseOver="$(this).addClass(\'focus\')" onMouseOut="$(this).removeClass(\'focus\')">';
                  }
                  else
                  {
                    $html .= '<a id="link'.$id.'" class="folder open link'.$id.'" href="javascript: void(0)" onClick="admin_toggleTree(\''.$id.'\')" onMouseOver="$(this).addClass(\'focus\')"  onMouseOut="$(this).removeClass(\'focus\')">';
                  }
                  $html .= '</a>';
                  
                  if($javascript != '')
                  {
                    $html .= '<a href="javascript: void(0);" onClick="$(\'#'.$javascript.'\')[0].value = \''.$node['location'].'\';" style="padding: 0;">';
                  }
                  else
                  {
                      $html .= '<a href="javascript: void(0)" onClick="admin_toggleTree(\''.$id.'\')" style="padding: 0;">';
                  }
                  $html .= '<img src="/img/icons/page_folder.png" />';
                  $html .= $key;
                  $html .= '</a>';
                  if($hidden && !isset($_COOKIE['menu'.$id]))
                  {
                    $html .= '<div class="treeNode hidden sub'.$id.'">';
                  }
                  else
                  {
                    $html .= '<div class="treeNode sub'.$id.'">';
                  }
                  $html .= pages_renderTree($node, $theme, $hidden, $javascript, $folderOnly);
                  $html .= '</div>';
              }
              else if(!$folderOnly)
              {
                  if($javascript != '')
                  {
                      $html .= '<a href="javascript: void(0);" onClick="$(\'#'.$javascript.'\')[0].value = \''.$node->page_location.$node->page_name.'\';">';
                  }
                  else
                  {
                      $html .= '<a class="deleteIcon" href="/'.Config::get('pagedelete', 'pagedelete', 'admin').'&page_id='.$node->page_id.'"></a>';
                      $html .= '<a class="viewIcon" href="'.$node->page_location.$node->page_name.'"></a>';
                      $html .= '<a href="/'.Config::get('pageedit', 'pageedit', 'admin').'&page_id='.$node->page_id.'" >';
                  }
                  if($node->page_name == 'index')
                  {
                      $html .= '<img src="/img/icons/page_home.png" />';
                  }
                  else
                  {
                      $html .= '<img src="/img/icons/page_page.png" />';
                  }
                  $html .= $node->page_name;
                  $html .= '</a>';
              }
              $html .= '</div>';
          }
      }
      
      return $html;
  }