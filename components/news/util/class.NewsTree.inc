<?php
import('/components/page/util/class.Tree.inc');

class NewsItem extends TreeItem
{
    public function __toString()
    {
        $html = "";
        if(!$this->getRoot()->onlyShowFolders)
        {
            $html .= "<div class=\"hideIcons\" onmouseout=\"$(this).addClass('hideIcons');\" onmouseover=\"$(this).removeClass('hideIcons');\">";
            $html .= "<a class=\"deleteIcon treeIcon\" href=\"{$this->deleteURL}\"><img src=\"/img/icons/page_delete.png\" /></a>";
            $html .= "<a class=\"viewIcon treeIcon\" href=\"{$this->viewURL}\"><img src=\"/img/icons/page_view.png\" /></a>";
            $html .= "<a href=\"{$this->editURL}\">";
            $html .= '<img src="/img/icons/news.png" />';
            $html .= "{$this->name}</a>";
            $html .= "</div>";
        }
        return $html;
    }
}

class NewsFolder extends TreeFolder
{
    public function __toString()
    {
        $id = md5($this->getLocation());
        
        $html = "";
        $html .= "<div class=\"hideIcons\" onmouseout=\"$(this).addClass('hideIcons');\" onmouseover=\"$(this).removeClass('hideIcons');\">";
        $html .= "<a class=\"addIcon treeIcon\" href=\"/".Config::get("newsadd", "newsadd", "admin")."\"><img src=\"/img/icons/add.png\" /></a>";
        $html .= "<a class=\"viewIcon treeIcon\" href=\"/".Config::get("newsadmin", "newsadmin", "admin")."\"><img src=\"/img/icons/overview.png\" /></a>";
        if(!isset($_COOKIE['menu'.$id]))
        {
            $html .= "<a class=\"folder link{$id} clickableTreeArrow\" href=\"#\"></a>";
        } 
        else
        {
            $html .= "<a class=\"folder link{$id} open clickableTreeArrow\" href=\"#\"></a>";
        }
        $html .= '<img src="/img/icons/news_folder.png" alt="folder"/>';
        if($this->getRoot()->javascript)
        {
            $html .= "<a href=\"{$this->getLocation()}\" class=\"clickableTreeItem javascript\" style=\"padding: 0;\">";
        }
        else
        {
            $html .= "<a href=\"#\" class=\"clickableTreeItem\" style=\"padding: 0;\">";
        }
        if($this->name == "")
        {
            $html .= "/";
        }
        else
        {
            $html .= $this->name;
        }
        $html .= '</a>';
        if(!isset($_COOKIE['menu'.$id]))
        {
            $html .= '<div class="treeNode hidden">';
        }
        else
        {
            $html .= '<div class="treeNode">';
        }
        
        foreach($this->children as $child)
        {
            $html .= $child->toHTML();
        }
        
        $html .= '</div>';
        $html .= "</div>";
        return $html;
    }
}