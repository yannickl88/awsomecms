/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package js
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Admin tree with nodes
 */
var Tree = function(element)
{
    this.children = new Array();
    this.html = null;

    /**
     * Constructor
     */
    this.init = function(element)
    {
        this.html = $("<div class='treeNode'></div>");
        var ref = this;
        
        $(element).ready(function(e)
        {
            $(element).append(ref.html);
        });
    };
    /**
     * Add a child to the folder
     * 
     * @param TreeItem child
     * @param Array locArray
     */
    this.addChild = function(child)
    {
        child.root = this;
        
        if (child.location == "/")
        {
            var x = 0;
            while(x < this.children.length)
            {
                if(this.children[x].name > child.name && this.children[x].type <= child.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                this.children.push(child);
                this.html.append(child.html);
            }
            else
            {
                this.children[x].html.before(child.html)
                this.children.splice(x, 0, child);
            }
            child.parent = this;
        }
        else
        {
            var locArray = child.location.substr(1, child.location.length-2).split("/");
            var loc = locArray.shift();
            var inserted = false;

            for (key in this.children)
            {
                var node = this.children[key];

                if (node.type == Tree.PAGE_FOLDER && node.getPath() == "/" + loc)
                {
                    inserted = node.addChild(child, locArray);
                    break;
                }
            }

            if (!inserted)
            {
                var folder = new TreeFolder(loc, "/");
                var x = 0;
                while(x < this.children.length)
                {
                    if(this.children[x].name > folder.name && this.children[x].type <= folder.type)
                    {
                        break;
                    }
                    x++;
                }
                if(x == this.children.length)
                {
                    this.children.push(folder);
                    this.html.children(".treeNode").append(folder.html);
                }
                else
                {
                    this.children[x].html.before(folder.html)
                    this.children.splice(x, 0, folder);
                }
                folder.parent = this;
                folder.root = this;
                folder.addChild(child, locArray);
                this.html.append(folder.html);
            }
        }
    };

    this.init(element);
}
// Constants
Tree.PAGE = 1;
Tree.PAGE_FOLDER = 2;
Tree.CONTENT = 3;
Tree.CONTENT_FOLDER = 4;

/**
 * Folder page item
 */
var TreeItem = function(name, location, link)
{
    this.name = name;
    this.location = location;
    this.parent = null;
    this.link = link;
    this.type = Tree.PAGE;
    this.html = null;
    this.root = null;
    
    /**
     * Constructor
     */
    this.init = function()
    {
        this.html = $("<div class='hideIcons'></div>");
        this.html[0].object = this;
        this.html.append("<a class='deleteIcon treeIcon' href='#'><img src='/img/icons/page_delete.png' alt='delete'/></a>");
        this.html.append("<a class='viewIcon treeIcon' href='#'><img src='/img/icons/page_view.png' alt='view'/></a>");
        this.html.append("<a href='#'><img alt='page' src='/img/icons/page_page.png' />"+this.name+"</a>");
        this.html.mouseover(function(e){
            $(this).removeClass("hideIcons");
            e.preventDefault();
        });
        this.html.mouseout(function(e){
            $(this).addClass("hideIcons");
            e.preventDefault();
        });
    };
    
    this.init();
}
/**
 * Folder item for a tree
 */
var TreeFolder = function(name, location)
{
    this.name = name;
    this.location = location;
    this.children = new Array();
    this.parent = null;
    this.type = Tree.PAGE_FOLDER;
    this.html = null;
    this.openFolder = false;
    this.root = null;

    /**
     * Constructor
     */
    this.init = function()
    {
        var ref = this;
        
        this.html = $("<div></div>");
        this.html[0].object = ref;
        this.html.append("<a class='folder clickableTreeArrow' href='#'></a>");
        this.html.append("<img src='/img/icons/folder.png' alt='folder'/>");
        this.html.append("<a class='clickableTreeItem' style='padding: 0pt;' href='#'>"+this.name+"</a>");
        this.html.append("<div class='treeNode hidden'></div>");
        this.html.children(".clickableTreeArrow").bind({
            mouseover: function(e) {
                $(this).addClass("focus");
            },
            mouseout: function(e) {
                $(this).removeClass("focus");
            }
        });
        this.html.children(".clickableTreeItem, .clickableTreeArrow").click(function(e) {
            if(ref.openFolder)
            {
                ref.close();
            }
            else
            {
                ref.open();
            }
            e.preventDefault();
            return false;
        });
        
        //open when we have a cookie
        if(this.hasCookie(this.getPath()))
        {
            this.open();
        }
    };
    /**
     * Add a child to the folder
     * 
     * @param TreeItem child
     * @param Array locArray
     * @return success
     */
    this.addChild = function(child, locArray)
    {
        if(locArray.length == 0)
        {
            var x = 0;
            while(x < this.children.length)
            {
                if(this.children[x].name > child.name && this.children[x].type <= child.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                this.children.push(child);
                this.html.children(".treeNode").append(child.html);
            }
            else
            {
                this.children[x].html.before(child.html)
                this.children.splice(x, 0, child);
            }
            child.parent = this;
            return true;
        }
        
        var loc = locArray.shift();
        var inserted = false;

        for (key in this.children)
        {
            var node = this.children[key];

            if (node.type == Tree.PAGE_FOLDER && node.getPath() == this.getPath() + "/" + loc)
            {
                node.addChild(child, locArray);
                inserted = true;
                break;
            }
        }
        
        if (!inserted)
        {
            var folder = new TreeFolder(loc, this.getPath());
            var x = 0;
            while(x < this.children.length)
            {
                if(this.children[x].name > folder.name && this.children[x].type <= folder.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                this.children.push(folder);
                this.html.children(".treeNode").append(folder.html);
            }
            else
            {
                this.children[x].html.before(folder.html)
                this.children.splice(x, 0, folder);
            }
            folder.parent = this;
            folder.root = this.root;
            return folder.addChild(child, locArray);
        }
        
        return inserted;
    };
    /**
     * Get the path of this folder
     * 
     * @return String
     */
    this.getPath = function()
    {
        if(this.location == "/")
        {
            return this.location+this.name;
        }
        else
        {
            return this.location+"/"+this.name;
        }
    };
    /**
     * Open the folder
     */
    this.open = function() 
    {
        this.html.children(".treeNode").removeClass("hidden");
        this.html.children(".clickableTreeArrow").addClass("open");
        this.openFolder = true;
        
        this.createCookie(this.getPath(), 1);
    };
    /**
     * Collapse the folder
     */
    this.close = function()
    {
        this.html.children(".treeNode").addClass("hidden");
        this.html.children(".clickableTreeArrow").removeClass("open");
        this.openFolder = false;
        
        this.eraseCookie(this.getPath());
    };
    /**
     * Create a Cookie
     * 
     * @param String name
     * @param String value
     * @param Date days
     * @return Boolean      False
     */
    this.createCookie = function(name, value, days) {
        if (days) 
        {
            var date = new Date();
            date.setTime(date.getTime()+(days*24*60*60*1000));
            var expires = "; expires="+date.toGMTString();
        }
        else 
            var expires = "";

        document.cookie = name+"="+value+expires+"; path=/";
    };
    /**
     * Read a Cookie
     * 
     * @param String name
     * @return String       or False on failure
     */
    this.readCookie = function(name) {
        var nameEQ = name + "=";
        var ca = document.cookie.split(';');
        for(var i=0;i < ca.length;i++) 
        {
            var c = ca[i];

            while (c.charAt(0)==' ') 
                c = c.substring(1,c.length);

            if (c.indexOf(nameEQ) == 0) 
                return c.substring(nameEQ.length,c.length);
        }
        return false;
    };
    /**
     * Remove a cookie
     * 
     * @param String name
     * @return Boolean      False
     */
    this.eraseCookie = function(name) {
        this.createCookie(name,"",-1);
    };
    /**
     * Check if there is a cookie
     * 
     * @param String name
     * @return Boolean
     */
    this.hasCookie = function(name) {
        return (this.readCookie(name));
    }
    
    this.init();
}

var tree = new Tree("#treeWrapper");
tree.addChild(new TreeItem("hey", "/blaat/basdf/zafdhg/asdf/"));
tree.addChild(new TreeItem("hoi", "/blaat/basdf/"));
tree.addChild(new TreeItem("13", "/"));
tree.addChild(new TreeItem("12", "/"));
tree.addChild(new TreeItem("bb", "/"));
tree.addChild(new TreeItem("aa", "/"));